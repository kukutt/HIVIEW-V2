#libnl库 编译
1.git下载opensource源码，此版本为3.2.x

    git clone git://github.com/tgraf/libnl.git

本次编译采用3.2.25的版本，切换方法如下：

    cd libnl
    git branch 3.2.25
    git checkout 3.2.25
    git reset --hard libnl3_2_25


2.编译

配置Makefile：
如果没有configure文件，需要执行`./autogen.sh`先生成configure

glibc版本：

    ./configure --host=mips-linux-gnu --prefix=${PWD}/_install_glibc

uclibc版本：

    CFLAGS="-muclibc -O2" CPPFLAGS="-muclibc -O2" LDFLAGS="-muclibc -O2" ./configure --host=mips-linux-gnu --prefix=${PWD}/_install_uclibc

如果需要编译链接的wpa，则需要加入`--disable-shared`，编译静态的libnl。例如：

glibc版本：

    ./configure --host=mips-linux-gnu --prefix=${PWD}/_install_glibc --disable-shared

uclibc版本：

    CFLAGS="-muclibc -O2" CPPFLAGS="-muclibc -O2" LDFLAGS="-muclibc -O2" ./configure --host=mips-linux-gnu --prefix=${PWD}/_install_uclibc --disable-shared

然后 ：

    make；make install

---

#libopenssl 库编译
1.git下载opensource源码

    git clone git://git.openssl.org/openssl.git

注意，请使用1.0.2f版本，1.1.0版本的原生源码不支持uclibc。

切换到  1.0.2f版本

    cd openssl
    git branch 1.0.2f
    git checkout 1.0.2f
    git reset --hard OpenSSL_1_0_2f

2.编译

第一步：

配置Makefile：

glibc:

    ./Configure linux-generic32 shared -DL_ENDIAN --prefix=${PWD}/_install_glibc --openssldir=${PWD}/_install_glibc

uclibc:

    ./Configure linux-generic32 shared -DL_ENDIAN --prefix=${PWD}/_install_uclibc --openssldir=${PWD}/_install_uclibc

如果需要编译链接的wpa，则需要加入`no-shared no-dso`，编译静态的libnl。例如：

glibc:

    ./Configure linux-generic32 no-shared no-dso -DL_ENDIAN --prefix=${PWD}/_install_glibc --openssldir=${PWD}/_install_glibc

uclibc:

    ./Configure linux-generic32 no-shared no-dso -DL_ENDIAN --prefix=${PWD}/_install_uclibc --openssldir=${PWD}/_install_uclibc


第二步：

修改makefile

glibc版本:

默认 glibc版本 直接执行第三步

uclibc版本：

    vi Makefile

CFLAG 和 SHARED_LDFLAGS 添加-muclibc


    CFLAG= -fPIC -DOPENSSL_PIC -DOPENSSL_THREADS -D_REENTRANT -DDSO_DLFCN -DHAVE_DLFCN_H -DL_ENDIAN
			-O3 -fomit-frame-pointer -Wall
	...
    SHARED_LDFLAGS=
改成：

    CFLAG= -fPIC -DOPENSSL_PIC -DOPENSSL_THREADS -D_REENTRANT -DDSO_DLFCN -DHAVE_DLFCN_H -DL_ENDIAN
			 -O3 -fomit-frame-pointer -Wall -muclibc
	...

    SHARED_LDFLAGS= -muclibc

第三步：

    make CC=mips-linux-gnu-gcc RANLIB=mips-linux-gnu-ranlib LD=mips-linux-gnu-ld MAKEDEPPROG=mips-linux-gnu-gcc PROCESSOR=MIPS
    make install

---

#编译 wpa_suppliant
1.git下载opensource源码，假设版本为2.5

    git clone git://w1.fi/hostap.git
    cd hostap
    git branch 2.5
    git checkout 2.5
    git reset --hard hostap_2_5

2.编译
第一步：

    cd hostap/wpa_supplicant
    cp defconfig .config

第二步：修改.config 添加 libnl openssl 库路径配置

    vi .config

在文件末尾添加如下

glibc版本：

    CC=mips-linux-gnu-gcc
    PWD-libnl = /your/install/path/_install_glibc
    PWD-openssl = /your/install/path/_install_glibc

    CFLAGS += -I/$(PWD-libnl)/include/libnl3/
    LIBS += -L/$(PWD-libnl)/lib
    LIBS_c += -L/$(PWD-libnl)/lib
    LIBS_p += -L/$(PWD-libnl)/lib
    CFLAGS += -I/$(PWD-openssl)/include/
    LIBS += -L/$(PWD-openssl)/lib
    LIBS_c += -L/$(PWD-openssl)/lib
    LIBS_p += -L/$(PWD-openssl)/lib

    LDFLAGS += -lpthread -lm

    CONFIG_LIBNL32=y
    CONFIG_EAP_FAST=y
    CONFIG_TLS=openssl

uclibc版本:


    CC=mips-linux-gnu-gcc
    PWD-libnl = /your/install/path/_install_uclibc
    PWD-openssl = /your/install/path/_install_uclibc

    CFLAGS += -I/$(PWD-libnl)/include/libnl3/
    LIBS += -L/$(PWD-libnl)/lib
    LIBS_c += -L/$(PWD-libnl)/lib
    LIBS_p += -L/$(PWD-libnl)/lib
    CFLAGS += -I/$(PWD-openssl)/include/
    LIBS += -L/$(PWD-openssl)/lib
    LIBS_c += -L/$(PWD-openssl)/lib
    LIBS_p += -L/$(PWD-openssl)/lib

    CFLAGS += -muclibc
    LDFLAGS += -muclibc -lpthread -lm

    CONFIG_LIBNL32=y
    CONFIG_EAP_FAST=y
    CONFIG_TLS=openssl

第三步:

    make -j4;

**注意**：如果libnl和openssl编译为静态库，则wpa是静态链接的，如果前两者是动态链接的，则编译出的是动态链接的wpa

**注意**：编译出来的库和应用都是not stripped，可以用 mips-linux-gnu-strip 命令节省空间

*例子：*

    mips-linux-gnu-strip  wpa_supplicant

