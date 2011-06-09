/**
 * \addtogroup apps
 * @{
 */

/**
 * \defgroup httpd Web server
 * @{
 * The uIP web server is a very simplistic implementation of an HTTP
 * server. It can serve web pages and files from a read-only ROM
 * filesystem, and provides a very small scripting language.

 */

/**
 * \file
 *         Web server
 * \author
 *         Adam Dunkels <adam@sics.se>
 */


/*
 * Copyright (c) 2004, Adam Dunkels.
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: httpd.c,v 1.2 2006/06/11 21:46:38 adam Exp $
 */

#include "uip.h"
#include "httpd.h"
#include "httpd-fs.h"
#include "httpd-cgi.h"
#include "http-strings.h"

#include "global-conf.h"

// debug #include "usart.h"


#include <string.h>
#include <avr/pgmspace.h>

#define STATE_WAITING 0
#define STATE_OUTPUT  1
#define STATE_UNUSED  3


/*---------------------------------------------------------------------------*/
static unsigned short
generate_part_of_file(void *state)
{
  struct httpd_state *s = (struct httpd_state *)state;

  if(s->file.len > uip_mss()) {
    s->len = uip_mss();
  } else {
    s->len = s->file.len;
  }
  memcpy_P(uip_appdata, s->file.data, s->len);
  
  return s->len;
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_file(struct httpd_state *s))
{
  PSOCK_BEGIN(&s->sout);
  
  do {
    PSOCK_GENERATOR_SEND(&s->sout, generate_part_of_file, s);
    s->file.len -= s->len;
    s->file.data += s->len;
  } while(s->file.len > 0);
      
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_len_part_of_file(void *state)
{
  struct httpd_state *s = (struct httpd_state *)state;

  memcpy_P(uip_appdata, s->file.data, s->len);
  
  return s->len;
}

static
PT_THREAD(send_part_of_file(struct httpd_state *s))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_len_part_of_file, s);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static void
next_scriptstate(struct httpd_state *s)
{
  PGM_P p;
  p = strchr_P(s->scriptptr, ISO_nl) + 1;
  s->scriptlen -= (unsigned short)(p - s->scriptptr);
  s->scriptptr = p;
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_script(struct httpd_state *s))
{


  PGM_P ptr;
  
  PT_BEGIN(&s->scriptpt);


  while(s->file.len > 0) {

    /* Check if we should start executing a script. */
    if(pgm_read_byte_near(s->file.data) == ISO_percent &&
       pgm_read_byte_near(s->file.data + 1) == ISO_bang) {
      s->scriptptr = s->file.data + 3;
      s->scriptlen = s->file.len - 3;

      if(pgm_read_byte_near(s->scriptptr - 1) == ISO_colon) {

            strncpy_P(s->tmp_str, s->scriptptr + 1, sizeof(s->tmp_str) -1);
			if(httpd_fs_open(s->tmp_str, &s->file) ||
				httpd_fs_open(s->tmp_str + 1 , &s->file)) {
				PT_WAIT_THREAD(&s->scriptpt, 
           send_file(s));
			} else {
            // could not open the file.
			}

      } else {
            strncpy_P(s->tmp_str, s->scriptptr, sizeof(s->tmp_str) -1);
			((s->tmp_str)[sizeof(s->tmp_str) - 1]) = '\0';
					PT_WAIT_THREAD(&s->scriptpt, 
           httpd_cgi(s->tmp_str)(s, s->tmp_str));
      }
      next_scriptstate(s);
      
      /* The script is over, so we reset the pointers and continue
    	 sending the rest of the file. */
      s->file.data = s->scriptptr;
      s->file.len = s->scriptlen;
    } else {
      /* See if we find the start of script marker in the block of HTML
	        to be sent. */

      if(s->file.len > uip_mss()) {
	      s->len = uip_mss();
      } else {
	      s->len = s->file.len;
      }

      if(pgm_read_byte_near(s->file.data) == ISO_percent) {
	      ptr = strchr_P(s->file.data + 1, ISO_percent);

      } else {
	      ptr = strchr_P(s->file.data, ISO_percent);

      }

      if(ptr != NULL &&
      	 ptr != s->file.data) {
        	s->len = (int)(ptr - s->file.data);
        	if(s->len >= uip_mss()) {
        	  s->len = uip_mss();
      	}
      }

      PT_WAIT_THREAD(&s->scriptpt, send_part_of_file(s));

      s->file.data += s->len;
      s->file.len -= s->len;
      
    }
  }
  
  PT_END(&s->scriptpt);


	/*
  PGM_P ptr;
  
  PT_BEGIN(&s->scriptpt);

  while(s->file.len > 0) {
    // Check if we should start executing a script.
    if( (pgm_read_byte(s->file.data) == ISO_percent) &&
        (pgm_read_byte(s->file.data + 1) == ISO_bang)) {
      s->scriptptr = s->file.data + 3;
      s->scriptlen = s->file.len - 3;

      if(pgm_read_byte(s->scriptptr - 1) == ISO_colon) {
          strncpy_P(s->tmp_str, s->scriptptr + 1, sizeof(s->tmp_str) -1);
        if (httpd_fs_open(s->tmp_str, &s->file))
        {
	        PT_WAIT_THREAD(&s->scriptpt, send_file(s));
        }
      } else {
	    PT_WAIT_THREAD(&s->scriptpt,
		       httpd_cgi(s->scriptptr)(s, s->scriptptr));
      }

      next_scriptstate(s);
      
      // The script is over, so we reset the pointers and continue
	  // sending the rest of the file.
      s->file.data = s->scriptptr;
      s->file.len = s->scriptlen;
    } else {
      // See if we find the start of script marker in the block of HTML
	  //   to be sent. 

      if(s->file.len > uip_mss()) {
        s->len = uip_mss();
      } else {
        s->len = s->file.len;
      }

      if(pgm_read_byte(s->file.data) == ISO_percent) {
        ptr = strchr_P(s->file.data + 1, ISO_percent);
      } else {
        ptr = strchr_P(s->file.data, ISO_percent);
      }

      if(ptr != NULL &&
	     ptr != s->file.data) {
        s->len = (int)(ptr - s->file.data);

        if(s->len >= uip_mss()) {
            s->len = uip_mss();
        }
      }

      PT_WAIT_THREAD(&s->scriptpt, send_part_of_file(s));
      s->file.data += s->len;
      s->file.len -= s->len;
    }
  }
  
  PT_END(&s->scriptpt);
  */
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_headers(struct httpd_state *s, const char *statushdr))
{

  char *ptr;

  PSOCK_BEGIN(&s->sout);

  PSOCK_SEND_PSTR(&s->sout, statushdr);

  ptr = strrchr(s->filename, ISO_period);

  if(ptr == NULL) {
    PSOCK_SEND_PSTR(&s->sout, http_content_type_binary);
	PSOCK_SEND_PSTR(&s->sout, http_content_length);
	PSOCK_SEND_STR(&s->sout, s->tmp_str);
	PSOCK_SEND_PSTR(&s->sout, http_crnlcrln);
  } else if(strncmp_P( ptr,http_html, 5) == 0 ||
	        strncmp_P( ptr,http_shtml, 6) == 0) {
    PSOCK_SEND_PSTR(&s->sout, http_content_type_html);
  } else if(strncmp_P(ptr,http_css,  4) == 0) {
    PSOCK_SEND_PSTR(&s->sout, http_content_type_css);
  } else if(strncmp_P(ptr,http_png, 4) == 0) {
    PSOCK_SEND_PSTR(&s->sout, http_content_type_png);
	PSOCK_SEND_PSTR(&s->sout, http_content_length);
	PSOCK_SEND_STR(&s->sout, s->tmp_str);
	PSOCK_SEND_PSTR(&s->sout, http_crnlcrln);
  } else if(strncmp_P( ptr, http_gif, 4) == 0) {
    PSOCK_SEND_PSTR(&s->sout, http_content_type_gif);
	PSOCK_SEND_PSTR(&s->sout, http_content_length);
	PSOCK_SEND_STR(&s->sout, s->tmp_str);
	PSOCK_SEND_PSTR(&s->sout, http_crnlcrln);
  } else if(strncmp_P( ptr, http_jpg, 4) == 0) {
    PSOCK_SEND_PSTR(&s->sout, http_content_type_jpg);
	PSOCK_SEND_PSTR(&s->sout, http_content_length);
	PSOCK_SEND_STR(&s->sout, s->tmp_str);
	PSOCK_SEND_PSTR(&s->sout, http_crnlcrln);
  } else {
    PSOCK_SEND_PSTR(&s->sout, http_content_type_plain);
  }

  PSOCK_END(&s->sout);
									  												

/*  PT_BEGIN(&s->outputpt);

  char *ptr;

  //PSOCK_BEGIN(&s->sout);

  // save the current state of the file
  s->tmp_charp = s->file.data;
  s->tmp_int   = s->len;

  s->file.data = (char *) statushdr;
  s->len = strlen_P(statushdr);   
  PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));

  ptr = strrchr(s->filename, ISO_period);
  if(ptr == NULL) {
    s->file.data = (char *)http_content_type_binary;
    s->len = strlen_P(http_content_type_binary);   
    PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
	s->len = 0; // use as flag to say, sent content lengh
  } else if(strncmp_P(ptr, http_html, 5) == 0 ||
	        strncmp_P(ptr, http_shtml, 6) == 0) {
    s->file.data = (char *)http_content_type_html;
    s->len = strlen_P(http_content_type_html);   
    PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
  } else if(strncmp_P(ptr, http_css, 4) == 0) {
    s->file.data = (char *)http_content_type_css;
    s->len = strlen_P(http_content_type_css);   
    PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
  } else if(strncmp_P(ptr, http_png, 4) == 0) {
    s->file.data = (char *)http_content_type_png;
    s->len = strlen_P(http_content_type_png);   
    PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
	s->len = 0; // use as flag to say, sent content lengh
  } else if(strncmp_P(ptr, http_gif, 4) == 0) {
    s->file.data = (char *)http_content_type_gif;
    s->len = strlen_P(http_content_type_gif);   
    PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
	s->len = 0; // use as flag to say, sent content lengh
  } else if(strncmp_P(ptr, http_jpg, 4) == 0) {
    s->file.data = (char *)http_content_type_jpg;
    s->len = strlen_P(http_content_type_jpg);   
    PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
	s->len = 0; // use as flag to say, sent content lengh
  } else {
    s->file.data = (char *)http_content_type_plain;
    s->len = strlen_P(http_content_type_plain);   
    PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
  }

  // ugly flag hack to send the content len
//  if (s->len == 0)
//  {
  //  s->file.data = (char *)http_content_length;
 //   s->len = strlen_P(http_content_length);
 //   PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
    snprintf(s->str_tmp, sizeof(s->str_tmp) -1, "%d", s->tmp_int);
	PSOCK_SEND_STR(&s->sout, s->str_tmp);
	s->file.data = (char *)http_crnlcrln;
	s->len = strlen_P(http_crnlcrln);
	PT_WAIT_THREAD(&s->outputpt, send_part_of_file(s));
//  }

  // restore the state of the file
  s->file.data = s->tmp_charp;
  s->len = s->tmp_int;
  PT_END(&s->outputpt);
  */
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_output(struct httpd_state *s))
{
  PT_BEGIN(&s->outputpt);
 
  char *ptr;

  if(!httpd_fs_open(s->filename, &s->file)) {
    httpd_fs_open(http_404_html, &s->file);
    strcpy_P(s->filename, http_404_html);
    PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_404));
    PT_WAIT_THREAD(&s->outputpt, send_file(s));
  } else {
    snprintf(s->tmp_str, sizeof(s->tmp_str) -1, "%d", s->file.len);
    //snprintf(s->str_tmp, 8, "%d", s->len);
    PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_200));
    ptr = strchr(s->filename, ISO_period);
    if(ptr != NULL && strncmp_P(ptr, http_shtml, 6) == 0) {
      PT_INIT(&s->scriptpt);
      PT_WAIT_THREAD(&s->outputpt, handle_script(s));
    } else {
      PT_WAIT_THREAD(&s->outputpt, send_file(s));
    }
  }

  PSOCK_CLOSE(&s->sout);
  PT_END(&s->outputpt);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_input(struct httpd_state *s))
{
  PSOCK_BEGIN(&s->sin);
#if defined(HTTP_POST_SUPPORT)
  int cont_len = 0;
#endif

  PSOCK_READTO(&s->sin, ISO_space);

  if(strncmp_P(s->inputbuf, http_get, 3) == 0) {
    s->request_type = GET;
#if defined(HTTP_POST_SUPPORT)
  } else if(strncmp_P(s->inputbuf, http_post, 4) == 0) {
    s->request_type = POST;
#endif
  } else {
    PSOCK_CLOSE_EXIT(&s->sin);
  }
  PSOCK_READTO(&s->sin, ISO_space);

  if(s->inputbuf[0] != ISO_slash) {
    PSOCK_CLOSE_EXIT(&s->sin);
  }

    /* read and store the file name */
    if(s->inputbuf[1] == ISO_space) {
        strncpy_P(s->filename, http_index_html, sizeof(s->filename));
    } else {
        s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;

#if defined(HTTP_GET_PARAM_SUPPORT)
    // note this code block taken from uhttpd-avr project
        int i;
        for(i=0 ; s->inputbuf[i]!=0 && s->inputbuf[i] != ISO_question ; i++);

        if( s->inputbuf[i] == ISO_question )
        {
            s->inputbuf[i]=0;
            strncpy(s->param,&s->inputbuf[i+1],sizeof(s->param));
            s->param_len = strlen(s->param);
        }
        else
        {
            s->param[0] = 0;
            s->param_len = 0;
        }
    // end note
#endif                                                      
        strncpy(s->filename, &s->inputbuf[0], sizeof(s->filename));
    }

  /*  httpd_log_file(uip_conn->ripaddr, s->filename);*/

  s->state = STATE_OUTPUT;

  /* read all of the clients input data */
  while(1) {
    PSOCK_READTO(&s->sin, ISO_nl);

#if defined(HTTP_POST_SUPPORT)
    if(s->request_type == POST) {
      if(cont_len != 0) {
          /* does this line contain any data */
          if(PSOCK_DATALEN(&s->sin) > 2) {
			s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
            strncpy(s->param,&s->inputbuf[0],sizeof(s->param));
            s->param_len = strlen(s->param);
			break;
          }
      }else if(strncmp_P(s->inputbuf, http_content_length, 15) == 0) {
        s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
        cont_len = atoi(&s->inputbuf[16]);
        if (cont_len > MAX_PARAM_DATA) {
            strncpy_P(s->filename, http_413_html, sizeof(s->filename));
			break;
        }
        s->param[0] = 0;
        s->param_len = 0;
      }
    }
#endif
#if 0 /* compile out but, leave as example */
    if(strncmp_P(s->inputbuf, http_referer, 8) == 0) {
      s->inputbuf[PSOCK_DATALEN(&s->sin) - 2] = 0;
      /*      httpd_log(&s->inputbuf[9]);*/
    }
#endif
  }
  
  PSOCK_END(&s->sin);
}
/*---------------------------------------------------------------------------*/
static void
handle_connection(struct httpd_state *s)
{
  handle_input(s);
  if(s->state == STATE_OUTPUT) {
    handle_output(s);
  }
}

