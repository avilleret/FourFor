#!/bin/bash
export PATH=/opt/gcc-8.2.0/bin:$PATH
export LD_LIBRARY_PATH=/opt/gcc-8.2.0/lib:$LD_LIBRARY_PATH
HERE=`dirname $(readlink -f $0)`

if [ "${HOSTNAME}" == "ecg" ]; then
	${HERE}/biomedical_display-armv7-Release
else
	${HERE}/video_player-armv7-Release
fi
