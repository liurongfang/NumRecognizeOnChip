/**************************************************************************************
*	描述：这个文件对main.c里面用到的变量和函数进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/


#ifndef _MY_MAIN_
#define _MY_MAIN_

#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "lcd.h"	  
#include "ov7670.h"
#include "usmart.h"

#include "img.h"
#include "memmgr.h"

//定义LCD屏幕尺寸
#define LCD_H 320
#define LCD_W 240

//定义获取图像的尺寸
#define IMG_H 80
#define IMG_W 60

#define NUM 5
#define TZ  13

#define THRES 40  	  

extern u8 ov_sta;	//在exit.c里面定义，状态
extern u8 ov_frame;	//在timer.c里面定义，帧率

u8 thres = 60;

//函数定义		 
u16 system_init(void);		//系统启动时的初始化
void camera_refresh(u8 **img, u16 yScale, u16 xScale);		//更新LCD显示
void img_display(u8 **img, u16 height, u16 width, u16 x, u16 y, u8 mode);	//从数组更新图像到LCD
void print2serial(u8 **img, u16 height, u16 width);			//输出到串口
u16 ImageHandle(u8 **tz, u8 **img, u16 srcHeight, u16 srcWidth, u16 num);		//在这里完成图像处理的相关操作

#endif



