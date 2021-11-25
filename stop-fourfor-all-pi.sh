#!/bin/bash +e

ips=(tv cartel irm ecg midi raspberrypi)

for i in "${ips[@]}"
do
	echo Push to $i
	ssh -o StrictHostKeyChecking=no pi@$i.local 'sudo systemctl stop fourfor'
done