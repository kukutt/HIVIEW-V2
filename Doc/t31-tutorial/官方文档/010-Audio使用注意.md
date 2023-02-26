# Audio使用注意

----------


1. 内核配置  
  
        CONFIG_COMPILE_JZSOUND_INTO_KO=y  

2. driver/audio/oss2/目录下执行make，生成audio.ko驱动并在系统启动后安装驱动
3. libaudioProcess.so音频的算法库，需要放在root中的/lib、/usr/lib、或者`LD_LIBRARY_PATH`指定路径
4. AO和AI的采样率需要一致
5. 支持的算法有降噪NS功能，AGC（录放），HPF，以及AEC
6. 音量的调节建议模拟gain使用sample的默认设置，然后调节vol数据gain来设置音量