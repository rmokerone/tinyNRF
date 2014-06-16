#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

void uartInit(unsigned int baud)
{
    DDRD |= 0x02;
    UBRR0H = (unsigned char)(baud >> 8);
    UBRR0L = (unsigned char) baud;
//    UCSR0A = 0X00;
//    UCSR0A &= 0XFC;
//8位数据位 1位停止位 无奇偶校验位
    UCSR0C = (1 << UCSZ00)|(1<<UCSZ01);
//接收结束中断使能 接受使能 发送使能
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
//    UCSR0B |= (1<<RXCIE0);
}

int uartPutchar (char c, FILE * stream)
{
    if (c == '\n')
	uartPutchar ('\r', stream);
    loop_until_bit_is_set (UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
/*
unsigned char uart_rcvcahr (void)
{   while (!(UCSR0A & (1 <<RXC0)));
    return UDR0;
}

ISR(USART_RX_vect)
{
    unsigned char status;
    status = UCSR0A;
    if ((status&((1<<DOR0)|(1<<FE0)))==0)
    {
	uart_data = UDR0;
    }
    else
        init_uart (BAUD_SETTING);
}
*/