/*uart.h*/
#ifndef __UART_H__
#define __UART_H__

//设置波特率
#define BAUD 9600

//定义晶振频率
#define CRYSTAL 8000000

//计算波特率要设置的数值
#define BAUD_SETTING (unsigned int)((unsigned long)CRYSTAL/(16*(unsigned long)BAUD)-1)

//UART初始化函数
void uartInit(unsigned int baud);

//UART发送函数
int uartPutChar (char c, FILE * stream);

//UART开启接收中断
unsigned char uartRxISR(void);

//UART软件查询接受数据函数
unsigned char uartRecvChar (void);

//定义UART接收到的数据类型为volatile (类型限定词）
volatile unsigned char uartRecvData;

#endif
