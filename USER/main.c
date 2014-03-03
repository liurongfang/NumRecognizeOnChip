

//�ο�����ԭ��minStm32����������ͷʵ�����
//2014��02��23��

#include "main.h"

//�����ã��ȴ�6s
void wait()
{
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}

//������	  
int main(void)
{	
	u8 i = 0;
	u8 **img =  NULL;
	u8 **tz = NULL;

	
	//�ȴ�ϵͳ�������
	while(system_init());


	while(1)
	{
		img = alloc_mem2d_u8( IMG_H, IMG_W);
		tz = alloc_mem2d_u8( NUM, TZ);
		  
		//back:
		//��ʼ����						 	 
	 	while(1)
		{	
	 		camera_refresh(img, 4, 4);	//������ʾ	 
			
	 		if(i != ov_frame)		//DS0��˸.
			{
				i = ov_frame;
				LED0 = !LED0;
	 		}
	
			if (KEY0 == 0)
			{
				delay_ms(1000); 		//����
				if (KEY0 == 0)
					break;
			}
		}
	
	
		LCD_ShowString(40,50,200,200,16,"Continue...");
	
		thres  = GlobalThreshold(img, IMG_H, IMG_W)>>1;		//����ֵ����ֵ��1/2����Ϻ�
		printf("galobal thres:%d\r\n", thres);
//		thres = otsuThreshold(img, IMG_H, IMG_W);
//		printf("ostu thres:%d", thres);
//		img_display(img, IMG_H, IMG_W, (LCD_W-IMG_W)/2-1, (LCD_H-IMG_H)/2-1);  //��ʾ����Ļ�м�
//		print2serial(img, IMG_H, IMG_W);				   //�����ã�����ȥ��
	
		//wait();
	
		//ͼ����
		ImageHandle(tz, img, IMG_H, IMG_W, NUM);
		//while(KEY1 == 0);
		//goto back;
	
	
	
//		//��ʼʶ��
//		LCD_Fill(1,1,239,200,WHITE);
//		POINT_COLOR = BLUE;		//������ʾ��ϢΪ��ɫ
//		LCD_ShowString(40,50,200,200,16,"Start Recognize!");
//		delay_ms(20000);
//		LCD_Fill(1,1,239,319,WHITE);
	
		//�ͷ��ڴ�
		delete_mem2d_u8( img,IMG_H, IMG_W);
		delete_mem2d_u8( tz, NUM, TZ);

	}
	
	return 0;	   
}


//ϵͳ����ʱ�ĳ�ʼ��
u16 system_init(void)
{
	//������ʼ��	
   	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,9600);	 	//���ڳ�ʼ��Ϊ9600
	delay_init(72);	   	 	//��ʱ��ʼ�� 
  	OV7670_Init();			//��ʼ��OV7670	  

	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD
	if(lcddev.id==0X6804) 	//ǿ��������Ļ�ֱ���Ϊ320*240.��֧��3.5�����
	{
		lcddev.width=240;
		lcddev.height=320; 
	}
	usmart_dev.init(72);	//��ʼ��USMART		
 			    
	//��ʾ��ʾ��Ϣ
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(40,50,200,200,16,"platform:Mini STM32 V2.0");	
	LCD_ShowString(40,70,200,200,16,"chip type:STM32F103RBT6");	
	LCD_ShowString(40,90,200,200,16,"Code by:RongfangLiu");
	LCD_ShowString(40,110,200,200,16,"2014/02/23");  	 
	LCD_ShowString(40,150,200,200,16,">>Number Recognize<<");	 
  	LCD_ShowString(40,190,200,200,16,"OV7670 Init...");	  
	while(OV7670_Init())//��ʼ��OV7670
	{
		LCD_ShowString(40,210,200,200,16,"OV7670 Error!!");
		delay_ms(200);
	    LCD_Fill(40,230,239,246,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(40,210,200,200,16,"OV7670 Init OK");
	delay_ms(1500);	 	   	  
	Timerx_Init(10000,7199);			//TIM3,10Khz����Ƶ��,1�����ж�									  
	EXTI15_Init();						//ʹ�ܶ�ʱ������
	OV7670_Window_Set(10,174,240,320);	//���ô���	  
  	OV7670_CS=0;

	return 0;
}		


//������ͷ����LCD��ʾ
void camera_refresh(u8 **img, u16 yScale, u16 xScale)
{
	u32 i,j;
	u16 color_y,color_u,color_v;		//��������������RGBֵ
 	u16 color;		//��������RGB565�ĵ�������ֵ
	u8 color_gray;	//��������Ҷ�ֵ
	
		 
	if(ov_sta==2)
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������ 
		LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
			
 		OV7670_CS=0;	 			//Ƭѡʹ��
 		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK=0;				//
		OV7670_RCK=1;
		OV7670_RCK=0;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK=1;  

		//ѭ����ȡ240x320�����ص�
		for (j=0; j<240; j++)
		{
			for (i = 0; i<320; i++)
			{
				GPIOB->CRL=0X88888888;	 //����IO��Ϊ����״̬
				
				//�����ض�ȡ����	   
				OV7670_RCK=0; 
				color = OV7670_DATA;		//������
				OV7670_RCK=1; 
				
				color_gray = color;		 //����Yͨ�������Ҷ�ֵ������ֵ��
				color<<=8;			
						  
	 			OV7670_RCK=0;
				color|=OV7670_DATA;		//������		  
				OV7670_RCK=1; 

				GPIOB->CRL=0X33333333;		//����IO��Ϊ���״̬����LCDʹ��
				
				color_y = color&0xf800;		//rgb565��ʽ��ȡ����	  
				color_u = color>>11;			  
				color >>=5;
				color_v =color&0x07e0; 
						
				color= color_y + color_u + color_v;  		//��ʾ
								 	 
				LCD_WR_DATA(color);
				if (i != 319)	   //���Է��ֵ�319��ȫ�ǰ�ɫ�ģ���֪����ô���£����ԾͲ�Ҫ�ˣ���318�еĴ���
				{
					img[i/yScale][j/xScale] = color_gray;	//�͵�ͼ�����飬�ϵ��£����Ҵ洢,��1��1�ǲ������ȣ�����img[IMG_H-1]��һ��û��
				}
				//if (i >316) printf("c%d:%d ",i,color_gray);  //������
			} 
		}
		//printf("#\n");		   //������

 		OV7670_CS=1; 		//�ر�Ƭѡʹ��					 
		OV7670_RCK=0; 
		OV7670_RCK=1; 
		EXTI->PR=1<<15;     		//���LINE8�ϵ��жϱ�־λ
		ov_sta=0;					//��ʼ��һ�βɼ�
 		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 	  				 	 
	} 
}

