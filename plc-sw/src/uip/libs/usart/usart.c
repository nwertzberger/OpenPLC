

#include "usart.h"

#ifdef UBRR0
  #if defined(UBRR1) && defined(ENABLE_UBRR1)
	  #include "usart2.c"
  #else
    #if defined(UBRR1)
	  #warning "Chip has two USART interfaces.  Only one enabled"
	#endif
	// we have two but, only use the first one
    #include "usart1.c"
  #endif
#else // only the single uart
  #include "one_usart.c"
#endif
