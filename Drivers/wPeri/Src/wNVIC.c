/*******************************************
*	�ļ���wNVIC
*	������ϵͳ�жϷ���
********************************************/

#include "includes.h"

void wNVICInit(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����ϵͳ�ж����ȼ����� 2
	
	wNVICCfg(USART1_IRQn,1,1);											//Usart1 ���տ����ж�����
}

void wNVICCfg(u8 wIRQn,u8 wPreemption,u8 wSubp)	{	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = wIRQn;							//�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=wPreemption;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =wSubp;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}
