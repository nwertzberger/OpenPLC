

#ifndef __USART_H__
#define __USART_H__

#include <avr/io.h>

#include "global-conf.h"

#ifdef UBRR0
  #if defined(UBRR1) && defined(ENABLE_UBRR1)
	  #include "usart2.h"
  #else
    #if defined(UBRR1)
	  #warning "Chip has two USART interfaces.  Only one enabled"
	#endif
	// we have two but, only use the first one
    #include "usart1.h"
  #endif
#else // only the single uart
  #include "one_usart.h"
#endif

#ifdef USART_EXTRA_SEND_FORMATS
  #warning "Turning on will cost you an extra 1150 bytes!!!" 
  #warning "  If you use the basic functions it's only about 90 bytes!!!" 
#endif

#endif // end - #ifndef __USART_H__
