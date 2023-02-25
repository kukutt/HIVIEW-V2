**白嫖方式搞个webrtc玩玩之codespaces里的网络摄像机**

# 目的
openhisilicon/HIVIEW是一个非常优秀的音视频项目, 其实现web/http/webrtc/rtsp/rtmp/onvif等等网络摄像机(简称：IPC)基础功能  
codespaces是一个github发布一个非常优秀集IDE/编译/调试的虚拟服务器   
故, 白嫖之~(用最简单最经济的途径来入门音视频)   
当然, 建议还是使用自己虚拟机or开发板来实现这一系列研究和学习   

# 参考资料
* [HIVIEW2](https://github.com/kukutt/hiview) : 基于openhisilicon/HIVIEW做了一些修改, 主要目的增加其他平台支持(第一站x86)
* [openhisilicon/HIVIEW](https://github.com/openhisilicon/HIVIEW) : 原始的开源网络摄像头项目（感谢提供这么优秀的项目）, 目前仅支持海思平台
* [视频教程](https://space.bilibili.com/582757748?spm_id_from=333.337.search-card.all.click) : 也是原项目作者的视频教程（再次感谢~）

# 开始吧~
## 开启codespaces

![1](pic\1.png)

![2](pic\2.png)

![3](pic\3.png)

![4](pic\4.png)

## 环境准备

经过一段时间等待，codespaces创建完成，就是一个网页版的VScode，终端可以输入命令，先准备一下环境吧~

![5](E:\HIVIEW-V2\Doc\入门\pic\5.png)

```
sudo apt-get update
# 原项目的x86平台用32位编辑的, 故如果64位系统需要做兼容
sudo apt-get install gcc-multilib g++-multilib build-essential module-assistant
# 由于x86使用ffmpeg产生测试视频使用, 如果你有其他视频.h264文件, 可以忽略
sudo apt-get install ffmpeg
```

## 编译

```
. ./build/x86
make
```

## 修改webrtc的地址

需要修改一下webrtc url, 如下图

![6](E:\HIVIEW-V2\Doc\入门\pic\6.png)

## 运行

```
# 运行指令
rootfs/x86/run.sh
# 关闭指令
rootfs/x86/run.sh kill
```

## 预览

可以在自己电脑使用chrome预览。  

1. 打开网址[https://kukutt.glitch.me/](https://kukutt.glitch.me/)
2. 输入中[修改webrtc的地址章节](#修改webrtc的地址)中的webrtc_url后缀

![7](E:\HIVIEW-V2\Doc\入门\pic\7.png)

![8](E:\HIVIEW-V2\Doc\入门\pic\8.png)