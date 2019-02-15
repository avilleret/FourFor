#!/bin/bash 

HERE=`dirname $(readlink -f $0)`

modprobe snd-virmidi

aconnect -x
sleep 0.5

sudo aconnect 20:0 24:0
sudo aconnect 20:1 24:0
sudo aconnect 25:0 20:0
sudo aconnect 25:0 20:1

sleep 0.5

/usr/local/bin/raveloxmidi -N -c ${HERE}/raveloxmidi.conf
