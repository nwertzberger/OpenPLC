#ifndef __SIMPLE_HTTPD_H__
#define __SIMPLE_HTTPD_H__
#include "psock.h"

struct simple_httpd_state {
	struct psock p;
};


void simple_httpd_appcall(void);
void simple_httpd_init(void);

#if defined PORT_APP_MAPPER
	#define SIMPLE_HTTPD_APP_CALL_MAP {simple_httpd_appcall, 80, 0},
	struct simple_httpd_state httpd_state_list[UIP_CONF_MAX_CONNECTIONS];
#else
	#define SIMPLE_HTTPD_APP_CALL_MAP
	#define UIP_APPCALL simple_httpd_appcall
	typedef struct simple_httpd_state uip_tcp_appstate_t;
#endif

#endif /* __SIMPLE_HTTPD_H__ */
