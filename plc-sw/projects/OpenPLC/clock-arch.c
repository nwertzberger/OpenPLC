#include "global-conf.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include "clock-arch.h"

//Counted time
clock_time_t clock_datetime = 0;

//Overflow itnerrupt
ISR(TIMER0_OVF_vect)
{
	clock_datetime += 1;
//	TIFR0 |= (1<<TOV0);
}

//Initialise the clock
void clock_init(){
cli(); // disable all interrupts
TCCR1B = 0; // disable ticking
TIMSK = (TIMSK & ~(0x3c)) | 0x04; // enable overflow interrupt
TCNT1H = 0; // reset counter
TCNT1L = 0;
ICR1H = 40000U >> 8; // set overflow value
ICR1L = 40000U & 0xff;
TCCR1A = 0xfe; // 11 11 11 10, set channel config
TCCR1B = 0x1a; // 00011010 start ticking 
sei(); // reenable interrupts


/*	//Activate overflow interrupt for timer0
	TIMSK0 |= (1<<TOIE0);

	//Use prescaler 1024
	TCCR0B |= ((1<<CS12)|(1<<CS10));

	//Activate interrupts
	sei();
*/
}

//Return time
clock_time_t clock_time(){
	clock_time_t time;

	cli();
	time = clock_datetime;
	sei();

	return time;
}
