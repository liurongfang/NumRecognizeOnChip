

//参考正点原子minStm32开发板摄像头实验代码
//2014年02月23日

#include "main.h"

//测试用，等待6s
void wait()
{
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}

//主函数	  
int main(void)
{	
	u8 i = 0;
	u8 **img =  NULL;
	u8 **tz = NULL;

	
	//等待系统启动完成
	while(system_init());


	while(1)
	{
		img = alloc_mem2d_u8( IMG_H, IMG_W);
		tz = alloc_mem2d_u8( NUM, TZ);

		//OV7670_Special_Effects(1);		//1:负片效果
		  
		//back:
		//开始工作						 	 
	 	while(1)
		{	
	 		camera_refresh(img, 4, 4);	//更新显示	 
			
	 		if(i != ov_frame)			//DS0闪烁.
			{
				i = ov_frame;
				LED0 = !LED0;
	 		}
	
			if (KEY0 == 0)
			{
				delay_ms(1000); 		//消抖
				if (KEY0 == 0)
					break;
			}
		}
	
	
		LCD_ShowString(40,50,200,200,16,"Continue...");
	
		thres  = GlobalThreshold(img, IMG_H, IMG_W)>>1;		//经验值，阈值的1/2分离较好
		printf("galobal thres:%d\r\n", thres);
//		thres = otsuThreshold(img, IMG_H, IMG_W);
//		printf("ostu thres:%d", thres);
		img_display(img, IMG_H, IMG_W, (LCD_W-IMG_W)/2-1, (LCD_H-IMG_H)/2-1, 2);  //显示在屏幕中间
//		print2serial(img, IMG_H, IMG_W);				   //测试用，不可去掉
	
		//wait();
	
		//图像处理
		ImageHandle(tz, img, IMG_H, IMG_W, NUM);
		//while(KEY1 == 0);
		//goto back;
	
	
	
//		//开始识别
//		LCD_Fill(1,1,239,200,WHITE);
//		POINT_COLOR = BLUE;		//设置提示信息为蓝色
//		LCD_ShowString(40,50,200,200,16,"Start Recognize!");
//		delay_ms(20000);
//		LCD_Fill(1,1,239,319,WHITE);
	
		//释放内存
		delete_mem2d_u8( img,IMG_H, IMG_W);
		delete_mem2d_u8( tz, NUM, TZ);

	}
	
	return 0;	   
}


//系统启动时的初始化
u16 system_init(void)
{
	//基本初始化	
   	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,9600);	 	//串口初始化为9600
	delay_init(72);	   	 	//延时初始化 
  	OV7670_Init();			//初始化OV7670	  

	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD
	if(lcddev.id==0X6804) 	//强制设置屏幕分辨率为320*240.以支持3.5寸大屏
	{
		lcddev.width=240;
		lcddev.height=320; 
	}
	usmart_dev.init(72);	//初始化USMART		
 			    
	//显示提示信息
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(40,50,200,200,16,"platform:Mini STM32 V2.0");	
	LCD_ShowString(40,70,200,200,16,"chip type:STM32F103RBT6");	
	LCD_ShowString(40,90,200,200,16,"Code by:RongfangLiu");
	LCD_ShowString(40,110,200,200,16,"2014/02/23");  	 
	LCD_ShowString(40,150,200,200,16,">>Number Recognize<<");	 
  	LCD_ShowString(40,190,200,200,16,"OV7670 Init...");	  
	while(OV7670_Init())//初始化OV7670
	{
		LCD_ShowString(40,210,200,200,16,"OV7670 Error!!");
		delay_ms(200);
	    LCD_Fill(40,230,239,246,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(40,210,200,200,16,"OV7670 Init OK");
	delay_ms(1500);	 	   	  
	Timerx_Init(10000,7199);			//TIM3,10Khz计数频率,1秒钟中断									  
	EXTI15_Init();						//使能定时器捕获
	OV7670_Window_Set(10,174,240,320);	//设置窗口	  
  	OV7670_CS=0;

	return 0;
}		


//从摄像头更新LCD显示
void camera_refresh(u8 **img, u16 yScale, u16 xScale)
{
	u32 i,j;
	u16 color_y,color_u,color_v;		//定义变量用来存放RGB值
 	u16 color;		//用来保存RGB565的单个像素值
	u8 color_gray;	//用来保存灰度值
	
		 
	if(ov_sta==2)
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右 
		LCD_SetCursor(0x00,0x0000);	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
			
 		OV7670_CS=0;	 			//片选使能
 		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK=0;				//
		OV7670_RCK=1;
		OV7670_RCK=0;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK=1;  

		//循环读取240x320个像素点
		for (j=0; j<240; j++)
		{
			for (i = 0; i<320; i++)
			{
				GPIOB->CRL=0X88888888;	 //设置IO口为输入状态
				
				//上升沿读取数据	   
				OV7670_RCK=0; 
				color = OV7670_DATA;		//读数据
				OV7670_RCK=1; 
				
				color_gray = color;		 //保存Y通道，即灰度值（亮度值）
				color<<=8;			
						  
	 			OV7670_RCK=0;
				color|=OV7670_DATA;		//读数据		  
				OV7670_RCK=1; 

				GPIOB->CRL=0X33333333;		//设置IO口为输出状态，给LCD使用
				
				color_y = color&0xf800;		//rgb565方式读取数据	  
				color_u = color>>11;			  
				color >>=5;
				color_v =color&0x07e0; 
						
				color= color_y + color_u + color_v;  		//显示
								 	 
				if (i != 319)	   //测试发现第319行全是白色的，不知道怎么回事，所以就不要了，用318行的代替
				{
					img[i/yScale][j/xScale] = color_gray;	//送到图像数组，上到下，左到右存储,加1减1是补偿精度，否则img[IMG_H-1]这一行没有
				}
				//if (i >316) printf("c%d:%d ",i,color_gray);  //测试用

				LCD_WR_DATA(~color);
			} 
		}
		//printf("#\n");		   //测试用

 		OV7670_CS=1; 		//关闭片选使能					 
		OV7670_RCK=0; 
		OV7670_RCK=1; 
		EXTI->PR=1<<15;     		//清除LINE8上的中断标志位
		ov_sta=0;					//开始下一次采集
 		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 	  				 	 
	} 
}

