#!/bin/bash +e

ips=(tv  cartel irm ecg midi raspberrypi)

cd build-rpi
ninja install

for i in "${ips[@]}"
do
	echo "

	=========> Push to $i <========="
	#rsync -hvrPtl  FourFor/ pi@$i.local:/opt/FourFor
	rsync -hvrPtl  FourFor/ pi@$i.local:/opt/FourFor

done