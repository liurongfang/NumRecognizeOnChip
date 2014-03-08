/**************************************************************************************
*	����������ļ���ĺ�����ͼ����к��ڴ����������ȡ
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include "img.h"


//����ȥ����ɢ�㺯��Ҫ�õı���
u16 g_lianxushu = 0;		//ȥ����ɢ����������ɢ����Ŀ
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
	RectLink *p = rlink;	//����ע�⣬���Բ���ֱ�Ӳ���rlink

	printf("��������ӡ��õľ�������\r\n");
	do
	{
		printf("��%d�����Σ�{%d,%d,%d,%d}\r\n",i,p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
		i++;
		p = p->next;
	}
	while(p != NULL);
	printf("�ܹ�%d������\r\n",i);

}


//���Է�����ַ����ڵľ�������
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth)
{
	u16 i,j;				//ָ��Src
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
			if (FORECOLOR == Src[i][j])		//ɨ�赽���֡�
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
	rect.Y2 = srcHeight;
	InitRectLink(p, rect);		//��rect��ʼ��rectLink
	p = rectlink;


	//�������£���������ɨ��,��ֱ���С�
	for(j = 0; j<srcWidth; j++)
	{
		allWhite = TRUE;	//���赱ǰ���С�ȫ��Ϊ����ɫ���ص�
		for(i = 0; i<srcHeight; i++)
		{
			//�ҳ������ַ��ľ�������
			if (FORECOLOR == Src[i][j])		
			{
				allWhite = FALSE;
				if (cutFlag == FALSE)	//���Ϊǰ��ɫ���ص� && �иû�п�ʼ
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

		if (allWhite && cutFlag /*&& (j-p->data.X1)>2*/)		//�����ǰ��ȫ��Ϊ��ɫ���ص㣬�����и��Ѿ���ʼ����ȴ���2
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

//	ShowRectLink(rectlink);
//	printf("Pause\r\n");

	//����ϸ����������
	p = rectlink;		//pָ�븴λ��rectlinkͷ
	count = 0;
	for (; count < num && p->next != NULL; p = p->next)			//׼����һ��������������ݣ����һ��p = NULL
	{
		for (i = p->data.Y1; i <= p->data.Y2; i++)
		{
			allWhite = TRUE;	//���赱ǰ���С�ȫ��Ϊ����ɫ���ص�
			for (j = p->data.X1; j <= p->data.X2; j++)
			{
				if (FORECOLOR == Src[i][j])
				{
					allWhite = FALSE;
					if (cutFlag == FALSE)	//���Ϊǰ��ɫ���ص� && �иû�п�ʼ
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

//			printf("Rect{%d,%d,%d,%d}\r\n",p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
//			printf("count:%dcheck\r\n",count);

		}//��ѭ������

	}//����ѭ������

	printf("End\r\n");
	ShowRectLink(rectlink);

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
					if (FORECOLOR == Src[i][j])
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
				if (FORECOLOR == Src[i][j])
				{
					tz[m][8]++;
				}
			}
		}

		//ˮƽ����
		i = (u16 )(height*1/3.0 );
		for (j = p->data.X1; j<p->data.X2; j++)
		{
			if (FORECOLOR == Src[i][j]) tz[m][9]++;
		}
		
		i = (u16 )(height*2/3.0 );
		for (j = p->data.X1; j<p->data.X2; j++)
		{
			if (FORECOLOR == Src[i][j]) tz[m][10]++;
		}

		//��ֱ����
		 j = (u16 )(width*1/3.0 );
		 for (i = p->data.Y1; i<p->data.Y2; i++)
		 {
			if (FORECOLOR == Src[i][j]) tz[m][11]++;
		 }

		 j = (u16 )(width*2/3.0 );
		 for (i = p->data.Y1; i<p->data.Y2; i++)
		 {
			if (FORECOLOR == Src[i][j]) tz[m][12]++;
		 }

		 p = p->next;	//׼����һ�����־�������

	}//�����ֲ����

	return 0;
}


//��ͼ���࣬�궨��CR������0-1���໹��0-255����
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth)
{
	u16 i, j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = CR - Src[i][j];		//0-1����
		}
	}

}

//��ͼ�������ֵȫ����Ϊtag
void SetImg(u8 **Dst, u16 srcHeight, u16 srcWidth, u8 tag)
{
	u16 i,j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = tag;
		}

	}

}

//��ͼ���ֵ��������Ҷ�ͼ
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


//ϸ��ͼ������ʹ���������Ҹ����ԣ�Ҫ��ǰ��ɫΪ1������ɫΪ0 ,lx:heighe,ly:width
void ThinnerRosenfeld(u8 **image, u16 lx, u16 ly)
{
    char *f, *g;
    char n[10];
    signed char a[5] = {0, -1, 1, 0, 0};
    signed char b[5] = {0, 0, 0, 1, -1};
    char nrnd, cond, n48, n26, n24, n46, n68, n82, n123, n345, n567, n781;
    short k, shori;
    unsigned long i, j;
    long ii, jj, kk, kk1, kk2, kk3, size;
    size = (long )lx * (long )ly;

    g = (char *)malloc(size);
    if(g==NULL)
    {
        printf("error in alocating mmeory!\n");
        return;
    }

    f = (char *)image[0];
    for(kk=0l; kk<size; kk++)
    {
        g[kk] = f[kk];
    }

    do
    {
        shori = 0;
        for(k=1; k<=4; k++)
        {
            for(i=1; i<lx-1; i++)
            {
                ii = i + a[k];

                for(j=1; j<ly-1; j++)
                {
                    kk = i*ly + j;

                    if(!f[kk])
                        continue;

                    jj = j + b[k];
                    kk1 = ii*ly + jj;

                    if(f[kk1])
                        continue;

                    kk1 = kk - ly -1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[3] = f[kk1];
                    n[2] = f[kk2];
                    n[1] = f[kk3];
                    kk1 = kk - 1;
                    kk3 = kk + 1;
                    n[4] = f[kk1];
                    n[8] = f[kk3];
                    kk1 = kk + ly - 1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[5] = f[kk1];
                    n[6] = f[kk2];
                    n[7] = f[kk3];

                    nrnd = n[1] + n[2] + n[3] + n[4]
                        +n[5] + n[6] + n[7] + n[8];
                    if(nrnd<=1)
                        continue;

                    cond = 0;
                    n48 = n[4] + n[8];
                    n26 = n[2] + n[6];
                    n24 = n[2] + n[4];
                    n46 = n[4] + n[6];
                    n68 = n[6] + n[8];
                    n82 = n[8] + n[2];
                    n123 = n[1] + n[2] + n[3];
                    n345 = n[3] + n[4] + n[5];
                    n567 = n[5] + n[6] + n[7];
                    n781 = n[7] + n[8] + n[1];

                    if(n[2]==1 && n48==0 && n567>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[6]==1 && n48==0 && n123>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[8]==1 && n26==0 && n345>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[4]==1 && n26==0 && n781>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[5]==1 && n46==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[7]==1 && n68==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[1]==1 && n82==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[3]==1 && n24==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    cond = 1;
                    if(!cond)
                        continue;
                    g[kk] = 0;
                    shori = 1;
                }
            }

            for(i=0; i<lx; i++)
            {
                for(j=0; j<ly; j++)
                {
                    kk = i*ly + j;
                    f[kk] = g[kk];
                }
            }
        }
    }while(shori);

    free(g);
}

