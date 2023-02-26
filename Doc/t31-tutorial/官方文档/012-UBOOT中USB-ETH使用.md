# UBOOT中USB-ETH使用

1. 启动uboot。
2. 插入网卡（目前仅支持asx88x72系列网卡)
3. 输入usb start，正常情况下终端会输出以下结果
  
		usb start  
	    (Re)start USB...  
	    USB0:   Core Release: 3.00a  
	    scanning bus 0 for devices... 2 USB Device(s) found  
       	scanning usb for storage devices... 0 Storage Device(s) found  
       	scanning usb for ethernet devices... 1 Ethernet Device(s) found  
4. 执行 env print命令，查看ethact变量是否为usb网卡(asx的usb网卡为asxN,N为网卡号),
如果不是，则执行setenv ethact asx0将活动网卡设置为usb网卡。
 
