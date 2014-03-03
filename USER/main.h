/**************************************************************************************
*	����������ļ���main.c�����õ��ı����ͺ�����������
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
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

//����LCD��Ļ�ߴ�
#define LCD_H 320
#define LCD_W 240

//�����ȡͼ��ĳߴ�
#define IMG_H 80
#define IMG_W 60

#define NUM 5
#define TZ  13

#define THRES 40  	  

extern u8 ov_sta;	//��exit.c���涨�壬״̬
extern u8 ov_frame;	//��timer.c���涨�壬֡��

u8 thres = 60;

//��������		 
u16 system_init(void);		//ϵͳ����ʱ�ĳ�ʼ��
void camera_refresh(u8 **img, u16 yScale, u16 xScale);		//����LCD��ʾ
void img_display(u8 **img, u16 height, u16 width, u16 x, u16 y, u8 mode);	//���������ͼ��LCD
void print2serial(u8 **img, u16 height, u16 width);			//���������
u16 ImageHandle(u8 **tz, u8 **img, u16 srcHeight, u16 srcWidth, u16 num);		//���������ͼ�������ز���

#endif



