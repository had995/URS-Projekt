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

char string[10];
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
	
	int pozicija = 20 + user;
	
	eeprom_write_float((float *) pozicija, distance); /* writing distance value to nth position in eeprom */
	
}

int read_distance(int user){
	
	int pozicija = 30 + user;
	
	distance_red = eeprom_read_float((float *) pozicija);
	
	return distance_red;
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
		
		rotate_motors(value,low_margin,high_margin);
		
		if(distance_counter == 20){ /* measuring distance every 200 ms */
		
		mesure_distance();
		
		distance_counter = 0;

		}
		
		if(start_screen_flag){
		
		lcd_clrscr();
		lcd_puts("USER1   USER2");
		lcd_gotoxy(0,1);
		lcd_puts("KEY1    KEY2");
		
		}
		if(bit_is_clear(PINB,0)){
			
			user1_flag = 1; // user 1 is chosen
			start_screen_flag = 0;
		}else if(bit_is_clear(PINB,1)){
			
			user2_flag = 1;  // user 2 is chosen
			start_screen_flag = 0;
		}
		
		
		if(user1_flag && start_screen_flag == 0){
			
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
				
				if(bit_is_clear(PINB,1)){
					
					save_distance(mesure_distance(),1); /* saving distance(hight) for user, calculating average */
					
					// standing position ???
				
				//ok, problem je da on pristupi ovoj if petlji iako nesmije, zašto? a niti ne spremi udaljenost
				
				}
			}
				
				if(distance_user1_exists){
					
					lcd_clrscr();
					lcd_puts("STOJECA POVRATAK:");
					lcd_gotoxy(0,1);
					lcd_puts("KEY1    KEY2");
					
					if(bit_is_clear(PINB,0)){
						
						// set a logic to changing to a standing position
						// mby 2 memory spaces for each user
						
					}
					if(bit_is_clear(PINB,1)){
						
						start_screen_flag = 1; // return
					}	
				}
			}
			
		distance_counter++; 
		
		_delay_ms(10);
	}
}
		
		


