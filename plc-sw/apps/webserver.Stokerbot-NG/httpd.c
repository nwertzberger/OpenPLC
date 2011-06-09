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
//#include "httpd-cgi.h"
#include "http-strings.h"
#include "config.h"
#include "ds18x20.h"

#include <string.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#define STATE_WAITING 0
#define STATE_OUTPUT  1

#define ISO_nl      0x0a
#define ISO_space   0x20
#define ISO_bang    0x21
#define ISO_percent 0x25
#define ISO_period  0x2e
#define ISO_slash   0x2f
#define ISO_colon   0x3a

/*---------------------------------------------------------------------------*/
static unsigned short
generate_part_of_file(void *state) {
    struct httpd_state *s = (struct httpd_state *) state;

    if (s->file.len > uip_mss()) {
        s->len = uip_mss();
    } else {
        s->len = s->file.len;
    }
    memcpy_P(uip_appdata, s->file.data, s->len);

    return s->len;
}
/*
static unsigned short
generate_script_part_of_file(void *state) {
    struct httpd_state *s = (struct httpd_state *) state;

    uint8_t filelength = s->file.len;
    uint16_t filepos = 0;

//Simple way to leave 100bytes per 1400bytes for script... need tweaking
    if (s->file.len > (uip_mss()-100))
	filelength = uip_mss()-100;

   uint16_t *fileptr = s->file.data;
   while (filelength > 0)
   {
      char c = pgm_read_byte(fileptr);
      if (c == '$')
      {
	//Found script var
	char script[20];
	memset(script, '\0', 20);
        uint8_t scriptpos = 1;
	bool endfound = false;

	while (scriptpos < 20 && !endfound)
	{
		c = pgm_read_byte(fileptr+scriptpos);
		if (c == '$')
			endfound = true;
		else
			script[scriptpos-1] = c;
	}
	if (endfound)
	{
		printf("Script variable is %s \r\n", script);
		//place result from script into uip_appdata
		fileptr += scriptpos; //Advance pointer to after the script tag
		s->len += scriptpos;
		continue;
	}
	//else didnt find end of script, probaly not a script afterall
      }
      uip_appdata[filepos] = c;
      s->len++;
   }

    return s->len;
}
*/
/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_file(struct httpd_state *s)) {
    PSOCK_BEGIN(&s->sout);

    do {
        if (s->script)
            PSOCK_GENERATOR_SEND(&s->sout, generate_part_of_file, s);
        else
            PSOCK_GENERATOR_SEND(&s->sout, generate_part_of_file, s);
        s->file.len -= s->len;
        s->file.data += s->len;
    } while (s->file.len > 0);

    PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(send_part_of_file(struct httpd_state *s)) {
    PSOCK_BEGIN(&s->sout);

    PSOCK_SEND(&s->sout, s->file.data, s->len);

    PSOCK_END(&s->sout);
}

static
PT_THREAD(send_headers(struct httpd_state *s, const char *statushdr)) {
    char *ptr;

    PSOCK_BEGIN(&s->sout);

    PSOCK_SEND_STR(&s->sout, statushdr);

    ptr = strrchr(s->filename, ISO_period);
    if (ptr == NULL) {
        s->script = false;
        PSOCK_SEND_STR(&s->sout, http_content_type_binary);
    } else if (strncmp(http_htm, ptr, 4) == 0 || strncmp(http_shtml, ptr, 6) == 0) {
        s->script = true;
        PSOCK_SEND_STR(&s->sout, http_content_type_html);
    } else if (strncmp(http_css, ptr, 4) == 0) {
        s->script = false;
        PSOCK_SEND_STR(&s->sout, http_content_type_css);
    } else if (strncmp(http_png, ptr, 4) == 0) {
        s->script = false;
        PSOCK_SEND_STR(&s->sout, http_content_type_png);
    } else if (strncmp(http_gif, ptr, 4) == 0) {
        s->script = false;
        PSOCK_SEND_STR(&s->sout, http_content_type_gif);
    } else if (strncmp(http_jpg, ptr, 4) == 0) {
        s->script = false;
        PSOCK_SEND_STR(&s->sout, http_content_type_jpg);
    } else {
        s->script = true;
        PSOCK_SEND_STR(&s->sout, http_content_type_plain);
    }
    PSOCK_END(&s->sout);
}


static
PT_THREAD(send_sensor_list(struct httpd_state *s)) {
    static uint16_t i;
    PSOCK_BEGIN(&s->sout);

        //script output
        bool first = true;
        PSOCK_SEND_STR(&s->sout, "var sensors=[");

        for (i=0; i < MAXSENSORS; i++) {
            if (sensorValues[(i * SENSORSIZE) + FAMILY] != 0) {
                if (!first)
                    PSOCK_SEND_STR(&s->sout, ",");
                else
                    first = false;

                int frac = sensorValues[(i * SENSORSIZE) + VALUE2] * DS18X20_FRACCONV; //Ganger de sidste par bits, med det step DS18B20 bruger

                sprintf_P(tempbuf, PSTR("[%u, '%02X%02X%02X%02X%02X%02X%02X%02X', '%c%d.%04d']"), i,
                        sensorValues[(i * SENSORSIZE) + FAMILY],
                        sensorValues[(i * SENSORSIZE) + ID1],
                        sensorValues[(i * SENSORSIZE) + ID2],
                        sensorValues[(i * SENSORSIZE) + ID3],
                        sensorValues[(i * SENSORSIZE) + ID4],
                        sensorValues[(i * SENSORSIZE) + ID5],
                        sensorValues[(i * SENSORSIZE) + ID6],
                        sensorValues[(i * SENSORSIZE) + CRC],
                        sensorValues[(i * SENSORSIZE) + SIGN],
                        sensorValues[(i * SENSORSIZE) + VALUE1],
                        frac
                        );

                PSOCK_SEND_STR(&s->sout, tempbuf);
            }
        }

        PSOCK_SEND_STR(&s->sout, "];");

    PSOCK_END(&s->sout);
}

static
PT_THREAD(send_ajax_list(struct httpd_state *s)) {
    static uint16_t i;
    PSOCK_BEGIN(&s->sout);

        //script output
        PSOCK_SEND_STR(&s->sout, "1-wire<br>");

        for (i=0; i < MAXSENSORS; i++) {
            if (sensorValues[(i * SENSORSIZE) + FAMILY] != 0) {
                int frac = sensorValues[(i * SENSORSIZE) + VALUE2] * DS18X20_FRACCONV; //Ganger de sidste par bits, med det step DS18B20 bruger

                sprintf_P(tempbuf, PSTR("%02X%02X%02X%02X%02X%02X%02X%02X: %c%d.%04d<br>"),
                        sensorValues[(i * SENSORSIZE) + FAMILY],
                        sensorValues[(i * SENSORSIZE) + ID1],
                        sensorValues[(i * SENSORSIZE) + ID2],
                        sensorValues[(i * SENSORSIZE) + ID3],
                        sensorValues[(i * SENSORSIZE) + ID4],
                        sensorValues[(i * SENSORSIZE) + ID5],
                        sensorValues[(i * SENSORSIZE) + ID6],
                        sensorValues[(i * SENSORSIZE) + CRC],
                        sensorValues[(i * SENSORSIZE) + SIGN],
                        sensorValues[(i * SENSORSIZE) + VALUE1],
                        frac
                        );

                PSOCK_SEND_STR(&s->sout, tempbuf);
            }
        }

        PSOCK_SEND_STR(&s->sout, "<br>");

    PSOCK_END(&s->sout);
}

static
PT_THREAD(send_var_list(struct httpd_state *s)) {
    PSOCK_BEGIN(&s->sout);

    //script output
    sprintf(tempbuf, "var FIRM_MAJOR=%u;\r\nvar FIRM_MINOR=%u;\r\n", SBNG_VERSION_MAJOR,SBNG_VERSION_MINOR);
    PSOCK_SEND_STR(&s->sout, tempbuf);
    sprintf(tempbuf, "var SYSID='%02X%02X%02X%02X%02X%02X%02X%02X';\r\n", systemID[0], systemID[1], systemID[2], systemID[3], systemID[4], systemID[5], systemID[6], systemID[7]);
    PSOCK_SEND_STR(&s->sout, tempbuf);

    PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_output(struct httpd_state *s)) {
    PT_BEGIN(&s->outputpt);

    if (strncmp(s->filename, "/slist.js",9) == 0) {
        PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_200));
        PT_WAIT_THREAD(&s->outputpt, send_sensor_list(s));
    } else if (strncmp(s->filename, "/ajax_slist.htm",15) == 0) {
        PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_200));
        PT_WAIT_THREAD(&s->outputpt, send_ajax_list(s));
    } else if (strncmp(s->filename, "/var.js",7) == 0) {
        PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_200));
        PT_WAIT_THREAD(&s->outputpt, send_var_list(s));
    } else {
        if (!httpd_fs_open(s->filename, &s->file)) {
            httpd_fs_open(http_404_html, &s->file);
            strcpy(s->filename, http_404_html);
            PT_WAIT_THREAD(&s->outputpt,
                    send_headers(s,
                    http_header_404));
            
            PT_WAIT_THREAD(&s->outputpt,
                    send_file(s));
        } else {
            PT_WAIT_THREAD(&s->outputpt,
                    send_headers(s,
                    http_header_200));

            PT_WAIT_THREAD(&s->outputpt,
                    send_file(s));
        }
    }
    PSOCK_CLOSE(&s->sout);
    PT_END(&s->outputpt);
}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(handle_input(struct httpd_state *s)) {
    PSOCK_BEGIN(&s->sin);

    PSOCK_READTO(&s->sin, ISO_space);


    if (strncmp(s->inputbuf, http_get, 4) != 0) {
        PSOCK_CLOSE_EXIT(&s->sin);
    }
    PSOCK_READTO(&s->sin, ISO_space);

    if (s->inputbuf[0] != ISO_slash) {
        PSOCK_CLOSE_EXIT(&s->sin);
    }

    if (s->inputbuf[1] == ISO_space) {
        strncpy(s->filename, http_index_html, sizeof (s->filename));
    } else {
        s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
        strncpy(s->filename, &s->inputbuf[0], sizeof (s->filename));
    }

    /*  httpd_log_file(uip_conn->ripaddr, s->filename);*/

    s->state = STATE_OUTPUT;

    while (1) {
        PSOCK_READTO(&s->sin, ISO_nl);

        if (strncmp(s->inputbuf, http_referer, 8) == 0) {
            s->inputbuf[PSOCK_DATALEN(&s->sin) - 2] = 0;
            /*      httpd_log(&s->inputbuf[9]);*/
        }
    }

    PSOCK_END(&s->sin);
}

/*---------------------------------------------------------------------------*/
static void
handle_connection(struct httpd_state *s) {
    handle_input(s);
    if (s->state == STATE_OUTPUT) {
        handle_output(s);
    }
}

/*---------------------------------------------------------------------------*/
void
httpd_appcall(void) {
    struct httpd_state *s = (struct httpd_state *) &(uip_conn->appstate);

    if (uip_closed() || uip_aborted() || uip_timedout()) {
    } else if (uip_connected()) {
        PSOCK_INIT(&s->sin, s->inputbuf, sizeof (s->inputbuf) - 1);
        PSOCK_INIT(&s->sout, s->inputbuf, sizeof (s->inputbuf) - 1);
        PT_INIT(&s->outputpt);
        s->state = STATE_WAITING;
        /*    timer_set(&s->timer, CLOCK_SECOND * 100);*/
        s->timer = 0;
        handle_connection(s);
    } else if (s != NULL) {
        if (uip_poll()) {
            ++s->timer;
            if (s->timer >= 20) {
                uip_abort();
            }
        } else {
            s->timer = 0;
        }
        handle_connection(s);
    } else {
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
httpd_init(void) {
    uip_listen(HTONS(80));
}
/*---------------------------------------------------------------------------*/
/** @} */
