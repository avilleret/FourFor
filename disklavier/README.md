# Disklavier script

This folder contains `pimidi` submodule and script to connect Yamaha Disklavier through RTP Midi (aka Apple Midi Network).

# Building

Clone the `FourFor` repo on the RPi, then go to `pimidi/raveloxmidi` folder and :

    ./autogen.sh
    ./configure
    make -j4
    sudo make install

If you get an error concerning missing `pthread` lib, copy the last `gcc` call and add `-lpthread` at the end.

See https://github.com/ravelox/pimidi/issues/27 for more information on this issue.

# RPi configuration
Hard link systemd unit file to `/etc/systemd/system` folder and enable it :

    sudo ln raveloxmidi.service /etc/systemd/system
    sudo systemctl enable raveloxmidi
    sudo systemctl start raveloxmidi

# OSX configuration

Go to `Audio and Midi Settings` and double click on the `Network` item in the `Midi Studio` window.
Then create a new session and connect to the `disklavier` device which should show in the bottom left view.
