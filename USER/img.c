/**************************************************************************************
*	描述：这个文件里的函数对图像进行后期处理和特征提取
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	引用需联系作者
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include "img.h"
#include "usart.h"
#include "usmart.h"

//u16 after_h = 0;
//u16 after_w = 0;

//创建矩形链表
RectLink *CreateRectLink(u16 num)
{
	RectLink *head = NULL;		//建立头指针
	RectLink *p, *pTmp;			//p代表当前单元，pTmp代表最后一个单元
	u16 i = 0;

	//是第一个节点，与head直接联系起来
	head = p = pTmp = (RectLink *)malloc(sizeof(RectLink));
	p->next = NULL;
	//head->next = p;
	//pTmp = head->next;

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

	//printf("Create!%d\n",i);
	//system("pause");



	return (head);

}

//用给定的DRect初始化矩形链表
u16 InitRectLink(RectLink *head, DRect rect)
{
	RectLink *p = head;
	while (p->next != NULL)
	{
		p->data = rect;
		//printf("Hello!%d %d %d %d",head->data.X1,head->data.Y1,head->data.X2,head->data.Y2);
		p = p->next;
	}
	p->data = rect;
	//printf("Hello!%d %d %d %d",head->data.X1,head->data.Y1,head->data.X2,head->data.Y2);

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
	RectLink *p = rlink;

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
//倾斜度矫正
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
void DetectRect(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, DRect *rect)
{
	u16 i,j;			//指向Src
	u16 k = 0, l = 0;		//指向Dst
	//DRect rect = {0,0,0,0};
	u16 minX = srcWidth;
	u16 maxX = 0;
	u16 minY = srcHeight;
	u16 maxY = 0;

	//assert(Src != NULL);		//参数检查，断言
	//if (Src == NULL)
	//{
	//	return;
	//}

	//算法一
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

	//在图像全白时将图像界限作为矩形边界
	if ((minX == srcWidth - 1 ) && (maxX == 1 ) && (minY == srcHeight - 1 ) && (maxY ==1 ))
	{
		minX = 1;
		maxX = srcWidth - 1;
		minY = 1;
		maxY = srcHeight - 1;
	}

	//将得到的坐标转换为矩形对角坐标,并外扩一个像素
	(*rect).X1 = (minX - 1)<0?0:(minX - 1);
	(*rect).Y1 = (minY - 1)<0?0:(minY - 1);
	(*rect).X2 = maxX + 1;
	(*rect).Y2 = maxY + 1;

	//将源图像中的目标矩形区域的像素复制给目标图像
	for (i = (*rect).Y1, k = 0; i <= (*rect).Y2; i++, k++)
	{
		for (j = (*rect).X1, l = 0; j <= (*rect).X2; j++, l++)
		{
			
			Dst[k][l] = Src[i][j];
		}
	}

	//同步修改图像宽度和高度
	//after_h = rect.Y2 - rect.Y1 + 1;
	//after_w = rect.X2 - rect.X1 + 1;

	//return (rect);
}



//分离出单个字符组成的数组，多个循环，耗时较长
void DetectNum(u8 **Src, u16 srcHeight, u16 srcWidth , RectLink *rectlink, u16 num)
{
	u16 i, j, count = 0;
	//u16 x, y;		//单个字符的坐标
	u16 cutFlag = FALSE;		//切割开始的标志
	u16 allWhite = TRUE;		//当前列全部为白色的标志
	//u16 pos[60] = {0};		//用来保存字符的始末X，最多 60/2=30 个字符
	DRect rect = {0, 0, 0, 0};

	//RectLink *rectlink = CreateRectLink(num);		//创建带有num个矩形的矩形链表
	RectLink *p = rectlink;		//用来指向rectlink表
	//printf("Hello!");
	InitRectLink(p, rect);		//用rect初始化rectLink
	p = rectlink;
	//printf("Hello!");

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

		if (allWhite && cutFlag)		//如果当前列全部为白色像素点，切割毕
		{
			cutFlag = FALSE;	//切割毕
			p->data.X2 = j-1;		//记录结束X坐标

			//printf("NUM:%d\nRECT:{%d %d %d %d}\n",count,p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
			count++;			//切割计数加1
			p = p->next;		//准备下一个链表来存放数据
		}

		if (count == num)		//如果切割了num个字符，完成，结束跳出
		{
			break;
		}//else contiune
		
	}

	//printf("TEST1,OK\n");
	//system("pause");

	//num = count;		//保存字符个数值

	//下面细化数字区域
	p = rectlink;		//p指针复位到rectlink头
	count = 0;
	for (; count < num; p = p->next)			//准备下一个链表来存放数据，最后一个p = NULL
	{
		//printf("TEST+,OK, In the p is:%#x\n",p);
		//system("pause");

		//if (p == 0) break;
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

	////拷贝单个字符进图像数组
	//for (count = 0; count<num ; count++)
	//{
	//	x = 0;		//单幅图的“指针”置零
	//	y = 0;

	//	for (i = 0; i<srcHeight; i++)
	//	{
	//		for (j = pos[count*2]; j<pos[count*2+1]; j++)
	//		{
	//			Dst[count][x++][y++] = Src[i][j];
	//		}
	//	}
	//}

	//return (rectlink);
}

//归一化单个字符到指定宽和高，并紧缩重排
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num)
{
	//u16 i,j;	//指向源图像的变量
	//u16 k,l;	//指向目标图像的变量
	u16 m;
	RectLink *p = rlink;		//从rlink取数据
	DRect srcRect = {0, 0, 0, 0};		//源图像中的矩形区域
	DRect dstRect = {0, 0, (STD_W - 1), (STD_H - 1)};		//目标图像中的矩形区域，初始化,8x16宽高
	//DRect oldDstRect = dstRect;

	SetImg(Dst, dstHeight, dstWidth, 1);		//将dst全部填充为白色
	//printf("After setimg(),before standardImg()!\n");
	//displayImg(Dst, dstHeight, dstWidth);
	//system("pause");

	for (m = 0; m<num; m++)
	{
		srcRect = p->data;		//获取源图像矩形区域
		/*for (i = p->data.Y1; i < p->data.Y2; i++)
		{
			for (j  = p->data.X1; j < p->data.X2; j++)
			{
				printf("before\n");
				
				printf("after\n");
			}
			printf("I'am change!I'am change!I'am change!\n");
		}*/

		StandardImg(Dst, Src, dstRect, srcRect);	//从srcRect放缩到dstRect

		//printf("longlonglonglonglonglonglonglonglonglonglonglonglonglong\n");

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
u16 StandardImg(u8 **Dst, u8 **Src, DRect dstRect, DRect srcRect)
{
	u16 i = srcRect.Y1, j = srcRect.X1;		//指向源图像
	u16 k = dstRect.Y1, l = dstRect.X1;		//指向目标图像
	u16 s, t;		//指向增量
	double xScale = 0, yScale = 0;
	u16 srcHeight = srcRect.Y2 - srcRect.Y1 + 1;		//得到源图像宽高
	u16 srcWidth = srcRect.X2 - srcRect.X1 + 1;
	u16 dstHeight = dstRect.Y2 - dstRect.Y1 + 1;
	u16 dstWidth = dstRect.X2 - dstRect.X1 + 1;

	//得到放缩比，这里可以用定点数优化之，Q16
	yScale = (double ) (srcHeight / (double )dstHeight);
	xScale = (double ) (srcWidth / (double )dstWidth);

	//printf("%d/%d=%0.2lf %d/%d=%0.2lf\n",srcHeight,dstHeight,yScale,srcWidth,dstWidth,xScale);
	//system("pause");

	//循环遍历，稀疏采样
	for (k = dstRect.Y1; k <= dstRect.Y2; k++)
	{
		s = (k - dstRect.Y1);		//计算y增量
		for (l = dstRect.X1; l <= dstRect.X2; l++)
		{
			
			t = (l - dstRect.X1);//计算x增量
			Dst[k][l] = Src[(u16 )(i + s * yScale + 0.5) ][(u16 )(j + t * xScale + 0.5) ];
			//调试用，结果发现错误
			/*if ((i + s * yScale + 0.5) > srcRect.Y2)
			{
				printf("in std, y error!\n");
				printf("1 & 2 is:%d %d\n",(u16 )(i + s * yScale + 0.5),srcRect.Y2);
				system("pause");
			}
			if ((j + t * xScale + 0.5) > srcRect.X2)
			{
				printf("in std, x error!\n");
				printf("1 & 2 is:%d %d,x1=%d\n",(u16 )(j + t * xScale + 0.5),srcRect.X2,srcRect.X1);
				printf("s:%d and t:%d\n",s,t);
				system("pause");
			}*/
		}
	}

	//after_h = dstHeight;
	//after_w = dstWidth;

	return 0;
}

