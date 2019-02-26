#!/usr/bin/python3

from sliplib import decode

import serial
import socket
import glob

UDP_IP="192.168.32.13"
UDP_PORT=5566

serial_ports = list()

for dev in glob.glob("/dev/ttyUSB*"):
	print("Connect to " + dev)
	serial_ports.append(serial.Serial(dev, 115200, timeout=0.01))


sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

print("Start reading...")

while True:
	for ser in serial_ports:
		# print("read " + ser.port)
		data = ser.read(128)
		decoded = decode(data)
		sock.sendto(decoded, (UDP_IP, UDP_PORT))
		# print(decoded)