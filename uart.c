#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"

//UART初始化函数
void uartInit(unsigned int baud)
{
    DDRD |= (1 << PD1);
    PORTD |= (1 << PD0) | (1 << PD1);

    UBRR0H = (unsigned char)(baud >> 8);
    UBRR0L = (unsigned char) baud;
    UCSR0A = 0X00;
//    UCSR0A &= 0XFC;
//8位数据位 1位停止位 无奇偶校验位
    UCSR0C = (1 << UCSZ00)|(1<<UCSZ01);
//接收结束中断使能 接受使能 发送使能
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
//    UCSR0B |= (1<<RXCIE0);
}

//UART发送函数
int uartPutChar (char c, FILE * stream)
{
    if (c == '\n')
	uartPutChar ('\r', stream);
    loop_until_bit_is_set (UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

//软件查询方式接收数据
unsigned char uartRecvChar (void)
{   while (!(UCSR0A & (1 <<RXC0)));
    return UDR0;
}

//UART开启接收中断
//返回值为寄存器标志位设置的数值
//为1时返回正常，为0时是指失败
unsigned char uartRxISR(void)
{
    UCSR0B |= (1<<RXCIE0);
    //开启全局中断
    sei();
    //返回寄存器设置的位数值
    return (UCSR0B & (1 << RXCIE0));
}

//中断触发方式接受数据
ISR (USART_RX_vect)
{
    unsigned char status;
    status = UCSR0A;
    if ((status&((1<<DOR0)|(1<<FE0)))==0)
    {
	    uartRecvData = UDR0;
    }
    else
        uartInit (BAUD_SETTING);
    //printf ("status = %x\n", status);
    //printf ("uartRecvData = 0x%x\n", uartRecvData);
    //开启回显方式
    //printf ("%c", uartRecvData);
}

