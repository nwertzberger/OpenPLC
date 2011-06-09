#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <string.h>
#include <avr/interrupt.h>

#include "global-conf.h"
#include <util/delay.h>

#include "port_app_mapper.h"
#include "config.h"
#include "uip_arp.h"
#include "network.h"
#include "enc28j60.h"
#include "timer.h"
#include "clock-arch.h"
#include "AVR035.h"
#include "uart.h"
#include "webclient.h"
#include "resolv.h"
#include "ds18x20.h"
#include "eeprom.h"
#include "analog.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

void save_ip_addresses(void);
void read_ip_addresses(void);
void loadSimpleSensorData(void);
void updateSimpleSensors(void);
void updateCounters(void);
void timedSaveEeprom(void);
void timedAlarmCheck(void);

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar0, NULL, _FDEV_SETUP_WRITE);
//FILE lcd_str = FDEV_SETUP_STREAM(uart_putchar1, NULL, _FDEV_SETUP_WRITE);

extern volatile uint16_t tick;
extern volatile uint16_t tickS;

uint8_t systemID[8];
uint8_t mymac[6] = {0x56,0x55,0x58,0x10,0x00,0x29};

uint8_t gwip[4] = {192,168,1,1};
uint8_t myip[4] = {192,168,1,67};
uint8_t netmask[]={255,255,255,0};
uint8_t dnsip[]={8,8,8,8}; // the google public DNS, don't change unless there is a real need

ISR(WDT_vect) {
    printf("Watchdog caused reset\r\n");
    timedSaveEeprom();
    while(true); //Next WDT timeout will reset chip
}

