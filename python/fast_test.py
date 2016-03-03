#!/usr/bin/python
# -*- coding: utf-8 -*-
import serial
import sys

port = serial.Serial("/dev/ttyUSB1", baudrate=9600, timeout=3.0)
# synchronizace
ts = port.read(16)
while ts == " ":
	port.write(b'e')
	ts = port.read(16)
# spuštění šifry
port.write(b'e')
port.write("1234567887654321") # ot
out = port.read(16)
print "cipher: ", ", ".join("{:02x}".format(ord(c)) for c in out) # št
port.write(b'd')
port.write(bytes(out))
out = port.read(16)
print "open: ", out # ot

