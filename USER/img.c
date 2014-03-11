/**************************************************************************************
*	描述：这个文件里的函数对图像进行后期处理和特征提取
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include "img.h"


//定义去除离散点函数要用的变量
u16 g_lianxushu = 0;		//去除离散点里限制离散点数目
//DPoint visted[20];
//bool lab[4000] = {FALSE};


//创建矩形链表
RectLink *CreateRectLink(u16 num)
{
	RectLink *head = NULL;		//建立头指针
	RectLink *p, *pTmp;			//p代表当前单元，pTmp代表最后一个单元
	u16 i = 0;

	//是第一个节点，与head直接联系起来
	head = p = pTmp = (RectLink *)malloc(sizeof(RectLink));
	p->next = NULL;

	//不是第1个节点，用pTmp做中继连接到表尾部
	//这里有num-1次循环
	while (--num)
	{
		p = (RectLink *)malloc(sizeof(RectLink));		//新申请内存空间
		p->next = NULL;			//尾部值NULL
		pTmp->next = p;			//将当前“单元”链接到链表后面
		pTmp = pTmp->next;		//指针后移一位
		i++;
	}

	return (head);

}

//用给定的DRect初始化矩形链表
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

//销毁链表
u16 DeRectLink(RectLink *head)
{
	RectLink *p;

	while(head->next != NULL)
	{
		p = head->next;
		free(head);
		head = p;
	}
	free(head);		//head->next == NULL，最后一个节点，释放掉
	head = NULL;	//将头结点置为空

	return 0;
}

//显示得到的矩形链表
void ShowRectLink(RectLink *rlink)
{
	u16 i = 0;
	RectLink *p = rlink;	//绝对注意，绝对不能直接操作rlink

	printf("接下来打印获得的矩形区域：\r\n");
	do
	{
		printf("第%d个矩形：{%d,%d,%d,%d}\r\n",i,p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
		i++;
		p = p->next;
	}
	while(p != NULL);
	printf("总共%d个矩形\r\n",i);

}


//粗略分离出字符所在的矩形区域
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth)
{
	u16 i,j;				//指向Src
	DRect rect = {0, 0, 0, 0};	//保存矩形
	u16 minX = srcWidth;
	u16 maxX = 0;
	u16 minY = srcHeight;
	u16 maxY = 0;

	//求出X方向的极大极小坐标和Y方向的极大极小坐标，就是矩形区域
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (FORECOLOR == Src[i][j])		//扫描到“字”
			{
				if (i<minY) minY = i;	//各方向求极值，得到矩形边框
				if (i>maxY) maxY = i;
				if (j<minX) minX = j;
				if (j>maxX) maxX = j;
			}
		}
	}

	//在图像全白时将图像界限作为矩形边界,防bug
	if ((minX == srcWidth - 1 ) && (maxX == 1 ) && (minY == srcHeight - 1 ) && (maxY ==1 ))
	{
		minX = 1;
		maxX = srcWidth - 1;
		minY = 1;
		maxY = srcHeight - 1;
	}

	//将得到的坐标转换为矩形对角坐标,并外扩一个像素
	rect.X1 = (minX - 1)<0?0:(minX - 1);
	rect.Y1 = (minY - 1)<0?0:(minY - 1);
	rect.X2 = maxX + 1;
	rect.Y2 = maxY + 1;

	return rect;
}



//分离出单个字符所在的矩形，多个循环，耗时较长
void DetectNum(u8 **Src, u16 srcHeight, u16 srcWidth , RectLink *rectlink, u16 num)
{
	u16 i, j, count = 0;
	u16 cutFlag = FALSE;		//切割开始的标志
	u16 allWhite = TRUE;		//当前列全部为白色的标志
	DRect rect = {0, 0, 0, 0};

	RectLink *p = rectlink;		//用来指向rectlink表
	rect.Y2 = srcHeight;
	InitRectLink(p, rect);		//用rect初始化rectLink
	p = rectlink;


	//从上往下，从左往右扫描,竖直“切”
	for(j = 0; j<srcWidth; j++)
	{
		allWhite = TRUE;	//假设当前“列”全部为背景色像素点
		for(i = 0; i<srcHeight; i++)
		{
			//找出单个字符的矩形区域
			if (FORECOLOR == Src[i][j])		
			{
				allWhite = FALSE;
				if (cutFlag == FALSE)	//如果为前景色像素点 && 切割还没有开始
				{
					cutFlag = TRUE;		//置切割标志位：开始
					p->data.X1 = j;		//记录开始X值
				}
				else		//如果切割开始了，跳出循环
				{
					break;
				}
			}
		}

		if (allWhite && cutFlag /*&& (j-p->data.X1)>2*/)		//如果当前列全部为白色像素点，并且切割已经开始，宽度大于2
		{
			cutFlag = FALSE;		//切割结束
			p->data.X2 = j-1;		//记录结束X坐标

			count++;			//切割计数加1
			p = p->next;		//准备下一个链表来存放数据
		}

		if (count >= num)		//如果切割了num个字符，完成，结束跳出
		{
			break;
		}//else contiune
		
	}

