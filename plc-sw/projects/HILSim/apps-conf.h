#ifndef __APPS_CONF_H__
#define __APPS_CONF_H__


//#define PORT_APP_MAPPER 1

//Here we include the header file for the application(s) we use in our project.
#include "phys.h"
#include "jsoncmd.h"

//#define DHCP_DEBUG 1
#include "dhcpc.h"


#if defined PORT_APP_MAPPER
	#include "port_app_mapper.h"
#endif
#endif /*__APPS_CONF_H__*/
