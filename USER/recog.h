/**************************************************************************************
*	描述：这个文件对recog.c里用到的函数和变量进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#ifndef _MY_RECOG_
#define _MY_RECOG_

#include "base.h"
#include <math.h>

//拟采用闵可夫斯基向量距离进行试识别（曼哈顿距离与欧氏距离）
void MinkowskiMetric(u8 *result, u8 **tzDb, u16 num, u16 tzCount);

#endif