//���������ͼ��LCD
void img_display(u8 **img, u16 height, u16 width, u16 x, u16 y, u8 mode)
{
	u16 i,j;
	u16 color;

	LCD_Fill( 0, 0, 239, 319,WHITE);		//sxy,exy
	//LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������
	LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	

//	for(i=sy;i<=ey;i++)
//	{
//	 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
//		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
//		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//���ù��λ�� 	    
//	}

	if (mode == 1)
	{		
		for (i = 0; i<height; i++)
		{
			LCD_SetCursor(x, y+i);	//���ù��λ�� 
			LCD_WriteRAM_Prepare();     //��ʼд��GRAM
	
			for (j = 0; j<width; j++)	   //ԭͼ��С��ʾ
			{
				//��gray�������ݸ�RGB565����λ���
				color = img[i][j]>>3;		//�Ƶ���λ����R,6λ
				color <<= 6;				//�ճ�6λ����G
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
			for (j = 0; j<width; j++)	   //��ֵ����ʾ
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
			for (j = 0; j<width; j++)	   //��ֵ����ʾ
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

	//LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 

}

//���������
void print2serial(u8 **img, u16 height, u16 width)
{
	u16 i,j;
	
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			printf("%d ",img[i][j]);
			//printf("%d",img[i][j]>thres?1:0);	 //��ֵ��
			//printf("%x",img[i][j]>100?1:0);
		}
		printf("\r\n");
	}
}

//���������ͼ�������ز���������ͼ��Ϊ�Ҷ�ͼ,���ֺڱ���
u16 ImageHandle(u8 **tz, u8 **img, u16 srcHeight, u16 srcWidth, u16 num)
{
	//��������
	u8 **img1 = NULL;
	u8 **alignImg = NULL;
	RectLink *rlink = NULL;
	DRect srcRect = {0, 0, 0, 0};		//���������������ڵľ�������
	DRect dstRect = {0, 0, 0, 0};
	u16 h = 0, w = 0;			//ͼ��Ŀ��

	alignImg = alloc_mem2d_u8(STD_H, num*STD_W);	//num -> 1 ���������׼�ߴ��ַ���8x16
	rlink = CreateRectLink(num);

	//Ԥ����
	BinaryImg(img, img, srcHeight, srcWidth, thres);		//100����ֵ����ֵ��֮���Ǻڿ�׵�
	//floodfill(img, srcHeight, srcWidth);					//���ڿ��Աߵİ�ɫ����
	InvertImg(img, img , srcHeight, srcWidth);				//���࣬��ɺ��ְ׿�

	//ȥ��С�������
	img_display(img, srcHeight, srcWidth, (LCD_W-IMG_W)/2-1, (LCD_H-IMG_H)/2-1, 3);
	print2serial(img, srcHeight, srcWidth);
	
	//��ʼ
	//��б�Ƚ���
	//SlopeAdjust(img, img, srcHeight, srcWidth);

	//������ַ����ڵľ�������
//	srcRect = DetectRect(img, srcHeight, srcWidth);
//	//����������򳤿�
//	h = srcRect.Y2 - srcRect.Y1 + 1;
//	w = srcRect.X2 - srcRect.X1 + 1;
//	dstRect.X2 = w;
//	dstRect.Y2 = h;
//	img1 = alloc_mem2d_u8(h, w);			//�������������ַ���������
//	CopyImg(img1, img, dstRect, srcRect);		
//
//	//����������ַ����ڵľ�������,rlink����
//	DetectNum(img1, h, w, rlink, num);
//
//	ShowRectLink(rlink);	//�������������ȥ��
//
//	
//	//�������ţ��ַ��ߴ��һ��
//	StdAlignImg(alignImg, img1, STD_H, num*STD_W, h, w, rlink, num);
//	
//	//img_display(alignImg, srcHeight, srcWidth, 0, 0, 1);
//	ShowRectLink(rlink);	//��ʾ�ߴ��һ��֮��ľ�������
//	
//	//������ȡ
//	TZTQ13(tz, alignImg, STD_H, num*STD_W, rlink, num);		   //13������

	//�ͷ��ڴ�
	delete_mem2d_u8(alignImg, STD_H, num*STD_W);
//	delete_mem2d_u8(img1, h, w);					//�ͷ�Դͼ��
	DeRectLink(rlink);

	return 0;
}















