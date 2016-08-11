/*
 * bfin_gcc.h
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
#ifndef _BF592_GCC_
#define _BF592_GCC_
#ifdef __ECC__
#error This header file is for gcc.
#endif /* __ECC__ */

#define _inline_ __attribute__((always_inline))

#include <sys/exception.h>

typedef __attribute__((interrupt_handler)) void (*ex_handler_fn_isr)(void);
#define DECL_INT_HANDLER(handler) __attribute__((interrupt_handler)) void handler(void)

/* 割り込みハンドラを登録 */
void bfin_register_handler(interrupt_kind kind, ex_handler_fn_isr fn);

#endif /* _BF_VDSP_ */
