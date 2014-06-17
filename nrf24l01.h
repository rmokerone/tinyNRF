/**********************************************************************
	> File Name: nrf24l01.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Fri 13 Jun 2014 02:41:39 PM CST
 *********************************************************************/

#ifndef __NRF24L01_H__
#define __NRF24L01_H__

//包含公用头文件
#include "main.h"
#include "spi.h"

extern void printAddr (uchar *);

#define TX_ADR_WIDTH    5   // 5  bytes TX(RX) address width

#define TX_PLOAD_WIDTH  32  // 32 bytes TX payload


#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address #define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define STA_MARK_RX     0X40
#define STA_MARK_TX     0X20
#define STA_MARK_MX     0X10	

/*
 * 进入power-down模式
 */
void powerDown(void);

/*
 * 进入到Tx发送模式
 */
void enterTxMode(uchar * txAddr, uchar* txBuf);

/*
 * 进入到Rx接收模式
 */
void enterRxMode(uchar* rxAddr);

/*
 * 清空TX寄存器或RX寄存器
 * 当RT = 1时清空TX
 * 当RT = 0时清空RX 
 */
void flushRT(uchar RT);

/*
 * 清空所有寄存器中的数值
 */
void flushAll(void);

/*
 * 将数据接收地址设置rxAddr, 并将接收到的数据放入rxBuf中
 * 若成功返回1 
 * 若失败返回0
 */
uchar nrfRecv(uchar* rxAddr, uchar* rxBuf);

/*
 * 将数据发送地址设置为txAddr,并将txBuf中的数据发送出去
 * 若成功返回1
 * 若失败返回0
 */
uchar nrfSend(uchar* txAddr, uchar* txBuf);

/*
 * 读取相关寄存器判断模块是否工作正常
 */
void checkId(void);

#endif
