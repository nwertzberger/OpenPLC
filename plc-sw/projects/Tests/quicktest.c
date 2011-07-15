#include <inttypes.h>
#include <avr/io.h>
#define led_on(X) PORTD |= 1 << (X + 2)
#define led_off(X) PORTD &= ~(1 << (X + 2))

#define WCR 0x40
#define RCR 0x00

typedef enum {
    ERDPTL = 0x00,
    ERDPTH,
    EWRPTL,
    EWRPTH,
    ETXSTL,
    ETXSTH,
    ETXNDL,
    ETXNDH,
    ERXSTL,
    ERXSTH,
    ERXNDL,
    ERXNDH,
    ERXRDPTL,
    ERXRDPTH,
    ERXWRPTL,
    ERXWRPTH,
    EDMASTL,
    EDMASTH,
    EDMANDL,
    EDMANDH,
    EDMADSTL,
    EDMADSTH,
    EDMACSL,
    EDMACSH,
    EIE = 0x1b,
    EIR,
    ESTAT,
    ECON2,
    ECON1
} Bank1Regs;

inline void eth_write(uint8_t addr, uint8_t d)
{
    PORTB &= ~0x10;
    SPDR = WCR | addr;
    while(!(SPSR & (1<<SPIF)));
    SPDR = d;
    while(!(SPSR & (1<<SPIF)));
    PORTB |= 0x10;
}

inline uint8_t eth_read(uint8_t addr)
{
    PORTB &= ~0x10;
    SPDR = RCR | addr;
    while(!(SPSR & (1<<SPIF)));
    SPDR = 0;
    while(!(SPSR & (1<<SPIF)));
    PORTB |= 0x10;
    return SPDR;
}

int main(int argc, const char *argv[])
{
    DDRD = 0xfc;

    // Test the LED's (Light zero)
    led_on(0);

    // Set up SPI
    DDRB = 0xb0;
    PORTB |= 0x10;
    SPCR = (1<<SPE) | (1<<MSTR);
    // Wait for chip to come online.
    while (!(eth_read(ESTAT) & 0x01));
    led_on(1);

    // Select the chip
    if (eth_read(ERDPTL) == 0xfa) {
        led_on(2);
    }
    else {
        led_off(2);
    }

    // Switch bank to bank 1
    while (1) {
        if (eth_read(ERDPTL) == 0xfa) {
            led_on(3);
        }
        else {
            led_off(3);
        }
    }
    return 0;
}
