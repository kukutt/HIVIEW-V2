#!/bin/bash

function buildcode(){
    echo buildall start
    make clean
    make
    [ $? -eq 0 ] || exit -1
    echo buildall end
}

if [ "n"$1 == "n" ];then
    sudo apt-get update > /dev/null 2>&1
    sudo apt-get -y install jq > /dev/null 2>&1

    # api变量解析
    jstype=`cat $GITHUB_EVENT_PATH | jq ".inputs.type" | sed 's/\"//g'`
    jaliyuntk=`cat $GITHUB_EVENT_PATH | jq ".inputs.aliyuntk" | sed 's/\"//g'`
else
    jstype=$1
    jaliyuntk=$2
fi

mkdir ./output/
if [ "n"$jstype = "nx86" ]; then
    sudo apt-get -y install gcc-multilib g++-multilib
    sudo apt-get -y install build-essential module-assistant
    sudo apt-get -y remove libreadline-dev
    . ./build/x86
    buildcode
    cp lib/x86/* ./output/
    tar -zcvf x86.tgz lib/x86/*
elif [ "n"$jstype = "nt31" ]; then
    ./tools/aliyun.sh $jaliyuntk /work/work/jz/t31/mips-gcc540-glibc222-32bit-r3.3.0.tar.gz
    [ $? -eq 0 ] || exit -1
    tar -zxvf ./work/work/jz/t31/mips-gcc540-glibc222-32bit-r3.3.0.tar.gz
    [ $? -eq 0 ] || exit -1
    export PATH=$PWD/mips-gcc540-glibc222-32bit-r3.3.0/bin:$PATH
    echo $PATH
    . ./build/t31
    buildcode
    [ $? -eq 0 ] || exit -1
    ./ins.sh t31
    [ $? -eq 0 ] || exit -1
    cp ./nfs/t31/upg/app.tar ./output/
    [ $? -eq 0 ] || exit -1
else
    echo error type $jstype
fi
