//nrf24l01主程序
#include "main.h"
#include "uart.h"
#include "nrf24l01.h"

//常用TX数据接收地址
uchar TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};

static FILE mystdout = FDEV_SETUP_STREAM (uartPutChar, NULL, _FDEV_SETUP_WRITE);

/*
 * stdout 输出终端初始化
 */
void termialInit(void);
/*
 * tinyNrf LOGO OUTPUT
 */
void logoPrint(void);

/*
 * printf MAC address 
 */
void printAddr(uchar * souAddr);

/*
 * 获取命令
 */
void getCommand(void);

/*
 * 输出命令提示信息
 */
void printCmdRe();

/*
 * 地址扫描模块
 */
uchar scanAddr(void);

//定义数据接收和发送缓冲区
//uchar txBuf[TX_PLOAD_WIDTH];
uchar rxBuf[TX_PLOAD_WIDTH];
//定义数据发送地址
uchar txAddr[TX_ADR_WIDTH];

int main (void)
{
    //定义状态返回值
    uchar ret;

    uchar adCnt;
    //初始化发送地址
    for (adCnt = 0; adCnt < TX_ADR_WIDTH; adCnt ++)
        txAddr[adCnt] = TX_ADDRESS[adCnt];

    //初始化打印终端
    termialInit();
    //开启硬件接收中断
    ret = uartRxISR();
    if (!ret)
        printf ("Error: enable Rx ISR error!\n");
    //spi模块初始化
    spiInit();
    //输出logo
    logoPrint();

    while (1)
    {
        //输出命令提示信息
        printCmdRe();
        //获取命令
        getCommand();
    }

    printf ("Find address =");
    printAddr (txAddr);

    return 0;
}

/*
 * 扫描可用的终端模块
 * 若在一轮中发现可用的终端后
 * 将txAddr更新为该地址 并返回值1
 * 若在一轮中没有发现可用的终端地址
 * 将txAddr设置为0x01 并返回值0
 */
 uchar scanAddr(void)
 {
     uchar  scanCnt;
    //扫描可用的终端
    printf ("Scaning enable address...\n");

    //如果扫描成功则一直扫描
    for (scanCnt = 0; scanCnt < 0xff; scanCnt ++)
     {
         txAddr [TX_ADR_WIDTH - 1] = scanCnt + 1;
         printf ("Testing address = ");
         printAddr (txAddr);
         if (nrfRecv(txAddr, rxBuf))
         {
             printf ("Found Address------");
             printAddr(txAddr);
             printf ("Find end!\n");
             return 1;
         }
     }
     txAddr[TX_ADR_WIDTH - 1] = 0x01;
     printf ("Find end!\n");
     return 0;
}

/*
 * 输出命令提示信息
 */
void printCmdRe()
{
    printf ("\n");
    printf ("c : Check Nrf24L01 moudle OK.\n");
    printf ("s : Scaning enable address.\n");
    printf ("t : Just do an Test.\n");
    printf ("\n");
}

/*
 * 获取命令
 */
void getCommand(void)
{
    while (uartRecvData == 0);
        //printf ("0x%x", uartRecvData);
    switch (uartRecvData)
    {
        //检测NRF模块是否正常
        case 'c':
            checkId();
            break;
        //扫描可用的终端地址
        case 's':
            scanAddr();
            checkId();
            break;
         //测试命令
        case 't':
            printf ("recv a command\n");
            break;
        default :
            printf ("ERROR COMMAND: 0x%x\n", uartRecvData);
    }

    //清空数据接收缓存
    uartRecvData = 0;
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

