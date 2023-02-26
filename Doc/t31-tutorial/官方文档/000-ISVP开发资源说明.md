## ISVP开发资源说明
*说明，此页面中的超链接需要将整个software文件夹下载后才能正常打开。*

###资源目录
* [SDK最新版本](./sdk "Latest SDK Version")
* 文档
  * [SDK API页面](./doc/API-html/index.html "SDK API")
  * [FAQ](./doc/900-FAQ.html "FAQ")
  * 开发参考：
     * [Nor Flash烧录方法](./doc/001-Nor_Flash_烧录方法.html "HOWTO Flashing SPI Nor")
     * [Toolchain使用说明](./doc/002-Toolchain使用说明.html "Toolchain")
     * [编译说明](./doc/003-编译说明.html "HOWTO compile")
     * [u-boot kernel开发指导](./doc/004-u-boot_kernel_开发指导.html "uboot kernel development guide")
     * [ISP驱动及Sensor驱动说明](./doc/005-isp驱动及sensor驱动说明.html "isp & sensor driver")
     * [rootfs说明](./doc/006-rootfs说明.html "rootfs")
     * [版本升级说明](./doc/007-版本升级说明.html "update")
     * [zram使用说明](./doc/008-zram使用说明.html)
     * [GPIO接口及操作方法](./doc/009-GPIO接口及操作方法.html "GPIO")
  * 调试参考：
     * [调试信息获取及反馈](./doc/100-调试信息获取及反馈.html "HOWTO capture log")
     * [telnet使用方法](./doc/101-telnet使用方法.html "telnet")
     * [NFS & tftp使用方法](./doc/102-NFS_tftp使用方法.html "NFS")
     * [wpa_supplicant使用方法](./doc/103-wpa_supplicant使用方法.html "wpa_supplicant")
     * [gdbserver使用方法](./doc/104-gdbserver使用方法.html "gdbserver")
  * 工具类文档：
     * [Carrier使用说明](./doc/201-Carrier使用说明.pdf "Carrier usage")

* 资源包
  * 交叉编译工具链(glibc,uclibc 2in1)：[32-bit](./resource/toolchain/mips-gcc472-glibc216-32bit.7z "toolchain-32-bit")，[64-bit](./resource/toolchain/mips-gcc472-glibc216-64bit.7z "toolchain-64-bit")
  * [参考Rootfs](./resource/rootfs "Sample Rootfs")
  * 工具
     * [图像查看工具Carrier](./resource/tools/carrier "Carrier")

* 开发板资源
  * [串口小板(FT232 USB转串口)](./dev_board/Uart "Windows驱动以及说明")

------------------------
###ISVP简介
ISVP是Ingenic Smart Video Platform的简称，包括SDK，交叉编译工具链，调试工具，硬件参考设计等等。开发者可在ISVP下快速的进行项目开发。

ISVP可以给开发者提供：

* 高并行度的数据流水线，可方便进行模块间数据流绑定配置
* 全面的图像及编码参数配置API
* 开源的u-boot，kernel及ko driver，可进行深入的自定义开发
* HTML API文档，方便进行数据结构索引及搜索
* Demo工具Carrier，可在UI上快速更改调试参数

开发环境环境搭建步骤如下：

* 下载完整的ISVP资源，并解压相关资源(7z解压的方式请见附**#1**)
* 安装交叉编译编译工具链
* 尝试编译uboot，kernel，与samples
* 在开发板上运行图象查看工具Carrier，实时查看效果
* 根据API文档以及uboot-kernel开发指导文档进行应用程序开发

附：

* **1**. 7z压缩具有较高的压缩比，对于Windows系统可以采用WinRAR进行解压(注：在Windows上解压可能会产生文件名乱码的问题)，对于Linux，可使用`7z x xxx.7z`命令进行解压。但是有些Linux发行版默认并不带有7z命令，各发行版安装7z命令方式如下：
  * Ubuntu: **`apt-get install p7zip-full`**
  * Fedora/CentOS: **`yum install p7zip`**
* **2**. 串口板在Windows上需要安装驱动，在Linux平台上无需安装，直接在Minicom中选择ttyUSB0作为串口设备即可