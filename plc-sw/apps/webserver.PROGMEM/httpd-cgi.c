/**
 * \addtogroup httpd
 * @{
 */

/**
 * \file
 *         Web server script interface
 * \author
 *         Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2001-2006, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: httpd-cgi.c,v 1.2 2006/06/11 21:46:37 adam Exp $
 *
 */

#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd-cgi.h"
#include "httpd-fs.h"

#include "websrv_helper_functions.h"

#include <stdio.h>
#include <string.h>


#if defined ENABLE_CGI_FILE_STATS
#include "cgi-code/file_stats.c"
HTTPD_CGI_CALL(file_stats, "file_stats", file_stats_cgi);
#else
#define ENABLE_CGI_FILE_STATS_LIST
#endif

#if defined ENABLE_CGI_NET_STATS
#include "cgi-code/net_stats.c"
HTTPD_CGI_CALL(net_stats, "net_stats", net_stats_cgi);
#else
#define ENABLE_CGI_NET_STATS_LIST
#endif

#if defined ENABLE_CGI_MYCGI
#include "cgi-code/mycgi.c"
HTTPD_CGI_CALL(mycgi, "mycgi", mycgi_out);
#else
#define ENABLE_CGI_MYCGI_LIST
#endif

#if defined ENABLE_CGI_HELLO
#include "cgi-code/hello.c"
HTTPD_CGI_CALL(hello, "hello", run_hello);
#else
#define ENABLE_CGI_HELLO_LIST
#endif

#if defined ENABLE_CGI_GET_SET_VALUE
static char getsetvalue_str_g[20] = {0,0};
#include "cgi-code/get_set_value.c"
HTTPD_CGI_CALL(getsetvalue_get, "get_set_value", getsetvalue_out);
HTTPD_CGI_CALL(getsetvalue_set, "getset_p.shtml", getsetvalue_in);
#else
#define ENABLE_CGI_GET_SET_VALUE_LIST 
#endif

#if defined ENABLE_CGI_TCP_CONNECTIONS
#include "cgi-code/tcp.c"
HTTPD_CGI_CALL(tcp_connections, "tcp_connections", tcp_stats);
#else
#define ENABLE_CGI_TCP_CONNECTIONS
#endif

static const struct httpd_cgi_call *calls[] = { 
ENABLE_CGI_FILE_STATS_LIST
ENABLE_CGI_NET_STATS_LIST
ENABLE_CGI_MYCGI_LIST
ENABLE_CGI_HELLO_LIST
ENABLE_CGI_GET_SET_VALUE_LIST
ENABLE_CGI_TCP_CONNECTIONS_LIST
    NULL };

/*---------------------------------------------------------------------------*/
static
PT_THREAD(nullfunction(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/

httpd_cgifunction
httpd_cgi_lookup(const char *name, const struct httpd_cgi_call **searchlist)
{
  const struct httpd_cgi_call **f;

  /* Find the matching name in the table, return the function. */
  for(f = searchlist; *f != NULL; ++f) {
    if((strlen((*f)->name) > 2) &&
	    (strncmp((*f)->name, name, strlen((*f)->name)) == 0)) {
      return (*f)->function;
    }
  }
  return nullfunction;
}

httpd_cgifunction
httpd_cgi(const char *name)
{
  return(httpd_cgi_lookup(name,calls));
}


