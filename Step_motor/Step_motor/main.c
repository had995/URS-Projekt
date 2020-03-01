#define F_CPU 7372800UL
#include <util/delay.h>
#include <avr/io.h>


int main(void)
{
	
	int i;
	
	DDRD = 0xFF;
	//PORTD = 0b00110000;
	
	while(1){  // IN1 - OUT1 - ORANGE 
			   // IN2 - OUT2 - PINK
			   // IN3 - OUT3 - YELLOW
			   // IN4 - OUT4 - BLUE
		
	//for(i=10;i!=0;i--)
	//{
		PORTD=0b00001001; // STEP 1
		/*PORTD = ~PORTD;
		_delay_ms(50);
		PORTD=0b00000101; // STEP 2
		PORTD = ~PORTD;
		_delay_ms(50);
		PORTD=0b00001011; // STEP 3
		PORTD = ~PORTD;
		_delay_ms(50);
		PORTD=0b00001001; // STEP 4
		PORTD = ~PORTD;
		_delay_ms(50);
		PORTD=0b00001011; // STEP 5
		PORTD = ~PORTD;
		_delay_ms(50);
		PORTD=0b00001010; // STEP 6
		PORTD = ~PORTD;
		_delay_ms(50);
		PORTD=0b00001110; // STEP 7
		PORTD = ~PORTD;
		_delay_ms(50);
		PORTD=0b00000110; // STEP 8
		PORTD = ~PORTD;
		_delay_ms(50);*/
	}
	_delay_ms(1000);
	/*for(i=10;i!=0;i--)
	{
		PORTD=0b10110000;
		_delay_ms(50);
		PORTD=0b01110000;
		_delay_ms(50);
		PORTD=0b00111000;
		_delay_ms(50);
		PORTD=0b00110100;
		_delay_ms(50);
	}*/
	//}
}

