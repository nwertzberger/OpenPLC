#Chip choice
#MCU		= atmega168
#CHIP	= m168
#### larger chip needed for webserver.PROGMEM
MCU	= atmega644
CHIP	= m644p

#Project name
NAME    = TuxGraphics.644

#Compiled apps
#APPS	= simple-httpd
#### webserver.PROGMEM needs 19k of RAM for the default setup
#APPS	= webserver.PROGMEM  
#APPS	= dhcpc telnetd webserver.PROGMEM port_app_mapper
APPS	= dhcpc webserver.PROGMEM port_app_mapper
#APPS	= dhcpc simple-httpd

HTTPD_FS_DIRNAME = httpd-fs
#HTTPD_FS_DIRNAME = httpd-fs.netsetup

#Compiled drivers
DEVS	= enc28j60 #The rest are determined by makefiles for apps

#Compiled libs
#LIBS	= #Determined by makefiles for apps or drivers
#LIBS += misc 1wire analog eeprom 
#LIBS += usart
LIBS += 

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
