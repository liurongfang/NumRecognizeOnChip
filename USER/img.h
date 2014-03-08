/**************************************************************************************
*	����������ļ��Ƕ�img.c���õ��ĺ�����������
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. ������ע������
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/


#ifndef _MY_IMG_H_
#define _MY_IMG_H_

#include "base.h"

//ͼ��ߴ��һ���Ŀ�͸�
#define STD_W 8
#define STD_H 16

//����ǰ��ɫ�ͱ���ɫ,1����ɫ��0����ɫ
#define FORECOLOR 1
#define BACKCOLOR 0

//0x1:0-1���࣬0xff:0-255����
#define CR 0x1

//���νṹ�Ķ���
typedef struct Rct
{
	u16 X1;		//���Ͻ�X��Y����
	u16 Y1;
	u16 X2;		//���½�X��Y����
	u16 Y2;
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


//��������Ϊnum�ĵ����������
//���룺num Ҫ����������
RectLink *CreateRectLink(u16 num);

//�ø�����DRect��ʼ����������
//���룺head ����ͷָ�룬rect ��ʼ�����νṹ
u16 InitRectLink(RectLink *head, DRect rect);

//��������
u16 DeRectLink(RectLink *head);

//��ʾ�õ��ľ�������
//���룺 rlink ����ͷָ��
//���������printf()�������
void ShowRectLink(RectLink *rlink);

//���Է�����ַ����ڵľ�������
//���룺Src ͼ������ָ�룬srcHeight ͼ��߶ȣ�srcWidth ͼ����
//������õ��ľ��νṹ
DRect DetectRect(u8 **Src, u16 srcHeight, u16 srcWidth);

//��б�Ƚ���
//���룺Dst ��Ž������ͼ�����飬Src ��������ͼ������ָ�룬srcHeight ͼ��߶ȣ�srcWidth ͼ����
void SlopeAdjust(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth);

//��һ�������ַ���ָ����͸�
//���룺Dst ��ųߴ��һ����ͼ�����飬Src ����һ��ͼ������ָ�룬srcHeight ͼ��߶ȣ�srcWidth ͼ����
//rlink ��Src����õ��ַ���������num ͼ�������ֵĸ���
u16 StdAlignImg(u8 **Dst, u8 **Src, u16 dstHeight, u16 dstWidth, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//����ͼ�񣬴�Դͼ��ľ������򿽱���Ŀ��ͼ��ľ�������
//���룺Dst ���Ŀ��ͼ��Src ���Դͼ��dstRect Ŀ��ͼ���еľ�������srcRect Դͼ���еľ�������
u16 CopyImg(u8 **Dst, u8 **Src, DRect dstRect, DRect srcRect);

//������ȡ��13������
//���룺tz ��������ľ���Src ����ȡ��ͼ�����飬srcHeight ͼ��߶ȣ�srcWidth ͼ����
//rlink ��Src����õ��ַ���������num ͼ�������ֵĸ���
u16 TZTQ13(u8 **tz, u8 **Src, u16 srcHeight, u16 srcWidth, RectLink *rlink, u16 num);

//����������ַ����ڵľ��Σ������ھ��νṹ����rlink��
//���룺Src ����ȡ��ͼ��srcHeight ͼ��߶ȣ�srcWidth ͼ����
//rlink ��Src����õ��ַ���������num ͼ�������ֵĸ���
void DetectNum(u8 **Src, u16 srcHeight, u16 srcWidth , RectLink *rectlink, u16 num);

//��ͼ���࣬0���1,1���0����ͼ����CR-0֮�䷴�࣬�궨��CR���޸�
//���룺Dst Ŀ��ͼ��Src Դͼ��srcHeight ͼ��߶ȣ�srcWidth ͼ����
void InvertImg(u8 **Dst, u8 **Src , u16 srcHeight, u16 srcWidth);

//��ͼ�������ֵȫ����Ϊtag
//���룺Dst Ŀ��ͼ��Src Դͼ��srcHeight ͼ��߶ȣ�srcWidth ͼ���ȣ�tag Ҫ�õ�ֵ
void SetImg(u8 **Dst, u16 srcHeight, u16 srcWidth, u8 tag);

//��ͼ���ֵ��
//���룺Dst Ŀ��ͼ��Src Դͼ��srcHeight ͼ��߶ȣ�srcWidth ͼ���ȣ�thres ��ֵ
void BinaryImg(u8 **Dst, u8 **Src, u16 srcHeight, u16 srcWidth, u8 thres);

//����ͼ���ȫ����ֵ
//���룺img ������ĻҶ�ͼ��height ͼ��߶ȣ�width ͼ����
//�������ֵ��u8 �ͣ�
u8 GlobalThreshold(u8 **img, u16 height, u16 width);

//OSTU��ͼ�����ֵ
//���룺img ������ĻҶ�ͼ��height ͼ��߶ȣ�width ͼ����
//�������ֵ��u8 �ͣ�
u8 otsuThreshold(u8 **img, u16 height, u16 width);

//����䣬�п��ܻ�����µġ��ס�
//���룺img �����ĻҶ�ͼ��height ͼ��߶ȣ�width ͼ����
u16 floodfill(u8 **img, u16 height, u16 width);


//ϸ��ͼ������ʹ���������Ҹ����ԣ�Ҫ��ǰ��ɫΪ1������ɫΪ0 ,lx:heighe,ly:width
//���룺image ͼ�����飬lx ͼ��߶ȣ�ly ͼ����
void ThinnerRosenfeld(u8 **image, u16 lx, u16 ly);

#endif

