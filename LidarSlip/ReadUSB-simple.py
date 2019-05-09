#!/usr/bin/python3

import sliplib
import serial
import socket
import sys

dev="/dev/ttyUSB0"

if len(sys.argv) > 1:
	dev="/dev/"+sys.argv[1]

UDP_IP="192.168.32.13"
#UDP_IP="127.0.0.1"
UDP_PORT=5566

print("Connecting to " + dev)
ser = serial.Serial(dev, 115200, timeout=0.01)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

while True:
	try:
		data = ser.read(128)
		decoded = sliplib.decode(data)
		sock.sendto(decoded, (UDP_IP, UDP_PORT))
		# print(decoded)
	except sliplib.ProtocolError as err:
		# print("protocol error: {0}".format(err))
		pass
	except serial.SerialException as err:
		print("error reading {0} error: {1}".format(ser.port, err))
		quit()
		pass
	except socket.error as err:
	    # print("socket error: {0}".format(err))
	    pass
	except Exception as err:
		print("generic error: {0}".format(err))
		quit()
