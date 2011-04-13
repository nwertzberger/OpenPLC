/* Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * All work Copyright 2010 University of Nebraska, Omaha.
 *
 * This header is responsible for initializing the configuration registers and
 * setting up the proper pragmas for normal microprocessor usage.
 */
#ifndef ARCH_INIT_PRIV_H
#define ARCH_INIT_PRIV_H

/***************************************************************************
 * Compiler notification about stack location and size.
 * TODO: Look into this, because I am confused.
 */
#pragma stack 0x200 0xff

/***************************************************************************
 * CONFIG1L:-       -       USBDIV  CPUDIV1 CPUDIV0 PLLDIV2 PLLDIV1 PLLDIV0
 *                          1       0       0       1       0       1
 * F(USB) = PLL/2 = 48MHz
 * CPU = PLL/2 = 48 MHz
 * PLLDIV = 6 (24MHz src)
 */
__code char __at __CONFIG1L conf1l = 0x25;

/***************************************************************************
 * CONFIG1H:IESO    FCMEN   -       -       FOSC3   FOSC2   FOSC1   FOSC0
 *          0       0                       1       1       1       1
 * FOSC = HS oscillator
 * PLL enabled (HSPLL)
 */
__code char __at __CONFIG1H conf1h = 0x0f;

/***************************************************************************
 * CONFIG2L:-       -       VREGEN  BORV1   BORV0   BOREN1  BOREN0  _PWRTEN
 *                          1       1       1       1       1       1
 * Internal USB regulator enabled
 * v_{brownout} = MINIMUM
 * Brownout reset in hardware only
 * No fixed powerup time
 */
__code char __at __CONFIG2L conf2l = 0x3f;

/***************************************************************************
 * CONFIG2H:-       -       -       WDTPS3  WDTPS2  WDTPS1  WDTPS0  WDTEN
 *                                  1       1       1       1       0
 * Disable watchdong timer
 * Set for maximum postscale: 1:32,768
 */
__code char __at __CONFIG2H conf2h = 0x1e;


/***************************************************************************
 * CONFIG3H:MCLRE   -       -       -       -       LPT1OSC PBADEN  CCP2MX
 *          1                                       0       0
 *
 *
 */
__code char __at __CONFIG3H conf3h = 0x80;

/***************************************************************************
 * CONFIG4L:_DEBUG  XINST   ICPRT   -       -       LVP     -       STVREN
 *          1       0       0                       0               1
 * Disable background debugger
 * Disable extended instructions (Due to SDCC, NOT BECAUSE I WANT TO)
 * Disable dedicated programming port
 * Disable single-supply ICSP
 * Enable reset on stack overflow/underflow (for now)
 */
__code char __at __CONFIG4L conf4l = 0x81;

/***************************************************************************
 * CONFIG5L:-       -       -       -       CP3     CP2     CP1     CP0
 *                                          1       1       1       1
 * Turn off any code protection
 */
__code char __at __CONFIG5L conf5l = 0x0f;

/***************************************************************************
 * CONFIG5H:CPD     CPB     -       -       -       -       -       -
 *          1       1       
 * Turn off more code protection for Data and Boot Block
 */
__code char __at __CONFIG5H conf5h = 0xc0

/***************************************************************************
 * CONFIG6L:-       -       -       -       WRT3    WRT2    WRT1    WRT0
 *                                          1       1       1       1
 * Turn off write protection for everything.
 */
__code char __at __CONFIG6L conf6l = 0x0f;

/***************************************************************************
 * CONFIG6H:WRTD    WRTB    WRTC    -       -       -       -       -
 *          1       1       1
 * 
 * Turn off write protection for Data, eeprom, and config registers.
 */
__code char __at __CONFIG6H conf6h  = 0xe0;


/***************************************************************************
 * CONFIG7L:-       -       -       -       EBTR3   EBTR2   EBTR1   EBTR0
 *                                          1       1       1       1
 * Turn off table read protection from other blocks.
 */
__code char __at __CONFIG7L conf7l = 0x0f;

/***************************************************************************
 * CONFIG7H:-       EBTRB   -       -       -       -       -       -
 *                  1
 * Turn off table read protection for the boot block.
 */
__code char __at __CONFIG7H conf7h = 0x40;

#endif

