
#include "usart.h"


/* For Atmel devices which have 2xUSART, and which have the register names set up for 2, eg UDR0, UDR1 */



// Initialise USART

void USART0_init(uint16_t baudval) {

	UBRR0 = baudval;							// Set baud rate
	UCSR0A &= ~(1<<U2X0);						// Turn off double speed 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);				// Enable receiver and transmitter
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);			// Set frame format: 8data,  1 stop bit
	UCSR0A |= (1<<TXC0);						// clear any existing transmits
}


void USART1_init(uint16_t baudval) {

	UBRR1 = baudval;
	UCSR1A &= ~(1<<U2X1);
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	UCSR1C =  (1<<UCSZ11) | (1<<UCSZ10);
	UCSR1A |= (1<<TXC1);
}


// Transmit a single character

void USART0_transmit(uint8_t data) {

	while ( !( UCSR0A & (1<<UDRE0)) )			// Wait for empty transmit buffer
    ;
	UCSR0A |= (1<<TXC0); 						// clear txc flag
	UDR0 = data;								// Put data into buffer, sends the data
}


void USART1_transmit(uint8_t data) {
	while ( !( UCSR1A & (1<<UDRE1)) )
    ;
	UCSR1A |= (1<<TXC1);
	UDR1 = data;
}




void USART0_transmit_string(const char* data) {
	while(*data) {
		while ( !( UCSR0A & (1<<UDRE0)) )
		;
		UDR0 = *data;
		++data;
	}
}


void USART1_transmit_string(const char* data) {
	while(*data) {
		while ( !( UCSR1A & (1<<UDRE1)) )
		;
		UDR1 = *data;
		++data;
	}
}


uint8_t USART0_receive( void ) {

	while ( !(UCSR0A & (1<<RXC0)) ) 
		;
	return UDR0;
}

uint8_t USART1_receive( void ) {

	while ( !(UCSR1A & (1<<RXC1)) ) 
		;
	return UDR1;
}


void USART0_flush(void) {
	while (UCSR0A & (1<<RXC0)) {
		 USART0_receive();
	}
}

void USART1_flush(void) {
	while (UCSR1A & (1<<RXC1)) {
		 USART1_receive();
	}
}


#ifdef USART_EXTRA_SEND_FORMATS
// Transmit an unsigned 8 bit value as a series of up to three decimal characters
void USART0_transmit_uint8(uint8_t val) {
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
		USART0_transmit(buf[ptr]);
	}
}


// Transmit a signed 8 bit value as a possible minus chracter followed by up to three decimal characters
void USART0_transmit_int8(int8_t val) {
	unsigned char buf[3];
	int8_t ptr;
	if (val < 0) {
	  USART0_transmit('-');
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
		USART0_transmit(buf[ptr]);
	}
}


// Transmit an unsigned 16 bit value as up to five decimal characters
void USART0_transmit_uint16(uint16_t val) {
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
		USART0_transmit(buf[ptr]);
	}
}


// Transmit a signed 16 bit value as a possible minus chracter followed by up to five decimal characters
void USART0_transmit_int16(int16_t val) {
	unsigned char buf[5];
	if (val < 0) {
	  USART0_transmit('-');
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
		USART0_transmit(buf[ptr]);
	}
}




// Transmit an unsigned 8 bit value as a series of up to three decimal characters
void USART1_transmit_uint8(uint8_t val) {
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
		USART1_transmit(buf[ptr]);
	}
}


// Transmit a signed 8 bit value as a possible minus chracter followed by up to three decimal characters
void USART1_transmit_int8(int8_t val) {
	unsigned char buf[3];
	int8_t ptr;
	if (val < 0) {
	  USART1_transmit('-');
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
		USART1_transmit(buf[ptr]);
	}
}


// Transmit an unsigned 16 bit value as up to five decimal characters
void USART1_transmit_uint16(uint16_t val) {
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
		USART1_transmit(buf[ptr]);
	}
}


// Transmit a signed 16 bit value as a possible minus chracter followed by up to five decimal characters
void USART1_transmit_int16(int16_t val) {
	unsigned char buf[5];
	if (val < 0) {
	  USART1_transmit('-');
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
		USART1_transmit(buf[ptr]);
	}
}



// Transmit an 8 bit unsigned value as 8 binary digits
void USART0_transmit_bin(uint8_t val) {
	unsigned char buf[3];
	int8_t ptr;
	for(ptr=8;ptr>0;--ptr) {
		if ((val >> (ptr-1)) & 0x01) {
			USART0_transmit('1');
		} else {
			USART0_transmit('0');			
		}
	}
}

void USART1_transmit_bin(uint8_t val) {
	unsigned char buf[3];
	int8_t ptr;
	for(ptr=8;ptr>0;--ptr) {
		if ((val >> (ptr-1)) & 0x01) {
			USART1_transmit('1');
		} else {
			USART1_transmit('0');			
		}
	}
}




// Transmit the bottom 4 bits of an 8 bit value as one character 0-9,A-F
void USART0_transmit_nibble(uint8_t nibble) {
	nibble &= 0x0F ;
    if (nibble>9) {
    	nibble += 0x37 ;
    } else {
    	nibble += 0x30 ;
    }
    USART0_transmit(nibble);
}	

void USART1_transmit_nibble(uint8_t nibble) {
	nibble &= 0x0F ;
    if (nibble>9) {
    	nibble += 0x37 ;
    } else {
    	nibble += 0x30 ;
    }
    USART1_transmit(nibble);
}	



// Transmit the an 8 bit value as two hex digits 0-9,A-F
void USART0_transmit_hex(uint8_t val) {
	USART0_transmit_nibble(val >>4) ;
	USART0_transmit_nibble(val) ;
}

void USART1_transmit_hex(uint8_t val) {
	USART1_transmit_nibble(val >>4) ;
	USART1_transmit_nibble(val) ;
}






void usart0_out_iso_time(uint8_t *time_store) {

	USART0_transmit(' ');
	USART0_transmit_hex(time_store[6]);				// yy
	USART0_transmit('-');
	USART0_transmit_hex(time_store[4]);				// mm
	USART0_transmit('-');
	USART0_transmit_hex(time_store[3]);				// dd
	USART0_transmit(' ');
	USART0_transmit_hex(time_store[2]);				// hh
	USART0_transmit(':');
	USART0_transmit_hex(time_store[1]);				// mm
	USART0_transmit(':');
	USART0_transmit_hex(time_store[0]);				// ss
	
}

void usart1_out_iso_time(uint8_t *time_store) {

	USART0_transmit(' ');
	USART0_transmit_hex(time_store[6]);				// yy
	USART0_transmit('-');
	USART0_transmit_hex(time_store[4]);				// mm
	USART0_transmit('-');
	USART0_transmit_hex(time_store[3]);				// dd
	USART0_transmit(' ');
	USART0_transmit_hex(time_store[2]);				// hh
	USART0_transmit(':');
	USART0_transmit_hex(time_store[1]);				// mm
	USART0_transmit(':');
	USART0_transmit_hex(time_store[0]);				// ss
	
}
#endif
