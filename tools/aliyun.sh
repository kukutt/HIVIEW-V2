#!/bin/bash
# 获取工作路径
homePath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
workPath=$PWD
echo work dir [$homePath] [$workPath]

export PATH=$homePath/:$PATH
export ALIYUNPAN_CONFIG_DIR=$homePath/aliyunpan_cfg
export LD_LIBRARY_PATH=$homePath/lib:$LD_LIBRARY_PATH

if [ n$1 == n ];then
    echo "no token"
    echo "chrome -> F12 -> console"
    echo "JSON.parse(localStorage.getItem(\"token\")).refresh_token"
    exit -1
fi

ALIYUNZIP=$homePath/aliyunpan-v0.2.4-linux-amd64.zip
ALIYUNDIR=$homePath/aliyunpan-v0.2.4-linux-amd64/
ALIYUNBIN=$ALIYUNDIR/aliyunpan
if [ ! -f "$ALIYUNZIP" ];then
    wget https://github.com/tickstep/aliyunpan/releases/download/v0.2.4/aliyunpan-v0.2.4-linux-amd64.zip --show-progress -O $ALIYUNZIP
    rm -rf $ALIYUNDIR
    unzip $ALIYUNZIP -d $homePath
fi

if [ ! -d "$ALIYUNDIR" ];then
    unzip $ALIYUNZIP -d $homePath
fi

echo $ALIYUNBIN login -RefreshToken=$1
$ALIYUNBIN login -RefreshToken=$1
if [ n$2 == n ];then
    echo "no download file"
    echo "$1 /work/work/jz/t31/mips-gcc540-glibc222-32bit-r3.3.0.tar.gz"
    exit -2
fi

$ALIYUNBIN d $2 --save

