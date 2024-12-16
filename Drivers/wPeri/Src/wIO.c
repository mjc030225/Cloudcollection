/*********************************************
*文件：wIO.c
*描述：IO定义/IO配置
*********************************************/
#include "includes.h" 

//GPIO引脚设置 
//xgpio:xGPIOA~xGPIOI.
//pinx:范围:1<<0 ~ 16,位设置,每个位代表一个IO.可同时设置多个IO.
//iomode:输入输出模式. xIn：0，输入，xOut: 1,通用输出，xAf：2,复用，xAin：3,模拟
//ioOtype：xOut_PP:0,输出推挽，xOut_OD：1,输出开漏
//iospeed: x2M:0,2MHz;x25M：1,25MHz;x50M:2,50MHz;x100M:3,100MHz
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
		pos=1<<pinpos;										//一个个位检查 
		curpin=pinx&pos;									//检查引脚是否要设置
		if(curpin==pos) {									//需要设置
			gpiox->MODER&=~(3<<(pinpos*2)); 				//先清除原来的设置
			gpiox->MODER|=iomode<<(pinpos*2); 				//设置新的模式
			if((iomode==0X01)||(iomode==0X02)) 				//如果是输出模式/复用功能模式
			{
				gpiox->OSPEEDR&=~(3<<(pinpos*2)); 			//清除原来的设置
				gpiox->OSPEEDR|=(iospeed<<(pinpos*2));		//设置新的速度值
				gpiox->OTYPER&=~(1<<pinpos) ; 				//清除原来的设置
				gpiox->OTYPER|=iootype<<pinpos; 			//设置新的输出模式
			}
			gpiox->PUPDR&=~(3<<(pinpos*2)); 				//先清除原来的设置
			gpiox->PUPDR|=iopupd<<(pinpos*2); 				//设置新的上下拉
		}	
	}	
}

