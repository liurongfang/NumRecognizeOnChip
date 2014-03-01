本实验将实现如下功能：该实验实现ALIENTEK MiniSTM32开发板驱动ALIENTEK OV7670摄像头模块，在硬件连接正确后，我们可以在LCD模块上看到摄像头模块所拍摄到的内容。摄像头模块的各种参数设置，均通过USMART实现，比如光照模式、色饱和度、亮度、对比度以及特效等，都可以通过USMART设置，另外还可以通过USMART对OV7670的单个寄存器进行修改，从而方便大家调试。
DS0用于指示程序运行状态。

注意：
1，在连接OV7670模块与MiniSTM32开发板的时候，请将D0~D7连接线靠在一起，将其与数据线靠在一起，这样可以有效避免相互干扰（参考帖子：http://www.openedv.com/posts/list/9359.htm）。
2，ALIENTEK OV7670摄像头模块与ALIENTEK MiniSTM32开发板的连接方式为：
D0~D7-->PB0~PB7
WRST -->PA0
RRST -->PA4
SCL  -->PC4
SDA  -->PC5
OE   -->PA11
WEN  -->PA12
RCLK -->PA1
VSYNC-->PA15
 


               	正点原子@ALIENTEK
               	2012-10-31
		广州市星翼电子科技有限公司
                联系电话（传真）：020-38271790
	       	购买：http://shop62103354.taobao.com
                      http://shop62057469.taobao.com
               	技术支持论坛：www.openedv.com