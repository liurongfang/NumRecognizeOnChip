/**************************************************************************************
*	����������ļ���ĺ�����ͼ����к��ڴ����������ȡ
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include <math.h>
#include "img.h"
#include "usart.h"
#include "usmart.h"


//���庯��Ҫ�õı���
u16 g_lianxushu = 0;
DPoint visted[20];
bool lab[4000] = {FALSE};


//������������
RectLink *CreateRectLink(u16 num)
{
	RectLink *head = NULL;		//����ͷָ��
	RectLink *p, *pTmp;			//p����ǰ��Ԫ��pTmp�������һ����Ԫ
	u16 i = 0;

	//�ǵ�һ���ڵ㣬��headֱ����ϵ����
	head = p = pTmp = (RectLink *)malloc(sizeof(RectLink));
	p->next = NULL;

	//���ǵ�1���ڵ㣬��pTmp���м����ӵ���β��
	//������num-1��ѭ��
	while (--num)
	{
		p = (RectLink *)malloc(sizeof(RectLink));		//�������ڴ�ռ�
		p->next = NULL;			//β��ֵNULL
		pTmp->next = p;			//����ǰ����Ԫ�����ӵ��������
		pTmp = pTmp->next;		//ָ�����һλ
		i++;
	}

	return (head);

}

//�ø�����DRect��ʼ����������
u16 InitRectLink(RectLink *head, DRect rect)
{
	RectLink *p = head;
	while (p->next != NULL)
	{
		p->data = rect;
		p = p->next;
	}
	p->data = rect;

	return 0;
}

//��������
u16 DeRectLink(RectLink *head)
{
	RectLink *p;

	while(head->next != NULL)
	{
		p = head->next;
		free(head);
		head = p;
	}
	free(head);		//head->next == NULL�����һ���ڵ㣬�ͷŵ�
	head = NULL;	//��ͷ�����Ϊ��

	return 0;
}

//��ʾ�õ��ľ�������
void ShowRectLink(RectLink *rlink)
{
	u16 i = 0;
	RectLink *p = rlink;	//����ע�⣬����ֱ�Ӳ���rlink

	printf("��������ӡ��õľ�������\n");
	do
	{
		printf("��%d�����Σ�{%d %d %d %d}\n",i,p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
		i++;
		p = p->next;
	}
	while(p != NULL);
	printf("�ܹ�%d������\n",i);

	//system("pause");
}
//��б�Ƚ���,��������
void SlopeAdjust(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth)
{
	u16 i,j;
	u16 i_src;
	//ͼ�����ߵ�ƽ���߶�
	double leftaver=0.0;

	//ͼ���Ұ�ߵ�ƽ���߶�
	double rightaver=0.0;

	//ͼ�����б��
	double slope;

	//ͳ��ѭ������
	u16 counts=0;
	//u16 x1,x2;
	//double delta_x;
	//
	////�������£��������ң���¼���󶥵�
	//for (i = 0; i<srcHeight; i++)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//��������ڵ�
	//		{
	//			x1 = j;		//��¼�µ�ǰxֵ����Ϊ�󶥵�x

	//			j = srcWidth;		//ǿ�Ƹ�ֵ���˳�ѭ��
	//			i = srcHeight;
	//		}
	//	}
	//}

	////�������ϣ��������ң���¼���󶥵㣬�������õ�deltaX
	//for (i = srcHeight - 1; i>=0; i--)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//��������ڵ�
	//		{
	//			x2 = j;		//��¼�µ�ǰxֵ����Ϊ�󶥵�x

	//			j = srcWidth;		//ǿ�Ƹ�ֵ���˳�ѭ��
	//			i = 0;
	//		}
	//	}
	//}

	//delta_x = x2 - x1;		//�õ�deltaX



	//ɨ�����ߵ�ͼ�����ɫ���ص�ƽ���߶�

	//��
	for (i=0;i<srcHeight;i++)
	{   

		//��
		for (j=0;j<srcWidth/2;j++)
		{
			//���Ϊ�ڵ�
			if (0 == Src[i][j])
			{

				//����߶Ƚ���ͳ�Ƶ���
				counts +=srcWidth/2 -j;
				leftaver += i*(srcWidth/2 -j);

			}

		}
	}

	//����ƽ���߶�
	leftaver /= counts;

	//��ͳ��ѭ���������¸�ֵ
	counts =0;

	//ɨ���Ұ�ߵ�ͼ�����ɫ���ص�ƽ���߶�

	//��
	for (i =0;i<srcHeight;i++)
	{

		//��
		for (j=srcWidth/2;j<srcWidth;j++)
		{
			//���Ϊ�ڵ�
			if (0 == Src[i][j])
			{

				//����ͳ�Ƶ���
				counts +=srcWidth -j;
				rightaver += i*(srcWidth -j);
			}
		}
	}

	//�����Ұ�ߵ�ƽ���߶�
	rightaver /= counts;

	//����б��
	slope = (leftaver - rightaver) / (srcWidth/2);

	//����б�ʣ��ѵ�ǰ��ͼ��ĵ�ӳ�䵽Դͼ��ĵ�

	//��
	for (i=0;i<srcHeight;i++)
	{
		//��
		for (j=0;j<srcWidth;j++)
		{	
			//����ӳ��λ��	
			i_src = (u16)(i - (j-srcWidth/2)*slope);

			//�������ͼ���⣬�����ð�ɫ
			if (/*i_src <0 ||*/ i_src >=srcHeight )
			{
				Dst[i][j] = 1;
			}
			else
			{	
				//����Դͼ�����ҵ㣬ȡ������ֵ

				//ָ���i_src�е�j�����ص�ָ��
				Dst[i][j] = Src[i][j];
			}
		}
	}

}