int main(void) {
    sei();
    uint8_t resetSource = MCUSR;
    MCUSR = 0;
    wdt_reset();
    wdt_disable();

    wdt_enable(WDTO_1S);
    WDTCSR |= (1 << WDIE);  //enable watchdog interrupt
    wdt_reset();
    cli();

    clock_init();
    usart_init(1000000, 9600);
    stdout = &uart_str;
    stderr = &uart_str;
    stdin = &uart_str;

    uip_ipaddr_t ipaddr;
    struct timer periodic_timer, arp_timer;
    uint16_t timer_OW, timer_Simple, timer_Count, timer_EEProm, timer_SendData, timer_IOalarm, timer_network;
    timer_OW = 0;
    timer_Simple = 0;
    timer_Count = 0;
    timer_EEProm = 0;
    timer_SendData = 0;
    timer_IOalarm = 0;
    timer_network = 0;
    
    if(resetSource & (1<<WDRF))
    {
        printf("Mega was reset by watchdog...\r\n");
    }

    if(resetSource & (1<<BORF))
    {
        printf("Mega was reset by brownout...\r\n");
    }

    if(resetSource & (1<<EXTRF))
    {
        printf("Mega was reset by external...\r\n");
    }

    if(resetSource & (1<<PORF))
    {
        printf("Mega was reset by power on...\r\n");
    }

//else jtag (disabled)

    //sensorScan = (uint8_t*) & tempbuf;

    if (eepromReadByte(0) == 255 || eepromReadByte(11) == 255)
    {
            printf_P(PSTR("Setting default values\r\n"));
            //Set defaults
            eepromWriteByte(0, 0); //init

            myip[0] = 192;
            myip[1] = 168;
            myip[2] = 1;
            myip[3] = 67; //47 in final versions

            netmask[0] = 255;
            netmask[1] = 255;
            netmask[2] = 255;
            netmask[3] = 0;

            gwip[0] = 192;
            gwip[1] = 168;
            gwip[2] = 1;
            gwip[3] = 1;

            dnsip[0] = 8;
            dnsip[1] = 8;
            dnsip[2] = 8;
            dnsip[3] = 8;

            eepromWriteByte(29, 80);  //web port
            eepromWriteByte(10, 0);  //dhcp off

            save_ip_addresses();
            wdt_reset();

            eepromWriteStr(200, "SBNG", 4);  //default password
            eepromWriteByte(204, '\0');
            eepromWriteByte(205, '\0');
            eepromWriteByte(206, '\0');
            eepromWriteByte(207, '\0');
            eepromWriteByte(208, '\0');
            eepromWriteByte(209, '\0');

            eepromWriteByte(100, 1); //Analog port 0 = ADC
            eepromWriteByte(101, 1); //Analog port 1 = ADC
            eepromWriteByte(102, 1); //Analog port 2 = ADC
            eepromWriteByte(103, 1); //Analog port 3 = ADC
            eepromWriteByte(104, 1); //Analog port 4 = ADC
            eepromWriteByte(105, 1); //Analog port 5 = ADC
            eepromWriteByte(106, 1); //Analog port 6 = ADC
            eepromWriteByte(107, 1); //Analog port 7 = ADC

            eepromWriteByte(110, 0); //Digital port 0 = OUT
            eepromWriteByte(111, 0); //Digital port 1 = OUT
            eepromWriteByte(112, 0); //Digital port 2 = OUT
            eepromWriteByte(113, 0); //Digital port 3 = OUT

      	    wdt_reset();
            for (uint8_t alarm=1; alarm<=4; alarm++)
            {
                    uint16_t pos = 400 + ((alarm-1)*15); //400 415 430 445

                    eepromWriteByte(pos+0, 0); //enabled
                    eepromWriteByte(pos+1, 0); //sensorid
                    eepromWriteByte(pos+2, 0); //sensorid
                    eepromWriteByte(pos+3, 0); //sensorid
                    eepromWriteByte(pos+4, 0); //sensorid
                    eepromWriteByte(pos+5, 0); //sensorid
                    eepromWriteByte(pos+6, 0); //sensorid
                    eepromWriteByte(pos+7, 0); //sensorid
                    eepromWriteByte(pos+8, 0); //sensorid
                    eepromWriteByte(pos+9, '<'); //type
                    eepromWriteByte(pos+10, 0); //value
                    eepromWriteByte(pos+11, 0); //target
                    eepromWriteByte(pos+12, 0); //state
                    eepromWriteByte(pos+13, 0); //reverse
                    eepromWriteByte(pos+14, 0); //not-used
            }

            eepromWriteByte(1, EEPROM_VERSION);
    }
/*
    findSystemID(systemID);

    if (systemID[0] == 0) {
        printf_P(PSTR("No system id found, add a DS2401 or use old software"));
//        fprintf(&lcd_str, "?f?y0?x00No system id found?nAdd a DS2401 or use old software?n");
        wdt_disable();
        wdt_reset();
        while (true);
    } else {
*/    
        //MAC will be 56 51 99 36 14 00 with example system id
        mymac[1] = systemID[1];
        mymac[2] = systemID[2];
        mymac[3] = systemID[3];
        mymac[4] = systemID[4];
        mymac[5] = systemID[5];
//    }
//    fprintf(&lcd_str, "?y1?x00ID: %02X%02X%02X%02X%02X%02X%02X%02X?n", systemID[0], systemID[1], systemID[2], systemID[3], systemID[4], systemID[5], systemID[6], systemID[7]);

    loadSimpleSensorData();

    //Set digital pins based on selections...
    for (uint8_t i=8; i<=11; i++)
    {
            if (simpleSensorTypes[i] == 0)
            {
                    //output
                    SETBIT(DDRC, (i-6));
            } else {
                    //input
                    CLEARBIT(DDRC, (i-6));
            }
    }


    network_init();

    timer_set(&periodic_timer, CLOCK_SECOND / 2);
    timer_set(&arp_timer, CLOCK_SECOND * 10);

    uip_init();

    //sættes hvert for sig for uip og enc, skal rettes til en samlet setting, så vi kan bruge mymac
    struct uip_eth_addr mac = { {UIP_ETHADDR0, UIP_ETHADDR1, UIP_ETHADDR2, UIP_ETHADDR3, UIP_ETHADDR4, UIP_ETHADDR5} };
//    struct uip_eth_addr mac = {mymac[0], mymac[1], mymac[2], mymac[3], mymac[4], mymac[5]};

    uip_setethaddr(mac);
    httpd_init();
    /*
    #ifdef __DHCPC_H__
            dhcpc_init(&mac, 6);
    #else
     */
    uip_ipaddr(ipaddr, myip[0], myip[1], myip[2], myip[3]);
    uip_sethostaddr(ipaddr);
    uip_ipaddr(ipaddr, gwip[0], gwip[1], gwip[2], gwip[3]);
    uip_setdraddr(ipaddr);
    uip_ipaddr(ipaddr, netmask[0], netmask[1], netmask[2], netmask[3]);
    uip_setnetmask(ipaddr);
    //#endif /*__DHCPC_H__*/

    printf("Setting ip to %u.%u.%u.%u \r\n", myip[0], myip[1], myip[2], myip[3]);

    resolv_init();
    uip_ipaddr(ipaddr, dnsip[0], dnsip[1], dnsip[2], dnsip[3]);
    resolv_conf(ipaddr);
    webclient_init();

    printf("Stokerbot NG R3 ready  -  Firmware %u.%u ...\r\n", SBNG_VERSION_MAJOR, SBNG_VERSION_MINOR);
//    fprintf(&lcd_str, "?y2?x00Firmware %u.%u ready.", SBNG_VERSION_MAJOR, SBNG_VERSION_MINOR);

//    SPILCD_init();

    wdt_reset();

    while (1) {
        //Only one event may fire per loop, listed in order of importance
        //If an event is skipped, it will be run next loop
        if (tickDiff(timer_Count) >= 1) {
            timer_Count = tick;
            wdt_reset(); //sikre at watchdog resetter os hvis timer systemet fejler, vi når her hvert 2ms
            updateCounters(); //bør ske i en interrupt istedet, for at garentere 2ms aflæsning
        } else if (tickDiffS(timer_IOalarm) >= 5) {
            printf("Timer : IO alarm \r\n");
            timer_IOalarm = tickS;
            timedAlarmCheck();
        } else if (tickDiffS(timer_OW) >= 2) {
            printf("Timer : OW \r\n");
            timer_OW = tickS;
            updateOWSensors();
        } else if (tickDiffS(timer_Simple) >= 5) {
            printf("Timer : Simple\r\n");
            timer_Simple = tickS;
            updateSimpleSensors();
        } else if (tickDiffS(timer_SendData) >= 59) {
            printf("Timer : webclient \r\n");
            timer_SendData = tickS;
            webclient_connect();
        } else if (tickDiffS(timer_EEProm) >= 60 * 30) {
            printf("Timer : eeprom \r\n");
            timer_EEProm = tickS;
            timedSaveEeprom();
        }

        //Net handling below



        if (tickDiff(timer_network) >= 1)
        {
            timer_network = tick;
            uip_len = network_read();

            if (uip_len > 0) {
                if (BUF->type == htons(UIP_ETHTYPE_IP)) {
                    uip_arp_ipin();
                    uip_input();
                    if (uip_len > 0) {
                        uip_arp_out();
                        network_send();
                    }
                } else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
                    uip_arp_arpin();
                    if (uip_len > 0) {
                        network_send();
                    }
                }

            } else if (timer_expired(&periodic_timer)) {
                timer_reset(&periodic_timer);
                //FLIPBIT(PORTC,5);
    //            printf("Timers : %u %u \r\n", tick, tickS);

                for (uint8_t i = 0; i < UIP_CONNS; i++) {
                    uip_periodic(i);
                    if (uip_len > 0) {
                        uip_arp_out();
                        network_send();
                    }
                }

    #if UIP_UDP
                for (uint8_t i = 0; i < UIP_UDP_CONNS; i++) {
                    uip_udp_periodic(i);
                    if (uip_len > 0) {
                        uip_arp_out();
                        network_send();
                    }
                }
    #endif /* UIP_UDP */

                if (timer_expired(&arp_timer)) {
                    timer_reset(&arp_timer);
                    uip_arp_timer();
                }
            }
        }
    }
    return 0;
}
























