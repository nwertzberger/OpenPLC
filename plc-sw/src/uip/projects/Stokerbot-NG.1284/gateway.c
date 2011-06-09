#include "uip.h"
#include "uiplib.h"
#include <string.h>
#include "resolv.h"

void uip_udp_appcall_gateway()
{
	resolv_appcall();
}

void uip_appcall_gateway()
{
//    printf("GW : %u %u %u\r\n",ntohs(uip_conn->lport), ntohs(uip_conn->rport),uip_ipaddr1(uip_conn->ripaddr));

    if (uip_conn->lport == HTONS(80))
    {
      httpd_appcall();
    }
    else if (uip_conn->rport == HTONS(80) || uip_conn->rport == HTONS(26600))
    {
      webclient_app();
   } else if (uip_conn->lport == HTONS(1000))
   {
     // not implemented lcd_appcall();
   } else {
       printf("Unknown packet ... \r\n");
       printf("rport : %u\r\n",ntohs(uip_conn->rport));
       printf("lport : %u\r\n",ntohs(uip_conn->lport));
       printf("LEN : %u\r\n",uip_conn->len);
       printf("IP : %u.%u.%u.%u\r\n",uip_ipaddr1(uip_conn->ripaddr),uip_ipaddr2(uip_conn->ripaddr),uip_ipaddr3(uip_conn->ripaddr),uip_ipaddr4(uip_conn->ripaddr));
   }
}

