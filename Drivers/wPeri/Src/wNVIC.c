/*******************************************
*	文件：wNVIC
*	描述：系统中断分配
********************************************/

#include "includes.h"

void wNVICInit(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置系统中断优先级分组 2
	
	wNVICCfg(USART1_IRQn,1,1);											//Usart1 接收空闲中断配置
}

void wNVICCfg(u8 wIRQn,u8 wPreemption,u8 wSubp)	{	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = wIRQn;							//中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=wPreemption;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =wSubp;				//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
}
