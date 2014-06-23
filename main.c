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
 * 获取进入TX或者是RX模式的命令
 */
void getRT(void);

/*
 * 输出命令提示信息
 */
 void printCmdRe(void);

/*
 * 输出输入或输出模式选择命令提示信息
 */
void printRTRe(void);

/*
 * 地址扫描模块
 */
uchar scanAddr(void);

/*
 * 设置发送或接收的数据地址
 */
uchar getAddr(void);

uchar mode = CMD_MODE;

int main (void)
{
    //定义状态返回值
    uchar ret;

    uchar adCnt;
    uchar iCnt;
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
        //进入命令模式(默认进入)
        if (mode == CMD_MODE)
        {
            //输出命令提示信息
            printCmdRe();
            //获取命令
            getCommand();
        }
        //进入正常模式
        else if (mode == NOMAL_MODE)
        {
            printf ("Enter NOMAL_MODE\n");
            //输出模式选择选项
            printRTRe();
            //等待接收选择的模式
            getRT();
        }
        //进入到数据发送模式
        else if (mode == TX_MODE)
        {
            //发送缓冲区中的数据
            if (txCnt != 0)
            {
                printf ("sendOne\n");
                nrfSend(txAddr, txBuf);
                txCnt = 0;
                for (iCnt = 0; iCnt < TX_PLOAD_WIDTH; iCnt ++)
                    txBuf[iCnt] = '\0';
            }
        }
        //进入到数据接收模式
        else if (mode == RX_MODE)
        {
            //printf ("Enter RX_MODE\n");
            //接收数据并放入到缓冲区rxBuf中
            nrfRecv(txAddr, rxBuf);
            //将rxBuf中的数据通过串口发送出去
            if (rxBuf[0] != '\0')
            {
                printf ("%s", rxBuf);
                for (iCnt = 0; iCnt < TX_PLOAD_WIDTH; iCnt++)
                    rxBuf[iCnt] = '\0';
            }
        }
        else
            printf ("ERROR: MODE SELECT ERROR!!!\n");
    }
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
         //如果接收到数据则跳出扫描程序
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
 * 输出输入或输出模式选择命令提示信息
 */
void printRTRe(void)
{
    printf ("\n");
    printf ("b : Back CMD MODE\n");
    printf ("r : Enter RX MODE\n");
    //printf ("s : Set Rx or Tx address\n");
    printf ("t : Enter TX MODE\n");
    printf ("\n");
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
    printf ("j : Jump commond mode, enter nomal mode.\n");
    printf ("\n");
}

/*
 * 获取进入TX或者是RX模式的命令
 */
void getRT(void)
{
    while (uartRecvData == '\0');

    switch (uartRecvData)
    {
        case 'b':
            mode = CMD_MODE;
            break;
        case 't':
            while (!getAddr());
            mode = TX_MODE;
            break;
        case 'r':
            while (!getAddr());
            mode = RX_MODE;
            break;
        default:
            printf ("ERROR: command error\n");
    }
    uartRecvData = '\0';
}

/*
 * 设置发送或接收的数据地址 
 * 此函数以后必定重写，写的太难受了。。。
 * 里面还存在问题，在输入的值超过输入返回内后
 * 返回值是将原来的值/255后再设置为通道的地址
 * 存在问题
 * 获取数据成功返回1
 * 获取数据失败返回0
 */
uchar getAddr(void)
{
    //接收到的通道数据十进制
    uchar numTmp = 0, loopCnt = 0;

    printf ("Please Enter your chose Address (0~255)\n");
    printf ("Please Enter : end of input\n");
    
    //清空UART数据接收缓存
    uartRecvData = '\0';
    
    for (loopCnt = 0; loopCnt < 4; loopCnt ++)
    {
        while (uartRecvData == '\0');
        if (uartRecvData == ':')
        {
            txAddr[TX_ADR_WIDTH - 1] = numTmp;
            printf ("SUCCESS Set address: ");
            printAddr(txAddr);
            return 1;
        }
        numTmp = numTmp * 10 + (uartRecvData - 48);
        uartRecvData = '\0';
    }
    printf ("Enter address error.\n");
    printf ("Please input address again.\n");
    return 0;
}

/*
 * 获取命令
 */
void getCommand(void)
{
    while (uartRecvData == '\0');
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
            //checkId();
            break;
         //测试命令
        case 't':
            printf ("recv a command\n");
            break;
        case 'j':
            mode = NOMAL_MODE;
            break;
        default :
            printf ("ERROR COMMAND: 0x%x\n", uartRecvData);
    }

    //清空数据接收缓存
    uartRecvData = '\0';
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

