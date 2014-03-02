/**************************************************************************************
*	����������ļ���ĺ�������ڴ�ռ���������ͷ�
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include "memmgr.h"
#include "base.h"
#include "lcd.h"

//����2άuchar�ڴ�
u8 **alloc_mem2d_u8( u16 height, u16 width)
{
	u16 i;
	u8 **p = NULL;

	//LCD_ShowString(40,50,200,200,16,"start malloc...");

	
	p = (u8 **)malloc(sizeof(u8 *)*height);

	//LCD_ShowString(40,50,200,200,16,"first malloc ok...");
	for (i = 0; i<height; i++)
	{
		p[i] = (u8 *)malloc(sizeof(u8 )*width);
	}

//������
//	for (i = 0; i<height; i++)
//	{
//		for (j = 0; j<width; j++)
//		{
//			p[i][j] = 0xff;
//		}
//	}

	//LCD_ShowString(40,50,200,200,16,"malloc...");

	if (p == NULL) 
	{
		printf("err:malloc failed!");
		LCD_ShowString(40,50,200,200,16,"malloc failed!");
	}

	return (p);
}

//�ͷ�2άuchar�ڴ�
void delete_mem2d_u8( u8 **img, u16 height, u16 width)
{
	u16 i;
	u8 **p = img;

	for (i = 0; i<height; i++)
	{
		free(p[i]);
	}
	free(p);

}

//����2άdouble�����ڴ�
double **allloc_mem2d_dbl(u16 height, u16 width)
{
	u16 i,j;

	//�����ڴ�
	double **mem = (double **)malloc(sizeof(double *)*height);
	for (i = 0; i<height; i++)
	{
		mem[i] = (double *)malloc(sizeof(double )*width);
	}

	//��ʼ��
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			mem[i][j] = 0.000001;
		}
	}

	return (mem);
}

//�ͷ�2άdouble�����ڴ�
void **detect_mem2d_dbl(double **mem, u16 height, u16 width)
{
	u16 i = 0;

	for (i = 0; i<height; i++)
	{
		free(mem[i]);
	}
	free(mem);

	return 0;
}

