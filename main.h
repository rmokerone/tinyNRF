/**********************************************************************
	> File Name: main.h
	> Author: Rmoker
	> Mail: rmokerone@gmail.com 
	> Created Time: Fri 13 Jun 2014 08:32:01 AM CST
 *********************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

/*
 * 定义项目引用的公用头文件
 */
#define F_CPU 8E6
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * 快捷设置寄存器的相关宏定义
 */
#define BIT(x)                  (1 << (x))
#define SETBITS(x, y)           ((x) |= (y))
#define CLEARBITS(x, y)         ((x) &= (~(y)))
#define SETBIT(x, y)            SETBITS((x), (BIT((y))))
#define CLEARBIT(x, y)          CLEARBITS((x), (BIT((y))))

/*
 * 定义Nrf24L01各个端口的引脚号
 */
#define CE              PB1
#define CSN             PB2
#define MOSI            PB3
#define MISO            PB4
#define SCK             PB5
#define IRQ             PD2

/*
 * 重命名数据变量
 */
#define uchar unsigned char
#define uint  unsigned int

#endif
