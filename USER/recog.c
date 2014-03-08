/**************************************************************************************
*	描述：这个文件里的函数完成内存空间的申请与释放
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include "recog.h"

//将特征归一化到0-1之间
void TZ_Std(double **tzDb,u8 **tz, u16 num, u16 tzCount)
{
	u16 i,j;
	u16 max = 0, min = 1000;

	//预处理，将u8 **tz变成double **tz
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tzDb[i][j] = (double)tz[i][j];		//强制将原矩阵类型：u8转换为double型
			if (tzDb[i][j] > max) max = tzDb[i][j];		//更新最大值、最小值
			if (tzDb[i][j] < min) min = tzDb[i][j];
		}
	}

	//将特征矩阵归一化到0和1之间，不归一化就注释掉这一段
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tzDb[i][j] = (tzDb[i][j] - min + 1 )/(max - min + 1);		//归一化到0和1之间，加1是防止分母为0
		}
	}
}

//拟采用闵可夫斯基向量距离进行试识别（曼哈顿距离与欧氏距离）,MinkowskiMetric
void Recognize(u8 *result, u8 **tz, u16 num, u16 tzCount)
{
	u16 i,j,k;
	u8 good;		//保存识别结果,result[num]
	double min = 1000000.0;		  //表示无穷大
	double s[5];	//保存测度的距离
	double **tzDb = NULL;
	const double model[5][13] = {	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
								};//标准特征矩阵

	tzDb = alloc_mem2d_dbl(num, tzCount);

	//将特征归一化
	TZ_Std(tzDb, tz, num, tzCount);

	//对待识别的num个数字
	for (k = 0; k<num; k++)
	{
		//清零
		//for (j = 0; j<num; j++) s[j] = 0;
		min = 1000000.0;	

		//循环与num个模板做差
		for (i = 0; i<num; i++)
		{
			for (j = 0; j<num; j++) s[j] = 0;

			for (j = 0; j<tzCount; j++)
			{
				s[i] += pow((tzDb[k][j] - model[i][j] ), 2);	//曼哈顿距离或者欧氏距离
			}
		}

		//寻找最好的作为结果
		for (i = 0; i<num; i++)
		{
			if (s[i] < min)
			{
				good = i;		//记录下最"像"哪个
				min = s[i];
			}
		}

		result[k] = (char )good;	//保存识别结果
	}
	result[num] = '\0';		//置结束符

	delete_mem2d_dbl(tzDb, num, tzCount);

	//return result;
}

