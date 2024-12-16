/*****************************************************
*文件：w_usart.c
*描述：usart1直接发送，DMA 接收
*      IO:		PA9/PA10
*      uart1:	115200,8，1，无校验
*	   	 DMARx：	DMA2_Stream2 DMA_Channel_4
*			 usart1支持printf
*****************************************************/

#include "includes.h"


//**************************************************************
//加入以下代码,支持 printf 函数,而不需要选择 use MicroLIB
#if 1
#pragma import(__use_no_semihosting) 
//标准库需要的支持函数 
struct __FILE { 
	int handle; 
}; 
FILE __stdout; 
//定义_sys_exit()以避免使用半主机模式 
void _sys_exit(int x) { 
	x = x; 
} 
//重定义 fputc 函数
int fputc(int ch, FILE *f)	{ 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){}; 
	USART_SendData(USART1,(uint8_t)ch); 
	return ch;
}
#endif
//****************************************************************

void wUart1Init(void){

	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							//使能USART1时钟
 
	//USART1端口配置
	wIOCfg(xGPIOA,PIN9|PIN10,xAf,xOut_PP,x50M,xUp);
	wIOCfg(xGPIOA,PIN11,xOut,xOut_PP,x50M,xUp);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 						//GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 						//GPIOA10复用为USART1
	
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;																			//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;																//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//收发模式
	USART_Init(USART1, &USART_InitStructure); 																				//初始化串口1
	
	USART_Cmd(USART1, ENABLE);  																											//使能串口1	

	//开启相关中断
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		

//	wUart1DMAInit();

}

void wUart1DMAInit(void)	{
    DMA_InitTypeDef DMA_InitStructure;
	
    Rx1Counter= 0;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);    											/* 1.使能DMA2时钟 */   
	//串口收DMA配置  
    DMA_DeInit(DMA2_Stream2); 																										/* 2.配置使用DMA接收数据 */

    DMA_InitStructure.DMA_Channel             = DMA_Channel_4;               			/* 配置DMA通道 */
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(USART1->DR));   			/* 源 */
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)Rx1DMAData;          		/* 目的 */
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralToMemory;    		/* 方向 */
    DMA_InitStructure.DMA_BufferSize          = RX_BUF_LEN;                    		/* 长度 */                  
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    		/* 外设地址是否自增 */
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         		/* 内存地址是否自增 */
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_MemoryDataSize_Byte;      		/* 目的数据带宽 */
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;      		/* 源数据宽度 */
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Circular;             		/* 单次传输模式/循环传输模式 */
    DMA_InitStructure.DMA_Priority            = DMA_Priority_VeryHigh;        		/* DMA优先级 */
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;          		/* FIFO模式/直接模式 */
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull; 			/* FIFO大小 */
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;       		/* 单次传输 */
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;
    
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);																		/* 3. 配置DMA */
																																									/* 4.由于接收不需要DMA中断，故不设置DMA中断 */
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); 																	/* 5.使能串口的DMA接收 */
		
		DMA_Cmd(DMA2_Stream2,ENABLE);																									/* 7.使能DMA */
	
}
 
void wTx1Send(u8 *buf,u8 size)	{
	u8 i;
	
	for(i=0;i<size;i++) {																						//循环发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){}; 		//等待发送结束
		USART_SendData(USART1,buf[i]); 																//发送数据
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){}; 			//等待发送结束

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


