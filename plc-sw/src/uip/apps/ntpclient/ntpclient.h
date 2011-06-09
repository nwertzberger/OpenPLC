/*
 * ntpclient.h
 */
#ifndef __NTPCLIENT_H__
#define __NTPCLIENT_H__

#include "timer.h"
#include "pt.h"

#include "global-conf.h"

#define NTP_STATE_NO_TIME         0
#define NTP_STATE_INITIALIZED     1
#define NTP_STATE_SYNCHONIZED     2

struct ntp_time {
  uint32_t seconds;   // full seconds since 1900
  uint16_t fraction;  // fractions of a second in x/65536
};

// this type describes the correction factor generated
// from the ntp time signal
typedef int16_t ntp_adjust_t;

struct ntpclient_state {
  // data required for ntp communication
  struct pt pt;
  struct uip_udp_conn *conn;
  u8_t state;
  struct timer timer;
  u16_t ticks;

  // data required to keep track of the exact time
  struct ntp_time last_sync;
  ntp_adjust_t adjust;
};

struct ntp_tm {
  // date
  uint16_t year;   // year
  uint8_t month;   // month within year (1..12)
  uint8_t day;     // day within month (1..31)
  uint8_t weekday; // day within week (0..6)
  // time
  uint8_t hour;    // hour within day (0..23)
  uint8_t minute;  // minute within hour (0..59)
  uint8_t second;  // second within minute (0..59)

  uint8_t dst;     // daylight savings time (+1 hour)
};

extern void ntpclient_init(void);
extern void ntpclient_appcall(void);
extern void ntp_dst(struct ntp_tm *tm);
extern void ntp_tmtime(uint32_t sec, struct ntp_tm *tm);

// to be provided by clock-arch:
extern void clock_get_ntptime(struct ntp_time *time);
extern void clock_set_ntptime(struct ntp_time *time);


#if defined PORT_APP_MAPPER
	#define NTPCLIENT_APP_CALL_MAP {dhcpc_appcall, 0, 123},
#else
	#define NTPCLIENT_APP_CALL_MAP
	#define UIP_UDP_APPCALL ntpclient_appcall
	typedef struct ntpclient_state uip_udp_appstate_t;
#endif

#ifndef NTP_TZ
#error "You must define NTP_TZ to point to your timezone offset number variable"
#endif



#endif /* __NTPCLIENT_H__ */
