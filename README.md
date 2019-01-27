# FourFor
Software for FourFor show

    export PATH=/opt/cross-pi-gcc/bin:$PATH
    mkdir -p build-rpi
    cd build-rpi
    cmake .. -GNinja -DCMAKE_TOOLCHAIN_FILE=${PWD}/../../ofnode/of/dev/arm-linux-gnueabihf.cmake -DCMAKE_SYSROOT=`realpath "${PWD}/../../ofnode/sysroot"` -DCMAKE_INSTALL_PREFIX=${PWD}/FourFor -DOF_ROOT="${PWD}/../../ofnode/of"
    ninja
    ninja install


    rsync -hvrPtl  FourFor/ pi@raspberrypi.local:~/FourFor