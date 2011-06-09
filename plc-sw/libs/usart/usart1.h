#ifndef __USART1_H__
#define __USART1_H__


/**
 * \brief Initialise USART and set baud rate.
 *
 * \param baudval    Value to put in UBRR0 to selectbaud rate.
 *
 *             Initialise USART on newer microcontrollers which use the newer 
 *				register and bit position names which include a USART number
 *
 *			   The baudval parameter will depend on the CPU clock frequency
 *				see the tables in the microcontroller documentation for suitable values
 *
 *			   Clears any existing transmissions, turns off double speed, enables transmitter
 *				and receiver.
 *
 *			   Note that this initialisation DOESN'T enable the interrupts Rx complete and Tx Complete
 */
void USART_init(uint16_t baudval);
//inline void USART_init(uint16_t baudval);


/**
 * \brief	Transmit a string via USART at currently set baud rate.
 *
 * \param data    Pointer to start of string.
 *
 *             Transmit a string via the USART, using polling, not interrutps. 
 *				Doesn't change baud rate.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty
 *
 */
void USART_transmit_string(const char* data);
//inline void USART_transmit_string(const unsigned char* data);

// define a alias
#define sendString(astring) USART_transmit_string(astring)

/**
 * \brief      Transmit a character via USART at currently set baud rate.
 *
 * \param data    Character to transmit.
 *
 *             Transmit a character via the USART, using polling, not interrutps. 
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty
 */
void USART_transmit(unsigned char data );

/**
 * \brief      Receive an 8 bit value from the USART by polling.
 *
 * \return  value    Value received.
 *
 *			   Uses polling, so waits for receive buffer to fill before returning.
 *
 */
uint8_t USART_receive( void );



/**
 * \brief      Repeatedly receive and discard values received by the USART until the input buffer empties.
 *
 *			   Uses polling.
 *
 *			   Note that in the event of continuous input, this function may never complete.
 *
 */
 void USART_flush(void);


#ifdef USART_EXTRA_SEND_FORMATS
/**
 * \brief      Transmit an 8 bit unsigned integer via USART at currently set baud rate.
 *
 * \param val    Character to transmit.
 *
 *             Transmit an 8 bit unsigned integer via the USART, as a series of up to 
 *				three decimal characters, using polling, not interrutps. Doesn't output
 *				leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 */
void USART_transmit_uint8(uint8_t val);


/**
 * \brief      Transmit an 8 bit signed integer via USART at currently set baud rate.
 *
 * \param val    Value to transmit.
 *
 *             Transmit an 8 bit signed integer via the USART, as an optional minus sign followed by 
 *				up to three decimal characters, using polling, not interrutps. 
 *				Doesn't output leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART_transmit_int8(int8_t val);


/**
 * \brief      Transmit a 16 bit unsigned integer via USART at currently set baud rate.
 *
 * \param val    Value to transmit.
 *
 *             Transmit a 16 bit unsigned integer via the USART, as a series of up to 
 *				five decimal characters, using polling, not interrutps. Doesn't output
 *				leading zeroes, so number of characters transmitted may vary.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 */
 void USART_transmit_uint16(uint16_t val);


/**
 * \brief      Transmit a 16 bit signed integer via USART at currently set baud rate.
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
void USART_transmit_int16(int16_t val);


/**
 * \brief      Interpret an 8 bit unsigned integer as a BCD value and transmit it as two ASCII characters.
 *
 * \param val    Value to transmit.
 *
 *             Interpret an 8 bit unsigned integer as a BCD value and transmit it via the USART as exactly 2 characters.
 *				Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART_transmit_bcd(uint8_t val);



/**
 * \brief      Transmit an 8 bit unsigned integer as eight ASCII characters 0 or 1.
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
void USART_transmit_bin(uint8_t val);


/**
 * \brief      Transmit the lowest 4 bits of an 8 bit unsigned integer as a single ASCII character 0-9 or A-F.
 *
 * \param nibble    Value to transmit.
 *
 *			   Doesn't interrupt any existing transmit already in progress.
 *				
 *			   Uses polling, so waits for transmit buffer to be empty before appending new characters
 *
 */
void USART_transmit_nibble(uint8_t nibble);




/**
 * \brief      Output a time value in ISO format to USART, from data stored in memory.
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
 void usart_out_iso_time(uint8_t *time_store);

#endif // end - #ifdef USART_EXTRA_SEND_FORMATS

#endif // end - #ifndef __USART1_H__
