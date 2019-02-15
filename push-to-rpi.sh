#!/bin/bash +e

ips=(tv  cartel IRM ECG midi)

cd build-rpi
ninja install

for i in "${ips[@]}"
do
	echo "

	=========> Push to $i <========="
	#rsync -hvrPtl  FourFor/ pi@$i.local:/opt/FourFor
	rsync -av  FourFor/ pi@$i.local:/opt/FourFor

done