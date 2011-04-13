BASEDIR	= ..
FILES	= cdc.c usb_stack.c main.c
LIBS	= -larch.lib
TARGET	= test_usb.hex

include ../Makefile.include

test_usb.hex: main.o cdc.o usb_stack.o
	$(CC) $(LDFLAGS) $^ -o $@

$(TARGET): $(FILES:.c=.o)
	$(AR) $(ARFLAGS) $@ $^
