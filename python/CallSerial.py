#!/usr/bin/python
import serial
import sys
from optparse import OptionParser


if __name__ == "__main__":
	parser = OptionParser(usage="usage: %prog [args]")
	parser.add_option("-e", "--encrypt", action="store_true", dest="enc", default=False, help="Encrypt")
	parser.add_option("-d", "--decrypt", action="store_true", dest="dec", default=False, help="Decrypt")
	parser.add_option("-i", "--get-input-file", action="store", type="string", dest="source", help="Source file")
	parser.add_option("-o", "--get-output-file", action="store", type="string", dest="destin", help="Destination file")
	parser.add_option("-p", "--arduino-port", action="store", type="string", dest="ardport", help="Arduino port")
	parser.add_option("-k", "--key-file", action="store", type="string", dest="keyin", help="Destination file")
	(args, opts) = parser.parse_args()

	if args.keyin:
		if args.ardport:
			port = serial.Serial(args.ardport, baudrate=9600, timeout=3.0)
			fs = open(args.keyin, 'rb')
			port.write(b'k')
			datain = fs.read(1)
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
			while 1:
				port.write(b'e')
				datain = fs.read(1)
				i = 0
				while datain != "" and i < 16:
					print(datain)
					port.write(datain)
					datain = fs.read(1)
					i += 1
				while i%16 != 0 and i != 0:
					print("pridal")				
					print(bytes([0]))				
					port.write(bytes([0]))
					i += 1
				rcv = port.read(16)
				print "A" , rcv
				fd.write(rcv)
				if datain == "":
					break
    		
	elif args.enc == True:
		if args.source and args.destin and args.ardport:
			port = serial.Serial(args.ardport, baudrate=9600, timeout=3.0)
			fs = open(args.source, 'rb')
			fd = open(args.destin, 'w')
			datain = fs.read(16)
			while len(datain):
				if len(datain) < 16:
					for i in range(0,16-len(datain)):
						datain += "0"
				datain = fs.read(16)
				port.write("d" + datain)
				rcv = port.read(16)
				fd.write(rcv)
				


