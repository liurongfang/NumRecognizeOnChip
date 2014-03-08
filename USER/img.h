/**************************************************************************************
*	描述：这个文件是对img.c里用到的函数进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/


#ifndef _MY_IMG_H_
#define _MY_IMG_H_

#include "base.h"

//图像尺寸归一化的宽和高
#define STD_W 8
#define STD_H 16

//定义前景色和背景色,1：白色，0：黑色
#define FORECOLOR 1
#define BACKCOLOR 0

//0x1:0-1反相，0xff:0-255反相
#define CR 0x1

//矩形结构的定义
typedef struct Rct
{
	u16 X1;		//左上角X和Y坐标
	u16 Y1;
	u16 X2;		//右下角X和Y坐标
	u16 Y2;
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


//创建长度为num的单向矩形链表
//输入：num 要建的链表长度
RectLink *CreateRectLink(u16 num);

//用给定的DRect初始化矩形链表
//输入：head 链表头指针，rect 初始化矩形结构
u16 InitRectLink(RectLink *head, DRect rect);

//销毁链表
u16 DeRectLink(RectLink *head);

//显示得到的矩形链表
//输入： rlink 链表头指针
//输出：调用printf()函数输出
void ShowRectLink(RectLink *rlink);

//粗略分离出字符所在的矩形区域
//输入：Src 图像数组指针，srcHeight 图像高度，srcWidth 图像宽度
//输出：得到的矩形结构
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth);

//倾斜度矫正
//输入：Dst 存放矫正后的图像数组，Src 待矫正的图像数组指针，srcHeight 图像高度，srcWidth 图像宽度
void SlopeAdjust(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth);

//归一化单个字符到指定宽和高
//输入：Dst 存放尺寸归一化的图像数组，Src 待归一的图像数组指针，srcHeight 图像高度，srcWidth 图像宽度
//rlink 在Src中求得的字符矩形链表，num 图像中数字的个数
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//拷贝图像，从源图像的矩形区域拷贝到目标图像的矩形区域
//输入：Dst 存放目标图像，Src 存放源图像，dstRect 目标图像中的矩形区域，srcRect 源图像中的矩形区域
u16 CopyImg(u8 **Dst, u8 **Src, DRect dstRect, DRect srcRect);

//特征提取，13个特征
//输入：tz 存放特征的矩阵，Src 待提取的图像数组，srcHeight 图像高度，srcWidth 图像宽度
//rlink 在Src中求得的字符矩形链表，num 图像中数字的个数
u16 TZTQ13(u8 **tz, u8 **Src, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//分离出单个字符所在的矩形，保存在矩形结构链表rlink里
//输入：Src 待提取的图像，srcHeight 图像高度，srcWidth 图像宽度
//rlink 在Src中求得的字符矩形链表，num 图像中数字的个数
void DetectNum(u8 **Src, u16 srcHeight, u16 srcWidth , RectLink *rectlink, u16 num);

//将图像反相，0变成1,1变成0，将图像在CR-0之间反相，宏定义CR可修改
//输入：Dst 目标图像，Src 源图像，srcHeight 图像高度，srcWidth 图像宽度
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth);

//将图像的像素值全部置为tag
//输入：Dst 目标图像，Src 源图像，srcHeight 图像高度，srcWidth 图像宽度，tag 要置的值
void SetImg(u8 **Dst, u16 srcHeight, u16 srcWidth, u8 tag);

//将图像二值化
//输入：Dst 目标图像，Src 源图像，srcHeight 图像高度，srcWidth 图像宽度，thres 阈值
void BinaryImg(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, u8 thres);

//计算图像的全局阈值
//输入：img 待计算的灰度图像，height 图像高度，width 图像宽度
//输出：阈值（u8 型）
u8 GlobalThreshold(u8 **img, u16 height, u16 width);

//OSTU求图像的阈值
//输入：img 待计算的灰度图像，height 图像高度，width 图像宽度
//输出：阈值（u8 型）
u8 otsuThreshold(u8 **img, u16 height, u16 width);

//简单填充，有可能会产生新的“孔”
//输入：img 待填充的灰度图像，height 图像高度，width 图像宽度
u16 floodfill(u8 **img, u16 height, u16 width);


//细化图像，用来使特征集中且更明显，要求前景色为1，背景色为0 ,lx:heighe,ly:width
//输入：image 图像数组，lx 图像高度，ly 图像宽度
void ThinnerRosenfeld(u8 **image, u16 lx, u16 ly);

#endif

