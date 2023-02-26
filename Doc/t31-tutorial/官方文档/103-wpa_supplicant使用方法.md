#wpa\_supplicant及wpa\_cli使用方法

----------

## 1. 概述
wpa\_supplicant是一个连接、配置WIFI的工具。它主要包含两个程序：wpa\_supplicant与wpa\_cli。二者的关系就是server与client的关系。通常情况下，我们可以通过wpa\_cli来进行WIFI的配置与连接，如果有特殊的需要，可以编写应用程序直接调用wpa\_supplicant的接口直接开发。  
本文主要讲述如何通过wpa\_cli进行WIFI的配置与连接。

## 2. 使用方法
### 2.1 启动wpa_supplicant应用

    $ wpa_supplicant -D nl80211 -i wlan0 -c /etc/wpa_supplicant.conf -B

注意：/etc/wpa\_supplicant.conf文件里，添加下面代码。

    ctrl_interface=/var/run/wpa_supplicant
    update_config=1

### 2.2 启动wpa_cli应用

    $ wpa_cli -i wlan0 scan             搜索附近wifi网络
    $ wpa_cli -i wlan0 scan_result      打印搜索wifi网络结果
    $ wpa_cli -i wlan0 add_network      添加一个网络连接

如果要连接加密方式是：[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS] (wpa加密)  
wifi名称是：wifi\_name  
wifi密码是：wifi\_psk  

    $ wpa_cli -i wlan0 set_network 0 ssid '"wifi_name"'
    $ wpa_cli -i wlan0 set_network 0 psk '"wifi_psk"'
    $ wpa_cli -i wlan0 enable_network 0

如果要连接加密方式是：[WEP][ESS] (wep加密)  
wifi名称是：wifi\_name  
wifi密码是：wifi\_psk  

    $ wpa_cli -i wlan0 set_network 0 ssid '"wpa_name"'
    $ wpa_cli -i wlan0 set_network 0 key_mgmt NONE
    $ wpa_cli -i wlan0 set_network 0 wep_key0 '"wap_psk"'
    $ wpa_cli -i wlan0 enable_network 0

如果要连接加密方式是：[ESS] (无加密)  
wifi名称是：wifi\_name

    $ wpa_cli -i wlan0 set_network 0 ssid '"wifi_name"'
    $ wpa_cli -i wlan0 set_network 0 key_mgmt NONE
    $ wpa_cli -i wlan0 enable_network 0

### 2.3 分配ip,netmask,gateway,dns

    $ udhcpc -i wlan0 -s /etc/udhcpc.script -q

执行完毕，就可以连接网络了。

### 2.4 保存连接

    $ wpa_cli -i wlan0 save_config

### 2.5 断开连接

    $ wpa_cli -i wlan0 disable_network 0

### 2.6 连接已有的连接

    $ wpa_cli -i wlan0 list_network             列举所有保存的连接
    $ wpa_cli -i wlan0 select_network 0         连接第1个保存的连接
    $ wpa_cli -i wlan0 enable_network 0         使能第1个保存的连接

### 2.7 断开wifi

    $ ifconfig wlan0 down
    $ killall udhcpc
    $ killall wpa_supplicant

## 3. wpa\_wifi\_tool使用方法

wpa\_wifi\_tool是基于wpa\_supplicant及wpa_cli的一个用于快速设置wifi的工具，方便调试时连接wifi使用。使用者无需运行步骤2中复杂的命令，即可实现wifi设置和连接等功能。  
注：此工具仅作为调试工具使用，实际wifi功能开发推荐使用wpa\_cli或者直接调用wpa\_supplicant实现。

使用方法：

* 运行wpa\_wifi\_tool
* 输入help进行命令查看
* s进行SSID扫描
* c[n]进行wifi连接，连接时若为新的SSID则需输入密码，若为已保存的SSID则可以使用保存过的密码或者重新输入密码
* e退出工具

其他使用方法请参考help