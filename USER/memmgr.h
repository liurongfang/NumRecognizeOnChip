/**************************************************************************************
*	����������ļ��Ƕ�memmgr.c���õ��ĺ�����������
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. ������ע������
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#ifndef _MY_MEM_
#define _MY_MEM_

#include "base.h"

//����2άuchar�ڴ�
u8 **alloc_mem2d_u8( u16 height, u16 width);
//�ͷ�2άuchar�ڴ�
void delete_mem2d_u8( u8 **img, u16 height, u16 width);
//����2άdouble�����ڴ�
double **allloc_mem2d_dbl(u16 height, u16 width);
//�ͷ�2άdouble�����ڴ�
void **detect_mem2d_dbl(double **mem, u16 height, u16 width);

#endif