void timedAlarmCheck(void)
{
	for (uint8_t alarm=1; alarm<=4; alarm++)
	{
		uint16_t pos = 400 + ((alarm-1)*15); //400 415 430 445

		if (eepromReadByte(pos+0) == 1)
		{
				uint8_t sensorpos = findSensor(
				eepromReadByte(pos+1),
				eepromReadByte(pos+2),
				eepromReadByte(pos+3),
				eepromReadByte(pos+4),
				eepromReadByte(pos+5),
				eepromReadByte(pos+6),
				eepromReadByte(pos+7),
				eepromReadByte(pos+8));

				int8_t value = (int8_t)sensorValues[(sensorpos*SENSORSIZE)+VALUE1];
				char sign = sensorValues[(sensorpos*SENSORSIZE)+SIGN];
				if (sign == '-') value *= -1;

				int8_t target = eepromReadByteSigned(pos+10);

				if (
					(eepromReadByte(pos+9) == 1 && value < target)
					||
					(eepromReadByte(pos+9) == 2 && value == target)
					||
					(eepromReadByte(pos+9) == 3 && value > target)
				)
				{
					//ALARM
					//DDR=in/out PIN=value/pullup PORT=state
					uint8_t pin = eepromReadByte(pos+11);

					if (pin >= 1 && pin <= 4)
					{
						if (eepromReadByte(pos+12) == 1)
						{
							SETBIT(PORTC, (1+pin));
						}
						else
							CLEARBIT(PORTC, (1+pin));
					}
 				} else if (eepromReadByte(pos+13) == 1) {
 					//REVERSE
					uint8_t pin = eepromReadByte(pos+11);
					if (pin >= 1 && pin <= 4)
					{
						if (eepromReadByte(pos+12) == 1)
							CLEARBIT(PORTC, (1+pin));
						else
							SETBIT(PORTC, (1+pin));
					}
 				}
		}
	}
}

//Called every 30minutes and only saved if theres any changes (100000 / 30minutes worst case = 5years)
void timedSaveEeprom(void)
{
	for (uint8_t i=0; i<4; i++)
	{
		if (simpleSensorTypes[i] == 3) //counter
		{
			uint8_t pos = 110 + (i*2);
			//save tjekker selv om det er nødvendigt
			eepromSaveWord(pos, simpleSensorValues[i]);
		}
	}
}