//���Է�����ַ����ڵľ�������
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth)
{
	u16 i,j;				//ָ��Src
//	u16 k = 0, l = 0;		//ָ��Dst
	DRect rect = {0, 0, 0, 0};	//�������
	u16 minX = srcWidth;
	u16 maxX = 0;
	u16 minY = srcHeight;
	u16 maxY = 0;

	//���X����ļ���С�����Y����ļ���С���꣬���Ǿ�������
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (0 == Src[i][j])		//ɨ�赽��ɫ��
			{
				if (i<minY) minY = i;	//��������ֵ���õ����α߿�
				if (i>maxY) maxY = i;
				if (j<minX) minX = j;
				if (j>maxX) maxX = j;
			}
		}
	}

	//��ͼ��ȫ��ʱ��ͼ�������Ϊ���α߽�,��bug
	if ((minX == srcWidth - 1 ) && (maxX == 1 ) && (minY == srcHeight - 1 ) && (maxY ==1 ))
	{
		minX = 1;
		maxX = srcWidth - 1;
		minY = 1;
		maxY = srcHeight - 1;
	}

	//���õ�������ת��Ϊ���ζԽ�����,������һ������
	rect.X1 = (minX - 1)<0?0:(minX - 1);
	rect.Y1 = (minY - 1)<0?0:(minY - 1);
	rect.X2 = maxX + 1;
	rect.Y2 = maxY + 1;

	return rect;
}



