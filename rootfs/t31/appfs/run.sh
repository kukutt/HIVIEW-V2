#!/bin/sh

save_log(){
    echo "$*" >> /tmp/uplog
    echo "$*"
}
 
check_cmd(){
    local i=0
    local f=0
    while [ "$i" -le $2 ]; do
        echo $1 | sh
        if [ $? -eq 0 ] ; then
            f=1
            break
        fi
        i=$(( i + 1 ))
        sleep 1
    done

    if [ ${f} -eq 0 ]; then
        save_log cmd[$1] "error"
    else
        save_log cmd[$1] "ok" $i
    fi
}

connect_eth(){
    ifconfig eth0 down
    ifconfig eth0 hw ether FA:3E:BE:07:DB:70
    ifconfig eth0 up
    ifconfig eth0 192.168.1.80
    set_dhcp eth0
}

connect_wifi(){
    if [ -f /modules/8733bu.ko ]; then
        echo 'lsusb | grep "0bda:f72b"' | sh
        if [ $? -ne 0 ] ; then
            return
        fi
        insmod /modules/8733bu.ko
        save_log "find 8733bu"
    elif [ -f /modules/ssv6x5x.ko ]; then
        echo 'lsusb | grep "8065:6000"' | sh
        if [ $? -ne 0 ] ; then
            return
        fi
        insmod /modules/ssv6x5x.ko stacfgpath=/modules/ssv6x5x-wifi.cfg
        save_log "find ssv6x5x"
    else
        save_log "not find wifi drv"
        return
    fi

    echo "ctrl_interface=/var/run/wpa_supplicant" > /tmp/wpa_supplicant.conf
    echo "ap_scan=1" >> /tmp/wpa_supplicant.conf
    wpa_supplicant -Dnl80211 -iwlan0 -c /tmp/wpa_supplicant.conf &
    check_cmd "wpa_cli -i wlan0 status" 30

    #wpa_cli -i wlan0 scan
    #wpa_cli -i wlan0 scan_result
    wpa_cli -i wlan0 add_network
    wpa_cli -i wlan0 set_network 0 ssid '"chouchouchou"'
    wpa_cli -i wlan0 set_network 0 psk '"xianxianxian"'
    wpa_cli -i wlan0 enable_network 0
    check_cmd "wpa_cli -i wlan0 status | grep wpa_state=COMPLETED" 100
    set_dhcp wlan0
}

set_dhcp(){
    udhcpc -b -i $1
    check_cmd "ping -c 1 www.baidu.com" 100
}

insert_avko(){
    insmod /modules/tx-isp-t31.ko isp_clk=200000000
    insmod /modules/sensor_*_t31.ko
    insmod /modules/avpu.ko clk_name='vpll' avpu_clk=400000000
    insmod /modules/audio.ko spk_gpio=-1
}

run_app(){
    mount -t vfat -o rw /dev/mmcblk0 /mnt/rec00000001/
    FRUN=/mnt/rec00000001/app
    if [ -f $FRUN/run.sh ];then
        [ -f /mnt/rec00000001/disk.cfg ] || echo -e -n "\x00\x00\x00\x00\x01\x00\x00\x00" > /mnt/rec00000001/disk.cfg
        rm -rf /tmp/rwapp
        ln -s $FRUN /tmp/rwapp
    else
        rm -rf /tmp/rwapp
        ln -s /board/app/ /tmp/rwapp
    fi

    if [ -f /app/run.sh ];then
        /app/run.sh &
        exit 0
    fi
}

# start script
connect_wifi
connect_eth
insert_avko
run_app
