BASEDIR	= ../..
FILES	= arch_startup.c arch_io.c
LIBS	=
TARGET	= $(BASEDIR)/lib/arch.lib


include ../../Makefile.include

$(TARGET): $(FILES:.c=.o)
	$(AR) $(ARFLAGS) $@ $^
