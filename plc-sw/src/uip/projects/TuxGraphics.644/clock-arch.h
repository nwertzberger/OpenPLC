#ifndef __CLOCK_ARCH_H__
#define __CLOCK_ARCH_H__

#include <stdint.h>

typedef uint16_t clock_time_t;
#define CLOCK_CONF_SECOND		(clock_time_t)47
								//(F_CPU / (1024*255)), this cannot be used as it gives overflows
								//Freqency divided prescaler and counter register size
                                // so at 12500000 its 47.87
                                // so at 25000000 its 95.74 --- which is too fast. 

#include "clock.h"

#endif /* __CLOCK_ARCH_H__ */
