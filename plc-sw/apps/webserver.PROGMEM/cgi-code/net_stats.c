#if defined ENABLE_CGI_NET_STATS
#if UIP_STATISTICS
static unsigned short
generate_net_stats(void *arg)
{
  struct httpd_state *s = (struct httpd_state *)arg;
  return (unsigned short)snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,
		  "%5u\n", ((uip_stats_t *)&uip_stat)[s->count]);
}
#endif /* UIP_STATISTICS */

static
PT_THREAD(net_stats_cgi(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

#if UIP_STATISTICS

  for(s->count = 0; s->count < sizeof(uip_stat) / sizeof(uip_stats_t);
      ++s->count) {
    PSOCK_GENERATOR_SEND(&s->sout, generate_net_stats, s);
  }
  
#endif /* UIP_STATISTICS */
  
  PSOCK_END(&s->sout);
}
#endif  /* ENABLE_CGI_NET-STATS */ 
