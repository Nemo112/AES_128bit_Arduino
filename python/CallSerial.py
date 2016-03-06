#!/usr/bin/python
import serial
import sys
from optparse import OptionParser
import os

if __name__ == "__main__":
	parser = OptionParser(usage="usage: %prog [args]")
	parser.add_option("-e", "--encrypt", action="store_true", dest="enc", default=False, help="Encrypt")
	parser.add_option("-d", "--decrypt", action="store_true", dest="dec", default=False, help="Decrypt")
	parser.add_option("-i", "--get-input-file", action="store", type="string", dest="source", help="Source file")
	parser.add_option("-o", "--get-output-file", action="store", type="string", dest="destin", help="Destination file")
	parser.add_option("-p", "--arduino-port", action="store", type="string", dest="ardport", help="Arduino port")
	parser.add_option("-k", "--key-file", action="store", type="string", dest="keyin", help="Destination file")
	(args, opts) = parser.parse_args()
	if args.ardport:
		# synchronizace
		port = serial.Serial(args.ardport, baudrate=9600, timeout=3.0)
		ts = port.read(16)
		while ts == " ":
			port.write(b'e')
			ts = port.read(16)

	if args.keyin:
		if args.ardport:
			fs = open(args.keyin, 'rb')
			port.write(b'k')
			datain = fs.read(16)
			i = 0
			while datain != "":
				port.write(datain)
				i += 1
			while i < 16:
				port.write(0)
				i += 1

	if args.dec == True:
		if args.source and args.destin and args.ardport:
			port = serial.Serial(args.ardport, baudrate=9600, timeout=3.0)
			fs = open(args.source, 'rb')
			fd = open(args.destin, 'wb')
			size = os.path.getsize(args.source)
			k = 0
			while 1:
				port.write(b'd')
				i = 0
				while i < 16:
					datain = fs.read(1)
					if len(datain) == 0:
						break
					#print(bytes(datain), i)
					port.write(datain)
					i += 1
				# print i
				while i%16 != 0 and i != 0:
					#print(i, " pridal ", bytes([0]))				
					port.write(b"\x00")
					i += 1
				rcv = port.read(16)
				#print "A" , rcv
				k += 16
				#print k, " " , size
				if k == size:
					j = len(rcv) - 1
					while j > 0 and rcv[j] == b"\x00":
						j -= 1
					rcv = rcv[0:j+1]
					if datain == "":
						break
				fd.write(rcv)
				if datain == "":
					break
			print "Done!"
    		
	elif args.enc == True:
		if args.source and args.destin and args.ardport:
			port = serial.Serial(args.ardport, baudrate=9600, timeout=3.0)
			fs = open(args.source, 'rb')
			fd = open(args.destin, 'wb')
			while 1:
				port.write(b'e')
				i = 0
				while i < 16:
					datain = fs.read(1)
					if len(datain) == 0:
						break
					#print(bytes(datain), i)
					port.write(datain)
					i += 1
				# print i
				while i%16 != 0 and i != 0:
					#print(i, " pridal ", bytes([0]))				
					port.write(b"\x00")
					i += 1
				rcv = port.read(16)
				#print "A" , rcv
				fd.write(rcv)
				if datain == "":
					break
			print "Done!"
				


