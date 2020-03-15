#define F_CPU 7372800UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>


#include "lcd.h"
#include "lcd.c"
#include "stepper.h"

char string[10];
long count;
double  distance;

#define  Trigger_pin	PD0	/* Trigger pin */
#define  low_margin		510 /* low margin for value of adc which spins motors */
#define  high_margin    640 /* high margin for value of adc which spins motors */

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}


void writeLCD(uint16_t adc) { 
	lcd_clrscr();

	char adcStr[16];
	itoa(adc, adcStr, 10);
	lcd_gotoxy(0,1);
	lcd_puts("ADC: ");
	lcd_gotoxy(6,1);
	lcd_puts(adcStr);
	
	_delay_ms(20);
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


double mesure_distance(){
	
	/* Give 10us trigger pulse on trig. pin to HC-SR04 */
	PORTD |= (1 << Trigger_pin);
	_delay_us(10);
	PORTD &= (~(1 << Trigger_pin));
	
	TCNT1 = 0;	/* Clear Timer counter */
	TCCR1B = 0x41;	/* Capture on rising edge, No prescaler*/
	TIFR = (1<<ICF1);	/* Clear ICP flag (Input Capture flag) */
	TIFR = (1<<TOV1);	/* Clear Timer Overflow flag */

	/*Calculate width of Echo by Input Capture (ICP) */
	
	while ((TIFR & (1 << ICF1)) == 0);/* Wait for rising edge */
	TCNT1 = 0;	/* Clear Timer counter */
	TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
	TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
	TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
	TimerOverflow = 0;/* Clear Timer overflow count */

	while ((TIFR & (1 << ICF1)) == 0);/* Wait for falling edge */
	count = ICR1 + (65535 * TimerOverflow);	/*When a capture is triggered according to the ICES1 setting, 
											the counter value is copied into the Input Capture Register (ICR1).
											The event will also set the Input Capture Flag (ICF1), and this
											can be used to cause an Input Capture Interrupt, if this interrupt is enabled. */
	/* 7372800 MHz Timer freq, 0.136us time for one instruction, sound speed =343 m/s, 34300 cm/s */
	/* distance = speed * time / 2 */
	distance = (double)count / 428.67;
	
	return distance;
}

	void distance_to_string(double distance){
		
		dtostrf(distance, 2, 2, string);/* distance to string */
		strcat(string, " cm   ");	/* Concat unit i.e.cm */
		lcd_clrscr();
		lcd_puts("Dist = ");
		lcd_puts(string);	/* Print distance */
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
	
	DDRD = 0x01;		/* Make trigger pin as output */
	PORTD = _BV(6);		/* Turn on Pull-up */
	
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;

	while (1) {
		
		value = ADC_Read(0);    /* Read ADC channel */
		
		writeLCD(value);
		
		rotate_motors(value,low_margin,high_margin);
		
		mesure_distance();
		
		distance_to_string(distance);
		
		_delay_ms(30);
	}
}
		
		


