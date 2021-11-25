#!/bin/bash

HERE=`dirname $(readlink -f $0)`

cd "${HERE}/Four2"
pd four2.pd &

cd "${HERE}/biomedical_signal_generator"
pd generator.pd &

cd "${HERE}/villerette"
pd villerette.pd &

cd "${HERE}/audio2midi"
pd Audio2Midi.pd &
