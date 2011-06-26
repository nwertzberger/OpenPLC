# Programmer: Nicholas Wertzberger
#      Email: wertnick@gmail.com

CC=avr-gcc
AR=avr-ar
PLATFORM=atmega1284p
TARGET=qtest

CFLAGS= -Wall -mmcu=$(PLATFORM) $(FFLAGS)
LFLAGS= -Wall -mmcu=$(PLATFORM) $(FFLAGS)
FFLAGS= -O0

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex $< $@
$(TARGET).elf: quicktest.c
	$(CC) $(CFLAGS) -o $@ $^

deploy: $(TARGET).hex
	sudo avrdude -c avrisp2 -p m1284p -P usb -b19200 -B 2 -U flash:w:$< -F

shell:
	sudo avrdude -c avrisp2 -p m1284p -P usb -t
