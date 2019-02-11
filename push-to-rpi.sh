#!/bin/bash +e

ips=(fourfortv  fourforcartel fourforIRM fourforECG fourformidi)

cd build-rpi
ninja install

for i in "${ips[@]}"
do
	echo Push to $i
	rsync -hvrPtl  FourFor/ pi@$i.local:~/FourFor
done