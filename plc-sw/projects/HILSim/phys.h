/**
 * Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * This class is responsible for setting and getting values for the physical IO.
 *
 *
 *
 */
#ifndef PHYS_H
#define PHYS_H

#define JSONCMD_CONF_GETCALLBACK phys_in
#define JSONCMD_CONF_SETCALLBACK phys_out

void phys_init(void);
void phys_in(int * data);
void phys_out(int * data);

#endif
