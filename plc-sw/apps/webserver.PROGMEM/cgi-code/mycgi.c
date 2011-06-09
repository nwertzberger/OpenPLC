#if defined ENABLE_CGI_MYCGI
static
PT_THREAD(mycgi_out(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_SEND_STR(&s->sout, "Hello World, From a cgi.");
  
  PSOCK_END(&s->sout);
}
#endif  /* ENABLE_CGI_MYCGI */
