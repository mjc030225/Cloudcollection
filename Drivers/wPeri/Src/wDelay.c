/*******************************************
*	文件：w_delay
*	描述：系统延时函数
*		w_delay_init(u8 SYSCLK)，延时函数初始化
*		void w_delay_us(u32 nus),微妙延时
*		void w_delay_ms(u16 nms)，毫秒延时
********************************************/

#include "includes.h"

 
static u8  fac_us=0;//us延时倍乘数
			   
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void w_delay_init(u8 SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		    
}

//延时nus
//nus为要延时的us数.	
//注意:nus的值,不要大于798915us
void w_delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
	
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器 
}
//延时nms
//注意nms的范围，对168M条件下,nms<=798ms 
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
void w_delay_ms(u16 nms)
{	 	 
	u32 temp;	 	
	SysTick->LOAD=nms*1000*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
	
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器 
} 

			 



































