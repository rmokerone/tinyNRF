/**********************************************************************
	> File Name: nrf24l01.c
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Fri 13 Jun 2014 02:43:35 PM CST
 *********************************************************************/

#include "nrf24l01.h"

/*
 * 进入power-down模式
 */
void powerDown(void)
{
    CE_CLR;
    spiRwReg (WRITE_REG + CONFIG, 0x0D);
    CE_EN;
    _delay_us(20);
}

/*
 * 进入到Tx发送模式
 */
void enterTxMode(uchar * txAddr, volatile uchar* txBuf)
{
    powerDown();

    CE_CLR;

    spiWriteBuf(WRITE_REG + TX_ADDR, txAddr, TX_ADR_WIDTH);
    spiWriteBuf(WRITE_REG + RX_ADDR_P0, txAddr, TX_ADR_WIDTH);
    spiWriteBuf(WR_TX_PLOAD, (uchar *)txBuf, TX_PLOAD_WIDTH);
    
    spiRwReg(WRITE_REG + EN_AA, 0x01);
    spiRwReg(WRITE_REG + EN_RXADDR, 0x01);
    spiRwReg(WRITE_REG + SETUP_RETR, 0x01);
    spiRwReg(WRITE_REG + RF_CH, 40);
    spiRwReg(WRITE_REG + RF_SETUP, 0x07);
    spiRwReg(WRITE_REG + CONFIG, 0x0e);

    CE_EN;
}

/*
 * 进入到Rx接收模式
 */
void enterRxMode(uchar* rxAddr)
{
    powerDown();

    CE_CLR;

    spiWriteBuf(WRITE_REG + RX_ADDR_P0, rxAddr, TX_ADR_WIDTH);

    spiRwReg(WRITE_REG + EN_AA, 0x01);
    spiRwReg(WRITE_REG + EN_RXADDR, 0x01);
    spiRwReg(WRITE_REG + RF_CH, 40);
    spiRwReg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);
    spiRwReg(WRITE_REG + RF_SETUP, 0x07);
    spiRwReg(WRITE_REG + CONFIG, 0x0f);

    CE_EN;
}

/*
 * 清空TX寄存器或RX寄存器
 * 当RT = 1时清空TX
 * 当RT = 0时清空RX 
 */
void flushRT(uchar RT)
{
    CSN_CLR;

    if (RT == 1)
        spiRw(FLUSH_TX);
    else 
        spiRw(FLUSH_RX);

    CSN_EN;
}

/*
 * 清空所有寄存器中的数值
 */
void flushAll(void)
{
    flushRT(0);
    flushRT(1);
    //写清空STATUS寄存器
    spiRwReg(WRITE_REG + STATUS, 0xff);
}

/*
 * 将数据接收地址设置rxAddr, 并将接收到的数据放入rxBuf中
 * 若成功返回1 
 * 若失败返回0
 */
uchar nrfRecv(uchar* rxAddr, uchar* rxBuf)
{
    uchar rxSta, ret;

    enterRxMode(rxAddr);
    //延时等待400us
    _delay_us(400);
    rxSta = spiRw (STATUS);
    spiRwReg(WRITE_REG + STATUS, 0xff);

    if (rxSta & STA_MARK_RX)
    {
        spiWriteBuf (RD_RX_PLOAD, rxBuf, TX_PLOAD_WIDTH);
        ret = 1;
    }
    else
        ret = 0;

    flushAll();
    return ret;
}

/*
 * 将数据发送地址设置为txAddr,并将txBuf中的数据发送出去
 * 若成功返回1
 * 若失败返回0
 */
uchar nrfSend(uchar* txAddr, volatile uchar* txBuf)
{
    uchar txSta, ret;
    uint iCnt = 0;
    enterTxMode(txAddr, txBuf);

    //接收到数据发送完成中断
    //或者是超过一定的查询次数
    while ((PIND * (1<<IRQ)) || (iCnt ++ > 2000));

    txSta = spiRw(STATUS);
    spiRwReg(WRITE_REG + STATUS, 0xff);

    if (txSta & STA_MARK_TX)
    {
        flushAll();
        ret = 1;
    }
        ret = 0;
    return ret;
}

/*
 * 读取相关寄存器判断模块是否工作正常
 */
void checkId(void)
{
    uchar tmpCnt;
    uchar p0Addr[TX_ADR_WIDTH];
    uchar p1Addr[TX_ADR_WIDTH];
    uchar txAddr[TX_ADR_WIDTH];

    for (tmpCnt = 0; tmpCnt < TX_ADR_WIDTH; tmpCnt ++)
    {
        p0Addr[tmpCnt] = 0;
        p1Addr[tmpCnt] = 0;
        txAddr[tmpCnt] = 0;
    }

    printf ("Checking ID ...\n");
    
    powerDown();

    CE_CLR;
    //这三个寄存器的数值在本芯片中不是固定的
    //所以不能够用来验证NRF芯片是否工作正常
    spiReadBuf (READ_REG + RX_ADDR_P0, p0Addr, TX_ADR_WIDTH);
    spiReadBuf (READ_REG + RX_ADDR_P1, p1Addr, TX_ADR_WIDTH);
    spiReadBuf (READ_REG + TX_ADDR, txAddr, TX_ADR_WIDTH);

    CE_EN;

    printf ("RX_ADDR_P0 = ");
    printAddr (p0Addr);
    printf ("RX_ADDR_P1 = ");
    printAddr (p1Addr);
    printf ("TX_ADDR = ");
    printAddr (txAddr);
}
