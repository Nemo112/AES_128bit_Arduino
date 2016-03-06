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
port.write(b'e')
#port.write(bytes([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15])) # ot
#"0123456701234567"
print "ot: ", "".join("{:02x}".format(ord(c)) for c in "0123456701234567") # št
print "ot: ", ", ".join("{:02x}".format(ord(c)) for c in "0123456701234567") # št
port.write("0123456701234567") # ot
out = port.read(16)
print "cipher", out
print "cipher: ", ", ".join("{:02x}".format(ord(c)) for c in out) # št
port.write(b'd')
port.write(bytes(out))
out = port.read(16)
print "open: ", out # ot

