# FourFor

Software for FourFor show

# Compilation

    export PATH=/opt/cross-pi-gcc/bin:$PATH
    mkdir -p build-rpi
    cd build-rpi
    cmake .. -GNinja -DCMAKE_TOOLCHAIN_FILE=${PWD}/../../ofnode/of/dev/arm-linux-gnueabihf.cmake -DCMAKE_SYSROOT=`realpath "${PWD}/../../ofnode/sysroot"` -DCMAKE_INSTALL_PREFIX=${PWD}/FourFor -DOF_ROOT="${PWD}/../../ofnode/of"
    ninja
    ninja install

# Synchronize install folder with RPi

    ninja install && rsync -hvrPtl  FourFor/ pi@raspberrypi.local:~/FourFor


# Transcode video for Raspberry Pi
    ffmpeg -i ${file} -vcodec libx264 -profile:v high -preset fast -crf 18 -b-pyramid none -vf scale=-1:720 -acodec ac3 -ab 1536k -scodec copy encoded/${file}