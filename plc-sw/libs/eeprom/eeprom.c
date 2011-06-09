#include "eeprom.h"
#include <string.h>

//	eepromSaveByte(10, 127);
//	uint8_t res = eepromReadByte(10);
// Se eeprom.h for hvilke pos der er til hvad

void eepromSaveByte(uint16_t pos, uint8_t value)
{
	if (eepromReadByte(pos) != value)
	{
		eeprom_write_byte((uint8_t*)pos, value);
	}
}

void eepromSaveWord(uint16_t pos, uint16_t value)
{
	if (eepromReadWord(pos) != value)
	{
		eeprom_write_word((uint16_t*)pos, value);
	}
}

int8_t eepromReadByteSigned(uint16_t pos)
{
	return eeprom_read_byte((uint8_t*)pos);
}

uint8_t eepromReadByte(uint16_t pos)
{
	return eeprom_read_byte((uint8_t*)pos);
}

uint16_t eepromReadWord(uint16_t pos)
{
	return eeprom_read_word((uint16_t*)pos);
}

void eepromSaveStr(uint16_t pos, char* target, uint8_t limit)
{
	for (uint8_t i=0; i<limit; i++)
	{
		eepromWriteByte(pos+i, target[i]);
	}	
}

void eepromReadStr(uint16_t pos, char* target, uint8_t limit)
{
	memset(target, '\0', limit);
	for (uint8_t i=0; i<limit; i++)
	{
		target[i] = eepromReadByte(pos+i);
	}
}
