#!/bin/bash

HERE=`dirname $(greadlink -f $0)`

cd "${HERE}/Four2"
/Applications/Pd-0.49-1.app/Contents/Resources/bin/pd four2.pd &

cd "${HERE}/biomedical_signal_generator"
 /Applications/Pd-0.49-1.app/Contents/Resources/bin/pd generator.pd &

cd "${HERE}/audio2midi"
 /Applications/Pd-0.49-1-i386.app/Contents/Resources/bin/pd Audio2Midi.pd &
