/**************************************************************************************
*	描述：这个文件是对memmgr.c里用到的函数进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#ifndef _MY_MEM_H_
#define _MY_MEM_H_

#include "base.h"

//申请2维uchar内存
u8 **alloc_mem2d_u8( u16 height, u16 width);
//释放2维uchar内存
void delete_mem2d_u8( u8 **img, u16 height, u16 width);
//分配2维double数组内存
double **alloc_mem2d_dbl(u16 height, u16 width);
//释放2维double数组内存
void delete_mem2d_dbl(double **mem, u16 height, u16 width);

#endif

