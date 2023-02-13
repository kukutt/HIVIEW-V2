#! /bin/sh

killfn(){
    killall -9 bsp.exe codec.exe rtsps.exe webs.exe
}



# 获取工作路径
homePath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
workPath=$PWD
runPath=$homePath/../../

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$runPath/lib/x86/
export PATH=$PATH:$runPath/bin/x86/
#export TZ=UTC-8:00
#sleep 1


if [ "n$1" = "nkill" ];then
killfn
elif [ "n$1" = "nenv" ];then
echo work dir [$homePath] [$workPath]
else
killfn

rm -rf $workPath/bin/www
ln -s $workPath/mod/webs/www $workPath/bin/www

rm /tmp/log*.log /tmp/log

mkdir -p $homePath/cfg/
bsp.exe     $homePath/factory/bsp_def.json $homePath/cfg/bsp_parm.json > /dev/null 2>&1 &
codec.exe  $homePath/cfg/codec_parm.json > /dev/null 2>&1 &
rtsps.exe  $homePath/cfg/rtsps_parm.json > /dev/null 2>&1 &
webs.exe   $homePath/cfg/webs_parm.json  > /dev/null 2>&1 &

#rec.exe    $homePath/cfg/rec_parm.json   > /dev/null 2>&1 &
#rtmps.exe  /app/cfg/rtmps_parm.json > /dev/null 2>&1 &
#onvif.exe  /app/cfg/onvif_parm.json > /dev/null 2>&1 &
#webs.exe   /app/cfg/webs_parm.json  > /dev/null 2>&1 &
#svp.exe    /app/cfg/svp_parm.json   > /dev/null 2>&1 &
#app.exe    /app/cfg/app_parm.json   > /dev/null 2>&1 &
#sips.exe    /app/cfg/sips_parm.json > /dev/null 2>&1 &
#srts.exe    /app/cfg/srts_parm.json > /dev/null 2>&1 &
fi
