/**************************************************************************************
*	描述：这个文件里的函数对图像进行后期处理和特征提取
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include <math.h>
#include "img.h"
#include "usart.h"
#include "usmart.h"


//定义函数要用的变量
u16 g_lianxushu = 0;
DPoint visted[20];
bool lab[4000] = {FALSE};


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
	RectLink *p = rlink;	//绝对注意，不能直接操作rlink

	printf("接下来打印获得的矩形区域：\n");
	do
	{
		printf("第%d个矩形：{%d %d %d %d}\n",i,p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
		i++;
		p = p->next;
	}
	while(p != NULL);
	printf("总共%d个矩形\n",i);

	//system("pause");
}
//倾斜度矫正,存在问题
void SlopeAdjust(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth)
{
	u16 i,j;
	u16 i_src;
	//图像左半边的平均高度
	double leftaver=0.0;

	//图像右半边的平均高度
	double rightaver=0.0;

	//图像的倾斜度
	double slope;

	//统计循环变量
	u16 counts=0;
	//u16 x1,x2;
	//double delta_x;
	//
	////从上往下，从左往右，记录下左顶点
	//for (i = 0; i<srcHeight; i++)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//如果碰到黑点
	//		{
	//			x1 = j;		//记录下当前x值，作为左顶点x

	//			j = srcWidth;		//强制赋值以退出循环
	//			i = srcHeight;
	//		}
	//	}
	//}

	////从下往上，从左往右，记录下左顶点，结合上面得到deltaX
	//for (i = srcHeight - 1; i>=0; i--)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//如果碰到黑点
	//		{
	//			x2 = j;		//记录下当前x值，作为左顶点x

	//			j = srcWidth;		//强制赋值以退出循环
	//			i = 0;
	//		}
	//	}
	//}

	//delta_x = x2 - x1;		//得到deltaX



	//扫描左半边的图像，求黑色象素的平均高度

	//行
	for (i=0;i<srcHeight;i++)
	{   

		//列
		for (j=0;j<srcWidth/2;j++)
		{
			//如果为黑点
			if (0 == Src[i][j])
			{

				//对其高度进行统计叠加
				counts +=srcWidth/2 -j;
				leftaver += i*(srcWidth/2 -j);

			}

		}
	}

	//计算平均高度
	leftaver /= counts;

	//将统计循环变量重新赋值
	counts =0;

	//扫描右半边的图像，求黑色象素的平均高度

	//行
	for (i =0;i<srcHeight;i++)
	{

		//列
		for (j=srcWidth/2;j<srcWidth;j++)
		{
			//如果为黑点
			if (0 == Src[i][j])
			{

				//进行统计叠加
				counts +=srcWidth -j;
				rightaver += i*(srcWidth -j);
			}
		}
	}

	//计算右半边的平均高度
	rightaver /= counts;

	//计算斜率
	slope = (leftaver - rightaver) / (srcWidth/2);

	//根据斜率，把当前新图像的点映射到源图像的点

	//行
	for (i=0;i<srcHeight;i++)
	{
		//列
		for (j=0;j<srcWidth;j++)
		{	
			//计算映射位置	
			i_src = (u16)(i - (j-srcWidth/2)*slope);

			//如果点在图像外，象素置白色
			if (/*i_src <0 ||*/ i_src >=srcHeight )
			{
				Dst[i][j] = 1;
			}
			else
			{	
				//否则到源图像中找点，取得象素值

				//指向第i_src行第j个象素的指针
				Dst[i][j] = Src[i][j];
			}
		}
	}

}

