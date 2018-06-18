#ifndef __BAGAPP_H
#define __BAGAPP_H
#include "sys.h"
#include "WM.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin ����APP   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/21
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

extern WM_HWIN BagAPPWin;           //BagAPP����
extern WM_HWIN BookAPPWin;          //BookAPP����
extern WM_HWIN BookshAPPWin;        //BookshAPP����
extern WM_HWIN BrowerAPPWin;        //BrowerAPP����

void CreateBagAPP(void);            //����BagAPP
void CreateBookAPP(void);           //����BookAPP
void CreateBookshAPP(void);          //����BookshAPP
void CreateBrowerAPP(void);         //����BrowerAPP
typedef struct
{
    u8 name[20];    //����
    u8 num[20];     //����
    u8 unit[4];    //��λ
    u8 level;       //�ȼ�
}Medicine_info;
#endif
