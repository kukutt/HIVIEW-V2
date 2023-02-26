
##T21 uboot kernel 开发指导
**目录：**

* [1.uboot修改](#1)
	* [1.1 如何tftpboot下载image](#1.1)
	* [1.2 如何挂载nfsroot](#1.2)
	* [1.3 如何更改mtd分区大小](#1.3)
	* [1.4 如何改变rmem内存大小](#1.4)
* [2. kernel修改](#2)
	* [2.1 如何配置TF卡的引脚](#2.1)
	* [2.2 如何选择内核压缩方式](#2.2)
	* [2.3 如何配置USB](#2.3)
	* [2.4 如何添加SDIO WiFi](#2.4)
	* [2.5 如何配置Auido音频](#2.5)
	* [2.6 如何添加一个新的板级](#2.6)
	* [2.7 SDIO WiFI 如何做Host AP](#2.7)
	* [2.8 如何添加一个SPI Flash](#2.8)
* [3. 预留内存及内核裁减](#3)

----------

<h3 id="1">1. uboot修改</h3>
<h4 id="1.1">1.1 如何tftpboot下载image</h4>

**第一步:**

板子上电启动，进入uboot命令行（读秒时按下Enter键）

**第二步** 通过set bootargs、setenv命令设置参数

设置板子IP，服务器IP，IP请根据实际情况修改：

	isvp# setenv gatewayip 192.168.1.100
	isvp# setenv ipaddr 192.168.1.1
	isvp# setenv netmask 255.255.255.0
	isvp# setenv serverip 192.168.1.200


下载文件到内存地址0x80600000处

	isvp# tftpboot 0x80600000 uImage

然后,用bootm命令即可启动内核

	bootm 0x80600000

**注意：** 以上设置重启会丢失，可通过下面的操作，每次开机会自动tftp 下载uImage，并启动。

	isvp# setenv gatewayip 192.168.1.100
	isvp# setenv ipaddr 192.168.1.1
	isvp# setenv netmask 255.255.255.0
	isvp# setenv serverip 192.168.1.200
	isvp# setenv bootfile uImage
	isvp# setenv bootcmd tftpboot\;bootm\;bootd
	isvp# save


----------

 <h4 id="1.2">1.2 如何挂载nfsroot</h4>

**第一步:**

板子上电启动，进入uboot命令行（读秒时按下Enter键）

**第二步：**

通过set bootargs命令重新设置参数


> isvp# set bootargs console=ttyS1,115200n8 mem=32M@0x0 ispmem=8M@0x5A00000 rmem=30M@0x6200000 init=/linuxrc ip=192.168.1.100:192.168.1.1:192.168.1.200:255.255.255.0 root=/dev/nfs nfsroot=193.169.4.2:/your/nfsroot/path rw

然后，boot命令启动，即可挂载nfsroot网络文件系统

*以上操作需要先搭建你的服务器的tftp 和nfs 服务*

----------

<h4 id="1.3">1.3 如何更改mtd分区大小</h4>

mtd分区大小，在uboot中以命令行参数的形式传入kernel，在include/configs/isvp.h中，

对于8M Flash，找到`CONFIG_SFC_NOR`，我们的参考平台定义如下

>  #define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=jz_sfc:256k(boot),2560k(kernel),2048k(rootfs),-(appfs)"

其中：` mtdparts=jz_sfc:256k(boot),2560k(kernel),2048k(rootfs),-(appfs)`就是MTD的分区情况,共4块分区，uboot 256k，kernel 2.5M，rootfs(squashfs)2M。-(appfs)具有自适应功能，当Flash为8M时，appfs分区会自动分配成3.25M；当Flash为16M时会自动分配成11.25M。

如果想修改，请保持总大小不变的情况下，按如上格式修改。一般uboot、kernel、rootfs大小固定不变，建议用户不要修改，可以调整appfs分区大小。

**示例1**

对于8MFlash，如果想添加一个1M的data分区，修改如下

>  #define CONFIG\_BOOTARGS BOOTARGS\_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=jz_sfc:256k(boot),2560k(kernel),2048k(rootfs),1024k(data),-(appfs)"


----------


<h4 id="1.4">1.4 如何改变rmem内存大小</h4>
rmem大小同样是在include/configs/isvp_t21.h中修改,我们参考平台定义如下

>  #define BOOTARGS\_COMMON "console=ttyS1,115200n8 mem=44M@0x0 rmem=20M@0x2c00000

其中：`mem=44M@0x0 rmem=20M@0x2c00000`就是对mem的相关设置
`mem=44M@0x0`，从0地址开始给系统分配36M内存，
`rmem=20M@0x2c00000`，从0x2c00000（44M）地址开始给SDk分配20M；

T21L/N内嵌一块**64M**的内存,如要修改rmem，请在总大小64M不变情况下按格式进行修改。

**示例：**

假设rmem减小4M，mem增大4M

如果想减小rmem，那么mem应该增大。所以mem大小为47M，从0地址开始。rmem为16M，地址应从48M开始。

>  #define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=48M@0x0 rmem=16M@0x3000000"

--------------------

<h3 id="2">2. kernel修改</h3>
<h4 id="2.1">2.1 如何配置TF卡的引脚</h4>

T10有2个MMC接口，MMC0一般用作TF卡，MMC1 用做SDIOWIFI。

首先在menuconfig中选上MMC0

	Location：
	  -> Device Drivers
		-> MMC/SD/SDIO card support (MMC [=y])
		  -> Ingenic(XBurst) MMC/SD Card Controller(MSC) v1.2 support
	选择 JZMMC_V12 MMC0
		JZMMC_V12 GPIO function select 选 GPIO B,with 4bit
		msc0 max frequency 写48000000

可能需要配置的引脚有:

- MMC\_RST、MMC\_CD、MMC\_WP、MMC\_PWR

以turkey板为例，修改路径为`arch/mips/xburst/soc_t21/chip_t21/isvp/turkey/board.h`，

	/* ****************************GPIO MMC START******************************** */
	#define GPIO_MMC_RST_N			-1
	#define GPIO_MMC_RST_N_LEVEL	LOW_ENABLE
	#define GPIO_MMC_CD_N			GPIO_PB(27)
	#define GPIO_MMC_CD_N_LEVEL		LOW_ENABLE
	#define GPIO_MMC_PWR			-1
	#define GPIO_MMC_PWR_LEVEL		LOW_ENABLE
	#define GPIO_MMC_WP_N			-1
	#define GPIO_MMC_WP_N_LEVEL		LOW_ENABLE
	/* ****************************GPIO MMC END******************************** */
假如要添加一个PWR pin（PA29 高有效），修改如下：

	/* ****************************GPIO MMC START******************************** */
	#define GPIO_MMC_RST_N			-1
	#define GPIO_MMC_RST_N_LEVEL	LOW_ENABLE
	#define GPIO_MMC_CD_N			GPIO_PB(27)
	#define GPIO_MMC_CD_N_LEVEL		LOW_ENABLE
	#define GPIO_MMC_PWR			GPIO_PA(29)
	#define GPIO_MMC_PWR_LEVEL		HIGH_ENABLE
	#define GPIO_MMC_WP_N			-1
	#define GPIO_MMC_WP_N_LEVEL		LOW_ENABLE
	/* ****************************GPIO MMC END******************************** */

**注意：** 不用的GPIO请置成-1

----------

<h4 id="2.2">2.2 如何选择内核压缩方式</h4>

目前支持如下几种格式

	Use the arrow keys to navigate this window or press the hotkey of x
	x  the item you wish to select followed by the <SPACE BAR>. Press    x
	x  <?> for additional information about this option.                 x
	x x                           ( ) Gzip                             x x
	x x                           ( ) Bzip2                            x x
	x x                           (X) LZMA                             x x
	x x                           ( ) LZO                              x x
	x x                                                                x x

直接make menuconfig 选择想要的压缩格式，保存。这里推荐LZMA压缩，压缩效率高。

	x   Location:                                                       x
	  x     -> General setup                                             x
	  x       -> Kernel compression mode (<choice> [=y])


**注意：**选择了某个压缩格式后，需要检查uboot中是否支持此种格式的解压,其中GZIP默认支持。

检查方法：

uboot中打开对应的配置文件，查看是否有添加相关宏定义，没有就添加一下。

t10 => include/configs/isvp.h

t20 => include/configs/isvp_t20.h

t21 => include/configs/isvp_t21.h

说明：

CONFIG_GZIP对应 gzip格式

CONFIG_LZMA对应 lzma格式

CONFIG_BZIP2对应 bzip2格式

CONFIG_LZO对应 lzo格式



----------

<h4 id="2.3">2.3 如何配置USB</h4>

1.引脚配置

可能需要配置的引脚有:

- USB\_ID、USB\_DETE、USB\_DRVVBUS

以turkey板为例，修改路径为`arch/mips/xburst/soc_t21/chip_t21/isvp/turkey/board.h`，

	/* ****************************GPIO USB START******************************** */
	#define GPIO_USB_ID			-1
	#define GPIO_USB_ID_LEVEL		LOW_ENABLE
	#define GPIO_USB_DETE			-1
	#define GPIO_USB_DETE_LEVEL		LOW_ENABLE
	#define GPIO_USB_DRVVBUS		-1
	#define GPIO_USB_DRVVBUS_LEVEL		HIGH_ENABLE
	/* ****************************GPIO USB END********************************** */

用户可以根据需要配置GPIO和高低有效，不用的请配成-1.


2.功能配置

需要在menuconfig中配置

*OTG Host 配置*

	Location：
		-> Device Drivers
		  -> USB support (USB_SUPPORT [=y])
			-> DesignWare USB2 DRD Core Support (USB_JZ_DWC2 [=y])
			 -> Driver Mode
	选择Host Mode Only

*Host and device 配置*

	第一步：
	Location：
		-> Device Drivers
		  -> USB support (USB_SUPPORT [=y])
			-> USB Gadget Drivers (<choice> [=y])
	选择Android Composite Gadget

	第二步：
	Location：
		-> Device Drivers
		  -> USB support (USB_SUPPORT [=y])
			-> DesignWare USB2 DRD Core Support (USB_JZ_DWC2 [=y])
			 -> Driver Mode
	选择Both Host and Device

----------

<h4 id="2.4">2.4 如何添加SDIO WiFi</h4>

* 1.添加BCM模块

BCM模块主要封装了四个板级电源管理接口函数，`bcm_wlan_power_on()`、`bcm_wlan_power_on()`、`bcm_manual_detect()`，
`bcm_customer_wlan_get_oob_irq()`实现对WIFI芯片的上电、下电以及对sdio的侦测，对上电，复位等GPIO都做了宏定义，客户如有变动，只需要需改相应的宏定义即可。

	symbol: bcm_pm_core [=y]
	prompt: bcm module power control core drive
	location:
		->device drivers
			->misc device
       defined at drivers/misc/kconfig:543

这些函数在`arch/mips/xburst/soc_t10/chip_t10/isvp/common/bcm_power_control.c`中实现。


*对于turkey板级：*

宏定义在`arch/mips/xburst/soc_t21/chip_t21/isvp/turkey/board.h`


	/* ****************************GPIO WIFI START******************************* */
	#define WL_WAKE_HOST	GPIO_PB(28)
	#define WL_REG_EN	GPIO_PC(16)
	#define WL_32K_EN	GPIO_PB(18)
	#define WL_MMC_NUM	1 //sdio use MMC1

	#define WLAN_PWR_EN	(-1)
	#define BCM_PWR_EN	(-1)
	/* ****************************GPIO WIFI END********************************* */

 *  2.添加WIFI驱动

用户需要移植对应的WIFI驱动，该驱动可以是脱离板级的，只需要在驱动相应的地方调用上面四个函数即可。

*内核目前支持AP6181,添加如下：*

    Symbol: BCMDHD_AP6181 [=y]
    Type  : tristate
    Prompt: Broadcom FullMAC wireless cards support
    Location:
        ->Devices Drivers
            ->Network devices support (NETDEVICE [=y])
                ->Wireless LAN (WLAN [=y])

 *  3.添加MMC驱动

menuconfig中选择 MMC1

	Location：
	  -> Device Drivers
		-> MMC/SD/SDIO card support (MMC [=y])
		  -> Ingenic(XBurst) MMC/SD Card Controller(MSC) v1.2 support
	选择 JZMMC_V12 MMC1
		JZMMC_V12 GPIO function select 选 GPIO C,with 4bit
		msc0 max frequency 写24000000

----------

<h4 id="2.5">2.5 如何配置Audio音频</h4>

turkey板目前没有添加音频部分的功能，如果用户在自己板级中想添加音频功能，请按如下配置

1.引脚配置

可能需要配置的引脚有:

- SPEAK\_EN

修改路径为`arch/mips/xburst/soc_t21/chip_t21/isvp/turkey/board.h`，

	#define GPIO_SPEAKER_EN	   -1	      /*speaker enable gpio*/
	#define GPIO_SPEAKER_EN_LEVEL	HIGH_ENABLE

以turkey板为例，修改路径为`arch/mips/xburst/soc_t21/chip_t21/isvp/turkey/board.h`，

2.menuconfig添加音频模块

	Symbol: T10_INTERNAL_CODEC [=y]
	Type  : boolean
	Prompt: t10 internal codec
	    Location:
	     -> Device Drivers
	      -> Sound card support (SOUND [=y])
	       -> Open Sound System (DEPRECATED) (SOUND_PRIME [=y])
	        -> Open Sound System of xburst (SOUND_OSS_XBURST [=y])
	         -> Jz On-Chip I2S driver (SOUND_JZ_I2S_V12 [=y])
	          -> xburst internal coedc (JZ_INTERNAL_CODEC_V12 [=y])

----------

<h4 id="2.6">2.6 如何添加一个新的板级</h4>


1.建立一个板级文件夹

参考平台板级路径在 `arch/mips/xburst/soc-t21/chip-t21/isvp/` 下

我们已test_board 为例

	mkdir arch/mips/xburst/soc-t21/chip-t21/isvp/test_board

将我们参考平台板级turkey下的所有文件拷贝一份

	cp arch/mips/xburst/soc-t21/chip-t21/isvp/turkey/* arch/mips/xburst/soc-t21/chip-t21/isvp/test_board

2.修改 `arch/mips/xburst/soc-t21/chip-t21/isvp/` 下Makefile、Kconfig文件，使我们的板级文件参与编译

**Kconfig:**

	choice
	prompt "Board type"
	depends on PLATFORM_ISVP
	default BOARD_MANGO

	config BOARD_MANGO
       bool "Mango"
       depends on PLATFORM_ISVP

	config BOARD_TEST_BOARD
       bool "test_board"
       depends on PLATFORM_ISVP

	endchoice

	config BOARD_NAME
	string
	default "mango" if BOARD_MANGO
	default "test_board" if BOARD_TEST_BOARD

**Makefile:**

	CFLAGS_KERNEL	+= -I$(srctree)/arch/mips/xburst/soc-t10/chip-t10/$(CONFIG_PRODUCT_NAME)/$(CONFIG_BOARD_NAME)

	obj-y		+= common/
	obj-$(CONFIG_BOARD_MANGO)		+= $(CONFIG_BOARD_NAME)/
	obj-$(CONFIG_BOARD_TEST_BOARD)		+= $(CONFIG_BOARD_NAME)/

3.添加一个defconig文件

同样拷贝参考平台defconig文件

	cp arch/mips/configs/isvp_mango_defconfig arch/mips/configs/isvp_test_board_defconfig

打开`arch/mips/configs/isvp_test_board_defconfig`文件，找到如下定义

	# CONFIG_SOC_T15 is not set
	CONFIG_SOC_T10=y
	CONFIG_JZRISC=y
	CONFIG_BOARD_NAME="mango"
	CONFIG_PRODUCT_NAME="isvp"
	CONFIG_CHIP_T10=y
	# CONFIG_BOARD_MUSCA is not set
	CONFIG_PLATFORM_ISVP=y
	CONFIG_BOARD_MANGO=y
	CONFIG_EXTAL_CLOCK=24

修改 CONFIG\_BOARD\_NAME、CONFIG\_BOARD\_MANGO，注意和刚才Kconfig 中定义一致

	# CONFIG_SOC_T15 is not set
	CONFIG_SOC_T10=y
	CONFIG_JZRISC=y
	CONFIG_BOARD_NAME="test_board"
	CONFIG_PRODUCT_NAME="isvp"
	CONFIG_CHIP_T10=y
	# CONFIG_BOARD_MUSCA is not set
	CONFIG_PLATFORM_ISVP=y
	CONFIG_BOARD_TEST_BOARD=y
	CONFIG_EXTAL_CLOCK=24

这样一个新的板级就建立完成了，用户可以在新的板级中DIY了。


----------

<h4 id="2.7">2.7 SDIO WiFi 如何做Host AP</h4>

SDIO WiFi 如果要切换成Host AP功能，需要先更换固件。正常WiFi功能与 Host AP 功能的固件是分开的，在切换模式时，要先切换固件。对于AP6181，方法如下：

先指定固件地址路径，

	 echo "/lib/firmware/fw_bcm40181a2_apsta.bin">/sys/module/bcmdhd/parameters/firmware_path

起WiFi时，会去对应路径加载固件，然后做相应功能。


<h4 id="2.8">2.8 如何添加一个SPI Flash</h4>

uboot和kernel需要同步添加,对于uboot

	vi drivers/spi/jz_spi.h

找到 jz\_spi\_support_table数组，把新加的flash按格式放进去，可以copy一份任意现有的flash配置，主要修改name、id\_manufactory、sector\_size、size，sector\_size是flash的擦大小，建议设成64K，提高擦除速度，如果不支持64K请设成32K。

以添加一个8M flash EN25QH64为例：

    {
	    .name           = "EN25QH64",
		.id_manufactory = 0x1c7017,
	    .page_size       = 256,
	    .sector_size      = (32 * 1024),
	    .addr_size = 3,
		.size           = (8 * 1024 * 1024),
	    .quad_mode = {
	        .dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
	        .WRSR_CMD = CMD_WRSR_1,
	        .RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
	        .RD_DATE_SIZE = 1,
	        .WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
	        .WD_DATE_SIZE = 1,
	        .cmd_read = CMD_QUAD_READ,
	#ifdef CONFIG_JZ_SFC
	        .sfc_mode = TRAN_SPI_QUAD,
	#endif
	    },
	},

如果要使用四线模式，请根据spec定义设置quad_mode参数。

对于kernel，

T10：

	vi arch/mips/xburst/soc-t10/chip-t10/isvp/common/spi_bus.c

T20：

	vi arch/mips/xburst/soc-t20/chip-t20/isvp/common/spi_bus.c

T21：

	vi arch/mips/xburst/soc-t21/chip-t21/isvp/common/spi_bus.c


找到spi\_nor\_pdata定义添加方法与uboot类似，只是参数名称不太一样，其中id对应uboot中id\_manufactory，erasesize对应uboot中sector\_size，chipsize对应uboot中size，pagesieze和sectorsize可以不改。

	    {
        .name           = "EN25QH64",
        .pagesize       = 256,
        .sectorsize     = 4 * 1024,
        .chipsize       = 8192 * 1024,
        .erasesize      = 32 * 1024,//4 * 1024,
        .id             = 0x1c7017,

        .block_info     = flash_block_info,
        .num_block_info = ARRAY_SIZE(flash_block_info),

        .addrsize       = 3,
        .pp_maxbusy     = 3,            /* 3ms */
        .se_maxbusy     = 400,          /* 400ms */
        .ce_maxbusy     = 8 * 10000,    /* 80s */

        .st_regnum      = 3,
	#ifdef CONFIG_SPI_QUAD
        .quad_mode = &flash_quad_mode[0],
	#endif
    },


--------
<h3 id="3">3. 预留内存及内核裁减</h3>
预留内存：rmem为系统Video Buffer所需内存

* rmem的内存可以使用SDK中的rmem计算器excel表格来计算得到。
* 先把rmem设置大一下，先让系统跑起来，然后执行 impdbg --sys;logcat 可以看到rmem实际使用的内存和剩余内存，然后把剩余内存从rmem拿出来给mem使用。

内核默认的config留有一定余量，而实际产品往往需要进行内核裁减以释放更多的空间。以下是几个常用可供裁减的选项及说明：

* CONFIG\_NETWORK\_FILESYSTEMS：网络文件系统，一般用来方便开发，但会占用较多空间，也可用tftp进行替代。若不支持NFS，可以Disable
* CONFIG\_KALLSYMS & CONFIG\_KALLSYMS_ALL：内核函数符号表，会占用较多空间，在panic时的函数栈可以显示出函数名。当内核稳定后，可以考虑Disable此功能。但建议完全稳定之前使能此功能。
* 其他文件系统。一般文件系统会占用较多空间，开发者可根据需求对文件系统的选项进行裁减，比如ext文件系统等
* USB。若系统使用SDIO WIFI而无USB接口的相关功能，可以完全Disable掉USB。

**注：内核的深度裁减有一定的技术难度，开发者尽量在深入了解配置的情况下再进行深度裁减。**
