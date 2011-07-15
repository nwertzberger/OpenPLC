#### larger chip needed for webserver.PROGMEM
MCU 	= atmega1284p
CHIP 	= m1284p

#Project name
NAME    = hilsim

#Compiled apps
APPS	= jsoncmd dhcpc

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
