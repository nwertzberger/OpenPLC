BASEDIR	= ..
FILES	= cdc.c usb_stack.c
LIBS	=
TARGET	= $(BASEDIR)/lib/usbstack.lib


include ../Makefile.include

$(TARGET): $(FILES:.c=.o)
	$(AR) $(ARFLAGS) $@ $^
