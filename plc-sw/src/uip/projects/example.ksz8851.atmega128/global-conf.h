//Project specific configurations
#ifndef __GLOBAL_CONF_H__
#define __GLOBAL_CONF_H__


#ifdef PORTB1
//Led on tuxgraphics board
#define led_conf()      DDRB |= (1<<DDB1)
#define led_low()       PORTB |= (1<<PORTB1)
#define led_high()      PORTB &= ~(1<<PORTB1)
#define led_blink()     PORTB ^= (1<<PORTB1)
#else
//Led on tuxgraphics board
#define led_conf()      DDRB |= (1<<DDB1)
#define led_low()       PORTB |= (1<<PB1)
#define led_high()      PORTB &= ~(1<<PB1)
#define led_blink()     PORTB ^= (1<<PB1)
#endif


//Define frequency
#define F_CPU 12500000UL
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


// KSZ8851 SPI port
#define KSZ8851_SPI_PORT	PORTB
#define KSZ8851_SPI_DDR		DDRB
#define KSZ8851_SPI_SCK		PB1
#define KSZ8851_SPI_MOSI	PB2
#define KSZ8851_SPI_MISO	PB3

// KSZ8851 CSN pin
#define KSZ8851_CSN_PORT	PORTB
#define KSZ8851_CSN_DDR		DDRB
#define KSZ8851_CSN_PIN		PB0

// KSZ8851 RESET pin
#ifdef USE_KSZ8851_HW_RESET
#error "not setup yep"
#define KSZ8851_RESET_PORT 	PORTD
#define KSZ8851_RESET_DDR 	DDRD
#define KSZ8851_RESET_PIN 	NOTUSED
#endif


//Include uip.h gives all the uip configurations in uip-conf.h
#include "uip.h"

#endif /*__GLOBAL_CONF_H__*/
