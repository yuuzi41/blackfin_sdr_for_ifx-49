/*
 * bfin_uart0.h
 *
 * Copyright(C) 2014 Shinya Kaneko
 *
 * Last Update: 2014/09/10
 *
 * [ご注意]
 * このソフトウェアはフリーソフトウェアです。どなたでも自由に改変して使用することができます。
 * このソフトウェアは無保証です。このソフトウェアの使用に起因し、ユーザまたは第三者に
 * 生じた損害に関し、一切の責任を負いません。
 *
 * [NOTE]
 * This program is free software, you can redistribute it and/or modify it.
 * There is no warranty for this free software.
 *
 */
#ifndef _BFIN_UART0_
#define _BFIN_UART0_

extern void bfin_uart0_init(void);
extern void bfin_uart0_putc(char ch);
extern void bfin_uart0_puts(const char *p_buf);
extern int32_t bfin_uart0_read_byte(void);

#endif /* _BFIN_UART0_ */
