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

#define  Trigger_pin	PD0	/* Trigger pin */
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
}

int main(void)
{
	
	int value;
	
	lcd_main_init();
	
	ADC_Init();
	
	STEPPER_Init();
	
	SENSOR_Init();
	
	while (1) {
		
		value = ADC_Read(0);    /* Read ADC channel */
		
		writeLCD(value); /* writing adc value */
		
		rotate_motors(value,low_margin,high_margin);
		
		if(distance_counter == 20){ /* measuring distance every 200 ms */
		
		mesure_distance();
		
		distance_counter = 0;

		}
		
		distance_to_string(distance); /* writing distance */
		
		distance_counter++; 
		
		_delay_ms(10);
	}
}
		
		


