#ifndef __wUSART_H
#define __wUSART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"

#define	RX_BUF_LEN	32
#define	TX_BUF_LEN	32

EXT u8 Tx1Buffer[TX_BUF_LEN]; 				//定义串口1发送缓存
EXT u8 Rx1Buffer[RX_BUF_LEN];				//定义串口1接收缓存
EXT u8 Tx1DMAData[TX_BUF_LEN];				//DMA发送数据
EXT u8 Rx1DMAData[RX_BUF_LEN];				//DMA接收数据
EXT u8 Rx1Counter;
EXT u8 FlagUart1TxOK;						//接收到数据
EXT u8 FlagUart1RxOK;						//接收到数据


void wUart1Init(void);
void wUart1DMAInit(void);
void wTx1Send(u8 *buf,u8 size);
void wUart1RxProcess(void);

#endif


