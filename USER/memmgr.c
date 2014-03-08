/**************************************************************************************
*	描述：这个文件里的函数完成内存空间的申请与释放
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include "memmgr.h"

//申请2维uchar内存
u8 **alloc_mem2d_u8( u16 height, u16 width)
{
	u16 i;
	u8 **p = NULL;
	
	p = (u8 **)malloc(sizeof(u8 *)*height);

	for (i = 0; i<height; i++)
	{
		p[i] = (u8 *)malloc(sizeof(u8 )*width);
	}

	if (p == NULL) 
	{
		printf("err:malloc failed!");
		LCD_ShowString(40,50,200,200,16,"malloc failed!");
	}

	return (p);
}

//释放2维uchar内存
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

//分配2维double数组内存
double **alloc_mem2d_dbl(u16 height, u16 width)
{
	u16 i,j;

	//申请内存
	double **mem = (double **)malloc(sizeof(double *)*height);
	for (i = 0; i<height; i++)
	{
		mem[i] = (double *)malloc(sizeof(double )*width);
	}

	//初始化
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			mem[i][j] = 0.000001;
		}
	}

	return (mem);
}

//释放2维double数组内存
void delete_mem2d_dbl(double **mem, u16 height, u16 width)
{
	u16 i = 0;

	for (i = 0; i<height; i++)
	{
		free(mem[i]);
	}
	free(mem);

}

