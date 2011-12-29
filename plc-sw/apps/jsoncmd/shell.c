#include "apps-conf.h"
#include "jsoncmd.h"
#include "shell.h"

#include <string.h>

// Constants
#define NUM_TOKENS 20

// function prototypes
static void inttostr(register char *str, unsigned int i);
static void default_callback(int * vals);

static int values[8];

void shell_init(void)
{
}

void shell_start(void)
{
}

// I need this to work yesterday... I'm sorry!
void shell_input(char * js)
{
    char * cmd = strchr(js, '"') + 1;
    if (strncmp(cmd,"getvalues", 4) == 0) {
        JSONCMD_CONF_GETCALLBACK(values);
        shell_prompt("{\"response\":\"OK\",\"values\":[");
        for (int i=0; i < 8; i++) {
            char buf[10];
            inttostr(buf, values[i]);
            shell_prompt(buf);
            if ( i < 7 ) {
                shell_prompt(",");
            }
        }
        shell_output("]}", "");
    }
    else if (strncmp(cmd, "setvalues", 4) == 0) {
        cmd = strchr(cmd, '[')+1;
        for( int i=0; i < 8; i++) {
            sscanf(cmd, "%d", &values[i]);
            cmd = strchr(cmd, ',') + 1;
        }
        JSONCMD_CONF_SETCALLBACK(values);
        shell_output("{\"response\":\"OK\"}","");

    }
    else {
        shell_output("{\"response\":\"error\"}","");
    }
}

/**
 * Does nothing, but saves the build if the user hasn't defined this yet...
 */
static void default_callback(int * vals)
{
}

// used to convert integers into strings...  Like printf!
static void inttostr(register char *str, unsigned int i)
{
    str[0] = '0' + i / 100;
    if(str[0] == '0') {
        str[0] = ' ';
    }
    str[1] = '0' + (i / 10) % 10;
    if(str[0] == ' ' && str[1] == '0') {
        str[1] = ' ';
    }
    str[2] = '0' + i % 10;
    str[3] = 0;
}

