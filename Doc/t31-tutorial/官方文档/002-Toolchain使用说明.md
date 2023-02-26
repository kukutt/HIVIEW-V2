#Toolchain使用说明

----------

Toochain即交叉编译工具链，是Linux Host机上用来编译和调试嵌入式设备程序的一系列工具的集合。ISVP中的Toolchain版本信息如下：

* gcc版本：4.7.2
* libc版本：
	* glibc版本：2.16
	* uclibc版本：0.9.33.2-nptl

## 1. 如何安装Toolchain
* 安装7z解压工具sudo apt-get install p7zip
* 根据Host机CPU位宽选择mips-gcc472-glibc216-32bit.7z或者mips-gcc472-glibc216-64bit.7z进行解压。例如：7z x mips-gcc472-glibc216-64bit.7z
* 通过export PATH=xxxx:$PATH命令，将toolchain下的bin目录添加到PATH环境变量中
* 测试toolchain可执行：  

        $ mips-linux-gnu-gcc --version
        mips-linux-gnu-gcc (Ingenic r2.3.3 2016.12) 4.7.2
        Copyright (C) 2012 Free Software Foundation, Inc.
        This is free software; see the source for copying conditions.  There is NO
        warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  若出现如上信息则可确认toolchain安装正确

* Toolchain版本查看方法：执行`mips-linux-gnu-gcc --version`

## 2. 如何进行glibc和uclibc编译
ISVP的toolchain包含了glibc和uclibc，因此基于glibc或者uclibc的程序均可使用此toolchan进行编译。

* glibc程序编译方法：默认link的libc即为glibc
* uclibc程序编译方法：C\_FLAGS+=-muclibc CXX\_FLAGS+=-muclibc，LD\_FLAGS+=-muclibc
* r2.3.3版本之后的Toolchain，uclibc程序可以使用`mips-linux-uclibc-gnu-XXX`的方式进行编译，不需要添加-muclibc的CFLAG及LDFLAG。
