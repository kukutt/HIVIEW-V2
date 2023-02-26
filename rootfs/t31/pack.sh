#!/bin/bash
# 获取工作路径
homePath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
workPath=${homePath}
echo work dir [$homePath] [$workPath]

if  [ "$1" ==   "clean" ] ;then
    echo clean
else
    pushd ${homePath}
    rm -rf ./_fakeroot.sh
    
    rm -rf app_dir/
    [ -f ./app.tar ] && {
        mkdir -p app_dir/
        tar -xvf ./app.tar -C app_dir/
    }
 
    rm -rf ./rootfs_pack/ ./rootfs.squashfs.img 
    cp -raf ./rootfs ./rootfs_pack/

    mkdir -p ./rootfs_pack/etc/sensor
        cp -raf board/sc5235-t31.bin ./rootfs_pack/etc/sensor/
        cp -raf board/sensor_sc5235_t31.ko ./rootfs_pack/modules/
        cp -raf board/8733bu.ko ./rootfs_pack/modules/
    cp -raf ${workPath}/lib/libalog.so ./rootfs_pack/lib/
    cp -raf ${workPath}/lib/t31/libaudioProcess.so ./rootfs_pack/lib/
    cp -raf ${workPath}/lib/libimp.so ./rootfs_pack/lib/
    mkdir -p ./rootfs_pack/board
    mkdir -p ./rootfs_pack/dev
    mkdir -p ./rootfs_pack/media
    mkdir -p ./rootfs_pack/mnt
    mkdir -p ./rootfs_pack/mnt/rec00000001/
    mkdir -p ./rootfs_pack/opt
    mkdir -p ./rootfs_pack/proc
    mkdir -p ./rootfs_pack/root
    mkdir -p ./rootfs_pack/run
    mkdir -p ./rootfs_pack/sys
    mkdir -p ./rootfs_pack/system
    mkdir -p ./rootfs_pack/tmp
    [ -d "./tmprootfs/" ] && cp -raf ./tmprootfs/* ./rootfs_pack/
    echo "chown -R 0:0 ./rootfs_pack" >> ./_fakeroot.sh
    echo "chmod -R 750 ./rootfs_pack" >> ./_fakeroot.sh
    echo "chmod -R -w ./rootfs_pack/usr/bin" >> ./_fakeroot.sh
    echo "chmod -R -w ./rootfs_pack/bin" >> ./_fakeroot.sh
    echo "mksquashfs ./rootfs_pack/ rootfs.squashfs.img  -b 64K -comp xz " >> ./_fakeroot.sh
    chmod 777 ./_fakeroot.sh
    echo "pack rootfs in fakeroot"
    fakeroot -- ./_fakeroot.sh
    rm -rf ./_fakeroot.sh
    
    rm -rf ./appfs_pack/ ./appfs.jffs2.img ./appfs.squashfs.img
    cp -raf ./appfs ./appfs_pack/
        echo "app -> appfs"
        mkdir -p ./appfs_pack/app/
        mkdir -p ./appfs_pack/app/bin/
        mkdir -p ./appfs_pack/app/lib/
        mkdir -p ./appfs_pack/app/factory/
        cp -raf ./board/bsp_def_${BOARDSEL}.json ./appfs_pack/app/factory/bsp_def.json
        cp -raf ./app_dir/www/ ./appfs_pack/app/
        cp -raf ./app_dir/run.sh ./appfs_pack/app/
        cp -raf ./app_dir/README ./appfs_pack/app/
        cp -raf ./app_dir/bin/bsp.exe ./appfs_pack/app/bin/
        cp -raf ./app_dir/bin/codec.exe ./appfs_pack/app/bin/
        cp -raf ./app_dir/bin/rec.exe ./appfs_pack/app/bin/
        cp -raf ./app_dir/bin/rtsps.exe ./appfs_pack/app/bin/
        cp -raf ./app_dir/bin/webs.exe ./appfs_pack/app/bin/
        cp -raf ./app_dir/lib/libcjson.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libfreetype.so ./appfs_pack/app/lib/libfreetype.so.6
        cp -raf ./app_dir/lib/librtsp.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libflv.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libcomm.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libfont.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libhttp.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libnm.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libcfifo.so ./appfs_pack/app/lib/
        #cp -raf ./app_dir/lib/libcrypto.so.1.1 ./appfs_pack/app/lib/
        #cp -raf ./app_dir/lib/libssl.so.1.1 ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libfreetype.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/librtp.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libmpp.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libmov.so ./appfs_pack/app/lib/
        cp -raf ./app_dir/lib/libtindex.so ./appfs_pack/app/lib/
        mkdir -p  ./appfs_pack/app/cfg/
    echo "chown -R 0:0 ./appfs_pack" >> ./_fakeroot.sh
    echo "chmod -R 750 ./appfs_pack" >> ./_fakeroot.sh
    # -e 为flash擦除最小大小 不同的flash不一样
    # mkfs.jffs2命令为 `apt-get install mtd-utils` 安装
    echo "mkfs.jffs2 -d ./appfs_pack -l -e 0x8000 -o appfs.jffs2.img" >> ./_fakeroot.sh
    echo "mksquashfs ./appfs_pack appfs.squashfs.img  -b 64K -comp xz " >> ./_fakeroot.sh
    chmod 777 ./_fakeroot.sh
    echo "pack appfs in fakeroot"
    fakeroot -- ./_fakeroot.sh
    rm -rf ./_fakeroot.sh
    
    popd
fi

