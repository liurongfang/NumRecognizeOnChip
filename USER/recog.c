/**************************************************************************************
*	����������ļ���ĺ�������ڴ�ռ���������ͷ�
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include "recog.h"

//������ɿɷ�˹���������������ʶ�������پ�����ŷ�Ͼ��룩
void MinkowskiMetric(u8 *result, u8 **tzDb, u16 num, u16 tzCount)
{
	u16 i,j,k,good;
	//u16 result[num];		//����ʶ����
	double min = 100000.0;
	double s[5];
	double model[5][13] = {{0.0}

							};//����ģ������

	//�Դ�ʶ���num������
	for (k = 0; k<num; k++)
	{
		//����
		for (j = 0; j<num; j++) s[j] = 0;

		//ѭ����num��ģ������
		for (i = 0; i<num; i++)
		{
			for (j = 0; j<tzCount; j++)
			{
				s[i] = abs(tzDb[k][j]-model[i][j]);	//�����پ���
			}
		}

		//Ѱ�Ҳ���С����Ϊ���
		for (j = 0; j<tzCount; j++)
		{
			if (s[j] < min)
			{
				good = j;		//��¼����"��"�ĸ�
				min = s[j];
			}
		}

		result[i] = good;	//����ʶ����
	}

	//return result;
}

