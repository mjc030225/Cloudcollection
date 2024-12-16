/*******************************************
*	�ļ���tickTask
*	������ʱ��Ƭ��������ʼ��
*		void tickTaskInit(u8 SYSCLK)��SysTick������100usʱ�����
*		void wDelay(u32 nus),��ʱ
********************************************/

#include "includes.h"
 
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
//1000us�δ�
void tickTaskInit(u8 SYSCLK){
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);								//ʱ��ԴSYSCLK/8
	SysTick->LOAD=1000*SYSCLK/8; 																				//100usʱ�����	  		 
	SysTick->VAL=0x00;        																					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_TICKINT_Msk ; 	//��ʼ����/�ж�ʹ�� 	
}

void wDelay(u16 Time){
	msTime=Time;
	while(msTime--){};
}

void msDelay(u16 nms){	 	 
	u32 i;	 	
	i=1000*nms;
	while(i--){};
}
