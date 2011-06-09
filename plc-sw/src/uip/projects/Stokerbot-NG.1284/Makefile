#Chip choice
MCU	= atmega1284p
CHIP	= m1284
F_CPU = 20000000 

#Project name
NAME    = AVRUIP

#Compiled apps
APPS    = webserver.PROGMEM webclient.Stokerbot-NG resolv port_app_mapper

#APPS	= webserver.Stokerbot-NG 
#APPS	= webserver.Stokerbot-NG webclient.Stokerbot-NG resolv.Stokerbot-NG lcd
#dhcp ntp

#Compiled drivers
DEVS	= enc28j60 #The rest are determined by makefiles for apps

#Compiled libs
LIBS	+= misc 1wire analog eeprom uart 
#LIBS	+= misc 1wire analog eeprom uart lcd
#wireless lcd i2c-eeprom i2c-adc i2c-io

#Project specific files
$(NAME)_SOURCES = main.c clock-arch.c
#$(NAME)_SOURCES = main.c clock-arch.c gateway.c

CFLAGS	= -I. -Wall -mmcu=$(MCU) -g -fpack-struct -Os -funsigned-char -funsigned-bitfields -mcall-prologues -Wstrict-prototypes -std=gnu99 -DF_CPU=$(F_CPU)

#Include global configurations
-include ../../MakeConf.include

#Include the genetic makefile
-include ../../Makefile.include

