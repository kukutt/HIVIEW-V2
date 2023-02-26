#zram使用说明

----------

zram是Linux内核提供的一种内存压缩技术。zram是一个标准逻辑块设备，可以作为swap或者格式化为逻辑块设备文件系统（vfat,ext2等）。zram可以缓解嵌入式设备内存较小带来的一些问题。

## zram内核支持
需要在内核使能如下CONFIG：

    CONFIG_ZSMALLOC=y
    CONFIG_PGTABLE_MAPPING=y
    CONFIG_ZRAM=y
    CONFIG_ZRAM_LZ4_COMPRESS=y

## zram swap
* 首先，需要使能kernel swap功能的支持：`CONFIG_SWAP=y`

* 其次，需要使能zram的支持（如上）。

* zram swap在系统中打开的方法如下：

        echo 100 > /proc/sys/vm/swappiness
        echo 8388608 > /sys/block/zram0/disksize
        mkswap /dev/zram0
        swapon /dev/zram0
其中，disksize是zram block设备的大小，一般建议为系统内存的30%左右，最大值不能超过系统最大内存的2倍
* `cat /proc/swaps`可以查看swap使用情况
* 以上的命令可以放在启动脚本中运行

## zram作为块设备
zram作为虚拟逻辑块设备，除了可作为swap外，还可以格式化为逻辑块设备文件系统，比如以下是一个例子：

    echo 100 > /proc/sys/vm/swappiness
    echo 8388608 > /sys/block/zram0/disksize
    mkfs.vfat /dev/zram0
    mkdir /tmp/zram
    mount /dev/zram0 /tmp/zram

这样，zram vfat分区就挂载成功了。这个方案的好处是，某些应用需要使用/tmp/ (tmpfs)作为临时目录，但是这种方法比较浪费内存。如果使用压缩的内存，可以获得更高的内存利用率。