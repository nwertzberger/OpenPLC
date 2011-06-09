#include "global-conf.h"
#include "uart.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <avr/io.h>

volatile unsigned int buffercounter = 0;

char usart_rx_buffer[50];
char *rx_buffer_pointer_in	= &usart_rx_buffer[0];
char *rx_buffer_pointer_out	= &usart_rx_buffer[0];
	
//http://www.nongnu.org/avr-libc/user-manual/group__util__setbaud.html
//Maybe use setbaud instead

void usart_init(unsigned long baudrate0, unsigned long baudrate1)
{ 
	UCSR0B =(1 << TXEN0 | 1 << RXEN0 | 1<< RXCIE0);
	UBRR0=(F_CPU / (baudrate0 * 16L) - 1);

        UCSR1B =(1 << TXEN0 | 1 << RXEN0 | 1<< RXCIE0);
        UBRR1=(F_CPU / (baudrate1 * 16L) - 1);
}

int uart_putchar0(char c, FILE *stream)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
	return 0;
}

int uart_putchar1(char c, FILE *stream)
{
    while(!(UCSR1A & (1<<UDRE1)));
    UDR1 = c;
	return 0;
}