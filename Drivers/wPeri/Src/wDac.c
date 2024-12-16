/*********************************************
*文件：wDac.c
*描述：DAC输出
*      IO:		PA4
*	   DMA：	
*********************************************/

#include "includes.h"

void wDACInit(void)
{
			
	DAC_InitTypeDef DAC_InitType;
//	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	wIOCfg(xGPIOA,PIN4,xAin,0,0,xDown);
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;//不使用触发
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生器
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;//输出缓存关闭

	DAC_Init(DAC_Channel_1,&DAC_InitType); //初始化DAC通道1
	DAC_Cmd(DAC_Channel_1, ENABLE); 	   //使能DAC通道1
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);//12位右对齐
}
