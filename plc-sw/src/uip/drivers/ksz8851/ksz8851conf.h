/*! \file ksz8851conf.h \brief Micrel KSZ8851 Ethernet Interface Driver Configuration. */
//*****************************************************************************
//
// File Name	: 'ksz8851conf.h'
// Title		: Micel KSZ8851 Ethernet Interface Driver Configuration
// Author		: Ag Primatic
// Created		: 1/19/2009
// Revised		: 1/19/2009
// Version		: 0.0
// Target MCU	: Atmel AVR series
// Editor Tabs	: 8
//
// Description	: This driver provides initialization and transmit/receive
//		functions for the KSZ8851 10/100 Ethernet Controller and PHY.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************


/* USERS NOTE:
 * Do not enter your hardware specific configurations here. Copy the settings
 * below and define them in your baord specific directory. Remember to also
 * define KSZ8851CONF_H so that these settings will not overwrite yours.
 * By modifying this in your board specific directory, you should be able to
 * update/upgrade your avr-uip distribution without having to modify updated
 * files.
 *
 *
 * DEVELOPERS NOTE:
 * Settings entered should be something rather common, and not update too often.
 */


#ifndef KSZ8851CONF_H
#define KSZ8851CONF_H

// KSZ8851 SPI port
#ifndef KSZ8851_SPI_PORT
#define KSZ8851_SPI_PORT	PORTB
#define KSZ8851_SPI_DDR		DDRB
#define KSZ8851_SPI_SCK		PB7
#define KSZ8851_SPI_MOSI	PB5
#define KSZ8851_SPI_MISO	PB6
#endif

// KSZ8851 CSN pin
#ifndef KSZ8851_CSN_PORT
#define KSZ8851_CSN_PORT	PORTB
#define KSZ8851_CSN_DDR		DDRB
#define KSZ8851_CSN_PIN		PB4
#endif

// KSZ8851 RESET pin
#ifdef USE_KSZ8851_HW_RESET
#define KSZ8851_RESET_PORT 	PORTD
#define KSZ8851_RESET_DDR 	DDRD
#define KSZ8851_RESET_PIN 	PD7
#endif

// MAC address for this interface
#ifdef ETHADDR0
#define KSZ8851_MAC0 ETHADDR0
#define KSZ8851_MAC1 ETHADDR1
#define KSZ8851_MAC2 ETHADDR2
#define KSZ8851_MAC3 ETHADDR3
#define KSZ8851_MAC4 ETHADDR4
#define KSZ8851_MAC5 ETHADDR5
#else
#define KSZ8851_MAC0 '0'
#define KSZ8851_MAC1 'F'
#define KSZ8851_MAC2 'F'
#define KSZ8851_MAC3 'I'
#define KSZ8851_MAC4 'C'
#define KSZ8851_MAC5 'E'
#endif

#endif