//����������ַ����ڵľ��Σ����ѭ������ʱ�ϳ�
void DetectNum(u8 **Src, u16 srcHeight, u16 srcWidth , RectLink *rectlink, u16 num)
{
	u16 i, j, count = 0;
	u16 cutFlag = FALSE;		//�иʼ�ı�־
	u16 allWhite = TRUE;		//��ǰ��ȫ��Ϊ��ɫ�ı�־
	DRect rect = {0, 0, 0, 0};

	RectLink *p = rectlink;		//����ָ��rectlink��
	InitRectLink(p, rect);		//��rect��ʼ��rectLink
	p = rectlink;

	rect.Y2 = srcHeight;


	//�������£���������ɨ��,��ֱ���С�
	for(j = 0; j<srcWidth; j++)
	{
		allWhite = TRUE;	//���赱ǰ���С�ȫ��Ϊ��ɫ���ص�
		for(i = 0; i<srcHeight; i++)
		{
			//�ҳ������ַ��ľ�������
			if (0 == Src[i][j])		
			{
				allWhite = FALSE;
				if (cutFlag == FALSE)	//���Ϊ��ɫ���ص� && �иû�п�ʼ
				{
					cutFlag = TRUE;		//���и��־λ����ʼ
					p->data.X1 = j;		//��¼��ʼXֵ
				}
				else		//����иʼ�ˣ�����ѭ��
				{
					break;
				}
			}
		}

		if (allWhite && cutFlag)		//�����ǰ��ȫ��Ϊ��ɫ���ص㣬�����и��Ѿ���ʼ
		{
			cutFlag = FALSE;		//�и����
			p->data.X2 = j-1;		//��¼����X����

			count++;			//�и������1
			p = p->next;		//׼����һ���������������
		}

		if (count >= num)		//����и���num���ַ�����ɣ���������
		{
			break;
		}//else contiune
		
	}

	//����ϸ����������
	p = rectlink;		//pָ�븴λ��rectlinkͷ
	count = 0;
	for (; count < num; p = p->next)			//׼����һ��������������ݣ����һ��p = NULL
	{
		for (i = p->data.Y1; i <= p->data.Y2; i++)
		{
			allWhite = TRUE;	//���赱ǰ���С�ȫ��Ϊ��ɫ���ص�
			for (j = p->data.X1; j <= p->data.X2; j++)
			{
				if (0 == Src[i][j])
				{
					allWhite = FALSE;
					if (cutFlag == FALSE)	//���Ϊ��ɫ���ص� && �иû�п�ʼ
					{
						cutFlag = TRUE;		//���и��־λ����ʼ
						p->data.Y1 = i;		//��¼��ʼYֵ
					}
					else		//����иʼ�ˣ�����ѭ��
					{
						break;
					}
				}//else continue
			
			}//��ѭ������

			if (allWhite && cutFlag)		//�����ǰ���С�ȫ��Ϊ��ɫ���ص�
			{
				cutFlag = FALSE;	//�и��
				p->data.Y2 = i-1;		//��¼����Y����

				count++;			//�и������1
			}

		}//��ѭ������

	}//����ѭ������

}

//��һ�������ַ���ָ����͸ߣ�����������
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num)
{
	u16 m;
	RectLink *p = rlink;		//��rlinkȡ����
	DRect srcRect = {0, 0, 0, 0};		//Դͼ���еľ�������
	DRect dstRect = {0, 0, (STD_W - 1), (STD_H - 1)};		//Ŀ��ͼ���еľ������򣬳�ʼ��,8x16���

	SetImg(Dst, dstHeight, dstWidth, 1);		//��dstȫ�����Ϊ��ɫ

	for (m = 0; m<num; m++)
	{
		srcRect = p->data;		//��ȡԴͼ���������

		CopyImg(Dst, Src, dstRect, srcRect);	//��Դͼ���srcRect���������Ŀ��ͼ���dstRect����

		//����ǰ����д�뵽rlik��
		p->data = dstRect;

		//������һ��ͼ���Ŀ���������
		dstRect.X1 = dstRect.X1 + STD_W;	//���գ�+ 1�Ϳճ�һ�����صĿ��
		dstRect.X2 = dstRect.X1 + STD_W - 1;

		p = p->next;		//׼����һ����������
	}

	return 0;
}

//��׼��ͼ��
u16 CopyImg(u8 **Dst, u8 **Src, DRect dstRect, DRect srcRect)
{
	u16 i = srcRect.Y1, j = srcRect.X1;		//ָ��Դͼ�����ʼ����
	u16 k = dstRect.Y1, l = dstRect.X1;		//ָ��Ŀ��ͼ�����ʼ����
	u16 s, t;								//ָ������
	double xScale = 0, yScale = 0;			//���ű�
	u16 srcHeight = srcRect.Y2 - srcRect.Y1 + 1;		//�õ�Դͼ��Ҫ��ȡ���ֵĿ��
	u16 srcWidth = srcRect.X2 - srcRect.X1 + 1;
	u16 dstHeight = dstRect.Y2 - dstRect.Y1 + 1;		//�õ�Ŀ��ͼ��Ҫ��ȡ���ֵĿ��
	u16 dstWidth = dstRect.X2 - dstRect.X1 + 1;

	//�õ������ȣ���������ö������Ż�֮��Q16
	yScale = (double ) (srcHeight / (double )dstHeight);
	xScale = (double ) (srcWidth / (double )dstWidth);

	//ѭ������������ӳ��
	for (k = dstRect.Y1; k <= dstRect.Y2; k++)
	{
		s = (k - dstRect.Y1 );		//����y����
		for (l = dstRect.X1; l <= dstRect.X2; l++)
		{
			
			t = (l - dstRect.X1);//����x����
			Dst[k][l] = Src[(u16 )(i + s * yScale + 0.5) ][(u16 )(j + t * xScale + 0.5) ];
		}
	}

	return 0;
}

