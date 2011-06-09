#Chip choice
#MCU		= atmega168
#CHIP	= m168
#### larger chip needed for webserver.PROGMEM
MCU = atmega328p
CHIP   = m328p

#Project name
NAME    = test

#Compiled apps
APPS	= simple-httpd
#### webserver.PROGMEM needs 19k of RAM for the default setup
#APPS	= webserver.PROGMEM
#APPS	= dhcpc webserver.PROGMEM

#Compiled drivers
DEVS	= enc28j60 #The rest are determined by makefiles for apps

#Compiled libs
#LIBS	= #Determined by makefiles for apps or drivers

#Project specific files
$(NAME)_SOURCES = main.c clock-arch.c

#Low fuse
LFUSE	= 0x60	

#High fuse
HFUSE	= 0xDF

#Include global configurations
-include ../../MakeConf.include

#Include the genetic makefile
-include ../../Makefile.include
