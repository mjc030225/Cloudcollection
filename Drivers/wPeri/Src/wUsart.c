/*****************************************************
*�ļ���w_usart.c
*������usart1ֱ�ӷ��ͣ�DMA ����
*      IO:		PA9/PA10
*      uart1:	115200,8��1����У��
*	   	 DMARx��	DMA2_Stream2 DMA_Channel_4
*			 usart1֧��printf
*****************************************************/

#include "includes.h"


//**************************************************************
//�������´���,֧�� printf ����,������Ҫѡ�� use MicroLIB
#if 1
#pragma import(__use_no_semihosting) 
//��׼����Ҫ��֧�ֺ��� 
struct __FILE { 
	int handle; 
}; 
FILE __stdout; 
//����_sys_exit()�Ա���ʹ�ð�����ģʽ 
void _sys_exit(int x) { 
	x = x; 
} 
//�ض��� fputc ����
int fputc(int ch, FILE *f)	{ 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){}; 
	USART_SendData(USART1,(uint8_t)ch); 
	return ch;
}
#endif
//****************************************************************

void wUart1Init(void){

	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							//ʹ��USART1ʱ��
 
	//USART1�˿�����
	wIOCfg(xGPIOA,PIN9|PIN10,xAf,xOut_PP,x50M,xUp);
	wIOCfg(xGPIOA,PIN11,xOut,xOut_PP,x50M,xUp);
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 						//GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 						//GPIOA10����ΪUSART1
	
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;																			//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;																//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); 																				//��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  																											//ʹ�ܴ���1	

	//��������ж�
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		

//	wUart1DMAInit();

}

void wUart1DMAInit(void)	{
    DMA_InitTypeDef DMA_InitStructure;
	
    Rx1Counter= 0;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);    											/* 1.ʹ��DMA2ʱ�� */   
	//������DMA����  
    DMA_DeInit(DMA2_Stream2); 																										/* 2.����ʹ��DMA�������� */

    DMA_InitStructure.DMA_Channel             = DMA_Channel_4;               			/* ����DMAͨ�� */
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(USART1->DR));   			/* Դ */
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)Rx1DMAData;          		/* Ŀ�� */
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralToMemory;    		/* ���� */
    DMA_InitStructure.DMA_BufferSize          = RX_BUF_LEN;                    		/* ���� */                  
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    		/* �����ַ�Ƿ����� */
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         		/* �ڴ��ַ�Ƿ����� */
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_MemoryDataSize_Byte;      		/* Ŀ�����ݴ��� */
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;      		/* Դ���ݿ�� */
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Circular;             		/* ���δ���ģʽ/ѭ������ģʽ */
    DMA_InitStructure.DMA_Priority            = DMA_Priority_VeryHigh;        		/* DMA���ȼ� */
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;          		/* FIFOģʽ/ֱ��ģʽ */
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull; 			/* FIFO��С */
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;       		/* ���δ��� */
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;
    
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);																		/* 3. ����DMA */
																																									/* 4.���ڽ��ղ���ҪDMA�жϣ��ʲ�����DMA�ж� */
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); 																	/* 5.ʹ�ܴ��ڵ�DMA���� */
		
		DMA_Cmd(DMA2_Stream2,ENABLE);																									/* 7.ʹ��DMA */
	
}
 
void wTx1Send(u8 *buf,u8 size)	{
	u8 i;
	
	for(i=0;i<size;i++) {																						//ѭ����������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){}; 		//�ȴ����ͽ���
		USART_SendData(USART1,buf[i]); 																//��������
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){}; 			//�ȴ����ͽ���

}

void wUart1RxProcess(void)	{

	
	if((Rx1Buffer[0]==0xA5)&&(Rx1Buffer[4]==0x5A)){
		
		if(((Rx1Buffer[1]<<8)+Rx1Buffer[2])==BusId){
		
			if(Rx1Buffer[3]&0x01)		
					PCout(8)=1;
			else										
					PCout(8)=0;
			if(Rx1Buffer[3]&0x02)		
					PCout(9)=1;
			else										
					PCout(9)=0;
			if(Rx1Buffer[3]&0x04)		
					PAout(8)=1;
			else										
					PAout(8)=0;
		}
	}

}