//������ȡ��13������������ѭ������ʱ�ϳ�
u16 TZTQ13(u8 **tz, u8 **Src, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num)
{
	//���ǽ�ͼ����Ϊһ��������ȡ
	u16 i, j;
	u16 m, a;
	u16 height,width;
	RectLink *p = rlink;


	//��ȡ��9��������������ֵ

	for (m = 0; m<num; m++)
	{
		//�õ���ǰ���ֵĸߺͿ�
		height = p->data.Y2 - p->data.Y1 + 1;
		width = p->data.X2 - p->data.X1 + 1;

		//��ʼ����������
		for (a = 0; a<13; a++)
		{
			tz[m][a] = 0;
		}
		
		//��8����ȡ��ÿ��������ɫ���صĺ���һ������
		for (a = 0; a<8; a++)
		{
			//��ֱ����ֳ�4��
			for (i = p->data.Y1 + (a/2)*height/4; i <= p->data.Y1-1 + (a/2+1)*height/4; i++)
			{
				//ˮƽ����ֳ�2��
				for (j = p->data.X1 + (a%2)*width/2; j <= p->data.X1-1 + (a%2+1)*width/2; j++)
				{
					if (0 == Src[i][j])
					{
						tz[m][a]++;
					}
				}
			}
		}


		//��ȡ�����غ���Ϊ��8������
		for (i = p->data.Y1; i <= p->data.Y2; i++)
		{
			for (j = p->data.X1; j <= p->data.X2; j++)
			{
				if (0 == Src[i][j])
				{
					tz[m][8]++;
				}
			}
		}

		//ˮƽ����
		i = (u16 )(height*1/3.0 );
		for (j = p->data.X1; j<p->data.X2; j++)
		{
			if (0 == Src[i][j]) tz[m][9]++;
		}
		
		i = (u16 )(height*2/3.0 );
		for (j = p->data.X1; j<p->data.X2; j++)
		{
			if (0 == Src[i][j]) tz[m][10]++;
		}

		//��ֱ����
		 j = (u16 )(width*1/3.0 );
		 for (i = p->data.Y1; i<p->data.Y2; i++)
		 {
			if (0 == Src[i][j]) tz[m][11]++;
		 }

		 j = (u16 )(width*2/3.0 );
		 for (i = p->data.Y1; i<p->data.Y2; i++)
		 {
			if (0 == Src[i][j]) tz[m][12]++;
		 }

		 p = p->next;	//׼����һ�����־�������

	}//�����ֲ����

	return 0;
}

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
			tzDb[i][j] = (double)tz[i][j];		//ǿ��ת��Ϊdouble��
			if (tzDb[i][j] > max) max = tzDb[i][j];		//�������ֵ����Сֵ
			if (tzDb[i][j] < min) min = tzDb[i][j];
		}
	}

	//�����������һ����0��1֮��
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tzDb[i][j] = (tzDb[i][j] - min + 1 )/(max - min + 1);		//��һ����0��1֮�䣬��1�Ƿ�ֹ��ĸΪ0
		}
	}
}


//��ͼ���࣬0���1,1���0
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth)
{
	u16 i, j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = 0x01 - Src[i][j];		//0-1����
		}
	}

	//return 0;
}