//Called every 20ms
void updateCounters(void)
{
	for (uint8_t i=0; i<4; i++)
	{
		if (simpleSensorTypes[i] == 3)
		{
			//Check if pin is high
			if (GETBIT(PINA,i)>0)
			{
				//and debounce is not set
				if (simpleSensorDebounce[i] == 0)
				{
					//Update counter
					simpleSensorValues[i]++;
					simpleSensorDebounce[i] = 1;
				}
			} else {
				simpleSensorDebounce[i] = 0;
			}
		}
	}
}

void updateSimpleSensors(void)
{
	ADC_Init();
	//Send ADC+digital pins
	//ADC pins
	for (uint8_t i=0; i<=7; i++)
	{
		uint8_t eepos = 100 + i;
		simpleSensorTypes[i] = eepromReadByte(eepos);

		if (simpleSensorTypes[i] == TYPE_ADC)  //ADC
		{
			simpleSensorValues[i] = readAdc(i);
		}

		if (simpleSensorTypes[i] == TYPE_DIGIN)  //DIGITAL
		{
			if (GETBIT(PINA, i) > 0)
				simpleSensorValues[i] = 1;
			else
				simpleSensorValues[i] = 0;
		}
	}
	ADCSRA &= ~(1 << ADEN); //disable ADC to save power

	//Digital pins
	for (uint8_t i=0; i<=3; i++)
	{
		uint8_t eepos = 110 + i;
		simpleSensorTypes[i+8] = eepromReadByte(eepos);

		if (simpleSensorTypes[i+8] == TYPE_DIGIN || simpleSensorTypes[i+8] == TYPE_DIGOUT)  //DIGITAL
		{
		  	//Digital pins is C2-5
			if (GETBIT(PINC, (i+2)) > 0)
				simpleSensorValues[i+8] = 1;
			else
				simpleSensorValues[i+8] = 0;
		}
	}
	//3=COUNTER, not handled here, 0=digital out
}


void read_ip_addresses(void)
{
	myip[0] = eepromReadByte(11);
	myip[1] = eepromReadByte(12);
	myip[2] = eepromReadByte(13);
	myip[3] = eepromReadByte(14);

	gwip[0] = eepromReadByte(15);
	gwip[1] = eepromReadByte(16);
	gwip[2] = eepromReadByte(17);
	gwip[3] = eepromReadByte(18);

	netmask[0] = eepromReadByte(19);
	netmask[1] = eepromReadByte(20);
	netmask[2] = eepromReadByte(21);
	netmask[3] = eepromReadByte(22);

	dnsip[0] = eepromReadByte(31);
	dnsip[1] = eepromReadByte(32);
	dnsip[2] = eepromReadByte(33);
	dnsip[3] = eepromReadByte(34);
}


void save_ip_addresses(void)
{
	eepromWriteByte(11, myip[0]);
	eepromWriteByte(12, myip[1]);
	eepromWriteByte(13, myip[2]);
	eepromWriteByte(14, myip[3]);

	eepromWriteByte(15, gwip[0]);
	eepromWriteByte(16, gwip[1]);
	eepromWriteByte(17, gwip[2]);
	eepromWriteByte(18, gwip[3]);

	eepromWriteByte(19, netmask[0]);
	eepromWriteByte(20, netmask[1]);
	eepromWriteByte(21, netmask[2]);
	eepromWriteByte(22, netmask[3]);

	eepromWriteByte(31, dnsip[0]);
	eepromWriteByte(32, dnsip[1]);
	eepromWriteByte(33, dnsip[2]);
	eepromWriteByte(34, dnsip[3]);
}

void loadSimpleSensorData()
{
	for (uint16_t i=0; i<MAXSENSORS*SENSORSIZE; i++)
		sensorValues[i] = 0;

	for (uint16_t i=0; i<MAXSENSORS*8; i++)
		sensorScan[i] = 0;

	//To load initial type values from eeprom
	for (int i=0; i<=7; i++)
	{
		uint8_t pos = 100+i;
		simpleSensorTypes[i] = eepromReadByte(pos);
		simpleSensorDebounce[i] = 0;

		if (simpleSensorTypes[i] == 3) //counter
		{
			uint8_t eepos = 140 + (i*2);
			simpleSensorValues[i] = eepromReadWord(eepos);
		} else {
			simpleSensorValues[i] = 0;
		}
	}

	//To load initial type values from eeprom
	for (int i=0; i<=3; i++)
	{
		uint8_t pos = 110+i;
		simpleSensorTypes[i+8] = eepromReadByte(pos);
		simpleSensorDebounce[i+8] = 0;

		if (simpleSensorTypes[i+8] == 3) //counter
		{
			uint8_t eepos = 160 + (i*2);
			simpleSensorValues[i+8] = eepromReadWord(eepos);
		} else {
			simpleSensorValues[i+8] = 0;
		}
	}
}
