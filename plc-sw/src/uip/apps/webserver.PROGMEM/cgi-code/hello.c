#include "websrv_helper_functions.h"

static PT_THREAD(run_hello(struct httpd_state *s, char *ptr))
{
	//NOTE:local variables are not preserved during the calls to proto socket functins
	static char hello_name[20]="";
    PSOCK_BEGIN(&s->sout);
	//check if there are parameters passed 
    if(s->param[0] && (find_key_val(s->param, hello_name, 20, "name") > 0))
    {
        PSOCK_SEND_PSTR(&s->sout,PSTR("<H1>Hello "));
        PSOCK_SEND_STR(&s->sout,hello_name);
        PSOCK_SEND_PSTR(&s->sout,PSTR("</H1><br>"));
    }

    PSOCK_SEND_PSTR(&s->sout,PSTR("\
<form action=\"/hello.shtml\" method=\"get\" bgcolor=\"#808080\">\
Enter your name: <input type=\"text\" name=\"name\" size=\"10\" value=\""));

    PSOCK_SEND_STR(&s->sout,hello_name);

    PSOCK_SEND_PSTR(&s->sout,PSTR("\
\"/><br><input type = \"submit\" value=\"Send\" size=\"8\"> <input type = \"reset\"  value=\"cancel\" size=\"8\">"));
    PSOCK_END(&s->sout);
}