//��ͼ�������ֵȫ����Ϊtag
void SetImg(u8 **Dst, u16 srcHeight, u16 srcWidth, u8 tag)
{
	/*u8 *p = (u8 *)&Dst;
	u16 i = 0;

	while(p[i] != '\0')
	{
		p[i] = (u8)tag;
		i++;
	}*/
	u16 i,j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = tag;
		}

	}

	//return 0;
}

//��ͼ���ֵ��
void BinaryImg(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, u8 thres)
{
	u16 i, j;

	//�������ɨ��
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (Src[i][j] >= thres)		//���ڵ�����ֵ
			{
				Dst[i][j] = 1;		//��Ϊ1	,��ɫ��
			} 
			else
			{
				Dst[i][j] = 0;		
			}
		}
	}

	//return 0;
}

//ȫ����ֵ����
//����һ����ʼ��ֵT����T���߷ֱ����ƽ����ֵT1��T2�����ȣ�T=(T1+T2)/2��ֱ�����ߵ�ƽ����ֵ���
u8 GlobalThreshold(u8 **img, u16 height, u16 width)
{
	u8 pg[256] = {0};		 //ֱ��ͼ����
	u16 i,j,t=0,t1,t2,k1,k2;
	double u=0,u1,u2;

	//for (i = 0; i<256; i++) pg[i]=0;
	for (i = 0; i<height; i++) //ֱ��ͼͳ��
	{
		for (j = 0; j<width; j++)
		{
			pg[ img[i][j] ]++;
		}
	}
			

	
	for (i=0;i<256;i++)
	{
		t += pg[i];			  //���㱻ͳ�Ƶ����ص���Ŀ
		u += i * pg[i];		  //��i�׻Ҷ�ֵ*��i��������Ŀ
	}
	k2 = (u16 )( u/t); //���㱻ͳ�����ص�Ҷȵ�ƽ��ֵ����ʼ�Ҷ�ֵ
	
	do {
		k1 = k2;
		t1 = 0; u1 = 0;
		for (i = 0; i<=k1; i++)	   //����ǰ�棬����ͻҶ�����ۼ�ֵ
		{
			t1 += pg[i];		 //����ͻҶ��鱻ͳ�Ƶ����ص���Ŀ
			u1 += i * pg[i];
		}

		t2 = t - t1;		   //�õ��߻Ҷ�������ص���Ŀ
		u2 = u - u1;		   //�õ��߻Ҷ���ļ�Ȩ�Ҷ�

		if (t1)
			u1=u1/t1; 	//����ͻҶ����ƽ��ֵ
		else
			u1=0;

		if (t2)
			u2=u2/t2;	//����߻Ҷ����ƽ��ֵ
		else
			u2=0;

		k2 = (u16 )((u1 + u2)/2 );	//�����µ���ֵ
	}while( k1 != k2);		//�����ֵδ�ȶ�������

	return k1;
}

//OSTU��ͼ�����ֵ
u8 otsuThreshold(u8 **img, u16 height, u16 width)
{
//	int width = frame->width;
//	int height = frame->height;
	u16 pixelCount[256];
	float pixelPro[256];
	u16 i, j, pixelSum = width * height, threshold = 0;
	//�����Ҷȼ�[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	//u8* data = (uchar*)frame->imageData;

	for(i = 0; i < 256; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
	for(i = 0; i < height; i++)
	{
		for(j = 0;j < width;j++)
		{
			pixelCount[ img[i][j] ]++;
		}
	}
	
	//����ÿ������������ͼ���еı���
	for(i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
	}

	for(i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for(j = 0; j < 256; j++)
		{
			if(j <= i)   //��������
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //ǰ������
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if(deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	return threshold;
}

//����䣬���ڿ���Χ�İ�ɫ�����п��ܻ�����µġ��ס�
u16 floodfill(u8 **img, u16 height, u16 width)
{
	u16 i,j;
	u16 minX = height, minY = width, maxX = 0, maxY = 0;
	
	for (i = 0; i<height; i++)		//����ַ����ڵľ������򣬺�����Ծ����������
	{
		for (j = 0; j<width; j++)
		{
			if (0 == img[i][j])	//���ɨ�赽�ڵ�
			{
				if (j < minX) minX = j;		//���¾���
				if (j > maxX) maxX = j;
				if (i < minY) minY = i;
				if (i > maxY) maxY = i;
			}
		}
	}
	
	for (i = minY; i<=maxY; i++)		//????,??????
	{
		for (j = minX; j<=maxX; j++)
		{
			if (0 == img[i][j])		//����Ǻڵ㣬ɨ����һ�л���
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//�����ص����
			}
		}
	}
	
	for (i = minX; i<=maxX; i++)		//????,??????
	{
		for (j = maxY; j>=minY; j--)
		{
			if (0 == img[i][j])		//����Ǻڵ㣬ɨ����һ�л���
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//�����ص����
			}
		}
	}
	
	for (j = minX; j<=maxX; j++)		//????,??????
	{
		for (i = minY; i<=maxY; i++)
		{
			if (0 == img[i][j])		//����Ǻڵ㣬ɨ����һ�л���
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//�����ص����
			}
		}
	}
	
	for (j = minX; j<=maxX; j++)		//????,??????
	{
		for (i = maxY-1; i>=minY; i--)
		{
			if (0 == img[i][j])		//����Ǻڵ㣬ɨ����һ�л���
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//�����ص����
			}
		}
	}
	
	return 0;
}


