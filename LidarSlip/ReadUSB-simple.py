#!/usr/bin/python3

from sliplib import decode
import serial
import socket
import sys

dev="/dev/ttyUSB0"

if len(sys.argv) > 1:
	dev=sys.argv[1]

UDP_IP="192.168.32.13"
UDP_PORT=5566

print("Connecting to " + dev)
ser = serial.Serial(dev, 115200, timeout=0.01)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

while True:
	try:
		data = ser.read(128)
		decoded = decode(data)
		sock.sendto(decoded, (UDP_IP, UDP_PORT))
		# print(decoded)
	except:
		# print("error reading " + ser.port)
		pass
