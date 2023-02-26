#NFS & TFTP使用方法

----------

## 1. NFS挂载
首先需要在内核中支持NFS，在kernel的menuconfig中使能：

    CONFIG_NFS_FS
    CONFIG_NFS_V2
    CONFIG_NFS_V3

挂载方法：

* 首先需要将NFS Server与开发板连接至同一网络
* 开启Host端的NFS Server（具体方法可baidu查询）
* 开发板上进行挂载：  `mount -o nolock [NFS server IP]:[PATH OF NFS] /mnt/nfs(mount point)`

## 2. tftp
在小容量存储的系统中，为了节省空间，因此经常裁减掉NFS的内核支持。在此情况下，还可以通过tftp进行文件传输。在Linux PC环境下搭建好tftp环境，在开发板上即可通过client程序获取文件。命令为：

    tftp -g -r [IP] [tftp文件路径] (-l 本地路径)

例如：

    tftp -g -r 192.168.1.100 sample-Encoder-h264

## 2. uboot中的tftp命令
uboot中的tftp可以把文件download到内存上，可用来烧录或者加载程序。命令为：

    tftp [addr] [tftp文件路径]

例如：

    tftp 0x80600000 uImage