//Ѱ��ͼ���еĹ������ؿ�
bool FindBlock(u8 **img, u16 height, u16 width, u16 x, u16 y, bool lab[], DPoint visted[],u16 lianxushu)
{
	u16 i,j,count;

	if(g_lianxushu >= lianxushu)	//���������������Ҫ�󣬷���
	{
		return TRUE;
	}
	else
	{
		g_lianxushu++;			//���ȼ�1
		lab[y*height+x] = TRUE;				   //�÷��ʱ�־
		visted[g_lianxushu - 1].x = x;		   //��¼�µ�ǰ����
		visted[g_lianxushu - 1].y = y;
	}
	
	if(g_lianxushu >= lianxushu)	//���������������Ҫ�󣬷���
	{
		return TRUE;
	}
	else							//����ݹ�,8�������
	{
		if ((x - 1 ) >= 0 && (y - 1 ) >= 0 && (x + 1 ) <= (width - 1 ) && (y + 1 ) < (height - 1 ) )
		{
			if (img[x-1][y-1] == 0 && !lab[(y-1)*height+(x-1)])		//���Ͻ�
				FindBlock(img, height, width, x-1, y-1, lab, visted, lianxushu);

			if (img[x][y-1] == 0 && !lab[(y-1)*height+(x)])		//��
				FindBlock(img, height, width, x, y-1, lab, visted, lianxushu);
	
			if (img[x+1][y-1] == 0 && !lab[(y-1)*height+(x+1)])		//���Ͻ�
				FindBlock(img, height, width, x+1, y-1, lab, visted, lianxushu);

			if (img[x-1][y] == 0 && !lab[(y)*height+(x-1)])		//��
				FindBlock(img, height, width, x-1, y, lab, visted, lianxushu);

			if (img[x-1][y+1] == 0 && !lab[(y+1)*height+(x-1)])		//���½�
				FindBlock(img, height, width, x-1, y+1, lab, visted, lianxushu);

	
			if (img[x+1][y] == 0 && !lab[(y)*height+(x+1)])		//��
				FindBlock(img, height, width, x+1, y, lab, visted, lianxushu);
	
	
			if (img[x][y+1] == 0 && !lab[(y+1)*height+(x)])		//��
				FindBlock(img, height, width, x, y+1, lab, visted, lianxushu);
	
			if (img[x+1][y+1] == 0 && !lab[(y+1)*height+(x+1)])		//���½�
				FindBlock(img, height, width, x+1, y+1, lab, visted, lianxushu);

		}
		else
		{
			return TRUE;	
		}

		if(g_lianxushu >= lianxushu)	//���������������Ҫ�󣬷���
		{
			return TRUE;
		} 
	}

	return FALSE;
}

//ɾ���������ص�
//void RemoveSeprateBlock(u8 **img, DPoint visted[])
//{
//	  u16 i,j;
//
//
//}




