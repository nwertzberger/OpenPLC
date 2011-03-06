/* Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * All work Copyright 2011 University of Nebraska, Omaha
 *
 * The following lays out the pins used on the OpenPLC board.
 */
#ifndef OPENPLC_IO_H
#define OPENPLC_IO_H

#include <types.h>

/*
 * Define Physical IO
 */

#define PHYS_IN_0 PORTA.AN0
#define PHYS_IN_1 PORTA.AN1
#define PHYS_IN_2 PORTA.AN2
#define PHYS_IN_3 PORTA.AN3
#define PHYS_IN_4 PORTA.AN4
#define PHYS_IN_5 PORTE.AN5
#define PHYS_IN_6 PORTE.AN6
#define PHYS_IN_7 PORTE.AN7

#define ANALOG_OUT_0    PORTD.RD0 
#define ANALOG_OUT_1    PORTD.RD1
#define ANALOG_OUT_2    PORTD.RD2
#define ANALOG_OUT_3    PORTD.RD3

#define DIGITAL_OUT_0   PORTD.RD4
#define DIGITAL_OUT_1   PORTD.RD5
#define DIGITAL_OUT_2   PORTD.RD6
#define DIGITAL_OUT_3   PORTD.RD7

/*
 * Define USB ports.
 */

/*
 * Define Ethernet ports.
 */

/*
 * Sets the output voltage on analog ports 0 through 3.
 * Voltage = 24V * voltage/255
 */
void set_analog_voltage(uint8_t port, uint8_t voltage);

/*
 * Sets the voltages for digital ports 0 through 3.
 * Voltage = 24V if voltage > 0, otherwise Voltage = 0.
 */
void set_digital_voltage(uint8_t port, uint8_t voltage);

#endif
