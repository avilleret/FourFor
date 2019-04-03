#!/bin/bash +e

HERE=`dirname $(readlink -f $0)`

ips=(tv  cartel irm ecg midi)

cd ${HERE}/build-rpi
ninja install

for i in "${ips[@]}"
do
	echo "

	=========> Push to $i <========="
	#rsync -hvrPtl  FourFor/ pi@$i.local:/opt/FourFor
	rsync -hvrPtl  FourFor/ pi@$i.local:/opt/FourFor

done

cd ${HERE}
rsync -hvrPtl LidarSlip pi@lidar.local:/opt/FourFor
#rsync -hvrPtl LidarSlip pi@lidar2.local:/opt/FourFor
