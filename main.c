//nrf24l01主程序
#include "main.h"
#include "uart.h"
#include "nrf24l01.h"

//常用TX数据接收地址
uchar TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};

static FILE mystdout = FDEV_SETUP_STREAM (uartPutchar, NULL, _FDEV_SETUP_WRITE);

/*
 * stdout 输出终端初始化
 */
void termialInit(void);
/*
 * tinyNrf LOGO OUTPUT
 */
void logoPrint(void);

/*
 * update address 
 */
void updateAddr(uchar* souAddr);

/*
 * printf MAC address 
 */
void printAddr(uchar * souAddr);

int main (void)
{
    uchar adCnt,scanRet = 0;
    //定义数据接收和发送缓冲区
    //uchar txBuf[TX_PLOAD_WIDTH];
    uchar rxBuf[TX_PLOAD_WIDTH];
    //定义数据发送地址
    uchar txAddr[TX_ADR_WIDTH];
    //初始化发送地址
    for (adCnt = 0; adCnt < TX_ADR_WIDTH; adCnt ++)
        txAddr[adCnt] = TX_ADDRESS[adCnt];
    //初始化打印终端
    termialInit();
    //spi模块初始化
    spiInit();
    //输出logo
    logoPrint();

    //扫描可用的终端
    printf ("Scaning enable address...\n");
    //如果扫描成功则一直扫描
    while (!scanRet)
    {
        printf ("Testing address = ");
        printAddr (txAddr);
        scanRet = nrfRecv(txAddr, rxBuf);
        //printf ("log: scanRet = %d\n", scanRet);
        checkId();
        //update Address of txaddr
        updateAddr(txAddr);
        printf ("Testing address end!!!\n");
    }
    printf ("Find address =");
    printAddr (txAddr);

    return 0;
}

/*
 * printf MAC address 
 */
void printAddr(uchar * souAddr)
{
    uchar tmpByte, bitCnt;
    for (bitCnt = 0; bitCnt < TX_ADR_WIDTH; bitCnt ++)
    {
        tmpByte = *souAddr ++;
        printf ("0x%x", tmpByte);
        if (bitCnt < TX_ADR_WIDTH - 1)
            printf (":");
    }
    printf ("\n");
}


/*
 * update address 
 */
void updateAddr(uchar* souAddr)
{
    uchar tmpByte;
    if (souAddr[TX_ADR_WIDTH - 1] == 0xff)
        souAddr[TX_ADR_WIDTH - 1] = 0x00;
    else
    {
        tmpByte = souAddr[TX_ADR_WIDTH - 1];
        souAddr[TX_ADR_WIDTH - 1] = tmpByte + 1;
    }
    //printf ("After Update address = ");
    //printAddr (souAddr);
}


/*
 * stdout 输出终端初始化
 */
void termialInit(void)
{
    //设置波特率为9600
    uartInit (BAUD_SETTING);
    stdout = &mystdout;
}

/*
 * tinyNrf LOGO OUTPUT
 */
void logoPrint(void)
{
    printf ("TINYNRF Version 0.1V\n");
    printf ("Make by Moker\n");
    printf ("Welcome use tinynrf\n");
}

