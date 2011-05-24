/* Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * The following is used to control the physical IO of the board.
 */
#ifndef ARCH_IO_H
#define ARCH_IO_H

#include <stdint.h>

/* Initializes inputs and outputs by setting them to either input or output
 * and setting all output voltages to 0.
 */
void arch_io_init();

/* Returns the voltage (in millivolts) for the given port
 * (0-7, 0 is farthest left)
 */
uint16_t arch_io_get_vin(uint8_t port);

/* Sets the output voltage (in millivolts) for the given output port.
 * (0-7, 0 is farthest left).
 * NOTE: For outputs 4-7, these are digital out and will be set to the maximum
 *       voltage for any value != 0
 */
void arch_io_set_vout(uint8_t port, uint16_t voltage);

/* Gets the current setting for the output voltage (in millivolts)
 * (0-7, 0 is farthest left)
 */
uint16_t arch_io_get_vout(uint8_t port);

#endif
