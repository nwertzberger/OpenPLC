

/*
 *
 * Downloaded from public site: http://www.cl.cam.ac.uk/teaching/1011/P31/lib/
 * Date: April 9, 2010
 * Author: unknown.  Contacted Instructor of class with no answer.
 *
 */

#include "global-conf.h"
#include "ds1302.h"
#include <util/delay_basic.h>
#include <util/delay.h>

/* support for DS1302 real time clock. Uses a bidirectional IO port and a clock pin.

NOTE. For compatibility with SD cards, it has been becessary to put the IO on one part and and the CE and SCK on another

requires definitions of ATMEL ports:

The IO pin can be (and is on the project PCB) on a different port to the control lines.

	RTC_IO_DDR    		the port that RTC IO line is on
	RTC_IO_PIN
	RTC_IO_PORT		    assumes here that we send the clock to the RTC using the SPICLK
	RTC_IO


	RTC_DDR    		the port that RTC is on
	RTC_PORT		    assumes here that we send the clock to the RTC using the SPICLK
	RTC_CE			chip select, active High
	RTC_SCK
	


At power up, define RTC_CE and RTC_SCK as outputs, RTC_IO as an input, and set RTC_CE inactive (=low)



Uses _delay_us() function from <util/delay_basic.h> and <util/delay.h>

*/



static void rtc_clock_up_down(void) {
	RTC_PORT |= (1<<RTC_SCK);
	_delay_us(2);
	RTC_PORT &= ~(1<<RTC_SCK);
}



void read_from_rtc(uint8_t command,uint8_t bytes,uint8_t* address_ptr) {

// NB strange timing on SCK - 8 bits of command and 8 of read data in 15 clocks 

	uint8_t result = 0;
	uint8_t i;
	uint8_t j;
	uint8_t bitvalue;

// need to revisit ----
	RTC_DDR = (1<<RTC_SCK) | (1<<RTC_CE);	

	RTC_PORT &= ~(1<<RTC_SCK);
	RTC_IO_DDR |= 1<<RTC_IO;					// set io pin for output 
	RTC_IO_PORT &= ~(1<<RTC_IO);				// io pin low 
	RTC_PORT |= 1<<RTC_CE;					// set ce high 
	_delay_us(4);

	// rtc uses least sig bit first 
	// send the command 
	for (i=0;i<8;i++) {
		RTC_PORT &= ~(1<<RTC_SCK);
		bitvalue = ((command >> i) & 0x01);
		if (bitvalue) {
			RTC_IO_PORT |= 1<<RTC_IO;
		} else {
			RTC_IO_PORT &= ~(1<<RTC_IO);
		}
		_delay_us(1);
		RTC_PORT |= (1<<RTC_SCK);
		_delay_us(1);
	}

	// exit loop with clock high 

	RTC_IO_DDR &= ~(1<<RTC_IO);				// set io pin for input 
	RTC_IO_PORT &= ~(1<<RTC_IO);				// and turn off pull up 
	
	for (j=0;j<bytes;j++) {
		result = 0;
		for (i=0;i<8;i++) {
			RTC_PORT |= (1<<RTC_SCK);
		_delay_us(1);
			RTC_PORT &= ~(1<<RTC_SCK);
		_delay_us(1);
			bitvalue = ((RTC_IO_PIN >> RTC_IO) & 0x01);
			result |= (bitvalue << i);
		}	
		address_ptr[j] = result;
	}	

	RTC_PORT &= ~(1<<RTC_CE);				// turn off ce 
	RTC_IO_DDR |= 1<<RTC_IO;					// set io pin for output 
	RTC_IO_PORT &= ~(1<<RTC_IO);				// io pin low 
	_delay_us(4);
	// io pin is an output on leaving 
}



void write_to_rtc(uint8_t command,uint8_t bytes,uint8_t* data_ptr) {

// parameters:
// bytes is number of bytes to be transferred 

	uint8_t i;
	uint8_t j;
	uint8_t bitvalue;

// need to revisit ----
	RTC_DDR = (1<<RTC_SCK) | (1<<RTC_CE);	

	RTC_PORT &= ~(1<<RTC_SCK);
	RTC_IO_DDR |= 1<<RTC_IO;					// io pin output 
	RTC_IO_PORT &= ~(1<<RTC_IO);				// set io low 
	RTC_PORT |= 1<<RTC_CE;					// assert ce 
	_delay_us(3);

	for (i=0;i<8;++i) {
		bitvalue = ((command >> i) & 0x01);
		if (bitvalue) {
			RTC_IO_PORT |= 1<<RTC_IO;
		} else {
			RTC_IO_PORT &= ~(1<<RTC_IO);
		}
		_delay_us(1);
		rtc_clock_up_down();
	}
	for (j=0;j<bytes;++j) {	
		for (i=0;i<8;++i) {
			bitvalue = ((data_ptr[j] >> i) & 0x01);
			if (bitvalue) {
				RTC_IO_PORT |= 1<<RTC_IO;
			} else {
				RTC_IO_PORT &= ~(1<<RTC_IO);
			}
			_delay_us(1);
			rtc_clock_up_down();
		}
	}

	_delay_us(1);

	RTC_PORT &= ~(1<<RTC_CE);				// deassert ce 
	_delay_us(4);
}




