#define F_CPU 7372800UL /* Define CPU Frequency 1MHz */
#include <avr/io.h> /* Include AVR std. library file */
#include <util/delay.h> /* Include delay header file */


int main(void)
{
	
	//int period;
	DDRA = 0x0F; /* Make PORTA lower pins as output */
	int period = 6; /* Set period in between two steps */
	
	while (1)
	{
		/* Rotate Stepper Motor clockwise with Half step sequence */
		for(int i=0;i<50;i++)
		{
			PORTA = 0x07;  //0111
			PORTA = ~PORTA;
			_delay_ms(period);
			PORTA = 0x03;  //0011
			PORTA = ~PORTA;
			_delay_ms(period);
			PORTA = 0x0B;
			PORTA = ~PORTA;
			_delay_ms(period);
			PORTA = 0x09;
			PORTA = ~PORTA;
			_delay_ms(period);
			PORTA = 0x0D;
			PORTA = ~PORTA;
			_delay_ms(period);
			PORTA = 0x0C;
			PORTA = ~PORTA;
			_delay_ms(period);
			PORTA = 0x0E;
			PORTA = ~PORTA;
			_delay_ms(period);
			PORTA = 0x06;
			PORTA = ~PORTA;
			_delay_ms(period);
		}
		PORTA = 0x07; /* Last step to initial position */
		_delay_ms(period);
		_delay_ms(10);
	}
}