//	ShowRectLink(rectlink);
//	printf("Pause\r\n");

	//下面细化数字区域
	p = rectlink;		//p指针复位到rectlink头
	count = 0;
	for (; count < num && p->next != NULL; p = p->next)			//准备下一个链表来存放数据，最后一个p = NULL
	{
		for (i = p->data.Y1; i <= p->data.Y2; i++)
		{
			allWhite = TRUE;	//假设当前“行”全部为背景色像素点
			for (j = p->data.X1; j <= p->data.X2; j++)
			{
				if (FORECOLOR == Src[i][j])
				{
					allWhite = FALSE;
					if (cutFlag == FALSE)	//如果为前景色像素点 && 切割还没有开始
					{
						cutFlag = TRUE;		//置切割标志位：开始
						p->data.Y1 = i;		//记录开始Y值
					}
					else		//如果切割开始了，跳出循环
					{
						break;
					}
				}//else continue
			
			}//行循环结束

			if (allWhite && cutFlag)		//如果当前“行”全部为白色像素点
			{
				cutFlag = FALSE;	//切割毕
				p->data.Y2 = i-1;		//记录结束Y坐标

				count++;			//切割个数加1
			}

//			printf("Rect{%d,%d,%d,%d}\r\n",p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
//			printf("count:%dcheck\r\n",count);

		}//列循环结束

	}//数字循环结束

	printf("End\r\n");
	ShowRectLink(rectlink);

}

//归一化单个字符到指定宽和高，并紧缩重排
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num)
{
	u16 m;
	RectLink *p = rlink;		//从rlink取数据
	DRect srcRect = {0, 0, 0, 0};		//源图像中的矩形区域
	DRect dstRect = {0, 0, (STD_W - 1), (STD_H - 1)};		//目标图像中的矩形区域，初始化,8x16宽高

	SetImg(Dst, dstHeight, dstWidth, 0);		//将dst全部填充为白色

	for (m = 0; m<num; m++)
	{
		srcRect = p->data;		//获取源图像矩形区域

		CopyImg(Dst, Src, dstRect, srcRect);	//将源图像的srcRect区域放缩到目标图像的dstRect区域

		//将当前矩形写入到rlik表
		p->data = dstRect;

		//计算下一幅图像的目标矩形区域
		dstRect.X1 = dstRect.X1 + STD_W;	//紧凑，+ 1就空出一个像素的宽度
		dstRect.X2 = dstRect.X1 + STD_W - 1;

		p = p->next;		//准备下一个矩形数据
	}

	return 0;
}

//标准化图像
u16 CopyImg(u8 **Dst, u8 **Src, DRect dstRect, DRect srcRect)
{
	u16 i = srcRect.Y1, j = srcRect.X1;		//指向源图像的起始坐标
	u16 k = dstRect.Y1, l = dstRect.X1;		//指向目标图像的起始坐标
	u16 s, t;								//指向增量
	double xScale = 0, yScale = 0;			//缩放比
	u16 srcHeight = srcRect.Y2 - srcRect.Y1 + 1;		//得到源图像要截取部分的宽高
	u16 srcWidth = srcRect.X2 - srcRect.X1 + 1;
	u16 dstHeight = dstRect.Y2 - dstRect.Y1 + 1;		//得到目标图像要截取部分的宽高
	u16 dstWidth = dstRect.X2 - dstRect.X1 + 1;

	//得到放缩比，这里可以用定点数优化之，Q16
	yScale = (double ) (srcHeight / (double )dstHeight);
	xScale = (double ) (srcWidth / (double )dstWidth);

	//循环遍历，缩放映射
	for (k = dstRect.Y1; k <= dstRect.Y2; k++)
	{
		s = (k - dstRect.Y1 );		//计算y增量
		for (l = dstRect.X1; l <= dstRect.X2; l++)
		{
			
			t = (l - dstRect.X1);//计算x增量
			Dst[k][l] = Src[(u16 )(i + s * yScale + 0.5) ][(u16 )(j + t * xScale + 0.5) ];
		}
	}

	return 0;
}

