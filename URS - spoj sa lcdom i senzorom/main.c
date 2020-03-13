
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"	/* Include LCD header file */
#include "lcd.c"

#define  Trigger_pin	PA7	/* Trigger pin */

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}
static int16_t time = 5;


void writeLCD(uint16_t adc) {
	lcd_clrscr();

	char adcStr[16];
	itoa(adc, adcStr, 10);
	//lcd_gotoxy(0,0);
	//lcd_puts("ADC: ");
	lcd_gotoxy(13,0);
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
	DDRA = ~_BV(0);		/* Make ADC port as input */
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
	char string[10];
	long count;
	double distance;
	
	int value;
	
	
		
	STEPPER_Init();
	
	DDRA = 0x80;		/* Make trigger pin as output */
	PORTD = _BV(6);		/* Turn on Pull-up -- */
	
	DDRD = _BV(4);
	
	
	
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 18;

	ADC_Init();
	
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_gotoxy(0, 0);
	lcd_puts("Ultrasonic");
	
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;		/* Set all bit to zero Normal operation */

	while(1)
	{
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		/*PORTA = (1 << Trigger_pin) | (1 << 0) ; 
		_delay_us(10);
		PORTA = (~(1 << Trigger_pin)) | (1 << 0);
		
		TCNT1 = 0;	/* Clear Timer counter */
		//TCCR1B = 0x41;	/* Capture on rising edge, No prescaler*/
		//TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		//TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */

		/*Calculate width of Echo by Input Capture (ICP) */
		/*
		while ((TIFR & (1 << ICF1)) == 0);/* Wait for rising edge */
		/*TCNT1 = 0;	/* Clear Timer counter */
		//TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
		//TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		//TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
		//TimerOverflow = 0;/* Clear Timer overflow count */

		//while ((TIFR & (1 << ICF1)) == 0);/* Wait for falling edge */
		//count = ICR1 + (65535 * TimerOverflow);	/* Take count */
		/* 8MHz Timer freq, sound speed =343 m/s */
		//distance = (double)count / 466.47;

		//dtostrf(distance, 2, 2, string);/* distance to string */
		//strcat(string, " cm   ");	/* Concat unit i.e.cm */
		//lcd_gotoxy(0, 1);
		//lcd_puts("Dist = ");
		//lcd_gotoxy(7, 1);
		//lcd_puts(string);	/* Print distance */
		//_delay_ms(200);
		
		value = ADC_Read(0);    /* Read ADC channel 0 */
		
		writeLCD(value);        // Ispis vrijednosti
		
		if(value < 652){
			
			rotate_clockwise();
		}
		if(value > 770){
			
			rotate_anticlockwise();
		}
		
	}
}

