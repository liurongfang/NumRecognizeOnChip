硬件连接注意：
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
3，DS0用于指示程序运行状态。