//从数组更新图像到LCD
void img_display(u8 **img, u16 height, u16 width, u16 x, u16 y, u8 mode)
{
	u16 i,j;
	u16 color;

//	LCD_Fill( 0, 0, 239, 319,WHITE);		//sxy,exy
	//LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右
	LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	

//	for(i=sy;i<=ey;i++)
//	{
//	 	LCD_SetCursor(sx,i);      				//设置光标位置 
//		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
//		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
//	}

	if (mode == 1)
	{		
		for (i = 0; i<height; i++)
		{
			LCD_SetCursor(x, y+i);	//设置光标位置 
			LCD_WriteRAM_Prepare();     //开始写入GRAM
	
			for (j = 0; j<width; j++)	   //原图缩小显示
			{
				//将gray复制三份给RGB565，低位舍掉
				color = img[i][j]>>3;		//移掉三位，给R,6位
				color <<= 6;				//空出6位，存G
				color |= img[i][j]>>2;		//6
				color <<=5;
				color |= img[i][j]>>3;		//5
	
				LCD_WR_DATA(color);
			}
		}
	}
	else
	if (mode == 2)
	{
		for (i = 0; i<height; i++)
		{
			for (j = 0; j<width; j++)	   //二值化显示
			{
				if (img[i][j] > thres)
				{
					color = 0xffff;
				}
				else
				{
					color = 0x0000;
				}
	
				LCD_WR_DATA(color);
			}
		}
	}
	else
	if (mode == 3)
	{
		for (i = 0; i<height; i++)
		{
			for (j = 0; j<width; j++)	   //二值化显示
			{
				if (img[i][j] == 1)
				{
					color = 0xffff;
				}
				else
				if (img[i][j] == 0)
				{
					color = 0x0000;
				}
	
				LCD_WR_DATA(color);
			}
		}
	}

	//LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 

}

//输出到串口
void print2serial(u8 **img, u16 height, u16 width)
{
	u16 i,j;
	
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			printf("%d ",img[i][j]);
			//printf("%d",img[i][j]>thres?1:0);	 //二值化
			//printf("%x",img[i][j]>100?1:0);
		}
		printf("\r\n");
	}
}

//在这里完成图像处理的相关操作，传入图像为灰度图,白字黑背景
u16 ImageHandle(u8 **tz, u8 **img, u16 srcHeight, u16 srcWidth, u16 num)
{
	//变量声明
	u8 **img1 = NULL;
	u8 **alignImg = NULL;
	RectLink *rlink = NULL;
	DRect srcRect = {0, 0, 0, 0};		//用来保存数字所在的矩形区域
	DRect dstRect = {0, 0, 0, 0};
	u16 h = 0, w = 0;			//图像的宽高

	alignImg = alloc_mem2d_u8(STD_H, num*STD_W);	//num -> 1 ，用来存标准尺寸字符，8x16
	rlink = CreateRectLink(num);

	//预处理
	BinaryImg(img, img, srcHeight, srcWidth, thres);		//100是阈值，二值化之后是黑块白底
	//floodfill(img, srcHeight, srcWidth);					//填充黑块旁边的白色区域
	InvertImg(img, img , srcHeight, srcWidth);				//反相，变成黑字白块

	//去除小块的噪声
	img_display(img, srcHeight, srcWidth, (LCD_W-IMG_W)/2-1, (LCD_H-IMG_H)/2-1, 3);
	print2serial(img, srcHeight, srcWidth);
	
	//开始
	//倾斜度矫正
	//SlopeAdjust(img, img, srcHeight, srcWidth);

	//分离出字符所在的矩形区域
//	srcRect = DetectRect(img, srcHeight, srcWidth);
//	//计算矩形区域长宽
//	h = srcRect.Y2 - srcRect.Y1 + 1;
//	w = srcRect.X2 - srcRect.X1 + 1;
//	dstRect.X2 = w;
//	dstRect.Y2 = h;
//	img1 = alloc_mem2d_u8(h, w);			//用来存分离出的字符矩形区域
//	CopyImg(img1, img, dstRect, srcRect);		
//
//	//分离出单个字符所在的矩形区域,rlink保存
//	DetectNum(img1, h, w, rlink, num);
//
//	ShowRectLink(rlink);	//输出到串口助手去了
//
//	
//	//紧缩重排，字符尺寸归一化
//	StdAlignImg(alignImg, img1, STD_H, num*STD_W, h, w, rlink, num);
//	
//	//img_display(alignImg, srcHeight, srcWidth, 0, 0, 1);
//	ShowRectLink(rlink);	//显示尺寸归一化之后的矩形链表
//	
//	//特征提取
//	TZTQ13(tz, alignImg, STD_H, num*STD_W, rlink, num);		   //13点特征

	//释放内存
	delete_mem2d_u8(alignImg, STD_H, num*STD_W);
//	delete_mem2d_u8(img1, h, w);					//释放源图像
	DeRectLink(rlink);

	return 0;
}