void read_time(uint8_t* address_ptr) {

	read_from_rtc(0xBF,7,address_ptr);
}



void send_time_to_rtc(uint8_t* time_ptr) {
// POINTER TO set of time data 
// data format is:  ss mm hh date month day year 0

	uint8_t dummy_ptr = 0 ;
	write_to_rtc(0x8E,1,&dummy_ptr);
	write_to_rtc(0xBE,8,time_ptr);

}



uint8_t bcd_to_hex(uint8_t bcd) {
	
	uint8_t msn = (bcd >> 4);
	return ( (msn * 10) + (bcd & 0x0F) ) ;
}


/*
uint32_t get_fattime() {
	
	uint8_t time_store[8];

	uint32_t fattime ;
	uint16_t time ;
	uint16_t date ;
	
	read_time(time_store);
	time_store[0] &= 0x7F ;				// top bit is set on the seconds reading 	

	time = bcd_to_hex(time_store[0]/2) ;
	uint8_t min = bcd_to_hex(time_store[1]) ;
	uint8_t hour = bcd_to_hex(time_store[2]) ;
	date = bcd_to_hex(time_store[3]) ;
	uint8_t month = bcd_to_hex(time_store[4]) ;
	uint8_t year = bcd_to_hex(time_store[6])+20 ;
	
	time |= (min << 5) ;
	time |= (hour << 11) ;
	date |= (month << 5) ;
	date |= (year << 9) ;
	
	fattime = time | ((uint32_t)date<<16);
	
	return fattime;
}


void date_dirname(uint8_t *time_store,uint8_t *dirname) {

	dirname[1] = hex2ascii_h(time_store[6]) ;				// last 2 digits of year
	dirname[2] = hex2ascii_l(time_store[6]) ;				// last 2 digits of year
	dirname[3] = hex2ascii_l(bcd_to_hex(time_store[4]));	// month as horrible 0x01 to 0x0C value
	dirname[4] = hex2ascii_h(time_store[3]) ;				// day
	dirname[5] = hex2ascii_l(time_store[3]) ;				// day
	dirname[6] = hex2ascii_h(time_store[2]) ;				// hour
	dirname[7] = hex2ascii_l(time_store[2]) ;				// hour
}


void filename_from_date(uint8_t *time_store,uint8_t *filename) {
	
	filename[1] = hex2ascii_l(bcd_to_hex(time_store[4]));// month as horrible 0x01 to 0x0C value
	filename[2] = (time_store[3] >> 4) + '0';			// 10s day
	filename[3] = (time_store[3] & 0x0F) + '0';			// day
	filename[4] = (time_store[2] >> 4) + '0';			// 10s hour
	filename[5] = (time_store[2] & 0x0F) + '0';			// hour
	filename[6] = (time_store[1] >> 4) + '0';			// 10s min
	filename[7] = (time_store[1] & 0x0F) + '0';			// mins
	filename[10] = (time_store[0] >> 4) + '0';			// 10s sec
	filename[11] = (time_store[0] & 0x0F) + '0';			// sec

}
*/
void iso_time(uint8_t *time_store,uint8_t *iso_time_store) {

	iso_time_store[0] = '2';
	iso_time_store[1] = '0';
	iso_time_store[2] = (time_store[6] >> 4) + '0';
	iso_time_store[3] = (time_store[6] & 0x0F) + '0';
	iso_time_store[4] = (time_store[4] >> 4) + '0';
	iso_time_store[5] = (time_store[4] & 0x0F) + '0';
	iso_time_store[6] = (time_store[3] >> 4) + '0';
	iso_time_store[7] = (time_store[3] & 0x0F) + '0';
	iso_time_store[8] = ' ';
	iso_time_store[9] = (time_store[2] >> 4) + '0';
	iso_time_store[10] = (time_store[2] & 0x0F) + '0';
	iso_time_store[11] = (time_store[1] >> 4) + '0';
	iso_time_store[12] = (time_store[1] & 0x0F) + '0';
	iso_time_store[13] = (time_store[0] >> 4) + '0';
	iso_time_store[14] = (time_store[0] & 0x0F) + '0';
	iso_time_store[15] = 0;	
}
/*
// Take a byte (hexval) and return the ASCII character for the highest 4 bits.
uint8_t hex2ascii_h(uint8_t hexval) {
	uint8_t tmp;
	tmp = (hexval >> 4) + '0' ;
	if (tmp > 0x39) {
		tmp += 7;
	}
	return tmp;
}


// Take a byte (hexval) and return the ASCII character for the lowest 4 bits.
uint8_t hex2ascii_l(uint8_t hexval) {
	uint8_t tmp;
	tmp = (hexval & 0x0F) + '0' ;
	if (tmp > 0x39) {
		tmp += 7;
	}
	return tmp;
}
*/
