# GPIO接口及操作方法

----------

GPIO是“General Purpose Input/Output”的简称，具有输入输出，功能复用的功能。开发者在开发硬件驱动时往往需要操作GPIO，以控制外设硬件。

ISVP使用Linux标准的GPIOLIB接口。GPIOLIB提供了统一的GPIO申请/释放/设置/获取接口，按照GPIOLIB的设定，需要在Kernel space进行调用。如果是User space需要操作GPIO，有两种方法可以选择：

* 通过sysfs GPIO接口进行操作
* 应用程序调用相关的驱动，驱动中实现GPIO的设置

## 1. GPIOLIB
### 2.1 头文件及API
GPIOLIB的头文件为：`include/linux/gpio.h`

在驱动程序中加入头文件引用：

    #include <linux/gpio.h>

API在头文件`include/asm-generic/gpio.h`中定义，例如：

    int gpio_request(unsigned gpio, const char *label);
    void gpio_free(unsigned gpio);
    int gpio_direction_input(unsigned gpio);
    int gpio_direction_output(unsigned gpio, int value);
    int gpio_get_value(unsigned int gpio);
    void gpio_set_value(unsigned int gpio, int value);
    int gpio_to_irq(unsigned int gpio);

等等。详细的文档说明可参考`kernel/Documentation/gpio.txt`。

### 2.2 参考代码
Linux kernel中标准驱动的GPIO操作均使用标准的GPIOLIB，比如Software I2C，Fixed regulator，以及中断等等。

## 2. sysfs GPIO

sysfs GPIO是Linux标准的用户空间操作GPIO的接口。用户可用过命令行或者应用程序直接设置GPIO的输入/输出，高低电平等属性。一般情况下，GPIO调试或者简单的GPIO应用（比如IR-Cut操作），可通过sysfs GPIO接口进行快速开发。

### 2.1 内核选项
在内核源码根目录下执行`$ make menuconfig`命令进入配置界面，选中以下选项：

    Device Drivers  --->
          -*- GPIO Support  --->
    	       [*]   /sys/class/gpio/... (sysfs interface)

一般情况下，内核的默认配置已经勾选了此选项。

### 2.2 sysfs GPIO的申请与释放
在操作sysfs GPIO之前需要对其进行申请。值得注意的是，由于申请sysfs GPIO会在内核request_gpio，因此在内核中已经申请过的GPIO在sysfs GPIO再次申请会失败。

申请/释放GPIO方法如下：

    $ cd /sys/class/gpio
    $ echo [gpio_num] > export         #申请GPIO
    $ echo [gpio_num] > unexport       #释放GPIO

注：gpio_num即GPIO号。计算公式为：

    PA(n) = 0 * 32 + n
    PB(n) = 1 * 32 + n
    PC(n) = 2 * 32 + n
    ...

例如：申请PB(10) = 1 * 32 + 10 = 42

    $ echo 42 > export
申请后在`/sys/class/gpio`目录下即会出现`gpio42`目录。

    $ echo 42 > unexport
释放后gpio42目录也会消失。释放后的GPIO状态并不会恢复，会保持申请时的状态（电平等）。

### 2.3 设置输入/输出方式
在申请GPIO后，进入gpioN目录，例如`gpio42`，进行如下操作：

    $ echo out > direction             #设置PB10为输出模式，设置后输出低电平
    $ echo in > direction              #设置PB10为输入模式

### 2.4 设置有效电平
gpioN目录下有active_low节点，表示当前GPIO的有限电平，默认为0，其意义为，当输入/输出value为0时，GPIO为低电平，当输入/输出value为1时，GPIO为高电平。同样的，当active_low为1时，当输入/输出value为0时，GPIO为高电平，当输入/输出value为1时，GPIO为低电平。 也就是说，GPIO的真实电平=value^active_low。

    $ echo 0 > active_low		#value是0,表示低电平。value是1,表示高电平
    $ echo 1 > active_low		#value是1,表示低电平。value是0,表示高电平

### 2.5 输入/输出
gpioN目录下有value节点，表示gpioN的电平：当GPIO为输入模式时，读取到value的值异或active_low即为GPIO的电平；当GPIO为输出模式时，写入到value的值异或active_low即为GPIO的输出电平。

    $ cat value                 #读取电平（输入模式）
    $ echo 0 > value            #设置电平（输出模式）