//粗略分离出字符所在的矩形区域
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth)
{
	u16 i,j;				//指向Src
//	u16 k = 0, l = 0;		//指向Dst
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
			if (0 == Src[i][j])		//扫描到黑色点
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
	InitRectLink(p, rect);		//用rect初始化rectLink
	p = rectlink;

	rect.Y2 = srcHeight;


	//从上往下，从左往右扫描,竖直“切”
	for(j = 0; j<srcWidth; j++)
	{
		allWhite = TRUE;	//假设当前“列”全部为白色像素点
		for(i = 0; i<srcHeight; i++)
		{
			//找出单个字符的矩形区域
			if (0 == Src[i][j])		
			{
				allWhite = FALSE;
				if (cutFlag == FALSE)	//如果为黑色像素点 && 切割还没有开始
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

		if (allWhite && cutFlag)		//如果当前列全部为白色像素点，并且切割已经开始
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

	//下面细化数字区域
	p = rectlink;		//p指针复位到rectlink头
	count = 0;
	for (; count < num; p = p->next)			//准备下一个链表来存放数据，最后一个p = NULL
	{
		for (i = p->data.Y1; i <= p->data.Y2; i++)
		{
			allWhite = TRUE;	//假设当前“行”全部为白色像素点
			for (j = p->data.X1; j <= p->data.X2; j++)
			{
				if (0 == Src[i][j])
				{
					allWhite = FALSE;
					if (cutFlag == FALSE)	//如果为黑色像素点 && 切割还没有开始
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

		}//列循环结束

	}//数字循环结束

}

//归一化单个字符到指定宽和高，并紧缩重排
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num)
{
	u16 m;
	RectLink *p = rlink;		//从rlink取数据
	DRect srcRect = {0, 0, 0, 0};		//源图像中的矩形区域
	DRect dstRect = {0, 0, (STD_W - 1), (STD_H - 1)};		//目标图像中的矩形区域，初始化,8x16宽高

	SetImg(Dst, dstHeight, dstWidth, 1);		//将dst全部填充为白色

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
					if (0 == Src[i][j])
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
				if (0 == Src[i][j])
				{
					tz[m][8]++;
				}
			}
		}

		//水平两条
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

		//竖直两条
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

		 p = p->next;	//准备下一个数字矩形区域

	}//数字轮查结束

	return 0;
}

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
			tzDb[i][j] = (double)tz[i][j];		//强制转换为double型
			if (tzDb[i][j] > max) max = tzDb[i][j];		//更新最大值、最小值
			if (tzDb[i][j] < min) min = tzDb[i][j];
		}
	}

	//将特征矩阵归一化到0和1之间
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tzDb[i][j] = (tzDb[i][j] - min + 1 )/(max - min + 1);		//归一化到0和1之间，加1是防止分母为0
		}
	}
}


//将图像反相，0变成1,1变成0
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth)
{
	u16 i, j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = 0x01 - Src[i][j];		//0-1反相
		}
	}

	//return 0;
}

//将图像的像素值全部置为tag
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

//将图像二值化
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

	//return 0;
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

