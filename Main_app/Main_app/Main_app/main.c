#define F_CPU 7372800UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "lcd.c"
#include "stepper.h"
#include "adc.h"

char string[10];
long count;
double  distance;
int16_t TimerOverflow = 0;
int16_t distance_counter = 0;
int16_t start_screen_flag = 1;
int16_t selecting_screen_flag = 0;
int16_t user1_flag = 0;
int16_t user2_flag = 0;

#define  Trigger_pin	PD5	/* Trigger pin */
#define  low_margin		510 /* low margin for value of adc which spins motors */
#define  high_margin    658 /* high margin for value of adc which spins motors */


#include "distance_sensor.h"


void writeLCD(uint16_t adc) { 
	lcd_clrscr();

	char adcStr[16];
	itoa(adc, adcStr, 10);
	lcd_gotoxy(0,1);
	lcd_puts("ADC: ");
	lcd_gotoxy(6,1);
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
		
		//distance_to_string(distance); /* writing distance */
		
		
		if(start_screen_flag){
		
		lcd_clrscr();
		lcd_puts("Adjust hight!!!");
		lcd_gotoxy(0,1);
		lcd_puts("Key1 --> DONE");
		
		}
		if(bit_is_clear(PINB,0)){
			
			selecting_screen_flag = 1;
			start_screen_flag = 0;
		}
		
		
		if(selecting_screen_flag){
			
			lcd_clrscr();
			lcd_puts("SELECT USER:");
			lcd_gotoxy(0,1);
			lcd_puts("USER1");
			lcd_gotoxy(7,1);
			lcd_puts("USER2");
		}
		if(bit_is_clear(PINB,0) && selecting_screen_flag){ // user1
			
			//save_distance(distance,1); /* saving distance(hight) for user, calculating average 
			
			selecting_screen_flag = 0;
			
			user1_flag = 1;
			
		}
		if(bit_is_clear(PINB,1) && selecting_screen_flag){ // user2
			
			//save_distance(distance,2);
			
			selecting_screen_flag = 0;
			
			user2_flag = 1;
			
		}
		
		if(selecting_screen_flag == 0 && user1_flag){
			
				lcd_clrscr();
				lcd_puts("WELCOME USER1");
				lcd_gotoxy(0,1);
				/* todo : using calculated distance to automatically set hight */
				lcd_puts("SJEDECA POZICIJA");
				/* using keys 3 and 4 to switch between sitting and standing */
		}
		if(selecting_screen_flag == 0 && user2_flag){
				
				lcd_clrscr();
				lcd_puts("WELCOME USER2");
			    lcd_gotoxy(0,1);
				lcd_puts("SJEDECA POZICIJA");
		}
		
		
		distance_counter++; 
		
		_delay_ms(10);
	}
}
		
		


