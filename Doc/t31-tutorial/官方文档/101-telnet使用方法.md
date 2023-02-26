#telnet使用说明

----------

开发时，可以通过Linux系统或者Windows XP(Win7需要手动开启telnet功能)的telnet工具连接开发板，登陆终端进行操作。使用步骤如下：

* 开发板启动并且连接到网络。ifconfig获取开发板的IP
* PC需要和开发板在同一网络（可以ping通）
* PC端执行telnet [Device IP]，例如: telnet 192.168.1.100
* 输入用户名: “root”。登陆成功后即进入开发板终端。