//OSTU求图像的阈值
u8 otsuThreshold(u8 **img, u16 height, u16 width)
{
//	int width = frame->width;
//	int height = frame->height;
	u16 pixelCount[256];
	float pixelPro[256];
	u16 i, j, pixelSum = width * height, threshold = 0;
	//遍历灰度级[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	//u8* data = (uchar*)frame->imageData;

	for(i = 0; i < 256; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//统计灰度级中每个像素在整幅图像中的个数
	for(i = 0; i < height; i++)
	{
		for(j = 0;j < width;j++)
		{
			pixelCount[ img[i][j] ]++;
		}
	}
	
	//计算每个像素在整幅图像中的比例
	for(i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
	}

	for(i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for(j = 0; j < 256; j++)
		{
			if(j <= i)   //背景部分
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分
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

//简单填充，填充黑块周围的白色区域，有可能会产生新的“孔”
u16 floodfill(u8 **img, u16 height, u16 width)
{
	u16 i,j;
	u16 minX = height, minY = width, maxX = 0, maxY = 0;
	
	for (i = 0; i<height; i++)		//求出字符所在的矩形区域，后面仅对矩形区域操作
	{
		for (j = 0; j<width; j++)
		{
			if (0 == img[i][j])	//如果扫描到黑点
			{
				if (j < minX) minX = j;		//更新矩形
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
			if (0 == img[i][j])		//如果是黑点，扫描下一行或列
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//将像素点填充
			}
		}
	}
	
	for (i = minX; i<=maxX; i++)		//????,??????
	{
		for (j = maxY; j>=minY; j--)
		{
			if (0 == img[i][j])		//如果是黑点，扫描下一行或列
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//将像素点填充
			}
		}
	}
	
	for (j = minX; j<=maxX; j++)		//????,??????
	{
		for (i = minY; i<=maxY; i++)
		{
			if (0 == img[i][j])		//如果是黑点，扫描下一行或列
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//将像素点填充
			}
		}
	}
	
	for (j = minX; j<=maxX; j++)		//????,??????
	{
		for (i = maxY-1; i>=minY; i--)
		{
			if (0 == img[i][j])		//如果是黑点，扫描下一行或列
			{
				break;
			}
			else
			{
				img[i][j] = 0;		//将像素点填充
			}
		}
	}
	
	return 0;
}


//寻找图像中的孤立像素块
bool FindBlock(u8 **img, u16 height, u16 width, u16 x, u16 y, bool lab[], DPoint visted[],u16 lianxushu)
{
	u16 i,j,count;

	if(g_lianxushu >= lianxushu)	//如果连续长度满足要求，返回
	{
		return TRUE;
	}
	else
	{
		g_lianxushu++;			//长度加1
		lab[y*height+x] = TRUE;				   //置访问标志
		visted[g_lianxushu - 1].x = x;		   //记录下当前坐标
		visted[g_lianxushu - 1].y = y;
	}
	
	if(g_lianxushu >= lianxushu)	//如果连续长度满足要求，返回
	{
		return TRUE;
	}
	else							//进入递归,8方向遍历
	{
		if ((x - 1 ) >= 0 && (y - 1 ) >= 0 && (x + 1 ) <= (width - 1 ) && (y + 1 ) < (height - 1 ) )
		{
			if (img[x-1][y-1] == 0 && !lab[(y-1)*height+(x-1)])		//左上角
				FindBlock(img, height, width, x-1, y-1, lab, visted, lianxushu);

			if (img[x][y-1] == 0 && !lab[(y-1)*height+(x)])		//上
				FindBlock(img, height, width, x, y-1, lab, visted, lianxushu);
	
			if (img[x+1][y-1] == 0 && !lab[(y-1)*height+(x+1)])		//右上角
				FindBlock(img, height, width, x+1, y-1, lab, visted, lianxushu);

			if (img[x-1][y] == 0 && !lab[(y)*height+(x-1)])		//左
				FindBlock(img, height, width, x-1, y, lab, visted, lianxushu);

			if (img[x-1][y+1] == 0 && !lab[(y+1)*height+(x-1)])		//左下角
				FindBlock(img, height, width, x-1, y+1, lab, visted, lianxushu);

	
			if (img[x+1][y] == 0 && !lab[(y)*height+(x+1)])		//右
				FindBlock(img, height, width, x+1, y, lab, visted, lianxushu);
	
	
			if (img[x][y+1] == 0 && !lab[(y+1)*height+(x)])		//下
				FindBlock(img, height, width, x, y+1, lab, visted, lianxushu);
	
			if (img[x+1][y+1] == 0 && !lab[(y+1)*height+(x+1)])		//右下角
				FindBlock(img, height, width, x+1, y+1, lab, visted, lianxushu);

		}
		else
		{
			return TRUE;	
		}

		if(g_lianxushu >= lianxushu)	//如果连续长度满足要求，返回
		{
			return TRUE;
		} 
	}

	return FALSE;
}

//删除孤立像素点
//void RemoveSeprateBlock(u8 **img, DPoint visted[])
//{
//	  u16 i,j;
//
//
//}




