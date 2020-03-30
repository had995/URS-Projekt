#define DISTANCE_SENSOR_H

/* Functions used for controlling distance sensor */

void SENSOR_Init(void){
	
	DDRD = _BV(5);		/* Make trigger pin as output */
	PORTD = _BV(6);		/* Turn on Pull-up */
	
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0; /* Turning back to 0 because of lcd */

}


ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
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
	distance = (float)count / 428.67;
	
	return distance;
}

void distance_to_string(double distance){
		
		dtostrf(distance, 2, 2, string);/* distance to string */
		//strcat(string, " cm");	/* Concat unit i.e.cm */
		//lcd_puts("Visina: ");
		lcd_gotoxy(10,1);
		lcd_puts(string);	/* Print distance */
}
	

