/* Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * The following code is used to control the hardware inputs and outputs on
 * the board.
 */

#include <stdint.h>
#include <pic18fregs.h>

#include "arch_io.h"
#include "core_debug.h"

#define NUM_PORTS   8

/*
 * Stores the appropriate voltages for output.  these are used in PWM.
 */
static uint16_t output_voltages[8];

/* Initializes inputs and outputs by setting them to either input or output
 * and setting all output voltages to 0.
 */
void arch_io_init()
{
    uint8_t i;

    /* Set up the analog input...       */
    TRISA = 0xff;   /* Ports A0-A3, A5  */
    TRISE = 0xff;   /* Ports E0-E2      */

    /* ADCON1:  -       -       VCFG1   VCFG0   PCFG3   PCFG2   PCFG1   PCFG0
     *                          0       0       0       1       1       1
     * Vref- = Vss
     * Vref+ = Vdd
     * A/D Ports = AD0 - AD7 (A0-A3, A5, E0-E2)
     */
    ADCON1 = 0x07;

    /* ADCON2:  ADFM    -       ACQT2   ACQT1   ACQT0   ADCS2   ADCS1   ADCS0
     *          1               0       1       0       1       1       0
     * Right justified A/D value (0-s on top).
     * Aquisition time = 4 TAD (TODO: see if this is enough. Apparently max
     *      impedance for accurate A/D is 2.5k, we are WAY over that.)
     * A/D conversion clock = Fosc/64 (OSC = 48 MHz).
     */
    ADCON2 = 0x96;

    /* Set up the output...         */
    TRISD = 0x00;   /* Ports D0-D7  */
    for(i=0; i<NUM_PORTS; i++)
    {
        arch_io_set_vout(i, 0);
    }

}

/* Returns the voltage (in millivolts) for the given port
 * (0-7, 0 is farthest left)
 * 0xffff = error.
 */
uint16_t arch_io_get_vin(uint8_t port)
{
    uint16_t voltage;

    /* List Assumptions...  */
    ASSERT(port < 8);

    ADCON0 = (port << 2) | 0x03;
    /* Busy wait for A/D conversion...
     * TODO: Update to protothread or other non-blocking choice.
     */
    while (ADCON0 & 0x02);
    voltage = (ADRESH << 8) | ADRESL;

    /* convert the voltage
     * 24000 mV = 1023 * 23.46 ~= 1023 * 24 = 1023 * 16 + 1023 * 8
     */
    voltage = (voltage << 4) | (voltage << 3);
    return voltage;
}

/* Sets the output voltage (in millivolts) for the given output port.
 * (0-7, 0 is farthest left).
 * NOTE: For outputs 4-7, these are digital out and will be set to the maximum
 *       voltage for any value != 0
 */
void arch_io_set_vout(uint8_t port, uint16_t voltage)
{
    ASSERT(port < 8);
    ASSERT(voltage <= 24000);

    output_voltages[port] = voltage;

    if (voltage > 0)
    {
        PORTD |= 1 << port;
    }
    else
    {
        PORTD &= ~(1 << port);
    }
}

/* Gets the current setting for the output voltage (in millivolts)
 * (0-7, 0 is farthest left)
 */
uint16_t arch_io_get_vout(uint8_t port)
{
    ASSERT(port < 8);
    return output_voltages[port];
}

