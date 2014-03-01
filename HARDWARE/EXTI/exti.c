#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "ov7670.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�ⲿ�ж� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/01  
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved	  
////////////////////////////////////////////////////////////////////////////////// 	  
 

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==1)	 //����2
	{
		LED0=!LED0;
		LED1=!LED1;	
	}		 
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}

////�ⲿ�ж�15~10�������
//void EXTI15_10_IRQHandler(void)
//{			
//	delay_ms(10);    //����			 
//	if(KEY0==0)      //����0
//	{
//		LED0=!LED0;
//	}else if(KEY1==0)//����1
//	{
//		LED1=!LED1;
//	}
//	EXTI->PR=1<<13;     //���LINE13�ϵ��жϱ�־λ  
//	EXTI->PR=1<<15;     //���LINE15�ϵ��жϱ�־λ  
//}
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0,PA13,PA15Ϊ�ж�����.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	JTAG_Set(JTAG_SWD_DISABLE);//�ر�JTAG��SWD   

	GPIOA->CRL&=0XFFFFFFF0;//PA0���ó�����	  
	GPIOA->CRL|=0X00000008;   
	GPIOA->CRH&=0X0F0FFFFF;//PA13,15���ó�����	  
	GPIOA->CRH|=0X80800000; 				   
	GPIOA->ODR|=1<<13;	   //PA13����,PA0Ĭ������
	GPIOA->ODR|=1<<15;	   //PA15����

	Ex_NVIC_Config(GPIO_A,0,RTIR); //�����ش���
	Ex_NVIC_Config(GPIO_A,13,FTIR);//�½��ش���
	Ex_NVIC_Config(GPIO_A,15,FTIR);//�½��ش���

	MY_NVIC_Init(2,2,EXTI0_IRQChannel,2);    //��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,1,EXTI15_10_IRQChannel,2);//��ռ2�������ȼ�1����2	   
}
/////////////////////////////////////////////////////////////////////////////////////
//�жϷ�����
u8 ov_sta;
void EXTI15_10_IRQHandler(void)
{			
	if(EXTI->PR&(1<<15))//��15�ߵ��ж�
	{     
		if(ov_sta<2)
		{
			if(ov_sta==0)
			{
				OV7670_WRST=0;	 	//��λдָ��		  		 
				OV7670_WRST=1;	
				OV7670_WREN=1;		//����д��FIFO
			}else OV7670_WREN=0;	//��ֹд��FIFO 	 
			ov_sta++;
		}
	}
	EXTI->PR=1<<15;     //���LINE15�ϵ��жϱ�־λ	
}
//�ⲿ�жϳ�ʼ������
//��ʼ��PA15Ϊ�ж�����.
void EXTI15_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	JTAG_Set(SWD_ENABLE);	//�ر�JTAG  	 
	GPIOA->CRH&=0X0FFFFFFF;	//PA15���ó�����	  
	GPIOA->CRH|=0X80000000; 				   
 	GPIOA->ODR|=1<<15;	   	//PA15����			   

	Ex_NVIC_Config(GPIO_A,15,FTIR);//�½��ش���
	MY_NVIC_Init(2,1,EXTI15_10_IRQChannel,2);//��ռ2�������ȼ�1����2	   
}











