#include "uip.h"
#include "uiplib.h"
#include "timer.h"
#include "webclient.h"
#include "config.h"
#include "ds18x20.h" //To get FRACCONV value
#include "resolv.h"

#include <avr/pgmspace.h>

#include <string.h>
u16_t * webserver;
char* hostname = "stokerlog.dk";

extern volatile uint16_t tickS;
uint16_t timer_webclient;

void resolv_found(char *name, u16_t *ipaddr)
{
    printf("DNS resolved %s to %u.%u.%u.%u\r\n",name,uip_ipaddr1(ipaddr),uip_ipaddr2(ipaddr),uip_ipaddr3(ipaddr),uip_ipaddr4(ipaddr));
}

void webclient_init(void) {
   if (webserver == NULL)
   {
   	webserver = resolv_lookup(hostname);
	if (webserver == NULL)
        {
            printf("Request dns query ... \r\n");
	    resolv_query(hostname);
        }
   }
}

void webclient_connect(void) {
    if (tickDiffS(timer_webclient) > 140)
    {
        printf("No reply to webclient recieved for 2minutes (%u -- %u -- %u), resetting...",tickDiffS(timer_webclient),timer_webclient,tickS);
	while (true); //watchdog will handle reset, but we cant hard reset enc unless we have the reset line...
    }

   printf("Connect\r\n");

   if (webserver == NULL)
   {
   	webserver = resolv_lookup(hostname);
	if (webserver == NULL)
        {
            printf("Request dns query ... \r\n");
	    resolv_query(hostname);
        }
   }

   if (webserver != NULL)
   {
     printf("Connecting ... ");
     uip_connect((uip_ipaddr_t*)webserver, HTONS(80));

     printf("Connect sent");
   }
}

void webclient_app(void) {
   if (uip_connected())
   {
       printf("Setting timer for webclient to %u \r\n", tickS);
	timer_webclient = tickS;
   }

   if(uip_connected() || uip_rexmit()) {
	  char *ptr = tempbuf;

      printf("GET\r\n");
	  sprintf(ptr, "GET /incoming.php?v=1&id=%02X%02X%02X%02X%02X%02X%02X%02X",
	  systemID[0],
	  systemID[1],
	  systemID[2],
	  systemID[3],
	  systemID[4],
	  systemID[5],
	  systemID[6],
	  systemID[7]
	  );

	  ptr += strlen(ptr);

	for (uint8_t i=0; i<8; i++)
	{
		sprintf_P(ptr, PSTR("&ADC%u=%u"), i, simpleSensorValues[i]);
		ptr += strlen(ptr);
	}

	for (uint8_t i=0; i<4; i++)
	{
		sprintf_P(ptr, PSTR("&D%u=%u"), i, simpleSensorValues[i+8]);
		ptr += strlen(ptr);
	}


    for (uint8_t i=0; i<MAXSENSORS; i++)
    {  
      if (sensorValues[(i*SENSORSIZE)+FAMILY] != 0)
      {
        int frac = sensorValues[(i*SENSORSIZE)+VALUE2]*DS18X20_FRACCONV;  //Ganger de sidste par bits, med det step DS18B20 bruger
    
        sprintf_P(ptr, PSTR("&%02X%02X%02X%02X%02X%02X%02X%02X=%c%d.%04d"),
              sensorValues[(i*SENSORSIZE)+FAMILY],
              sensorValues[(i*SENSORSIZE)+ID1],
              sensorValues[(i*SENSORSIZE)+ID2],
              sensorValues[(i*SENSORSIZE)+ID3],
              sensorValues[(i*SENSORSIZE)+ID4],
              sensorValues[(i*SENSORSIZE)+ID5],
              sensorValues[(i*SENSORSIZE)+ID6],
              sensorValues[(i*SENSORSIZE)+CRC],
              sensorValues[(i*SENSORSIZE)+SIGN],
              sensorValues[(i*SENSORSIZE)+VALUE1],
              frac
            );      

			ptr += strlen(ptr);	
      }
	}

	
		sprintf_P(ptr, PSTR(" HTTP/1.0\r\nHost: stokerlog.dk\r\nUser-Agent: Stokerbot NG %u.%u\r\n\r\n"),SBNG_VERSION_MAJOR,SBNG_VERSION_MINOR);
 		
	   printf("Requesting : %s \r\n", tempbuf);	
	   uip_send(tempbuf, strlen(tempbuf));

      return;
   }

   if (uip_aborted())
    printf("Webclient aborted \r\n");

   if (uip_timedout())
    printf("Webclient timeout\r\n");

    if (uip_poll())
      printf("Webclient poll\r\n");

   if(uip_newdata())
   {
    printf("New data\r\n");
	uint16_t len = uip_datalen();
	printf("Recieved %u bytes \r\n", len);
    //NULL terminate the incoming data buffer
    *((char*)uip_appdata + uip_datalen()) = '\0';
	printf("%s \r\n",(char*)uip_appdata);
   }
}
