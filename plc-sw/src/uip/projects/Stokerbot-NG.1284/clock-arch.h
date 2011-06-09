#ifndef __CLOCK_ARCH_H__
#define __CLOCK_ARCH_H__

//#include "global-conf.h"
#include <stdint.h>

typedef uint16_t clock_time_t;
#define CLOCK_CONF_SECOND		500
//tick=2ms
#include "clock.h"

void clock_init(void);
clock_time_t clock_time(void);

uint16_t tickDiff(uint16_t oldtick);
uint16_t tickDiffS(uint16_t oldtick);

#endif /* __CLOCK_ARCH_H__ */
