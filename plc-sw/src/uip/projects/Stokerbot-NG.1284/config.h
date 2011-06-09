#ifndef SBDEF
  #define SBDEF
  #include <stdio.h>
  #include "config.h"
  #include "AVR035.h"

#define EEPROM_VERSION 2
#define SBNG_VERSION_MAJOR 3
#define SBNG_VERSION_MINOR 0

//REV 1 - Ingen ekstern eeprom, ingen onboard 2401 til egen report
//REV 2 - Ekstern eeprom til hjemmesider, 2401 onboard til egen rapportering
//REV 3 - Atmega1284P - Small/Medium/Large
#define SBNG_TARGET 3

//#define WEB_DEBUG
//#define HTTPD_DEBUG
//#define OW_DEBUG     //Enable printf debugs from onewire

//Currently defined in uip...
#define BUFFER_SIZE 1500

#define MAXSENSORS 100 //tempbuf der bruges i sensorScan kan maks holde 31sensore, buffer der bruges ved afseldelse max 35sensore
#define SENSORSIZE 11

//Sensor array
enum SensorData { FAMILY, ID1, ID2, ID3, ID4, ID5, ID6, CRC, VALUE1, VALUE2, SIGN };

uint8_t sensorValues[MAXSENSORS*SENSORSIZE];
uint8_t sensorScan[MAXSENSORS*8]; //Kan evt. pege på tempbuf som i R2
//char tempbuf[MAXSENSORS*8];  //atleast 250byte
char tempbuf[1024];  //atleast 250byte

//8ADC+4digital for at der er plads til de digitale hvis de er input
uint16_t simpleSensorValues[12];
uint8_t simpleSensorTypes[12];
uint8_t simpleSensorDebounce[12];

#define TYPE_DIGOUT 0
#define TYPE_ADC 1
#define TYPE_DIGIN 2
#define TYPE_COUNTER 3

#endif
