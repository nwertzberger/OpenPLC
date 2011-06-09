#if defined ENABLE_CGI_GET_SET_VALUE
static
PT_THREAD(getsetvalue_out(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_SEND_STR(&s->sout, getsetvalue_str_g);

  PSOCK_END(&s->sout);
}

static
PT_THREAD(getsetvalue_in(struct httpd_state *s, char *ptr))
{
  // the input buffer should contain the data so try to grab the name / var out
  //  s->inputbuf
  PSOCK_BEGIN(&s->sout);

  PSOCK_SEND_STR(&s->sout, getsetvalue_str_g);

  PSOCK_END(&s->sout);
}
#endif /* ENABLE_CGI_GET_SET_VALUE */