//特征提取，13个特征，多重循环，耗时较长
u16 TZTQ13(u8 **tz, u8 **Src, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num)
{
	//还是将图像作为一整张来提取
	u16 i, j;
	u16 m, a;
	u16 height,width;
	RectLink *p = rlink;


	//提取第9个特征，总像素值

	for (m = 0; m<num; m++)
	{
		//得到当前数字的高和宽
		height = p->data.Y2 - p->data.Y1 + 1;
		width = p->data.X2 - p->data.X1 + 1;

		//初始化特征矩阵
		for (a = 0; a<13; a++)
		{
			tz[m][a] = 0;
		}
		
		//分8块提取，每块特征黑色像素的和做一个特征
		for (a = 0; a<8; a++)
		{
			//竖直方向分成4块
			for (i = p->data.Y1 + (a/2)*height/4; i <= p->data.Y1-1 + (a/2+1)*height/4; i++)
			{
				//水平方向分成2块
				for (j = p->data.X1 + (a%2)*width/2; j <= p->data.X1-1 + (a%2+1)*width/2; j++)
				{
					if (FORECOLOR == Src[i][j])
					{
						tz[m][a]++;
					}
				}
			}
		}


		//提取总像素和作为第8个特征
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

		//水平两条
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

		//竖直两条
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

		 p = p->next;	//准备下一个数字矩形区域

	}//数字轮查结束

	return 0;
}


//将图像反相，宏定义CR决定是0-1反相还是0-255反相
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth)
{
	u16 i, j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = CR - Src[i][j];		//0-1反相
		}
	}

}

//将图像的像素值全部置为tag
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

//将图像二值化，输入灰度图
void BinaryImg(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, u8 thres)
{
	u16 i, j;

	//逐个像素扫描
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (Src[i][j] >= thres)		//大于等于阈值
			{
				Dst[i][j] = 1;		//置为1	,白色点
			} 
			else
			{
				Dst[i][j] = 0;		
			}
		}
	}
}

//全局阈值函数
//给定一个初始阈值T，在T两边分别计算平均阈值T1和T2，不等，T=(T1+T2)/2，直到两边的平均阈值相等
u8 GlobalThreshold(u8 **img, u16 height, u16 width)
{
	u8 pg[256] = {0};		 //直方图数组
	u16 i,j,t=0,t1,t2,k1,k2;
	double u=0,u1,u2;

	//for (i = 0; i<256; i++) pg[i]=0;
	for (i = 0; i<height; i++) //直方图统计
	{
		for (j = 0; j<width; j++)
		{
			pg[ img[i][j] ]++;
		}
	}
			

	
	for (i=0;i<256;i++)
	{
		t += pg[i];			  //计算被统计的像素点数目
		u += i * pg[i];		  //第i阶灰度值*第i阶像素数目
	}
	k2 = (u16 )( u/t); //计算被统计像素点灰度的平均值，初始灰度值
	
	do {
		k1 = k2;
		t1 = 0; u1 = 0;
		for (i = 0; i<=k1; i++)	   //类似前面，计算低灰度组的累加值
		{
			t1 += pg[i];		 //计算低灰度组被统计的像素点数目
			u1 += i * pg[i];
		}

		t2 = t - t1;		   //得到高灰度组的像素点数目
		u2 = u - u1;		   //得到高灰度组的加权灰度

		if (t1)
			u1=u1/t1; 	//计算低灰度组的平均值
		else
			u1=0;

		if (t2)
			u2=u2/t2;	//计算高灰度组的平均值
		else
			u2=0;

		k2 = (u16 )((u1 + u2)/2 );	//计算新的阈值
	}while( k1 != k2);		//如果数值未稳定，继续

	return k1;
}


//细化图像，用来使特征集中且更明显，要求前景色为1，背景色为0 ,lx:heighe,ly:width
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

