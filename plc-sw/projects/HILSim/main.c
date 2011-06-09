#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include "timer.h"

#include "global-conf.h"
#include "uip_arp.h"
#include "network.h"
#include "apps-conf.h"
#include <string.h>
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])



//EEPROM parameters (TCP/IP parameters)
uint8_t EEMEM ee_enable_dhcp=USE_DHCP;

uint8_t EEMEM ee_eth_addr[6]=
   {UIP_ETHADDR0,UIP_ETHADDR1,UIP_ETHADDR2,UIP_ETHADDR3,UIP_ETHADDR4,UIP_ETHADDR5};

uint8_t EEMEM ee_ip_addr[4]= 
      {UIP_IPADDR0, UIP_IPADDR1, UIP_IPADDR2, UIP_IPADDR3};

uint8_t EEMEM ee_net_mask[4]= 
        {UIP_NETMASK0, UIP_NETMASK1, UIP_NETMASK2, UIP_NETMASK3};

uint8_t EEMEM ee_gateway[4]=
        {UIP_DRIPADDR0, UIP_DRIPADDR1, UIP_DRIPADDR2, UIP_DRIPADDR3};

// TCP/IP parameters in data memory
uint8_t _enable_dhcp;
uint8_t _eth_addr[6];
uint8_t _ip_addr[4];
uint8_t _net_mask[4];
uint8_t _gateway[4];

static struct uip_eth_addr  my_eth_addr = { .addr = {UIP_ETHADDR0,UIP_ETHADDR1,UIP_ETHADDR2,UIP_ETHADDR3,UIP_ETHADDR4,UIP_ETHADDR5}};

struct timer dhcp_timer;

int main(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
// Note: make the following call in code to reset device
// wdt_enable(WDTO_500MS); // reset in 2 seconds - requires #include <avr/wdt.h>
	led_conf();

	network_init();

	int i;
	uip_ipaddr_t ipaddr;
	struct timer periodic_timer, arp_timer;

	clock_init();

	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);

#ifdef DHCP_DEBUG
    led_low();
#endif
	uip_init();
    // must be done or sometimes arp doesn't work
    uip_arp_init();
	
    _enable_dhcp=eeprom_read_byte(&ee_enable_dhcp);
    if ((_enable_dhcp != 1) && (_enable_dhcp != 0))
    {   // if the setting is invalid, enable by default
        _enable_dhcp = 1;
        eeprom_write_byte(&ee_enable_dhcp,_enable_dhcp);
    }
    eeprom_read_block ((void *)_eth_addr, (const void *)&ee_eth_addr,6);

    // if the mac address in eeprom looks good, use it.
    if((_eth_addr[0] != 255) && (_eth_addr[0] != 0))
    {
        my_eth_addr.addr[0] = _eth_addr[0];
        my_eth_addr.addr[1] = _eth_addr[1];
        my_eth_addr.addr[2] = _eth_addr[2];
        my_eth_addr.addr[3] = _eth_addr[3];
        my_eth_addr.addr[4] = _eth_addr[4];
        my_eth_addr.addr[5] = _eth_addr[5];
    }

	uip_setethaddr(my_eth_addr);
