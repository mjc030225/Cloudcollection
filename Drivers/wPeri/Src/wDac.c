/*********************************************
*�ļ���wDac.c
*������DAC���
*      IO:		PA4
*	   DMA��	
*********************************************/

#include "includes.h"

void wDACInit(void)
{
			
	DAC_InitTypeDef DAC_InitType;
//	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	wIOCfg(xGPIOA,PIN4,xAin,0,0,xDown);
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;//��ʹ�ô���
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η�����
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;//�������ر�

	DAC_Init(DAC_Channel_1,&DAC_InitType); //��ʼ��DACͨ��1
	DAC_Cmd(DAC_Channel_1, ENABLE); 	   //ʹ��DACͨ��1
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);//12λ�Ҷ���
}
