/**
 * Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * This class is responsible for setting and getting values for the physical IO.
 *
 *
 *
 */

#include <inttypes.h>
#include <avr/io.h>

#include "phys.h"

void phys_init()
{
    DDRA = 0x00;
    DDRC = 0xff;
}

void phys_in(int * data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        data[i] = (PINA & (1 << i) ? 1 : 0);
    }
}

void phys_out(int * data)
{
    PORTC = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        PORTC |= (data[i] ? 1 : 0) << i;
    }
    PORTD = PORTC;
}
