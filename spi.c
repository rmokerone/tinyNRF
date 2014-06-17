/**********************************************************************
	> File Name: spi.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Fri 13 Jun 2014 08:40:15 AM CST
 *********************************************************************/

#include "spi.h"


/*
 * SPI初始化函数
 */
void spiInit(void)
{
    DDRB |= (1<<SCK) | (1<<MOSI) | (1<<CSN) | (1<<CE);
    SPCR |= (1<<SPE) | (1<<MSTR);
}

/*
 * SPI读写函数 
 * 将字节byte写入并返回读取的值(字节)
 */
uchar spiRw (uchar byte)
{
    SPDR = byte;
    while (!(SPSR & (1<<SPIF)));
    return SPDR;
}

/*
 * SPI写入寄存器函数
 * 将值value 写入到寄存器地址 reg 
 * 并返回读取到的状态
 */
uchar spiRwReg (uchar reg, uchar value)
{
    uchar status;

    CSN_CLR;                //CSN low, init SPI transaction
    status = spiRw (reg);   //select register
    spiRw(value);           //write value to it
    CSN_EN;                 //CSN high again

    return (status);        //return Nrf24L01 status byte
}

/*
 * SPI读字节函数
 * 从寄存器地址 reg 中读取一个字节并返回
 */
uchar spiRead (uchar reg)
{
    uchar regVal;

    CSN_CLR;                //CSN low, init SPI transaction
    spiRw (reg);            //select register
    regVal = spiRw (0);     // read value 
    CSN_EN;                 //CSN high, terminate SPI communication

    return (regVal);        //return register value 
}

/*
 * SPI写入连续的字节
 * 向寄存器地址 reg 中写入 *pBuf 中的字节 bytes 个
 */
uchar spiWriteBuf (uchar reg, uchar *pBuf, uchar bytes)
{
    uchar status, byte_ctr;

    CSN_CLR;
    status = spiRw (reg);
    for (byte_ctr = 0; byte_ctr < bytes; byte_ctr ++)
        spiRw(*pBuf ++);
    CSN_EN;
    
    return (status);
}

/*
 * SPI读取连续的字节
 * 返回读取到的状态值
 */
uchar spiReadBuf (uchar reg, uchar *pBuf, uchar bytes)
{
    uchar status, bytesCnt;

    CSN_CLR;
    status = spiRw(reg);
    for (bytesCnt = 0; bytesCnt < bytes; bytesCnt ++)
    {
        *pBuf ++ = spiRw(0xff);
    }
    CSN_EN;
    return status;
}
