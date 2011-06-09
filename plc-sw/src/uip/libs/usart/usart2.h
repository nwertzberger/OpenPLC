
#ifndef __USART2_H__
#define __USART2_H__

#include <avr/io.h>



/**
 * \brief Initialise USART0 and set baud rate.
 *
 * \param baudval    Value to put in UBRR0 to selectbaud rate.
 *
 *             Initialise USART0 on microcontrollers where there
 *				are two USARTs which use the newer register and
 *				bit position names which include a USART number
 *
 *			   The baudval parameter will depend on the CPU clock frequency
 *				see the tables in the microcontroller documentation for suitable values
 *
 *			   Clears any existing transmissions, turns off double speed, enables transmitter
 *				and receiver.
 *
 *			   Note that this initialisation DOESN'T enable the interrupts Rx complete and Tx Complete
 */
//inline void USART0_init(uint16_t baudval);
void USART0_init(uint16_t baudval);


/**
 * \brief Initialise USART1 and set baud rate.
 *
 * \param baudval    Value to put in UBRR1 to selectbaud rate.
 *
 *             Initialise USART1 on microcontrollers where there
 *				are two USARTs which use the newer register and
 *				bit position names which include a USART number
 *
 *			   The baudval parameter will depend on the CPU clock frequency
 *				see the tables in the microcontroller documentation for suitable values
 *
 *			   Clears any existing transmissions, turns off double speed, enables transmitter
 *				and receiver.
 *
 *			   Note that this initialisation DOESN'T enable the interrupts Rx complete and Tx Complete
 */
//inline void USART1_init(uint16_t baudval);
void USART1_init(uint16_t baudval);


/**
 * \brief	Transmit a string via USART0 at currently set baud rate.
 *
 * \param	data	Pointer to start of string.
 *
 *             Transmit a string via USART0, using polling, not interrutps. 
 *				Doesn't change baud rate.
 *				Doesn't interrupt any existing transmit already in progress.
 *			   Uses polling, so waits for transmit buffer to be empty
 *
 */
void USART0_transmit_string(const char* data);


/**
 * \brief	Transmit a string via USART1 at currently set baud rate.
 *
 * \param data    Pointer to start of string.
 *
 *             Transmit a string via USART1, using polling, not interrutps. 
 *				Doesn't change baud rate.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty
 *
 */
void USART1_transmit_string(const char* data);



/**
 * \brief      Transmit a character via USART0 at currently set baud rate.
 *
 * \param data    Character to transmit.
 *
 *             Transmit a character via USART0, using polling, not interrutps. 
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty
 */
void USART0_transmit( unsigned char data );


/**
 * \brief      Transmit a character via USART1 at currently set baud rate.
 *
 * \param data    Character to transmit.
 *
 *             Transmit a character via USART1, using polling, not interrutps. 
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty
 */
void USART1_transmit( unsigned char data );

/**
 * \brief      Receive an 8 bit value from USART0 by polling.
 *
 * \return  value    Value received.
 *
 *			   Uses polling, so waits for receive buffer to fill before returning.
 *
 */
uint8_t USART0_receive( void );


/**
 * \brief      Receive an 8 bit value from USART1 by polling.
 *
 * \return  value    Value received.
 *
 *			   Uses polling, so waits for receive buffer to fill before returning.
 *
 */
uint8_t USART1_receive( void );


/**
 * \brief      Repeatedly receive and discard values received by the USART0 until the input buffer empties.
 *
 *			   Uses polling.
 *
 *			   Note that in the event of continuous input, this function may never complete.
 *
 */
void USART0_flush(void);



/**
 * \brief      Repeatedly receive and discard values received by the USART1 until the input buffer empties.
 *
 *			   Uses polling.
 *
 *			   Note that in the event of continuous input, this function may never complete.
 *
 */
void USART1_flush(void);


#ifdef USART_EXTRA_SEND_FORMATS
/**
 * \brief      Transmit an 8 bit unsigned integer via USART0 at currently set baud rate.
 *
 * \param val    Character to transmit.
 *
 *             Transmit an 8 bit unsigned integer via USART0, as a series of up to 
 *				three decimal characters, using polling, not interrutps. Doesn't output
 *				leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 */
void USART0_transmit_uint8(uint8_t val);


/**
 * \brief      Transmit an 8 bit unsigned integer via USART1 at currently set baud rate.
 *
 * \param val    Character to transmit.
 *
 *             Transmit an 8 bit unsigned integer via USART1, as a series of up to 
 *				three decimal characters, using polling, not interrutps. Doesn't output
 *				leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 */
void USART1_transmit_uint8(uint8_t val);


