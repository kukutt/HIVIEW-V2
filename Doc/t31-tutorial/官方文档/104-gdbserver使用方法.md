#gdbserver使用方法

----------

gdbserver是嵌入式开发调试的主要工具，依赖开发板上的gdbserver程序以及交叉编译工具链中的mips-linux-gnu-gdb命令。gdbserver需要调试的PC和开发板网络相通。

本文仅介绍gdbserver的开启及连接方法，相关命令遵循标准gdbserver命令，详细内容请在互联网查询。

gdb以及gdbserver命令位置：

* gdb在toolchain文件夹下，位于`bin/mips-linux-gnu-gdb`
* gdbserver位于toolchain文件夹下：
  * glibc: `mips-linux-gnu/libc/usr/lib/bin/gdbserver`
  * uclibc: `mips-linux-gnu/libc/uclibc/usr/lib/bin/gdbserver`

## 1. PC运行gdb

    $ mips-linux-gnu-gdb [PC端应用程序路径]

例如：

    $ mips-linux-gnu-gdb sample-Encoder-h264
    GNU gdb (Ingenic 2015.02) 7.4.50.20120716-cvs
    Copyright (C) 2012 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
    and "show warranty" for details.
    This GDB was configured as "--host=i686-pc-linux-gnu --target=mips-linux-gnu".
    For bug reporting instructions, please see:
    <http://www.gnu.org/software/gdb/bugs/>...
    Reading symbols from sample-Encoder-h264...done.
    (gdb) 

注：

* 如果出现`(no debugging symbols found)`的提示，可能是因为应用程序strip过了（PC端），删除了debug需要的段，无法进行gdb调试
* 开发板上程序是可以strip的，Symbol的读取在PC端gdb工具完成。这也就是gdbserver相对与local gdb以及core dump的优势——开发板端可以strip——因为前两者都需要在开发板端load symbol，因此应用程序会变得非常大，无法放在flash中

## 2. 设置动态库搜索路径

应用程序往往动态链接libc、ld等库，当运行于动态库中break时（比如memcpy）往往无法跟踪到当前pc所在位置。这是因为动态库的加载地址是不固定的。因此需要在PC端gdb上指定动态库加载路径。方法是：

    set solib-search-path /opt/mips-gcc472-glibc216-64bit/mips-linux-gnu/libc/lib/:/opt/mips-gcc472-glibc216-64bit/mips-linux-gnu/lib

多个路径之间通过“：”来隔开。注：上述例子中仅指定了toolchain中glibc的libc系列库和libstdc++等主要外部库。

* glibc的动态库在toolchain文件夹下的相对目录为：`mips-linux-gnu/libc/lib`与`mips-linux-gnu/lib`
* uclibc的动态库在toolchain文件夹下的相对目录为：`mips-linux-gnu/libc/uclibc/lib`与`mips-linux-gnu/lib/uclibc`

## 3. 开发板启动gdbserver

    gdbserver [PC机IP:端口，与步骤2中端口需一致] [应用程序路径]

例如：

    gdbserver 192.168.1.100:1234 /tmp/sample-Encoder-h264

## 4. 链接开发板

    target remote [开发板IP:端口]

例如：

    target remote 192.168.1.101:1234




## 5. gdbserver连通

开发板会出现如下信息：

    (gdb) target remote 192.168.1.101:1234
    Remote debugging using 192.168.1.101:1234
    Reading symbols from /opt/mips-gcc472-glibc216-64bit/mips-linux-gnu/libc/lib/ld.so.1...done.
    Loaded symbols for /opt/mips-gcc472-glibc216-64bit/mips-linux-gnu/libc/lib/ld.so.1
    0x77fc7070 in __start () from /opt/mips-gcc472-glibc216-64bit/mips-linux-gnu/libc/lib/ld.so.1

至此，即可在PC端运行gdb命令。注：与gdbserver与local gdb不同，启动程序的命令为“c”(continue)。 