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

//Overflow interrupt
ISR(TIMER1_OVF_vect)
{
	clock_datetime += 1;
}

//Initialise the clock
void clock_init(){
cli(); // disable all interrupts
TCCR1B = 0; // disable ticking
TCNT1H = 0; // reset counter
TCNT1L = 0;
ICR1H = 40000U >> 8; // set overflow value
ICR1L = 40000U & 0xff;

TCCR1A = 0xf2; // 11 11 11 10, set channel config

TIMSK1 |= (1<<TOIE0);

//Use prescaler 1024
TCCR1B |= ((1<<CS12)|(1<<CS10));

//Activate interrupts
sei();
}

//Return time
clock_time_t clock_time(){
	clock_time_t time;

	cli();
	time = clock_datetime;
	sei();

	return time;
}
