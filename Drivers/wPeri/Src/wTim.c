/*******************************************
*	�ļ���wTim
*	������Tim3,����ʱ�ӣ�Ƶ�ʣ�84MHz/13125=6.4KHz 
********************************************/

#include "includes.h"

void wTim3Init(void){
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  										//APB1��84MHz
	
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0;  														//Ԥ��Ƶ
	TIM_TimeBaseInitStructure.TIM_Period = 13125-1; 												//����
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;						//�ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 							//��Ƶ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);										//����������ⴥ��
	TIM_Cmd(TIM3,ENABLE); 																									//����TIM3
	
}

