#include "global-conf.h"
#include "ksz8851.h"
#include <avr/io.h>
#include <util/delay.h>

unsigned int
network_read(void) {
	uint16_t len;

	len = ksz8851BeginPacketRetrieve();

	if (len == 0) {
	    return 0;
	}

	ksz8851RetrievePacketData((uint8_t *)uip_buf, len);
	ksz8851EndPacketRetrieve();

	return len;
}

void
network_send(void) {
    ksz8851BeginPacketSend(uip_len);

    if (uip_len <= UIP_LLH_LEN + 40) {
	ksz8851SendPacketData((uint8_t *)uip_buf, uip_len);
    } else {
	ksz8851SendPacketData((uint8_t *)uip_buf, 54);
	ksz8851SendPacketData((uint8_t *)uip_appdata, uip_len - UIP_LLH_LEN - 40);
    }

    ksz8851EndPacketSend();
}

void
network_init(void)
{
	//Initialise the device
	ksz8851Init();
}


void
network_get_MAC(uint8_t* macaddr)
{
    uint16_t	temp;

    // read MAC address registers
    // NOTE: MAC address in KSZ8851 is byte-backward

    /* Read QMU MAC address (low) */
    temp = ksz8851_regrd(REG_MAC_ADDR_01);
    macaddr[5] = temp & 0xff;
    macaddr[4] = (temp >> 8);

    /* Read QMU MAC address (middle) */
    temp = ksz8851_regrd(REG_MAC_ADDR_23);
    macaddr[3] = temp & 0xff;
    macaddr[2] = temp >> 8;

    /* Read QMU MAC address (high) */
    ksz8851_regrd(REG_MAC_ADDR_45);
    macaddr[1] = temp & 0xff;
    macaddr[0] = temp >>  8;
}

void
network_set_MAC(uint8_t* macaddr)
{
    // write MAC address
    // NOTE: MAC address in KSZ8851 is byte-backward

    /* Write QMU MAC address (low) */
    ksz8851_regwr(REG_MAC_ADDR_01, (macaddr[4] << 8) | macaddr[5]);
    /* Write QMU MAC address (middle) */
    ksz8851_regwr(REG_MAC_ADDR_23, (macaddr[2] << 8) | macaddr[3]);
    /* Write QMU MAC address (high) */
    ksz8851_regwr(REG_MAC_ADDR_45, (macaddr[0] << 8) | macaddr[1]);
}

