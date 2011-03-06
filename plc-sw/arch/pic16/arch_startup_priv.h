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
 *                          1       1       0       1       1       0
 * Internal USB regulator enabled
 * v_{brownout} = 2.79
 * Brownout reset in hardware only
 * No fixed powerup time
 */
__code char __at __CONFIG2L conf2l = 0x36;

/***************************************************************************
 * CONFIG2H:-       -       -       WDTPS3  WDTPS2  WDTPS1  WDTPS0  WDTEN
 *                                  1       1       1       1       1
 * Disable watchdong timer
 * Set for maximum postscale: 1:32,768
 */
__code char __at __CONFIG2H conf2h = 0x1e;

/***************************************************************************
 * CONFIG4L:_DEBUG  XINST   ICPRT   -       -       LVP     -       STVREN
 *          1       0       0                       0               0
 * Disable background debugger
 * Disable extended instructions (Due to SDCC, NOT BECAUSE I WANT TO)
 * Disable dedicated programming port
 * Disable single-supply ICSP
 * Disable reset on stack overflow/underflow (for now)
 */
__code char __at __CONFIG4L conf4l = 0x80;

#endif

