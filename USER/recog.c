/**************************************************************************************
*	����������ļ���ĺ�������ڴ�ռ���������ͷ�
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include "recog.h"

//��������һ����0-1֮��
void TZ_Std(double **tzDb,u8 **tz, u16 num, u16 tzCount)
{
	u16 i,j;
	u16 max = 0, min = 1000;

	//Ԥ������u8 **tz���double **tz
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tzDb[i][j] = (double)tz[i][j];		//ǿ�ƽ�ԭ�������ͣ�u8ת��Ϊdouble��
			if (tzDb[i][j] > max) max = tzDb[i][j];		//�������ֵ����Сֵ
			if (tzDb[i][j] < min) min = tzDb[i][j];
		}
	}

	//�����������һ����0��1֮�䣬����һ����ע�͵���һ��
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tzDb[i][j] = (tzDb[i][j] - min + 1 )/(max - min + 1);		//��һ����0��1֮�䣬��1�Ƿ�ֹ��ĸΪ0
		}
	}
}

//������ɿɷ�˹���������������ʶ�������پ�����ŷ�Ͼ��룩,MinkowskiMetric
void Recognize(u8 *result, u8 **tz, u16 num, u16 tzCount)
{
	u16 i,j,k;
	u8 good;		//����ʶ����,result[num]
	double min = 1000000.0;		  //��ʾ�����
	double s[5];	//�����ȵľ���
	double **tzDb = NULL;
	const double model[5][13] = {	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
									{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
								};//��׼��������

	tzDb = alloc_mem2d_dbl(num, tzCount);

	//��������һ��
	TZ_Std(tzDb, tz, num, tzCount);

	//�Դ�ʶ���num������
	for (k = 0; k<num; k++)
	{
		//����
		//for (j = 0; j<num; j++) s[j] = 0;
		min = 1000000.0;	

		//ѭ����num��ģ������
		for (i = 0; i<num; i++)
		{
			for (j = 0; j<num; j++) s[j] = 0;

			for (j = 0; j<tzCount; j++)
			{
				s[i] += pow((tzDb[k][j] - model[i][j] ), 2);	//�����پ������ŷ�Ͼ���
			}
		}

		//Ѱ����õ���Ϊ���
		for (i = 0; i<num; i++)
		{
			if (s[i] < min)
			{
				good = i;		//��¼����"��"�ĸ�
				min = s[i];
			}
		}

		result[k] = (char )good;	//����ʶ����
	}
	result[num] = '\0';		//�ý�����

	delete_mem2d_dbl(tzDb, num, tzCount);

	//return result;
}

