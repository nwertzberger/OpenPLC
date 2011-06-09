#ifndef __CLOCK_ARCH_H__
#define __CLOCK_ARCH_H__

#include <stdint.h>

typedef uint16_t clock_time_t;
#define CLOCK_CONF_SECOND		(clock_time_t)47
								//(F_CPU / (1024*255)), this cannot be used as it gives overflows
								//Freqency divided prescaler and counter register size

#include "clock.h"

#endif /* __CLOCK_ARCH_H__ */
