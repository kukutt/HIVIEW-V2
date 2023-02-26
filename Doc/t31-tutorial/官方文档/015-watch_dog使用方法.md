#watchdog 使用方法

----------

## 1. 内核选项
在内核源码根目录下执行`$ make menuconfig`命令进入配置界面，选中以下选项：
Location:
       -> Device Drivers
           -> Watchdog Timer Support (WATCHDOG [=y])
一般情况下，内核的默认配置已经勾选了此选项。

## 2. sample

参考SDK中WDT使用sample
