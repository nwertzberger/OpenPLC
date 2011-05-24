/*****************************************************************************
*  "A Very Simple Application" from the uIP 0.9 documentation
*****************************************************************************/

#include "app.h"


void example1_init(void)
{
	uip_listen(HTONS(1234));
}


void example1_app(void)
{
	if(uip_newdata() || uip_rexmit())
	{
		uip_send("ok\n", 3);
	}
}