int8_t alloc_state(void)
{
	int8_t found_state = -1;
	uint8_t index = 0;
	while (index < UIP_CONF_MAX_CONNECTIONS)
	{
		if (httpd_state_list[index].state == STATE_UNUSED)
		{
			found_state = index;
			break;
		}
		index++;
    }
								
	return (found_state);
}

/*---------------------------------------------------------------------------*/
void
httpd_appcall(void)
{
#if PORT_APP_MAPPER
	struct httpd_state *s;
#else
	struct httpd_state *s = (struct httpd_state *)&(uip_conn->appstate);
#endif


	if(uip_closed() || uip_aborted() || uip_timedout()) {
#if PORT_APP_MAPPER
//sendString("uip close/abort/timeout\r\n");
		if (uip_conn->appstate != -1)
		{
//sendString("uip close/abort/timeout - cleanup\r\n");
			httpd_state_list[((int8_t)uip_conn->appstate)].state = STATE_UNUSED;
			uip_conn->appstate = -1;
		}
#endif
	} else if(uip_connected()) {
#if PORT_APP_MAPPER
		if ((uip_conn->appstate = alloc_state()) == -1)
		{
			// we are out of state space.  close the connection
			// hope the client tries back again
//sendString("Out of http stats\r\n");
			uip_abort();
			return;
		}
		// set the app state
		s = &(httpd_state_list[((int8_t)uip_conn->appstate)]);
#endif
		PSOCK_INIT(&s->sin, s->inputbuf, sizeof(s->inputbuf) - 1);
		PSOCK_INIT(&s->sout, s->inputbuf, sizeof(s->inputbuf) - 1);
		PT_INIT(&s->outputpt);
		s->state = STATE_WAITING;
	    /*    timer_set(&s->timer, CLOCK_SECOND * 100);*/
		s->timer = 0;
		handle_connection(s);
#if PORT_APP_MAPPER
	} else if (uip_conn->appstate != -1) {
		s = &(httpd_state_list[((int8_t)uip_conn->appstate)]);
#else
	} else if(s != NULL) {
#endif
		if(uip_poll()) {
			++s->timer;
			// if the client just hasn't sent
			// anything in a while we end up here
			// this is where we can clean up dead connections.
			// 20 seems like a long time - if(s->timer >= 20) {
			// 16 seems to be a good number wtih 5 status
			if(s->timer >= 16) {
#if PORT_APP_MAPPER
				if (uip_conn->appstate != -1)
				{
					httpd_state_list[((int8_t)uip_conn->appstate)].state = STATE_UNUSED;
					uip_conn->appstate = -1;
				}
#endif
				uip_abort();
			}
    	} else {
			s->timer = 0;
		}
		handle_connection(s);
	} else {
#if PORT_APP_MAPPER
		if (uip_conn->appstate != -1)
		{
			httpd_state_list[((int8_t)uip_conn->appstate)].state = STATE_UNUSED;
			uip_conn->appstate = -1;
		}
#endif
		uip_abort();
	}
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Initialize the web server
 *
 *             This function initializes the web server and should be
 *             called at system boot-up.
 */
void
httpd_init(void)
{
#if PORT_APP_MAPPER
	uint8_t index = 0;
	while (index < HTTPD_MAX_CONNECTIONS)
	{
		httpd_state_list[index].state = STATE_UNUSED;
		index++;
	}
#endif
	uip_listen(HTONS(80));
}
/*---------------------------------------------------------------------------*/
/** @} */
