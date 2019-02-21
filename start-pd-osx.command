#!/bin/bash

HERE=`dirname $(greadlink -f $0)`

killall pd
cd "${HERE}/Four2"
  /Applications/Pd-0.49-1.app/Contents/Resources/bin/pd four2.pd &

sleep 0.5
cd "${HERE}/biomedical_signal_generator"
  /Applications/Pd-0.49-1.app/Contents/Resources/bin/pd generator.pd &

sleep 0.5 
cd "${HERE}/audio2midi"
  /Applications/Pd-0.49-1-i386.app/Contents/Resources/bin/pd Audio2Midi.pd &

sleep 0.5
cd "${HERE}/villerette"
  /Applications/Pd-0.49-1.app/Contents/Resources/bin/pd villerette.pd &

sleep 0.5
cd "${HERE}/disklavier"
  /Applications/Pd-0.49-1.app/Contents/Resources/bin/pd -noaudio receive_midi.pd &

sleep 0.5
cd "${HERE}/subtitle"
  /Applications/Pd-0.49-1.app/Contents/Resources/bin/pd -noaudio subtitle.pd &

# sleep 0.5
# cd "${HERE}/midiplayer"
#   /Applications/Pd-0.49-1.app/Contents/Resources/bin/pd -noaudio midiplayer.pd &

# sleep 0.5
# cd "${HERE}/audio2midi"
#   python -m SimpleHTTPServer 8080