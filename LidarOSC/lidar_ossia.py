#!/bin/python3

import serial

ser0 = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

while(True):
	line = ser0.readline()
	print(line)