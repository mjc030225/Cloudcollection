/*********************************************
*�ļ���wIO.c
*������IO����/IO����
*********************************************/
#include "includes.h" 

//GPIO�������� 
//xgpio:xGPIOA~xGPIOI.
//pinx:��Χ:1<<0 ~ 16,λ����,ÿ��λ����һ��IO.��ͬʱ���ö��IO.
//iomode:�������ģʽ. xIn��0�����룬xOut: 1,ͨ�������xAf��2,���ã�xAin��3,ģ��
//ioOtype��xOut_PP:0,������죬xOut_OD��1,�����©
//iospeed: x2M:0,2MHz;x25M��1,25MHz;x50M:2,50MHz;x100M:3,100MHz
//iopupd:	xNone:0,xUp:1,xDown:2
void wIOCfg(u8 xgpio,u32 pinx,u32 iomode,u32 iootype,u32 iospeed,u32 iopupd)	{  
	u32 pinpos=0,pos=0,curpin=0;
	GPIO_TypeDef* gpiox;
	
	switch(xgpio){
		case 0:	gpiox=GPIOA ;RCC->AHB1ENR|=1<<0;break;
		case 1: gpiox=GPIOB ;RCC->AHB1ENR|=1<<1;break;
		case 2:	gpiox=GPIOC ;RCC->AHB1ENR|=1<<2;break;
		case 3: gpiox=GPIOD ;RCC->AHB1ENR|=1<<3;break;
		case 4: gpiox=GPIOE ;RCC->AHB1ENR|=1<<4;break;
		case 5: gpiox=GPIOF ;RCC->AHB1ENR|=1<<5;break;
		case 6: gpiox=GPIOG ;RCC->AHB1ENR|=1<<6;break;
		case 7: gpiox=GPIOH ;RCC->AHB1ENR|=1<<7;break;
		case 8: gpiox=GPIOI ;RCC->AHB1ENR|=1<<8;break;
		default:break;
	}
	
	for(pinpos=0;pinpos<16;pinpos++)	{
		pos=1<<pinpos;										//һ����λ��� 
		curpin=pinx&pos;									//��������Ƿ�Ҫ����
		if(curpin==pos) {									//��Ҫ����
			gpiox->MODER&=~(3<<(pinpos*2)); 				//�����ԭ��������
			gpiox->MODER|=iomode<<(pinpos*2); 				//�����µ�ģʽ
			if((iomode==0X01)||(iomode==0X02)) 				//��������ģʽ/���ù���ģʽ
			{
				gpiox->OSPEEDR&=~(3<<(pinpos*2)); 			//���ԭ��������
				gpiox->OSPEEDR|=(iospeed<<(pinpos*2));		//�����µ��ٶ�ֵ
				gpiox->OTYPER&=~(1<<pinpos) ; 				//���ԭ��������
				gpiox->OTYPER|=iootype<<pinpos; 			//�����µ����ģʽ
			}
			gpiox->PUPDR&=~(3<<(pinpos*2)); 				//�����ԭ��������
			gpiox->PUPDR|=iopupd<<(pinpos*2); 				//�����µ�������
		}	
	}	
}