/**
 * \brief      Transmit an 8 bit signed integer via USART0 at currently set baud rate.
 *
 * \param val    Value to transmit.
 *
 *             Transmit an 8 bit signed integer via USART0, as an optional minus sign followed by 
 *				up to three decimal characters, using polling, not interrutps. 
 *				Doesn't output leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART0_transmit_int8(int8_t val);


/**
 * \brief      Transmit an 8 bit signed integer via USART1 at currently set baud rate.
 *
 * \param val    Value to transmit.
 *
 *             Transmit an 8 bit signed integer via USART1, as an optional minus sign followed by 
 *				up to three decimal characters, using polling, not interrutps. 
 *				Doesn't output leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART1_transmit_int8(int8_t val);




/**
 * \brief      Transmit a 16 bit unsigned integer via USART0 at currently set baud rate.
 *
 * \param val    Value to transmit.
 *
 *             Transmit a 16 bit unsigned integer via USART0, as a series of up to 
 *				five decimal characters, using polling, not interrutps. Doesn't output
 *				leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 */
void USART0_transmit_uint16(uint16_t val);


/**
 * \brief      Transmit a 16 bit unsigned integer via USART1 at currently set baud rate.
 *
 * \param val    Value to transmit.
 *
 *             Transmit a 16 bit unsigned integer via USART1, as a series of up to 
 *				five decimal characters, using polling, not interrutps. Doesn't output
 *				leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 */
void USART1_transmit_uint16(uint16_t val);


/**
 * \brief      Transmit a 16 bit signed integer via USART0 at currently set baud rate.
 *
 * \param val    value to transmit.
 *
 *             Transmit a 16 bit signed integer via the USART, as an optional minus sign followed by 
 *				up to five decimal characters, using polling, not interrutps. 
 *				Doesn't output leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART0_transmit_int16(int16_t val);


/**
 * \brief      Transmit a 16 bit signed integer via USART1 at currently set baud rate.
 *
 * \param val    value to transmit.
 *
 *             Transmit a 16 bit signed integer via the USART, as an optional minus sign followed by 
 *				up to five decimal characters, using polling, not interrutps. 
 *				Doesn't output leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART1_transmit_int16(int16_t val);


/**
 * \brief      Interpret an 8 bit unsigned integer as a BCD value and transmit it as two ASCII characters via USART0.
 *
 * \param val    Value to transmit.
 *
 *             Interpret an 8 bit unsigned integer as a BCD value and transmit it via the USART as exactly 2 characters.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART0_transmit_bcd(uint8_t val);


/**
 * \brief      Interpret an 8 bit unsigned integer as a BCD value and transmit it as two ASCII characters via USART1.
 *
 * \param val    Value to transmit.
 *
 *             Interpret an 8 bit unsigned integer as a BCD value and transmit it via the USART as exactly 2 characters.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART1_transmit_bcd(uint8_t val);


/**
 * \brief      Transmit an 8 bit unsigned integer as eight ASCII characters 0 or 1 via USART0.
 *
 * \param val    Value to transmit.
 *
 *			   Transmit an 8 bit unsigned integer as eight ASCII characters 0 or 1.
 *
 *			   Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART0_transmit_bin(uint8_t val);


/**
 * \brief      Transmit an 8 bit unsigned integer as eight ASCII characters 0 or 1 via USART1.
 *
 * \param val    Value to transmit.
 *
 *			   Transmit an 8 bit unsigned integer as eight ASCII characters 0 or 1.
 *
 *			   Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART1_transmit_bin(uint8_t val);


/**
 * \brief      Transmit the lowest 4 bits of an 8 bit unsigned integer as a single ASCII character 0-9 or A-F on USART0
 *
 * \param nibble    Value to transmit.
 *
 *			   Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART0_transmit_nibble(uint8_t nibble);


/**
 * \brief      Transmit the lowest 4 bits of an 8 bit unsigned integer as a single ASCII character 0-9 or A-F on USART1
 *
 * \param nibble    Value to transmit.
 *
 *			   Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART1_transmit_nibble(uint8_t nibble);




/**
 * \brief      Output a time value in ISO format to USART0, from data stored in memory.
 *
 * \param time_store    Pointer to 7 BCD values representing in order seconds,minutes,hours,day,month,day of week(ignored),year.
 *
 *			   Takes a set of values representing a time (typically in the order received from a RTC chip) and
 *				outputs them via the USART in an ISO like format:
 *
 *			   15:23:00 on 3rd April 2010 would be  "2010-04-03	15:23:00"
 *
 *			   Uses polling. Calls USART_transmit and USART_transmit_hex
 *
 *
 */
void usart0_out_iso_time(uint8_t *time_store);


/**
 * \brief      Output a time value in ISO format to USART1, from data stored in memory.
 *
 * \param time_store    Pointer to 7 BCD values representing in order seconds,minutes,hours,day,month,day of week(ignored),year.
 *
 *			   Takes a set of values representing a time (typically in the order received from a RTC chip) and
 *				outputs them via the USART in an ISO like format:
 *
 *			   15:23:00 on 3rd April 2010 would be  "2010-04-03	15:23:00"
 *
 *			   Uses polling. Calls USART_transmit and USART_transmit_hex
 *
 *
 */
void usart1_out_iso_time(uint8_t *time_store);

#endif // end - #ifdef USART_EXTRA_SEND_FORMATS
#endif // end - #ifndef __USART2_H__
