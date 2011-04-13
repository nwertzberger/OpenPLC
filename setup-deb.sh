#!/bin/bash
# Developed by Nicholas Wertzberger
# Copyright 2011 University of Nebraska, Omaha
# This is released under the CC-AT-SA-3.0 license. Read about it here:
# http://creativecommons.org/licenses/by-sa/3.0/
# This script does everything you need to set up OpenPLC for a debian-based
# platform.

echo I\'m going to jsut do my thing... You should have looked me over before
echo you ran me...

# Pre-req's
apt-get install gputils
apt-get install libusb-dev

# PIC programmer
wget http://ww1.microchip.com/downloads/en/DeviceDoc/pk2cmdv1.20LinuxMacSource.tar.gz
tar -xzf pk2cmdv1.20LinuxMacSource.tar.gz
cd pk2cmdv1.20LinuxMacSource/
make linux
make install
cd ..
rm -r pk2cmdv1.20LinuxMacSource/
rm pk2cmdv1.20LinuxMacSource.tar.gz

# SDCC
# I use the latest source in the event that there is a compiler issue, but
# this should be sufficient (i think).
apt-get install sdcc