//去除图像中连续长度少于length的孤立块
void RemoveNoise(u8 **img, u16 height, u16 width, u16 length)
{
	u16 i,j,k;
	DPoint save[40];		//保存要删除的点，删除时使用，查找时更新，删除后清除
	bool lab[60*80] = {FALSE};	//保存某点是否被访问过，只在查找时使用

	LCD_ShowString(40,50,200,200,16,"Noise anchor (1).");

	//将访问表清零
	for (i = 0; i<60*80; i++) lab[i] = FALSE;

	//开始循环遍历
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			//将保存的点坐标置0
			g_lianxushu = 0;

			//如果是前景色，查找是否是噪点(是否添加到访问表)
			if (img[i][j] == FORECOLOR)
			{
				//如果找到了没有访问过的孤立块，开
				//始根据获得的坐标将点置为背景色
				if (FindNoise(img, height, width, i, j, lab, save, length))	//== TRUE
				{
					//去除孤立像素块
					//RemoveIt();
					
					for (k = 0; k<g_lianxushu; k++)
					{
						img[save[k].y][save[k].x] = BACKCOLOR;
					}	
				}
				else
				{
					g_lianxushu = 0;		//将噪点计数器清零
				}

			}//else donothing
			else
			{
				//printf("g%d-%d ",i,j);
			}
			printf("%d.%d ",i,j);
		}
	}

}


//判断当前点的离散性
bool FindNoise(u8 **img, u16 height, u16 width,u16 x, u16 y, bool *lab, DPoint *save, u16 length)
{
//	u16 i,j;

	if(g_lianxushu >= length)	//连续长度大于最大长度，不是离散块，返回FALSE
	{
		return FALSE;
	}
	else
	{
		g_lianxushu++;			//连续长度加1
		lab[y*width+x] = TRUE;				   //更新访问表
		save[g_lianxushu - 1].x = x;		   //可以认为当前点是离散点
		save[g_lianxushu - 1].y = y;
	}
	
	if(g_lianxushu >= length)	//连续长度
	{
		return FALSE;
	}
	else							//????,8????
	{
		if ((x - 1 ) >= 0 && (y - 1 ) >= 0 && (x + 1 ) <= (width - 1 ) && (y + 1 ) < (height - 1 ) )
		{
			if (img[x-1][y-1] == 0 && !lab[(y-1)*width+(x-1)])		//左上
				FindNoise(img, height, width, x-1, y-1, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}

			if (img[x][y-1] == 0 && !lab[(y-1)*width+(x)])		//上
				FindNoise(img, height, width, x, y-1, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}
	
			if (img[x+1][y-1] == 0 && !lab[(y-1)*width+(x+1)])		//右上
				FindNoise(img, height, width, x+1, y-1, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}

			if (img[x-1][y] == 0 && !lab[(y)*width+(x-1)])		//左
				FindNoise(img, height, width, x-1, y, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}

			if (img[x-1][y+1] == 0 && !lab[(y+1)*width+(x-1)])		//左下
				FindNoise(img, height, width, x-1, y+1, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}

	
			if (img[x+1][y] == 0 && !lab[(y)*width+(x+1)])		//右
				FindNoise(img, height, width, x+1, y, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}
	
	
			if (img[x][y+1] == 0 && !lab[(y+1)*width+(x)])		//下
				FindNoise(img, height, width, x, y+1, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}
	
			if (img[x+1][y+1] == 0 && !lab[(y+1)*width+(x+1)])		//右下
				FindNoise(img, height, width, x+1, y+1, lab, save, length);
			if(g_lianxushu >= length)	//不是离散点
			{
				return FALSE;
			}

		}
		else
		{
			//doNothing	
		}

		if(g_lianxushu >= length)	//不是离散点
		{
			return FALSE;
		} 
	}

	return TRUE;
}


//直方图均衡
void Equalize(u8 **img, u16 height, u16 width)
{
	//循环变量
	u16	i,j;
	
	//临时变量
	u16	lTemp;	
	
	
	//映射表
	u16	bMap[256];
	
	//直方图
	long	lCount[256];
	
	//清零直方图
	for (i = 0; i < 256; i ++)
	{
		lCount[i] = 0;
	}
	
	//直方图统计
	for (i = 0; i < height; i ++)
	{
		for (j = 0; j < width; j ++)
		{
			lCount[img[i][j] ]++;
		}
	}
	
	//计算灰度映射表
	for (i = 0; i < 256; i++)
	{
		lTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			lTemp += lCount[j];
		}
		
		//计算映射表
		bMap[i] = (u16) (lTemp * 255 / height / width);
	}
	
	//根据映射表填充图像
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			//
			img[height - 1 - i][j] = bMap[(height - 1 - i)*width + j];
		}
	}
	
	
}

