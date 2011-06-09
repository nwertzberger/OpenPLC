

/*---------------------------------------------------------------------------*/
#if defined ENABLE_CGI_FILE_STATS
static unsigned short
generate_file_stats(void *arg)
{
  char *f = (char *)arg;
  return (unsigned short) snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%5u", (unsigned int)httpd_fs_count(f));
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(file_stats_cgi(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_file_stats, strchr(ptr, ' ') + 1);
  
  PSOCK_END(&s->sout);
}
#endif  /* ENABLE_CGI_FILE_STATS */
/*---------------------------------------------------------------------------*/