//特征提取，13个特征，多重循环，耗时较长
u16 TZTQ13(u8 **tz, u8 **Src, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num)
{
	//还是将图像作为一整张来提取
	u16 i, j;
	u16 m, a;
	//u16 te = 0;
	u16 height,width;
	RectLink *p = rlink;

	//水平两条，竖直两条，逐行扫描，逐列扫描,

	////水平逐行像素，h个特征
	//for (i = 0; i<srcHeight; i++)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])
	//		{
	//			TZ[i]++;
	//		}
	//	}
	//}

	////竖直逐列像素，w个特征
	//for (j = 0; j<srcWidth; j++)
	//{
	//	for (i = 0; i<srcHeight; i++)
	//	{
	//		if (0 == Src[i][j])
	//		{
	//			TZ[srcHeight + j]++;
	//		}
	//	}
	//}

	//提取前8个特征
	//for (m = 0; m<8; m++)
	//{
	//	for (i=u16(m/2)*8; i<(u16(m/2)+1)*8; i++)
	//	{
	//		for (j=m%2*8+k*width;j<(m%2+1)*8+k*width;j++)
	//		{
	//			TZ[m]++;
	//		}
	//	}
	//}


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
					//printf("p->data.Y1=%d + (a%4)*height/4=%d\n",(a%4)*height/4,p->data.Y1);
					//printf("p->data.X1=%d + (a%2)*width/2=%d\n",(a%2)*width/2,p->data.X1);
					//printf("the cur:(%d, %d)\n",i,j);
					//system("pause");
					if (0 == Src[i][j])
					{
						tz[m][a]++;
					}
					//te++;
				}
			}
			//printf("当前块像素数目:%d\n",te);
			//system("pause");
			//te = 0;
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

	//预处理，将UCHAR **tz变成double **tz
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



//特征提取，h+w+4个特征
//将图像反相，0变成1,1变成0
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth)
{
	u16 i, j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = 1 - Src[i][j];		//0-1反相
		}
	}

	//return 0;
}

//将图像的像素值全部置为tag
u16 SetImg(u8 **Dst, u16 srcHeight, u16 srcWidth, u8 tag)
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

	return 0;
}

//将图像二值化
u16 BinaryImg(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, u16 thres)
{
	u16 i, j;

	//逐个像素扫描
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (Src[i][j] > (u8)thres)		//大于阈值
			{
				Dst[i][j] = 1;		//置为1	,白色点
			} 
			else
			{
				Dst[i][j] = 0;		
			}
		}
	}

	return 0;
}



