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

    ninja install && rsync -hvrPtl  FourFor/ pi@raspberrypi.local:/opt/FourFor

    rsync -hvrPtl LidarSlip pi@lidar.local:/opt/FourFor

# Transcode video for Raspberry Pi
    ffmpeg -i ${file} -vcodec libx264 -profile:v high -preset fast -crf 18 -b-pyramid none -vf scale=-1:720 -acodec ac3 -ab 1536k -scodec copy encoded/${file}

# Quickly display image on RPi
    sudo fbi -d /dev/fb0 -T 1 -a il1.jpg

# Batch resize jpg

	for img in alto*.jpg; do convert $img -resize 1920x1080 $img; done
	for img in tenor*.jpg; do convert $img -resize 1920x1080 $img; done
	for img in basse*.jpg; do convert $img -resize 1920x1080 $img; done

# Get crash dump 

    coredumpctl dump > score.dump
