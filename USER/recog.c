/**************************************************************************************
*	描述：这个文件里的函数完成内存空间的申请与释放
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include "recog.h"

//拟采用闵可夫斯基向量距离进行试识别（曼哈顿距离与欧氏距离）
void MinkowskiMetric(u8 *result, u8 **tzDb, u16 num, u16 tzCount)
{
	u16 i,j,k,good;
	//u16 result[num];		//保存识别结果
	double min = 100000.0;
	double s[5];
	double model[5][13] = {{0.0}

							};//基本模板特征

	//对待识别的num个数字
	for (k = 0; k<num; k++)
	{
		//清零
		for (j = 0; j<num; j++) s[j] = 0;

		//循环与num个模板做差
		for (i = 0; i<num; i++)
		{
			for (j = 0; j<tzCount; j++)
			{
				s[i] = abs(tzDb[k][j]-model[i][j]);	//曼哈顿距离
			}
		}

		//寻找差最小的作为结果
		for (j = 0; j<tzCount; j++)
		{
			if (s[j] < min)
			{
				good = j;		//记录下最"像"哪个
				min = s[j];
			}
		}

		result[i] = good;	//保存识别结果
	}

	//return result;
}

