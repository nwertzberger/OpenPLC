
#include "usart.h"

/* For Atmel devices which have 1xUSART, and don't have the register names set up for >1 USART , eg UDR */


void USART_init(uint16_t baudval) {

	UBRR = baudval;
	UCSRA &= ~(1<<U2X);					// Turn off double speed 
	UCSRB = (1<<RXEN)|(1<<TXEN);				// Enable receiver and transmitter
	UCSRC = (1<<URSEL) | (1<<UCSZ1)| (1<<UCSZ0);		// Set frame format: 8data,  1 stop bit
								// URSEL required to select the UCSRC instead of the UBRRH 
								// required on ATMEGA8, ATMEGA32, not on ATTINY2313, check on other devices

	UCSRA |= (1<<TXC);					// clear any existing transmits
}


// Transmit a single character
void USART_transmit( unsigned char data ) {
/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
   		 ;
	UCSRA |= (1<<TXC); 		/* clear txc flag */
/* Put data into buffer, sends the data */
	UDR = data;
}


uint8_t USART_receive( void ) {

	while ( !(UCSRA & (1<<RXC)) ) 
		;
	return UDR;
}

void USART_flush(void) {
	while (UCSRA & (1<<RXC)) {
		 USART_receive();
	}
}


// Transmit a string
void USART_transmit_string(const unsigned char* data) {
	while(*data) {
		while ( !( UCSRA & (1<<UDRE)) )
		;
		UDR = *data;
		++data;
	}
}


#ifdef USART_EXTRA_SEND_FORMATS
// Transmit an unsigned 8 bit value as a series of up to three decimal characters
void USART_transmit_uint8(uint8_t val) {
	unsigned char buf[3];
	int8_t ptr;
	for(ptr=0;ptr<3;++ptr) {
		buf[ptr] = (val % 10) + '0';
		val /= 10;
	}
	for(ptr=2;ptr>0;--ptr) {
		if (buf[ptr] != '0') break;
	}
	for(;ptr>=0;--ptr) {
		USART_transmit(buf[ptr]);
	}
}


// Transmit a signed 8 bit value as a possible minus character followed by a series of up to three decimal characters
void USART_transmit_int8(int8_t val) {
	unsigned char buf[3];
	int8_t ptr;
	if (val < 0) {
	  USART_transmit('-');
	  val *= -1;
	}
	for(ptr=0;ptr<3;++ptr) {
		buf[ptr] = (val % 10) + '0';
		val /= 10;
	}
	for(ptr=2;ptr>0;--ptr) {
		if (buf[ptr] != '0') break;
	}
	for(;ptr>=0;--ptr) {
		USART_transmit(buf[ptr]);
	}
}



// Transmit an unsigned 16 bit value as up to five decimal characters
void USART_transmit_uint16(uint16_t val) {
	unsigned char buf[5];
	int8_t ptr;
	for(ptr=0;ptr<5;++ptr) {
		buf[ptr] = (val % 10) + '0';
		val /= 10;
	}
	for(ptr=4;ptr>0;--ptr) {
		if (buf[ptr] != '0') break;
	}
	for(;ptr>=0;--ptr) {
		USART_transmit(buf[ptr]);
	}
}


// Transmit a signed 16 bit value as a possible minus character followed by up to five decimal characters
void USART_transmit_int16(int16_t val) {
	unsigned char buf[5];
	if (val < 0) {
	  USART_transmit('-');
	  val *= -1;
	}
	int8_t ptr;
	for(ptr=0;ptr<5;++ptr) {
		buf[ptr] = (val % 10) + '0';
		val /= 10;
	}
	for(ptr=4;ptr>0;--ptr) {
		if (buf[ptr] != '0') break;
	}
	for(;ptr>=0;--ptr) {
		USART_transmit(buf[ptr]);
	}
}



// Transmit an 8 bit unsigned value as 8 binary digits

void USART_transmit_bin(uint8_t val) {
	unsigned char buf[3];
	int8_t ptr;
	for(ptr=8;ptr>0;--ptr) {
		if ((val >> (ptr-1)) & 0x01) {
			USART_transmit('1');
		} else {
			USART_transmit('0');			
		}
	}
}



// Transmit the bottom 4 bits of an 8 bit value as one character 0-9,A-F

void USART_transmit_nibble(uint8_t nibble) {
	nibble &= 0x0F ;
    if (nibble>9) {
    	nibble += 0x37 ;
    } else {
    	nibble += 0x30 ;
    }
    USART_transmit(nibble);
}	
	


// Transmit the an 8 bit value as two hex digits 0-9,A-F

void USART_transmit_hex(uint8_t val) {
	USART_transmit_nibble(val >>4) ;
	USART_transmit_nibble(val) ;
}





void usart0_out_iso_time(uint8_t *time_store) {

	USART_transmit(' ');
	USART_transmit_hex(time_store[6]);				// yy
	USART_transmit('-');
	USART_transmit_hex(time_store[4]);				// mm
	USART_transmit('-');
	USART_transmit_hex(time_store[3]);				// dd
	USART_transmit(' ');
	USART_transmit_hex(time_store[2]);				// hh
	USART_transmit(':');
	USART_transmit_hex(time_store[1]);				// mm
	USART_transmit(':');
	USART_transmit_hex(time_store[0]);				// ss
	
}

#endif // end - #ifdef USART_EXTRA_SEND_FORMATS
