/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 *
 * Some common utilities needed for IP and web applications
 *********************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// search for a string of the form key=value in
// a string that looks like q?xyz=abc&uvw=defgh HTTP/1.1\r\n
//
// The returned value is stored in strbuf. You must allocate
// enough storage for strbuf, maxlen is the size of strbuf.
// I.e the value it is declated with: strbuf[5]-> maxlen=5
uint8_t find_key_val(char *str,char *strbuf, uint8_t maxlen,char *key)
{
        uint8_t found=0;
        uint8_t i=0;
        char *kp;
        kp=key;
        while(*str &&  *str!=' ' && *str!='\n' && found==0){
                if (*str == *kp){
                        kp++;
                        if (*kp == '\0'){
                                str++;
                                kp=key;
                                if (*str == '='){
                                        found=1;
                                }
                        }
                }else{
                        kp=key;
                }
                str++;
        }
        if (found==1){
                // copy the value to a buffer and terminate it with '\0'
                while(*str &&  *str!=' ' && *str!='\n' && *str!='&' && i<maxlen-1){
                        *strbuf=*str;
                        i++;
                        str++;
                        strbuf++;
                }
                *strbuf='\0';
        }
        // return the length of the value
        return(i);
}


/*
 *  This function makes use of and alters the strbuf contents.
 */
/*uint8_t split_key_value(char *str,char *strbuf, uint8_t maxlen,char *key)
{

}*/
