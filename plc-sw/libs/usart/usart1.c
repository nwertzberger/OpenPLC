
#include "usart.h"


/* For Atmel devices which have 1xUSART, but have the register names set up for >1 USART, eg UDR0 */


void USART_init(uint16_t baudval) {
//inline void USART_init(uint16_t baudval) {
  	
//	UBRR0H = (uint8_t)(baudval>>8);	// Set baud rate
//	UBRR0L = (uint8_t)baudval;
	
	UBRR0 = baudval ;

	UCSR0A &= ~(1<<U2X0);					// Turn off double speed
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);			// Enable receiver and transmitter
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00) ;			// Set frame format: 8data,  1 stop bit
	UCSR0A |= (1<<TXC0);					// clear any existing transmits

}

// Transmit a single character
void USART_transmit(uint8_t data ) {

	while ( !( UCSR0A & (1<<UDRE0)) )			// Wait for empty transmit buffer
    		;
	UCSR0A |= (1<<TXC0); 					// clear txc flag
	UDR0 = data;						// Put data into buffer, sends the data
}


// Transmit a string
void USART_transmit_string(const char* data) {
	while(*data) {
		while ( !( UCSR0A & (1<<UDRE0)) )
		;
		UDR0 = *data;
		++data;
	}
}


uint8_t USART_receive( void ) {
	while ( !(UCSR0A & (1<<RXC0)) ) 
		;
	return UDR0;
}


void USART_flush(void) {
	while (UCSR0A & (1<<RXC0)) {
		 USART_receive();
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


// Transmit a signed 8 bit value as a possible minus chracter followed by up to three decimal characters
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


// Transmit a signed 16 bit value as a possible minus chracter followed by up to five decimal characters
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


void usart_out_iso_time(uint8_t *time_store) {

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

#endif
