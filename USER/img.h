/**************************************************************************************
*	描述：这个文件是对img.c里用到的函数进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/


#ifndef _MY_IMG_
#define _MY_IMG_

#include "base.h"

//标准化宽和高
#define STD_W 8
#define STD_H 16

//矩形结构的定义
typedef struct Rct
{
	u16 X1;		//左上角X和Y坐标
	u16 Y1;
	u16 X2;		//右下角X和Y坐标
	u16 Y2;
	//u16 lefttopX;
	//u16 lefttopY;
	//u16 rightbottomX;
	//u16 rightbottomY;
	// u16 leftbottomX;
	// u16 leftbottomY;
	// u16 righttopX;
	// u16 righttopY;
}DRect;

typedef struct Pot
{
	u16 x;
	u16 y;
}DPoint;


//建立矩形链表单元
typedef struct rectLink
{
	DRect data;
	struct rectLink *next;
}RectLink;

//一下三个函数均在mybmp.h中定义并在mybmp中实现
//extern u8 **allloc_mem2d(u16 height, u16 width);
//extern void delete_mem2d(u8 **mem, u16 height, u16 width);
//extern void displayImg(u8 **image, u16 height, u16 width);

//创建长度为num的单向矩形链表
RectLink *CreateRectLink(u16 num);

//用给定的DRect初始化矩形链表
u16 InitRectLink(RectLink *head, DRect rect);

//销毁链表
u16 DeRectLink(RectLink *head);

//显示得到的矩形链表
void ShowRectLink(RectLink *rlink);

//粗略分离出字符所在的矩形区域
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth);

//倾斜度矫正
void SlopeAdjust(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth);

//归一化单个字符到指定宽和高
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//标准化图像
u16 CopyImg(u8 **Dst, u8 **Src, DRect dstRect, DRect srcRect);

//特征提取，13个特征
u16 TZTQ13(u8 **tz, u8 **Src, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//将特征归一化到0-1之间
void TZ_Std(double **tzDb,u8 **tz, u16 num, u16 tzCount);

//分离出单个字符组成的数组
void DetectNum(u8 **Src, u16 srcHeight, u16 srcWidth , RectLink *rectlink, u16 num);

//将图像反相，0变成1,1变成0
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth);

//将图像的像素值全部置为tag
void SetImg(u8 **Dst, u16 srcHeight, u16 srcWidth, u8 tag);

//将图像二值化
void BinaryImg(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, u8 thres);

//计算图像的全局阈值
u8 GlobalThreshold(u8 **img, u16 height, u16 width);

//OSTU求图像的阈值
u8 otsuThreshold(u8 **img, u16 height, u16 width);

//简单填充，有可能会产生新的“孔”
u16 floodfill(u8 **img, u16 height, u16 width);

//寻找图像中的孤立像素块
bool FindBlock(u8 **img, u16 height, u16 width, u16 x, u16 y, bool lab[], DPoint visted[],u16 lianxushu);

#endif

