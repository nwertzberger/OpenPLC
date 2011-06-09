#ifndef __PORT_APP_MAPPER_H__
#define __PORT_APP_MAPPER_H__

#include "apps-conf.h"

void uip_appcall_port_app_mapper(void);
void uip_udp_appcall_port_app_mapper(void);

#if defined UIP_APPCALL || defined UIP_UDP_APPCALL
//#warn "*** PORT APP MAPPER WILL REDEFINE APPCALL ****"
	#undef UIP_APPCALL
//	#undef UIP_UDP_APPCALL
#endif

// now re-define the app call 
#define UIP_APPCALL uip_appcall_port_app_mapper
#define UIP_UDP_APPCALL uip_udp_appcall_port_app_mapper

#if defined PORT_APP_MAPPER
typedef int8_t uip_tcp_appstate_t;
typedef int8_t uip_udp_appstate_t;
#else
#error "port_app_mapper compiled but, PORT_APP_MAPPER not defined in apps-conf.h"
#endif

struct port_appcall_map
{
	void (*an_appcall)(void);
	uint16_t lport;
	uint16_t rport;
};

// Add the following to the top of your project app-conf.h file then include this header at the end of the file
// #define PORT_APP_MAPPER 1

#endif  // __PORT_APP_MAPPER_H__
