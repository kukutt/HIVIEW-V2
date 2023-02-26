#ISP驱动及Sensor驱动说明

----------

## 1. ISP Sensor驱动的编译及加载
ISP及Sensor驱动提供编译好的ko。修改或者重编kernel后，不需要更新ko。 加载时需要加载tx-isp-t30.ko，然后再加载sensor_xxx_t30.ko。

## 2. 图像效果bin文件位置
不同的Sensor以及镜头可能需要不同的效果参数配置，配置文件位置为：`/etc/sensor`目录下，文件名为[sensor].bin和ncu_[sensor].bin，例如：`ov2735b.bin，ncu_ov2735.bin`。如果没有这个文件图像颜色等可能不正常。实际产品中效果bin文件往往需要随版本迭代更新，因此`/etc/sensor`目录需要有读写权限。可供参考的方法之一是将`/etc/sensor`目录做成一个软链接，链接到一个rw的分区中，这样就可以在版本更新时单独更新bin文件了。

## 3. Sensor驱动的配置
Sensor驱动需要配置GPIO等参数，这些参数作为module_param存在，如果硬件是按照参考设计做的话，insmod时无需跟参数，使用默认值即可。但如果硬件设计有差别，那么需要在insmod时加上参数进行配置。

* reset,power_down的配置：加载模块时加入参数，例如:

        insmod sensor_xx_t30.ko reset_gpio=18 pwdn_gpio=20
  其中，gpio的值为GPIO编号，规则为：`num = 32 * n + bit`，例如：PA18的GPIO编号为18，PC20的GPIO编号为84
* DVP数据端口配置：加载模块时加入参数，例如：

        insmod sensor_xx_t30.ko sensor_gpio_func=1
  其中sensor_gpio_func为DVP Port的配置选项，0：PA Low-10bit，1：PA High-10bit，2：PA 12bit。

## 4. Sensor的探测识别
Sensor识别驱动位于drivers/misc/sensor_info目录下，用户可以通过ioctl或者proc接口进行Sensor型号的查询。使用方法可参考例子sample_sinfo.c。

## 5. Sensor的数据接口配置
Sensor的数据接口有多种，目前我们支持DVP以及MIPI CSI-2.其中T10仅支持DVP接口.有的senosr可能同时支持两种数据接口，为了区分，我们加入了模块参数data_interface.
* MIPI CSI-2数据接口配置:
        insmod sensor_xx_t30.ko data_interface=1
  其中，data_inteface为sensor数据接口配置选项。1: MIPI CSI-2 2：DVP

## 6. Sensor最高帧率配置接口
基于降低功耗的考量，我们在sensor驱动中加入了配置最高帧率的参数sensor_max_fps。
一般sensor驱动中支持30/25fps与15fps的切换。

* sensor帧率配置为15fps:

        insmod sensor_xx_t30.ko sensor_max_fps=15
  其中，sensor_max_fps为sensor帧率配置选项。15:配置为15fps 25: 配置为25fps
  默认为30/25fps模式。
为了能够更好的降低功耗，在调整此参数的同时需调整（7）参数。

## 7. ISP时钟配置
当Sensor数据输入不同时，为了功耗以及稳定性考虑，我们需要配置不同的ISP core clock，因此增加了模块参数isp_clk.现在960p@30fps使用60Mhz，1080p@30fps使用80Mhz，3M@25fps使用100Mhz。一般来说只有输入3M@25fps时需要配置此参数。
* 3M@25fps配置：
	insmod tx-isp-t30.ko isp_clk=100000000
  其中，isp_clk为ISP时钟频率。
