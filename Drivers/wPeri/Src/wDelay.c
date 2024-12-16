/*******************************************
*	�ļ���w_delay
*	������ϵͳ��ʱ����
*		w_delay_init(u8 SYSCLK)����ʱ������ʼ��
*		void w_delay_us(u32 nus),΢����ʱ
*		void w_delay_ms(u16 nms)��������ʱ
********************************************/

#include "includes.h"

 
static u8  fac_us=0;//us��ʱ������
			   
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void w_delay_init(u8 SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		    
}

//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ,��Ҫ����798915us
void w_delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ����� 
}
//��ʱnms
//ע��nms�ķ�Χ����168M������,nms<=798ms 
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
void w_delay_ms(u16 nms)
{	 	 
	u32 temp;	 	
	SysTick->LOAD=nms*1000*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ����� 
} 

			 



































