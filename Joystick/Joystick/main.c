#define F_CPU 7372800UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"
#include "lcd.c"

static int16_t time = 5;


void writeLCD(uint16_t adc) {
	lcd_clrscr();

	char adcStr[16];
	itoa(adc, adcStr, 10);
	lcd_gotoxy(0,0);
	lcd_puts("ADC: ");
	lcd_gotoxy(6,0);
	lcd_puts(adcStr);
}

void STEPPER_Init(){
	
	DDRB = 0x0F; /* Make PORTD lower pins as output */
}

// ORANGE - IN1 - OUT1	0	0	1	1	1	1	1	 0 
// YELLOW - IN3 - OUT3  1	0	0	0	1	1	1	 1
// PINK   - IN2 - OUT2	1   1   1   0   0   0   1	 1
// BLUE   - IN4 - OUT4	1	1	1	1	1	0	0	 0
// 4 --> NULTI PIN      07  03  0B  09  0D  0C  0E   06 (CLOCKWISE SEQUENCE)


void rotate_clockwise(){
	
		PORTB = 0x07;  //0111
		PORTB = ~PORTB;
		_delay_ms(time);
		PORTB = 0x03;  //0011
		PORTB = ~PORTB;
		_delay_ms(time);
		PORTB = 0x0B;  //1011
		PORTB = ~PORTB;
		_delay_ms(time);
		PORTB = 0x09;
		PORTB = ~PORTB;
		_delay_ms(time);
		PORTB = 0x0D;
		PORTB = ~PORTB;
		_delay_ms(time);
		PORTB = 0x0C;
		PORTB = ~PORTB;
		_delay_ms(time);
		PORTB = 0x0E;
		PORTB = ~PORTB;
		_delay_ms(time);
		PORTB = 0x06;
		PORTB = ~PORTB;
		_delay_ms(time);
		
		
	}
void rotate_anticlockwise(){
	
	PORTB = 0x06;  
	PORTB = ~PORTB;
	_delay_ms(time);
	PORTB = 0x0E; 
	PORTB = ~PORTB;
	_delay_ms(time);
	PORTB = 0x0C;
	PORTB = ~PORTB;
	_delay_ms(time);
	PORTB = 0x0D;
	PORTB = ~PORTB;
	_delay_ms(time);
	PORTB = 0x09;
	PORTB = ~PORTB;
	_delay_ms(time);
	PORTB = 0x0B;
	PORTB = ~PORTB;
	_delay_ms(time);
	PORTB = 0x03;
	PORTB = ~PORTB;
	_delay_ms(time);
	PORTB = 0x07;
	PORTB = ~PORTB;
	_delay_ms(time);
	
}

void ADC_Init()
{
	DDRA=0x00;			/* Make ADC port as input */
	ADCSRA = 0x87;		/* Enable ADC, fr/128  */
	ADMUX = 0x40;		/* Vref: Avcc, ADC channel: 0 */
	
}

int ADC_Read(char channel)
{
	int Ain,AinLow;
	
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and 
					Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
}

int main(void)
{
	DDRD = _BV(4);

	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 14;

	lcd_init(LCD_DISP_ON);
	
	
	int value, value1;
	
	ADC_Init();
	
	STEPPER_Init();

	while (1) {
		
		value = ADC_Read(0);    /* Read ADC channel 0 */
		
		writeLCD(value);        // Ispis vrijednosti 
		
		if(value < 652){
			
			rotate_clockwise();
		}
		if(value > 770){
			
			rotate_anticlockwise();
		}
		

		_delay_ms(1);
	}
}
		
		

