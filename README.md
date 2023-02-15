
# 编译
## x86下使用
```
. ./build/x86
make
./rootfs/x86/run.sh
```
说明：
1. 如果64位系统编译有问题, 可以尝试`sudo apt-get install gcc-multilib g++-multilib build-essential module-assistant`, 安装lib32兼容库;
2. 可以使用`./rootfs/x86/run.sh kill`杀掉运行进程

