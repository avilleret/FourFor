#!/bin/bash

HERE=`dirname $(readlink -f $0)`

pkill -f ReadUSB-simple.py
pkill -f ReadUSB-simple.py
pkill -f ReadUSB-simple.py
pkill -f ReadUSB-simple.py

for dev in /dev/ttyUSB*; do
	${HERE}/ReadUSB-simple.py $dev &
done