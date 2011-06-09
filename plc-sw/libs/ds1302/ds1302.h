
#ifndef _DS1302_H_
#define _DS1302_H_

#include <avr/io.h>
// not in uip #include <config.h>

#ifndef RTC_CE
#error "RTC_CE not set"
#endif 

#ifndef RTC_SCK
#error "RTC_SCK not set"
#endif

#ifndef RTC_PORT
#error "RTC_PORT not set"
#endif

#ifndef RTC_DDR
#error "RTC_DDR not set"
#endif

#ifndef REGULATOR_OFF
#endif

#ifndef RTC_IO_PORT
#error "RTC_IO_PORT not set"
#endif

#ifndef RTC_IO_PIN 
#error "RTC_IO_PIN not set"
#endif

#ifndef RTC_IO_DDR 
#error "RTC_IO_DDR not set"
#endif

#ifndef RTC_IO
#error "RTC_IO not set"
#endif



/** \brief		Read a set of data from the DS1302 Real time clock to memory
 *	\param		command		Command to send to RTC
 *	\param		bytes			Number of bytes to be returned
 *	\param		*address_ptr    Pointer to location to store returned values
 *
 */
void read_from_rtc(uint8_t command,uint8_t bytes,uint8_t* address_ptr);



/** \brief		Write a set of data to the DS1302 Real time clock from memory
 *	\param		command		Command to send to RTC. See datasheet of the device for details
 *	\param		bytes		Number of bytes to be sent
 *	\param		*data_ptr   Pointer to location in memory of first byte of set of data
 *
 *				There is no checking that the data is sensible, or that bytes is the correct number for command
 *
 */
void write_to_rtc(uint8_t command,uint8_t bytes,uint8_t* data_ptr);



/** \brief		Set the clock on the RTC, sending it 8 bytes  ss mm hh date month day year 0
 *	\param		*time_ptr    Pointer to location where clock set values are stored
 *
 *				Clock values are 8 bytes:  ss,mm,hh,dd,mm,dow,yy,0;
 */
void send_time_to_rtc(uint8_t* time_ptr);




/** \brief		Read a set of data from the DS1302 Real time clock into memory 
 *	\param		*address_ptr   Pointer to location in memory of first byte of set of data
 *
 *
 */
void read_time(uint8_t* address_ptr);



/** \brief		Convert a BCD value in a byte to the correct hex value
 *	\param		bcd			uint8_t value in BCD
 *	\return					returns uint8_t hex value converted from BCD
 *
 *				For example, 23 -> 0x17
 *
 */
uint8_t bcd_to_hex(uint8_t bcd);



/** \brief		Read the time and convert to a 32 bit value suitable for FAT filesystem timestamps
 *	\return		uint32_t   		FAT time encoded in a 32 bit value
 *
 *				See FATFS documentation for the specification of FAT time stamp format
 *				time_store must consist of an array of at least 7 bytes
 *              A side effect of calling this function is to update the time stores in time_store
 *              Format of time_store is (in BCD) ss,mm,hh,date,month,dow,yy
 *
 */
//uint32_t get_fattime();


/** \brief		Create a 7 byte directory name with date/time value
 *	\param		*time_store 	memory location of 7 bytes of the time to be converted. 
 *	\param		*dirname 		directory name to be created.
 *
 *				Note that this function only converts. It doesn't update *time_store with the correct time
 *
 *				dirname is overwritten with  yymddhh    
 *				m is 1-12 value expressed in Hex (eg C for December).
 *				Other values (BCD from the clock) are copied as is, without any conversion.
 *				eg 20101215 13:16:56  ->  11C1513
 *
 *     Note that both time_store and dirname must exist in memory as arrays of 7 or more bytes.
 *	To use the dirname as a string, use 8 bytes and null the 8th
 *
 */
//void date_dirname(uint8_t *time_store,uint8_t *dirname);



/** \brief		Create or overwrite a filename with date/time value
 *	\param		*time_store 	memory location of 7 bytes of the time to be converted. 
 *	\param		*filename 		filename name to be created or overwritten.
 *
 *				Note that this function only converts. It doesn't update *time_store with the correct time
 *
 *		IMPORTANT		Several characters of filename are NOT changed or set. 
 *						Make sure you set it to a valid character or the filesystem will generate an error.
 *
 *		filename is overwritten with ^mddhhmm^ss^    where positions with ^ are unchanged
 *			m is month 1-12 value expressed as a single Hex character (eg C for December).
 *			Other values (BCD from the clock) are copied as is, without any conversion.
 *
 *			eg filename ABCDEFGH.XYZ   plus date (15th December 2010)  20101215 13:16:56  ->  AC151316.56Z
 *
 *     Note that both time_store and filename must exist in memory as arrays of 7 and 12 (or more) bytes respectively.
 *		To use the filename as a string, use a 13 byte array and null the 13th, or create filename as a string of the correct length.
 *
 */
// void filename_from_date(uint8_t *time_store,uint8_t *filename);



/** \brief		Converts a time stored in memory (typically from a Real Time Clock chip) into ISO order
 *	\param		*time_store 		memory location of 7 bytes of the time to be converted. 
 *	\param		*iso_time_store 	memory location for the 16 byte result. 
 *
 *				Note that this function only converts. It doesn't update *time_store with the correct time
 *
 *				time_store format is   ss,mm,hh,dd,mm,dow,yy     yy counts from 2000
 *				iso_time_store shows   "20yymmdd hhmmss"		 16 bytes including trailing null making it a string.
 */
void iso_time(uint8_t *time_store,uint8_t *iso_time_store);

//uint8_t hex2ascii_h(uint8_t hexval);
//uint8_t hex2ascii_l(uint8_t hexval);

#endif
