#define F_CPU 7372800UL
#include <util/delay.h>
#include <avr/io.h>



static int i;

int main(void)
{
	
	DDRD=0xFF;
	
	while(1){
		
	for(i=10;i!=0;i--)
	{
		PORTD=0b00110100;
		_delay_ms(50);
		PORTD=0b00111000;
		_delay_ms(50);
		PORTD=0b01110000;
		_delay_ms(50);
		PORTD=0b10110000;
		_delay_ms(50);
	}
	_delay_ms(1000);
	for(i=10;i!=0;i--)
	{
		PORTD=0b10110000;
		_delay_ms(50);
		PORTD=0b01110000;
		_delay_ms(50);
		PORTD=0b00111000;
		_delay_ms(50);
		PORTD=0b00110100;
		_delay_ms(50);
	}
	}
}

