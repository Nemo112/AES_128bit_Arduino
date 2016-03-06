#!/usr/bin/python
# -*- coding: utf-8 -*-
import serial
import sys

port = serial.Serial("/dev/ttyUSB0", baudrate=9600, timeout=3.0)
# synchronizace
ts = port.read(16)
while ts == " ":
	port.write(b'e')
	ts = port.read(16)
# spuštění šifry
print b'k', bytes([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15])
#port.write(b'k')
#port.write(bytes([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15])) # ot
#out = port.read(16)
print "key: ", ", ".join("{:02x}".format(ord(c)) for c in bytes([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15])) # št
#out = port.read(16)
#print "open: ", out # ot

