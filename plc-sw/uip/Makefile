BASEDIR	=..
FILES	=uip.c uip_arp.c uiplib.c psock.c timer.c uip-neighbor.c
LIBS	=
TARGET	=../lib/uip.lib

include $(BASEDIR)/Makefile.include

../lib/uip.lib: $(FILES:.c=.o)
	$(AR) a $@ $^
