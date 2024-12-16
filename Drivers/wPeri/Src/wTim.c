/*******************************************
*	文件：wTim
*	描述：Tim3,采样时钟，频率：84MHz/13125=6.4KHz 
********************************************/

#include "includes.h"

void wTim3Init(void){
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  										//APB1，84MHz
	
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0;  														//预分频
	TIM_TimeBaseInitStructure.TIM_Period = 13125-1; 												//周期
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;						//上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 							//分频
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);										//更新溢出向外触发
	TIM_Cmd(TIM3,ENABLE); 																									//启动TIM3
	
}

