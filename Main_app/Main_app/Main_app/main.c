#define F_CPU 7372800UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>		/* used for writing  to eeprom */ 

#include "lcd.h"
#include "lcd.c"
#include "stepper.h"
#include "adc.h"

char string[15];
long count;
float  distance, distance_red = 0;
int16_t TimerOverflow = 0;
int16_t distance_counter = 0;
int16_t start_screen_flag = 1;
int16_t user1_flag = 0, distance_user1_exists = 0;
int16_t user2_flag = 0, distance_user2_exists = 0;

#define  Trigger_pin	PD5	/* Trigger pin, PD6 ECHO pin */ 
#define  low_margin		510 /* low margin for value of adc which spins motors */
#define  high_margin    658 /* high margin for value of adc which spins motors */


#include "distance_sensor.h"


void writeLCD(uint16_t adc) { 

	char adcStr[16];
	itoa(adc, adcStr, 10);
	lcd_gotoxy(11,1);
	lcd_puts(" ");
	lcd_puts(adcStr);
	
	_delay_ms(10);
}


void lcd_main_init(){
		DDRC = _BV(7);

		TCCR1A = _BV(COM1B1) | _BV(WGM10);
		TCCR1B = _BV(WGM12) | _BV(CS11);
		OCR1B = 14;

		lcd_init(LCD_DISP_ON);
		
		DDRD = 0x01;
}

void save_distance(float distance, int user){
	
	int pozicija = 100 + user;
	
	eeprom_write_float((float *) pozicija, distance); /* writing distance value to nth position in eeprom */
	
}

int read_distance(int user){
	
	int pozicija = 100 + user;
	
	distance_red = eeprom_read_float((float *) pozicija);
	
	return distance_red;
}

void show_distance(){
	
	
	distance_to_string(mesure_distance());
	
	_delay_ms(3000);
}


int main(void)
{
	
	int value;
	
	lcd_main_init();
	
	ADC_Init();
	
	STEPPER_Init();
	
	SENSOR_Init();
	
	PORTB = 0x0F;
	DDRB = 0;
	
	while (1) {
		
		value = ADC_Read(0);    /* Read ADC channel */
		
		//writeLCD(value); /* writing adc value */
		

		//rotate_motors(value,low_margin,high_margin);
		
		/*if(distance_counter == 20){ // measuring distance every 200 ms 
		
		mesure_distance();
		
		distance_counter = 0;

		} */
		
		if(start_screen_flag){
		
		lcd_clrscr();
		lcd_puts("USER1 USER2 VIS");
		lcd_gotoxy(0,1);
		lcd_puts("KEY1  KEY2 KEY3");
		
		
		if(bit_is_clear(PINB,0)){
			
			user1_flag = 1; // user 1 is chosen
			start_screen_flag = 0;
			_delay_ms(200); // debounce
		}else if(bit_is_clear(PINB,1)){
			
			user2_flag = 1;  // user 2 is chosen
			start_screen_flag = 0;
			_delay_ms(200);
		}else if(bit_is_clear(PINB,2)){
		
		show_distance();
		_delay_ms(200);
		
			}
		}
		
		
		
		
		if(user1_flag && start_screen_flag == 0){
			
			user2_flag = 0;
			
			if(read_distance(1) != 0){
				
				distance = read_distance(1); 
				
				// if hight already exists, set the hight of a table to this
				// also starting position( sitting standing should be automatically determined 
				// depending of a hight
				// average sitting table hight is 75 cm, so everything under 100 should be sitting
				
				distance_user1_exists = 1;
			}
			else{
				
				lcd_clrscr();
				lcd_puts("POSTAVITE VISINU");
				lcd_gotoxy(0,1);
				lcd_puts("KEY1 ---> DONE");
				
				rotate_motors(value,low_margin,high_margin);
				
				if(bit_is_clear(PINB,0)){
					
					distance = mesure_distance();
					
					save_distance(distance,1); /* saving distance(hight) for user, calculating average */
					
					// standing position ???
				
				distance_user1_exists = 1;
				
				_delay_ms(200);
				
				}
			}
				
				if(distance_user1_exists){
					
					lcd_clrscr();
					lcd_puts("STD P. RESET ESC"); // 16 charova
					lcd_gotoxy(0,1);
					lcd_puts("KEY1 KEY2 KEY3");
					
					if(bit_is_clear(PINB,0)){
						
						// set a logic to changing to a standing position
						// mby 2 memory spaces for each user
						
						_delay_ms(200);
					}
					if(bit_is_clear(PINB,1)){
						
						save_distance(0,1); // writing 0 to "reset" user height
						
						start_screen_flag = 1;
						distance_user1_exists = 0;
						
						_delay_ms(200);
					}
					
					if(bit_is_clear(PINB,2)){
						
						start_screen_flag = 1; // return
						
						_delay_ms(200);
					}	
				}
			}
			if(user2_flag && start_screen_flag == 0){
				
				user1_flag = 0;
				
				
				if(read_distance(2) != 0){
					
					distance = read_distance(2);
					
					// if hight already exists, set the hight of a table to this
					// also starting position( sitting standing should be automatically determined
					// depending of a hight
					// average sitting table hight is 75 cm, so everything under 100 should be sitting
					
					distance_user2_exists = 1;
				}
				else{
					
					lcd_clrscr();
					lcd_puts("POSTAVITE VISINU");
					lcd_gotoxy(0,1);
					lcd_puts("KEY1 ---> DONE");
					
					rotate_motors(value,low_margin,high_margin);
					
					if(bit_is_clear(PINB,0)){
						
						distance = mesure_distance();
						
						save_distance(distance,2); /* saving distance(hight) for user, calculating average */
						
						// standing position ???
						
						distance_user2_exists = 1;
						
						_delay_ms(200);
						
					}
				}
				
				if(distance_user2_exists){
					
					
					lcd_clrscr();
					lcd_puts("STD P. RESET ESC");
					lcd_gotoxy(0,1);
					lcd_puts("KEY1 KEY2 KEY3");
					
					if(bit_is_clear(PINB,0)){
						
						// set a logic to changing to a standing position
						// mby 2 memory spaces for each user
						
						_delay_ms(200);
						
					}
					if(bit_is_clear(PINB,1)){
						
						save_distance(0,2); // writing 0 to "reset" user height
						
						start_screen_flag = 1;
						distance_user2_exists = 0;
						
						_delay_ms(200);
					}
					
					if(bit_is_clear(PINB,2)){
						
						start_screen_flag = 1; // return
						
						_delay_ms(500);
					}
				}
			}
			
		//distance_counter++; 
		
		_delay_ms(10);
	}
}
		
		


