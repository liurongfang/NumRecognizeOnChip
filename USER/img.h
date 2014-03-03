/**************************************************************************************
*	����������ļ��Ƕ�img.c���õ��ĺ�����������
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. ������ע������
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/


#ifndef _MY_IMG_
#define _MY_IMG_

#include "base.h"

//��׼����͸�
#define STD_W 8
#define STD_H 16

//���νṹ�Ķ���
typedef struct Rct
{
	u16 X1;		//���Ͻ�X��Y����
	u16 Y1;
	u16 X2;		//���½�X��Y����
	u16 Y2;
	//u16 lefttopX;
	//u16 lefttopY;
	//u16 rightbottomX;
	//u16 rightbottomY;
	// u16 leftbottomX;
	// u16 leftbottomY;
	// u16 righttopX;
	// u16 righttopY;
}DRect;

typedef struct Pot
{
	u16 x;
	u16 y;
}DPoint;


//������������Ԫ
typedef struct rectLink
{
	DRect data;
	struct rectLink *next;
}RectLink;

//һ��������������mybmp.h�ж��岢��mybmp��ʵ��
//extern u8 **allloc_mem2d(u16 height, u16 width);
//extern void delete_mem2d(u8 **mem, u16 height, u16 width);
//extern void displayImg(u8 **image, u16 height, u16 width);

//��������Ϊnum�ĵ����������
RectLink *CreateRectLink(u16 num);

//�ø�����DRect��ʼ����������
u16 InitRectLink(RectLink *head, DRect rect);

//��������
u16 DeRectLink(RectLink *head);

//��ʾ�õ��ľ�������
void ShowRectLink(RectLink *rlink);

//���Է�����ַ����ڵľ�������
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth);

//��б�Ƚ���
void SlopeAdjust(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth);

//��һ�������ַ���ָ����͸�
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//��׼��ͼ��
u16 CopyImg(u8 **Dst, u8 **Src, DRect dstRect, DRect srcRect);

//������ȡ��13������
u16 TZTQ13(u8 **tz, u8 **Src, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//��������һ����0-1֮��
void TZ_Std(double **tzDb,u8 **tz, u16 num, u16 tzCount);

//����������ַ���ɵ�����
void DetectNum(u8 **Src, u16 srcHeight, u16 srcWidth , RectLink *rectlink, u16 num);

//��ͼ���࣬0���1,1���0
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth);

//��ͼ�������ֵȫ����Ϊtag
void SetImg(u8 **Dst, u16 srcHeight, u16 srcWidth, u8 tag);

//��ͼ���ֵ��
void BinaryImg(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, u8 thres);

//����ͼ���ȫ����ֵ
u8 GlobalThreshold(u8 **img, u16 height, u16 width);

//OSTU��ͼ�����ֵ
u8 otsuThreshold(u8 **img, u16 height, u16 width);

//����䣬�п��ܻ�����µġ��ס�
u16 floodfill(u8 **img, u16 height, u16 width);

//Ѱ��ͼ���еĹ������ؿ�
bool FindBlock(u8 **img, u16 height, u16 width, u16 x, u16 y, bool lab[], DPoint visted[],u16 lianxushu);

#endif

