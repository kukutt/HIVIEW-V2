#SPI-Norflash烧录方法

----------

## 方法一
* 1.思路是先通过网络tftp将烧录文件load到内存中，再烧录到nor中
     * 把内存先全部清为全f

            mw.b 0x80600000 0xff 0x1000000  --第三个参数是大小，这里清了16MB

     * 通过tftp load文件到内存

            tftp 0x80600000 u-boot-with-spl.bin

* 2.然后，将内存中的数据写入Nor

        sf probe
        sf erase 0x0 0x40000  --擦除uboot分区大小256K
        sf write 0x80600000 0x0 0x40000  --将uboot写入nor

* 也可以一次性将文件都load到内存中，再一起烧录。这种方法需要计算好各个文件的相对位置

        mw.b 0x80600000 0xff 0x1000000
        tftp 0x80600000 u-boot-with-spl.bin
        tftp 0x80640000 uImage
        tftp 0x808c0000 rootfs.squashfs
        tftp 0x80ac0000 appfs.jffs2
        sf probe
        sf erase 0x0 0x800000
        sf write 0x80600000 0x0 0x800000

## 方法二
对于第一次烧录Nor，或者没有网络的情况来说，需要从SD卡启动，并将uboot烧录到Nor中。之后的烧录就可以通过tftp进行。

* 1.格式化sd卡，并将卡启动uboot烧录到SD卡
--此步骤只需执行一次，若不更新卡起uboot则不需要再次执行此步骤。经过此步骤处理的SD卡可当作正常卡使用

  * a.将SD卡插入电脑
  * b.卸载sd卡，`umount /media/user/*`
  * c.将SD卡重新分区

            fdisk /dev/sdb
            > o	 --创建一个新的分区表
            > n	 --添加一个分区n
            --此时会提示：
            Partition type:
               p   primary (0 primary, 0 extended, 4 free)
               e   extended
            Select (default p):
            Using default response p
            --这里回车默认是p，创建主分区

            分区号 (1-4，默认为 1)：
            将使用默认值 1
            --这里回车默认创建1号分区

            起始 sector (2048-15130623，默认为 2048)：
            将使用默认值 2048
            Last sector, +扇区 or +size{K,M,G} (2048-15130623，默认为 15130623)：
            将使用默认值 15130623
            --2048号扇区在1MB的位置，给uboot空出了空间

            最后一步，输入w，向SD卡中写入分区表
            The partition table has been altered!

            Calling ioctl() to re-read partition table.
            Syncing disks.
            --到这里分区表已创建完成

  * d.执行sync，拔卡，重新插卡，PC将重新识别分区
  * e.格式化sd卡为通用的VFat文件系统：`mkfs.vfat /dev/sdb1`
  * f.编译u-boot。make isvp\_t10\_msc0，将生成的u-boot-with-spl.bin(不是u-boot-with-spl-mbr-gpt.bin)文件烧录进sd卡的17KB偏移处：

            dd if=u-boot-with-spl.bin of=/dev/sdb bs=1024 seek=17
    * 到此，可以用来烧录的SD卡制作完成。

* 2.插入SD卡，复制u-boot-with-spl.bin，uImage，rootfs\_glibc.jffs2，data.jffs2等到sd卡的vfat分区中

* 3.使用sd卡启动开发板，开机前几秒迅速按下回车键，进入uboot命令行
  * 首先，将需要烧录的文件读到内存中，以u-boot为例：
  * 把内存先全部清为全f

            mw.b 0x80600000 0xff 0x1000000  --第三个参数是大小，这里清了16MB

  * ls SD卡中的文件

            isvp# fatls mmc 0
               222944   u-boot-with-spl.bin
              2368879   uimage
            5 file(s), 0 dir(s)

  * load文件到内存

            fatload mmc 0 0x80600000 u-boot-with-spl.bin

  * 然后，将内存中的数据写入Nor

            sf probe
            sf erase 0x0 0x40000  --擦除uboot分区大小256K
            sf write 0x80600000 0x0 0x40000  --将uboot写入nor

## 方法三
此方法是在文件系统启动后的在线烧录方式。

* 通过nfs或tftp等方式，将SDK中mtd\_utils下的flash\_erase命令拷贝到设备
* 在设备端执行`flash_erase /dev/mtdX 0 0`，擦除需要更新的分区（不推荐擦除root分区）
* 将需要更新的bin文件通过nfs或者tftp等方式拷贝到设备
* `dd if=[bin文件路径] of=/dev/mtdX`，进行烧录