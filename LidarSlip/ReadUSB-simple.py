#!/usr/bin/python3

from sliplib import decode

import serial
import socket
import glob

UDP_IP="127.0.0.1"
UDP_PORT=5566

serial_ports = list()

for dev in glob.glob("/dev/ttyUSB*"):
	print("Connect to " + dev)
	serial.Serial(dev, 115200, timeout=0.01)

ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=0.01)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

while True:
	data = ser.read(128)
	decoded = decode(data)
	sock.sendto(decoded, (UDP_IP, UDP_PORT))
	print(decoded)