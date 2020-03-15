#define STEPPER_H

/* init of a stepper motor pins */

void STEPPER_Init(){
	
	DDRB = 0x0F; /* Make PORTB lower pins as output */
}

/* rotations of a step motor for one step, 5.625 degrees */

// ORANGE - IN1 - OUT1	0	0	1	1	1	1	1	 0
// YELLOW - IN3 - OUT3  1	0	0	0	1	1	1	 1
// PINK   - IN2 - OUT2	1   1   1   0   0   0   1	 1
// BLUE   - IN4 - OUT4	1	1	1	1	1	0	0	 0
// 4 --> NULTI PIN      07  03  0B  09  0D  0C  0E   06 (CLOCKWISE SEQUENCE)

void rotate_clockwise(){
	
	PORTB = 0x07;  //0111
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x03;  //0011
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0B;  //1011
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x09;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0D;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0C;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0E;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x06;
	PORTB = ~PORTB;
	_delay_ms(5);
}

void rotate_anticlockwise(){
	
	PORTB = 0x06;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0E;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0C;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0D;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x09;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x0B;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x03;
	PORTB = ~PORTB;
	_delay_ms(5);
	PORTB = 0x07;
	PORTB = ~PORTB;
	_delay_ms(5);
	
}

/* using two values to determine direction of stepper rotation */

void rotate_motors(int value, int low_margin, int high_margin){
	
	if(value < low_margin){
		
		rotate_clockwise();
	}
	if(value > high_margin){
		
		rotate_anticlockwise();
	}
	
}