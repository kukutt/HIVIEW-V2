# rootfs说明

----------

## 文件系统的选择
对于NorFlash Based系统，存储空间一般较小，因此会选用压缩文件系统。在君正平台推荐以下两种文件系统：

* squashfs：只读文件系统，压缩率高
* jffs2：可读写文件系统，可选择压缩方式

一种系统搭建的方案是，系统的rootfs以及不需要经常修改的系统分区采用squashfs，而配置分区等需要经常读写的分区采用jffs2。

另外，文件系统有glibc与uclibc两种选择，glibc的特点是支持功能全面，但是占用存储稍多；uclibc的特点是占用存储空间小于glibc，但是支持功能比glibc稍少。开发者应该根据自己实际情况选用适合的libc方式。

**对于一般的应用场景，推荐客户使用uclibc搭建系统。**

## 修改文件系统内容
可以将rootfs压缩包解压，替换需要修改的资源即可。例如，实际产品往往需要裁减或者自定义busybox，可以自行编译busybox，将busybox strip后copy到rootfs中，并将bin, sbin, user/bin, usr/sbin下的软连接通过-R的方式copy到目标目录

## 打包文件系统
* squashfs打包方式：
  `mksquashfs rootfs[输入文件加] rootfs.squashfs[输出文件名] -comp xz`
* jffs2打包方式：
  `mkfs.jffs2 -o jffs2.img[输出文件名] -r jffs2_dir[输入文件夹] -e 0x8000[擦除大小32K] -s 0x40000[页大小256K] -n -l -X zlib --pad=0x300000[输出镜像pad到3MB大小]`
  注：擦除大小和页大小要与内核中Flash的设置保持一致，否则会出现错误。

另外jffs2也可以通过设备端擦除再直接mount的方式创建。

## nostripped库
为了减小存储占用的大小，rootfs压缩包内的库都是stripped的。而在debug时，可能需要nostripped的库。nostripped库位于toolchain库录
下：

* glibc: `${toolchain}/mips-linux-gnu/libc/lib`以及`${toolchain}/mips-linux-gnu/libc/usr/lib`
* uclibc: `${toolchain}/mips-linux-gnu/libc/uclibc/lib`以及`${toolchain}/mips-linux-gnu/libc/uclibc/usr/lib`

## Demo rootfs简单说明
ISVP的Demo分区方式为：

* u-boot:256K
* uImage:2560K
* rootfs:2048K
* system:3328K

在ISVP的文件系统中，有两个指定的文件路径：

* /etc/sensor/ ——此目录下存放Sensor的效果bin文件
* /etc/webrtc_profile.ini ——回音消除参数文件

不同的产品可能以上两处会有不同，而且产品升级时也可能升级相关的参数文件，因此建议将以上两处做软连接到可读写或者可更新的路径下。

Demo的rootfs在启动后，会探测system分区是否可用，如果不可用，会进行格式化，并创建相关的目录结构。因此，第一次制作Demo系统，可以将整片Flash擦除，并烧录u-boot, uImage, rootfs即可。system分区会自动创建。

system分区的结构如下：

    system
    ├── bin   --此目录下放置应用程序
    ├── etc
    │   └── sensor   --/etc/sensor软链接到这里
    │       └── jxh42.bin  --效果文件
    ├── init
    │   └── app_init.sh    --开机执行初始化脚本
    └── lib
        ├── firmware   --/lib/firmware软链接到这里
        └── modules    --/lib/modules软链接到这里
            ├── sensor_jxh42.ko   --sensor驱动
            └── tx-isp.ko    --ISP驱动

注：Demo rootfs可作为参考，开发者可以根据产品的实际情况定义rootfs方案。
