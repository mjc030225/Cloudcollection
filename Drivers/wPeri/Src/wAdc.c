/*********************************************
*�ļ���wAdc.c
*������ADC1 DMA ����
*      IO:		PB1\PB0\PC5\PC4\PA7\PA6\PA5\PA4
*	   	 DMA��	DMA2_Stream0 DMA_Channel_0
*********************************************/

#include "includes.h"

void wADCIOInit(void){
	wIOCfg(xGPIOB,PIN1,xAin,0,0,xDown);
	wIOCfg(xGPIOB,PIN0,xAin,0,0,xDown);
	wIOCfg(xGPIOC,PIN5,xAin,0,0,xDown);
	wIOCfg(xGPIOC,PIN4,xAin,0,0,xDown);
	wIOCfg(xGPIOA,PIN7,xAin,0,0,xDown);
	wIOCfg(xGPIOA,PIN6,xAin,0,0,xDown);
	wIOCfg(xGPIOA,PIN5,xAin,0,0,xDown);
	wIOCfg(xGPIOA,PIN4,xAin,0,0,xDown);
}

void wADC1Init(void){	
	
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);                        		//ADC use APB2 BUS
	
	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;   					
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;											//HCLK=168MHz��APB2CLOCK=168/2=84MHz,ADC CLOCK<36MHz,84/4=21MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;					//�Ƕ���ģʽ������ģʽ�¿���
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_6Cycles;		//���ADC�����ǣ�����ADC֮�����ʱ������ADC����û������
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;          													//ɨ��ģʽ��������DISABLE			
	ADC_InitStructure.ADC_ContinuousConvMode =  DISABLE;														//�Ƿ�����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;					//T3_TGRO����
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;             							//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion =ADChNum;																	//��������
	ADC_Init(ADC1, &ADC_InitStructure);
	 
	//8ͨ������ʱ�䣺8*(12+3+6)/(84/4)=8us
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,2, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,3, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,4, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14,5, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,6, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,7, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,8, ADC_SampleTime_3Cycles);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);																//Դ���ݱ仯ʱ������DMA


	ADC_DMACmd(ADC1, ENABLE); 																								//ADC����DMAʹ��
}

void wDMA2Stream0Init(void){
	DMA_InitTypeDef       DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);															//DMA2ʱ��ʹ��
	
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;			  					//�������ַ��32λ����ADC1 DATA ADDRESS
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   							//DMA2_Stream Channel0���Ӷ�����裬��DMA_PeripheralInc_Enable
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;			//���ݿ��ȣ�HalfWord=>16bits
	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1In;		    							//DMA memory base address
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  												//�������ʱDMA_MemoryInc_Enable	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;							//HalfWord=>16bits	
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;	
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;													//DMA����
	DMA_InitStructure.DMA_BufferSize = SampleNum*ADChNum;														//
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  																	//����������DMA_Mode_Circular�����β�����DMA_Mode_Normal
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;															//DMA���ȼ�
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 													//FIFOģʽ/ֱ��ģʽ */     
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
}
void wADC1DMA2Trigger(void){
	
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	DMA_SetCurrDataCounter(DMA2_Stream0,SampleNum*ADChNum); 
	DMA_Cmd(DMA2_Stream0, ENABLE);
	ADC_Cmd(ADC1, ENABLE);	
}
void wADC1DMA2Init(void){
	wADCIOInit();
	wADC1Init();
	wDMA2Stream0Init();
}