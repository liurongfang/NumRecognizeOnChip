/**************************************************************************************
*	����������ļ���recog.c���õ��ĺ����ͱ�����������
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#ifndef _MY_RECOG_H_
#define _MY_RECOG_H_

#include "base.h"
#include "memmgr.h"

//��������һ����0-1֮��
void TZ_Std(double **tzDb,u8 **tz, u16 num, u16 tzCount);

//������ɿɷ�˹���������������ʶ�������پ�����ŷ�Ͼ��룩
void Recognize(u8 *result, u8 **tz, u16 num, u16 tzCount);

#endif
