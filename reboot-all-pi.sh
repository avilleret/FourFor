#!/bin/bash +e

ips=(tv cartel IRM ECG midi)

for i in "${ips[@]}"
do
	echo Push to $i
	ssh -o StrictHostKeyChecking=no pi@$i.local 'sudo reboot'
done