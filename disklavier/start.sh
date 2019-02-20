#!/bin/bash 

HERE=`dirname $(readlink -f $0)`

killall pd

/usr/local/bin/pd -noaudio -alsamidi -mididev 1 -nogui ${HERE}/midi_over_udp.pd &
PDPID=$!
sleep 0.5

aconnect -x
sleep 0.5

sudo aconnect 20:0 128:0
sudo aconnect 20:1 128:0
sudo aconnect 128:1 20:0
sudo aconnect 128:1 20:1

wait $PDPID

sleep 0.5
