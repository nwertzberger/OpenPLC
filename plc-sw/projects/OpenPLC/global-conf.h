//Project specific configurations
#ifndef __GLOBAL_CONF_H__
#define __GLOBAL_CONF_H__

#define led_conf()      DDRD  |= 0xfc
#define led_on(X)       PORTD |= 1 << (X + 2)
#define led_off(X)      PORTD &= ~(1 << (X + 2))

//Define frequency
#define F_CPU 20000000UL

//Mac adress definition for enc28j60
#define ETHADDR0		0x00
#define ETHADDR1		0xbd
#define ETHADDR2		0x3b
#define ETHADDR3		0x33
#define ETHADDR4		0x05
#define ETHADDR5		0x71

//Mac adress definition for uip
#define UIP_ETHADDR0    ETHADDR0
#define UIP_ETHADDR1    ETHADDR1
#define UIP_ETHADDR2    ETHADDR2
#define UIP_ETHADDR3    ETHADDR3
#define UIP_ETHADDR4    ETHADDR4
#define UIP_ETHADDR5    ETHADDR5

#define USE_DHCP 0
#define UIP_IPADDR0 192
#define UIP_IPADDR1 168
#define UIP_IPADDR2 2
#define UIP_IPADDR3 77
#define UIP_NETMASK0 255
#define UIP_NETMASK1 255
#define UIP_NETMASK2 255
#define UIP_NETMASK3 0
#define UIP_DRIPADDR0 192
#define UIP_DRIPADDR1 168
#define UIP_DRIPADDR2 2
#define UIP_DRIPADDR3 1

// ENC28J60 SPI port
#define ENC28J60_SPI_PORT		PORTB
#define ENC28J60_SPI_DDR		DDRB
#define ENC28J60_SPI_SCK        PORTB7
#define ENC28J60_SPI_MOSI       PORTB5
#define ENC28J60_SPI_MISO       PORTB6
#define ENC28J60_SPI_SS         PORTB4
#define ENC28J60_CONTROL_CS     PORTB2

// ENC28J60 control port
#define ENC28J60_CONTROL_PORT	PORTB
#define ENC28J60_CONTROL_DDR	DDRB

//Include uip.h gives all the uip configurations in uip-conf.h
#include "uip.h"

#endif /*__GLOBAL_CONF_H__*/
