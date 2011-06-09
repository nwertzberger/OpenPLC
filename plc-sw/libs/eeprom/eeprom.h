#include <avr/eeprom.h> 
#include <stdint.h>

/*
ATMEGA32 has 1024bytes om eeprom.

0-100	Reserved for system use
	1	55 if eeprom contains settings, FF if new eeprom, anything else if foeing/corrupted eeprom

	see settings.htm for usage
*/
#ifndef sbeeprom
	#define sbeeprom

	extern void eepromSaveByte(uint16_t pos, uint8_t value);
	extern void eepromSaveWord(uint16_t pos, uint16_t value);
	extern uint8_t eepromReadByte(uint16_t pos);
	extern int8_t eepromReadByteSigned(uint16_t pos);
	extern uint16_t eepromReadWord(uint16_t pos);
	extern void eepromSaveStr(uint16_t pos, char* target, uint8_t limit);
	extern void eepromReadStr(uint16_t pos, char* target, uint8_t limit);

	#define eepromWriteByte eepromSaveByte
	#define eepromWriteWord eepromSaveWord
	#define eepromWriteStr  eepromSaveStr
#endif
