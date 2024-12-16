/******************************************************************************************************************
*	�ļ���main
*	������	1��USART1��9600����У��
*				2��Э�飺	A5 12 34 04	5A
*	ע�⣺ϵͳ��Ƶ��168MHz
********************************************************************************************************************/

#define GLOBALS

#include "includes.h"
uint8_t FlagCounterUpdated = 0;     //��־λ������ָʾ����ֵ�Ƿ����  
volatile uint16_t counter = 0;      //����ֵ
uint16_t global_count = 0;
void x_cpu_init(void) {

	  //����GPIOʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
    //PA15/PD2/PC13��������
		tickTaskInit(168);
	  wIOCfg(xGPIOA,PIN15,xIn,xOut_PP,x50M,xUp);		
	  wIOCfg(xGPIOC,PIN13,xIn,xOut_PP,x50M,xUp);
	  wIOCfg(xGPIOD,PIN2,xIn,xOut_PP,x50M,xUp);
	  //PA8/PC8/PC9->���
		wIOCfg(xGPIOA,PIN8,xOut,xOut_PP,x50M,xNone);		
	  wIOCfg(xGPIOC,PIN8|PIN9,xOut,xOut_PP,x50M,xNone);
	  wDelay(10);
	//   msDelay(10);
}


uint8_t inKey = 0;

void x_get_input(void) {

    uint8_t sw1State = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET ;
	uint8_t sw2State = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == RESET ;
    uint8_t sw3State = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == RESET ;
    
	//����ӳ��
	if(sw1State){
		  inKey = 1; 
	}else if(sw2State){
		  inKey = 2;
	}else if(sw3State){
	    inKey = 4;
	}else{

		inKey = 0;
	}
}


//��ʼ��UART��USART4, 9600bps,��У�飩
void USART4_Init(void) {
    // 1. ���� GPIOA �� USART4 ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // GPIOA ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); // USART4 ʱ��

	  //USART1�˿�����
	  wIOCfg(xGPIOA,PIN0|PIN1,xAf,xOut_PP,x50M,xUp);
	  ///wIOCfg(xGPIOA,PIN11,xOut,xOut_PP,x50M,xUp);
	
    // 2. ���� GPIOA ����
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;      // PA0 (TX) �� PA1 (RX)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;               // ���ù���
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;             // �������
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;               // ����
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;          // GPIO �ٶ�
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // �� PA0 �� PA1 ����Ϊ USART4 ����
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4); // PA0 -> USART4_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4); // PA1 -> USART4_RX

    // 3. ���� USART4
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 115200;               // ������
    USART_InitStruct.USART_WordLength = USART_WordLength_8b; // 8 λ����
    USART_InitStruct.USART_StopBits = USART_StopBits_1;      // 1 λֹͣλ
    USART_InitStruct.USART_Parity = USART_Parity_No;         // ��У��λ
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // ʹ�ܷ��ͺͽ���
    USART_Init(UART4, &USART_InitStruct);

    //��������ж�
	  USART_ClearITPendingBit(UART4, USART_IT_IDLE);
	  USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);		


    // 4. ���� USART4
    USART_Cmd(UART4, ENABLE);
}


//������ֵ�Զ�������ʽ����
void SendCounterBinary()  {
    	u8 SendDat1[8];
	
	    SendDat1[0]=0xA5;
	    SendDat1[1]=0x5A;
	    SendDat1[2]=0x05;
	    SendDat1[3]=0x82;
	    SendDat1[4]=0x00;
	    SendDat1[5]=0x04;
	    SendDat1[6]=counter>>8;
	    SendDat1[7]=counter&0xff;
	    wTx1Send(SendDat1, 8);
}



//������ֵ��ʮ������ʽ����
void SendCounterDecimal(){
	u8 SendDat2[16];
	// u8 decStr[6];
	// u8 len;
	
	// sprintf(decStr,"%d", counter);
	
	SendDat2[0]=0xA5;
    SendDat2[1]=0x5A;
	SendDat2[2]=0x05;
    SendDat2[3]=0x82;
	SendDat2[4]=0x00;
	SendDat2[5]=0x00;
	SendDat2[6]=counter>>8;
	SendDat2[7]=counter&0xff;
	wTx1Send(SendDat2, 8);
}



// �����߼�����
void handle_counter(void) {
    static uint16_t holdTime = 0;  // ����ʱ�����
    static uint8_t prevKey = 0;   // ��һ�ΰ���״̬

    x_get_input();  // ��ȡ����״̬
	counter += 1; 
    if (inKey == 1) {  // ����SW1
        if (prevKey != 1) {  // �̰�
            counter = (counter + 1) % 256;
					  wDelay(10000);
					  FlagCounterUpdated = 1;
            holdTime = 0;
        } else {  // ����
            holdTime++;
            if (holdTime >= 20) {  // ÿ20������+1
                counter = (counter + 1) % 256;
							  wDelay(5000);
							  FlagCounterUpdated = 1;
                holdTime = 18;  // ��ֹ��������
            }
        }
				
				if(FlagCounterUpdated){
					SendCounterBinary();
					SendCounterDecimal();
					FlagCounterUpdated = 0;
				}
    } else if (inKey == 2) {  // ����SW2
        if (prevKey != 2) {  // �̰�
					counter = (counter > 0) ? counter - 1:0;
					  wDelay(5000);
					  FlagCounterUpdated = 1;
            holdTime = 0;
        } else {  // ����
            holdTime++;
            if (holdTime >= 20) {  // ÿ20�����ڡ�1
                counter = (counter - 1) % 256;
							  wDelay(5000);
							  FlagCounterUpdated = 1;
                holdTime = 18;
            }
        }
				if(FlagCounterUpdated){
					SendCounterBinary();
					SendCounterDecimal();
					FlagCounterUpdated = 0;
				}
    } else if (inKey == 4) {  // ����SW3,����
        counter = 0;
			  FlagCounterUpdated = 1;
        holdTime = 0;
			
			  SendCounterBinary();
				SendCounterDecimal();
			
    } else {  // δ�����κΰ���
        holdTime = 0;
    }

    prevKey = inKey;  // ���°���״̬
		//��ʾ����ֵ��LED
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, (counter & 0x01)? Bit_SET :Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, (counter & 0x02)? Bit_SET :Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, (counter & 0x04)? Bit_SET :Bit_RESET);
		
}

int main(void)
{ 	
	x_cpu_init();	   //��ʼ��GPIO
	wUart1Init();
	wUart1DMAInit();
	USART4_Init();    //��ʼ��USART4
	SysTick_Config(SystemCoreClock /1000);    //��ʼ��SystickΪ1ms
	
	
  while(1){
    handle_counter();        //���������߼�
		
		  if(FlagUart1RxOK){
			wUart1RxProcess();
			FlagUart1RxOK=0;
		 }
		  
	}
}

