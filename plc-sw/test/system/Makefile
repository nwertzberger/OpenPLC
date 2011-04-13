BASEDIR = ../..
FILES 	= test_startup.c test_io.c
TARGET 	= all
LIBS	= -larch.lib -lusbstack.lib

include $(BASEDIR)/Makefile.include

all: test_startup.hex test_io.hex test_usb.hex

test_startup.hex: test_startup.o
	$(CC) $(LDFLAGS) $^ -o $@

test_io.hex: test_io.o
	$(CC) $(LDFLAGS) $^ -o $@

test_usb.hex: test_usb.o
	$(CC) $(LDFLAGS) $^ -o $@
