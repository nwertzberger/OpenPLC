/* Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * All work Copyright 2010 University of Nebraska, Omaha.
 *
 * This code is intended to verify that the microcontroller is up and running.
 */

#include <pic18fregs.h>

#include "arch_startup.h"
#include "arch_startup_priv.h"     /* Only included in THIS FILE */

void delay_ms(long ms)
{
    long i;

    while (ms--)
        for (i=0; i < 330; i++)
            ;
}

void main()
{
    /* Startup the processor */
    arch_startup_init();
    TRISD=0;


    for (;;)
    {
        PORTD = 0xf0;
        delay_ms(1000);
        PORTD = 0x0f;
        delay_ms(1000);
    }
}

