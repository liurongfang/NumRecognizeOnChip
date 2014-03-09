

//参考正点原子minStm32开发板摄像头实验代码
//2014年02月23日

#include "main.h"

//定义LCD屏幕尺寸
#define LCD_H 320
#define LCD_W 240

//定义获取图像的尺寸
#define IMG_H 60
#define IMG_W 80

#define NUM 5
#define TZ  13

#define THRES 40  	  

extern u8 ov_sta;	//在exit.c里面定义，状态
extern u8 ov_frame;	//在timer.c里面定义，帧率

u8 thres = 60;

//函数定义		 
u16 system_init(void);		//系统启动时的初始化
void camera_refresh(u8 **img, u16 yScale, u16 xScale);		//更新LCD显示
void img_display(u8 **img, u16 height, u16 width, u16 x, u16 y, u8 mode);	//从数组更新图像到LCD
void print2serial(u8 **img, u16 height, u16 width);			//输出到串口
u16 ImageHandle(u8 **tz, u8 **img, u16 srcHeight, u16 srcWidth, u16 num);		//在这里完成图像处理的相关操作

//测试用，等待6s
void wait()
{
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
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
	u8 i;
	u8 **img =  NULL;
	u8 **tz = NULL;
	u8 *result = NULL;

	
	//等待系统启动完成
	while(system_init());


	while(1)
	{
		img = alloc_mem2d_u8( IMG_H, IMG_W);
		tz = alloc_mem2d_u8( NUM, TZ);
		result = (u8 *)malloc((NUM + 1) * sizeof(u8));

		//OV7670_Special_Effects(1);		//1:负片效果,0:正常
		  
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
	
	
		LCD_ShowString(40,50,200,200,16,"Will Continue...");
	
		thres  = GlobalThreshold(img, IMG_H, IMG_W);		//经验值，阈值的1/2分离较好
		printf("galobal thres:%d\r\n", thres);

		img_display(img, IMG_H, IMG_W, (LCD_W-IMG_W)/2-1, (LCD_H-IMG_H)/2-1, 0);  //显示在屏幕中间，灰度图0：原样显示
		//print2serial(img, IMG_H, IMG_W);				   //输出灰度图到串口，测试用
	
		//wait();
	
		//图像处理，传入灰度图
		ImageHandle(tz, img, IMG_H, IMG_W, NUM);
		//while(KEY1 == 0);
		//goto back;
		LCD_ShowString(40,50,200,200,16,"ImageHandle End!");

		//开始识别
		Recognize(result, tz, NUM, TZ);
	
		//输出识别结果
		LCD_Fill(1,1,239,200,WHITE);
		POINT_COLOR = BLUE;		//设置提示信息为蓝色
		LCD_ShowString(40,50,200,200,16,"Is Recognize...");
		LCD_ShowString(40,90,200,200,16,"Recognize Result:");
		for (i = 0; i<NUM; i++)	printf("\r\n%c",result[i]);
		LCD_ShowString(40,110,200,200,16,result);

		wait();
		wait();
		//LCD_Fill(1,1,239,319,WHITE);
	
		//释放内存
		delete_mem2d_u8( img,IMG_H, IMG_W);
		delete_mem2d_u8( tz, NUM, TZ);
		free(result);
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
		for (i = 239; i /*>=0*/; i--)
		{
			for (j=0; j<320; j++)
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
								 	 
				if (j != 319 && i != 239)	   //测试发现第319行全是白色的，不知道怎么回事，所以就不要了，用318行的代替
				{
					img[i/yScale][j/xScale] = color_gray;	//送到图像数组，上到下，左到右存储
				}
				//if (i >316) printf("c%d:%d ",i,color_gray);  //测试用

				LCD_WR_DATA(color);
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

//从数组更新图像到LCD,传入灰度图（0：正常显示，1：二值显示）或者二值图（2：二值显示）
void img_display(u8 **img, u16 height, u16 width, u16 x, u16 y, u8 mode)
{
	u16 i,j;
	u16 color;
	u8 temp;

	LCD_Fill( 0, 0, 239, 319,WHITE);		//sxy,exy
	//LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右
	LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向,从左到右，从上到下 
	

//	for(i=sy;i<=ey;i++)
//	{
//	 	LCD_SetCursor(sx,i);      				//设置光标位置 
//		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
//		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
//	}

	if (mode == 0)
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

//				if (img[i][j] > thres)
//				{
//					color = 0xffff;
//				}
//				else
//				{
//					color = 0x0000;
//				}
	
				LCD_WR_DATA(color);
			}
		}
	}
	else
	if (mode == 1)
	{
		for (i = 0; i<height; i++)
		{
			LCD_SetCursor(x, y+i);	//设置光标位置 
			LCD_WriteRAM_Prepare();     //开始写入GRAM
	
			for (j = 0; j<width; j++)	   //原图缩小显示
			{
				temp = (img[i][j]>thres)?0xffff:0x0;

				//将gray复制三份给RGB565，低位舍掉
				color = temp>>3;		//移掉三位，给R,6位
				color <<= 6;				//空出6位，存G
				color |= temp>>2;		//6
				color <<=5;
				color |= temp>>3;		//5

	
				LCD_WR_DATA(color);
			}
		}
	
	}
	else
	if (mode == 2)
	{
		for (i = 0; i<height; i++)
		{
			LCD_SetCursor(x, y+i);	//设置光标位置 
			LCD_WriteRAM_Prepare();     //开始写入GRAM
	
			for (j = 0; j<width; j++)	   //原图缩小显示
			{
				temp = img[i][j]?0xffff:0x0;

				//将gray复制三份给RGB565，低位舍掉
				color = temp>>3;		//移掉三位，给R,6位
				color <<= 6;				//空出6位，存G
				color |= temp>>2;		//6
				color <<=5;
				color |= temp>>3;		//5
	
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
	//InvertImg(img, img , srcHeight, srcWidth);				//选用反相，变成前景白色，背景黑色

	//输出，测试用
	LCD_ShowString(40,50,200,200,16,"In ImageHandle().");
	img_display(img, IMG_H, IMG_W, (LCD_W-IMG_W)/2-1, (LCD_H-IMG_H)/2-1, 2);	
	print2serial(img, srcHeight, srcWidth);

	//去除小块的噪声
	
	//倾斜度矫正
	//SlopeAdjust(img, img, srcHeight, srcWidth);

	//细化图像
	//ThinnerRosenfeld(img, srcHeight, srcWidth);
	//LCD_ShowString(40,50,200,200,16,"Thinnner end.");
	//img_display(img, IMG_H, IMG_W, (LCD_W-IMG_W)/2-1, (LCD_H-IMG_H)/2-1, 2);

	//分离出字符所在的矩形区域
	srcRect = DetectRect(img, srcHeight, srcWidth);
	LCD_ShowString(40,50,200,200,16,"DetectRect end.");

	//计算矩形区域长宽
	h = srcRect.Y2 - srcRect.Y1 + 1;
	w = srcRect.X2 - srcRect.X1 + 1;
	printf("srcRect{%d,%d,%d,%d}\r\n",srcRect.X1,srcRect.Y1,srcRect.X2,srcRect.Y2);
	printf("h:%d,w:%d\r\n",h,w);
	dstRect.X2 = w-1; 		//记住坐标都要比长宽少1
	dstRect.Y2 = h-1;
	img1 = alloc_mem2d_u8(h, w);			//用来存分离出的字符矩形区域，根据分离出的矩形大小确定
	LCD_ShowString(40,50,200,200,16,"anchor (1). end");
	printf("dstRect{%d,%d,%d,%d}\r\n",dstRect.X1,dstRect.Y1,dstRect.X2,dstRect.Y2);

	//拷贝得到的矩形区域给img1
	CopyImg(img1, img, dstRect, srcRect);
	img_display(img1, h, w, (LCD_W-w)/2-1, (LCD_H-h)/2-1, 2);
	LCD_ShowString(40,50,200,200,16,"Copy end ...");
	print2serial(img1, h, w);		

	//分离出单个字符所在的矩形区域,rlink保存
	DetectNum(img1, h, w, rlink, num);
	LCD_ShowString(40,50,200,200,16,"anchor (2). end");

	//输出获得的矩形链表到串口，测试用
	ShowRectLink(rlink);	
	
	//紧缩重排，字符尺寸归一化
	StdAlignImg(alignImg, img1, STD_H, num*STD_W, h, w, rlink, num);
	img_display(alignImg, STD_H, num*STD_W, (LCD_W-num*STD_W)/2-1, (LCD_H-STD_H)/2-1, 2);
	LCD_ShowString(40,50,200,200,16,"anchor (3). end");
	print2serial(alignImg, STD_H, num*STD_W);
	
	//输出获得的矩形链表到串口，测试用
	printf("归一化之后的矩形：\r\n");
	ShowRectLink(rlink);	//显示尺寸归一化之后的矩形链表
	
	//特征提取
	TZTQ13(tz, alignImg, STD_H, num*STD_W, rlink, num);		   //13点特征
	print2serial(tz, num, 13);
	LCD_ShowString(40,50,200,200,16,"anchor (4). end");

	//释放内存
	delete_mem2d_u8(alignImg, STD_H, num*STD_W);
	delete_mem2d_u8(img1, h, w);
	DeRectLink(rlink);

	return 0;
}















