/**********************************************************************
	> File Name: spi.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Fri 13 Jun 2014 08:34:19 AM CST
 *********************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

#include "main.h"

/*
 * NRF相关操作宏定义
 */
#define CSN_CLR         CLEARBIT(PORTB, CSN) //清零CSN
#define CSN_EN         SETBIT(PORTB, CSN)   //置位CSN

#define CE_CLR          CLEARBIT(PORTB, CE)  //清零CE 
#define CE_EN          SETBIT(PORTB, CE)    //置位CE 


/*
 * SPI初始化函数
 */
void spiInit(void);

/*
 * SPI读写函数 
 * 将字节byte写入并返回读取的值(字节)
 */
uchar spiRw (uchar byte);

/*
 * SPI写入寄存器函数
 * 将值value 写入到寄存器地址 reg 
 * 并返回读取到的状态
 */
uchar spiRwReg (uchar reg, uchar value);

/*
 * SPI读字节函数
 * 从寄存器地址 reg 中读取一个字节并返回
 */
uchar spiRead (uchar reg);

/*
 * SPI写入连续的字节
 * 向寄存器地址 reg 中写入 *pBuf 中的字节 bytes 个
 */
uchar spiWriteBuf (uchar reg, uchar *pBuf, uchar bytes);

#endif