//_enable_dhcp = 1;
    if (_enable_dhcp)
    {
#ifdef __DHCPC_H__
        // setup the dhcp renew timer the make the first request
        timer_set(&dhcp_timer, CLOCK_SECOND * 600);
	    dhcpc_init(&my_eth_addr, 6);
        dhcpc_request();
#endif
    }
    else
    {
        eeprom_read_block ((void *)_ip_addr, (const void *)&ee_ip_addr,4);
        eeprom_read_block ((void *)_net_mask,(const void *)&ee_net_mask,4);
        eeprom_read_block ((void *)_gateway, (const void *)&ee_gateway,4);

        // if the IP looks good in flash, use it
        if ((_ip_addr[0] != 255) && (_ip_addr[0] != 0))
        {
            uip_ipaddr(ipaddr, _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
            uip_sethostaddr(ipaddr);
            uip_ipaddr(ipaddr, _gateway[0], _gateway[1], _gateway[2], _gateway[3]);
            uip_setdraddr(ipaddr);
            uip_ipaddr(ipaddr, _net_mask[0], _net_mask[1], _net_mask[2], _net_mask[3]);
            uip_setnetmask(ipaddr);
        }
        else
        { // ip in flash didn't look good... use default
            uip_ipaddr(ipaddr, UIP_IPADDR0, UIP_IPADDR1, UIP_IPADDR2, UIP_IPADDR3);
            uip_sethostaddr(ipaddr);
            uip_ipaddr(ipaddr, UIP_DRIPADDR0, UIP_DRIPADDR1, UIP_DRIPADDR2, UIP_DRIPADDR3);
            uip_setdraddr(ipaddr);
            uip_ipaddr(ipaddr, UIP_NETMASK0, UIP_NETMASK1, UIP_NETMASK2, UIP_NETMASK3);
            uip_setnetmask(ipaddr);
        }
    }

    // start up the webserver
    //httpd_init();
	simple_httpd_init();
	//telnetd_init();


	while(1){
		uip_len = network_read();

		if(uip_len > 0) {
			if(BUF->type == htons(UIP_ETHTYPE_IP)){
				uip_arp_ipin(); // arp seems to have issues w/o this
				uip_input();
				if(uip_len > 0) {
					uip_arp_out();
					network_send();
				}
			}else if(BUF->type == htons(UIP_ETHTYPE_ARP)){
				uip_arp_arpin(); // this is correct
				if(uip_len > 0){
					network_send();
				}
			}

		}else if(timer_expired(&periodic_timer)) {
			timer_reset(&periodic_timer);


			for(i = 0; i < UIP_CONNS; i++) {
				uip_periodic(i);
				if(uip_len > 0) {
					uip_arp_out();
					network_send();
				}
			}

			#if UIP_UDP
			for(i = 0; i < UIP_UDP_CONNS; i++) {
				uip_udp_periodic(i);
				if(uip_len > 0) {
					uip_arp_out();
					network_send();
				}
			}
			#endif /* UIP_UDP */

			if(timer_expired(&arp_timer)) {
				timer_reset(&arp_timer);
				uip_arp_timer();
			}
		} else if (_enable_dhcp && timer_expired(&dhcp_timer)) {
#ifdef __DHCPC_H__
            // for now turn off the led when we start the dhcp process
            led_low();
            dhcpc_renew();
            timer_reset(&dhcp_timer);
#endif
        }
	}
	return 0;
}

#if UIP_CONF_LOGGING == 1
void uip_log(char *m)
{
	//TODO: Get debug information out here somehow, does anybody know a smart way to do that?
}
#endif

/*---------------------------------------------------------------------------*/


#ifdef __DHCPC_H__
void dhcpc_configured(const struct dhcpc_state *s)
{
    uip_ipaddr_t addr;

    // byte swap the network info
    _ip_addr[0] = (s->ipaddr[0]);
    _ip_addr[1] = (s->ipaddr[0]) >> 8;
    _ip_addr[2] = (s->ipaddr[1]);
    _ip_addr[3] = (s->ipaddr[1]) >> 8;

    _net_mask[0] = (s->netmask[0]);
    _net_mask[1] = (s->netmask[0]) >> 8;
    _net_mask[2] = (s->netmask[1]);
    _net_mask[3] = (s->netmask[1]) >> 8;

    _gateway[0] = (s->default_router[0]);
    _gateway[1] = (s->default_router[0]) >> 8;
    _gateway[2] = (s->default_router[1]);
    _gateway[3] = (s->default_router[1]) >> 8;

#ifdef DHCP_DEBUG
    eeprom_write_block (_ip_addr, &ee_ip_addr, 4);
    eeprom_write_block (_net_mask,&ee_net_mask,4);
    eeprom_write_block (_gateway, &ee_gateway, 4);
#endif
    // re-init just in case
	uip_setethaddr(my_eth_addr);

    // set ip
    uip_ipaddr(&addr, _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
    uip_sethostaddr(&addr);
    
    // set netmask
    uip_ipaddr(&addr,_net_mask[0], _net_mask[1], _net_mask[2], _net_mask[3]);
    uip_setnetmask(&addr);

    // set gateway
    uip_ipaddr(&addr,_gateway[0], _gateway[1], _gateway[2], _gateway[3]);
    uip_setdraddr(&addr);

//  code to use dhcp server lease time removed due to uint16_t overflow
//  issues with calculating the time.  Just use 5 minutes instead.  
    timer_set(&dhcp_timer, 5 * 60 * CLOCK_SECOND);

#ifdef DHCP_DEBUG
    // for now turn on the led when we get an ip
    led_high();
#endif
}
#endif

/*---------------------------------------------------------------------------*/
