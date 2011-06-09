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

volatile uint16_t seconds, tick;
volatile uint16_t tickS = 1;

//Overflow itnerrupt
ISR(TIMER1_COMPA_vect)
{
	clock_datetime += 1;
	tick++;
	
  if (tickDiff(seconds) >= 500)
  {
  	tickS++;
	  seconds = tick;
  }	
}

//Initialise the clock
void clock_init(){
   TCCR1B |= (1 << WGM12) | (1 << CS12); // Configure timer 1 for CTC mode
   OCR1A   = 125; // Compare value 125
   TCCR1A |= (1 << CS12); // Start timer at Fcpu/256
   
   TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
     
   sei();
}

//Return time
clock_time_t clock_time(){
	return clock_datetime;
}


uint16_t tickDiff(uint16_t oldtick)
{
	return tick - oldtick;
}

uint16_t tickDiffS(uint16_t oldtick)
{
	return tickS - oldtick;
}

