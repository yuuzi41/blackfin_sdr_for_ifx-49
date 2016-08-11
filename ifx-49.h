/*
 * ifx-49.h
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
#ifndef _IFX49_
#define _IFX49_

#include <cdefBF592-A.h>
#include <builtins.h>
#include <stdint.h>
#include <string.h>
#include <fract.h>
#ifdef __ECC__
#include "bfin_vdsp.h"
#else /* __ECC__ */
#include "bfin_gcc.h"
#endif /* __ECC__ */

#include "bfin_pll.h"
#include "bfin_gpio.h"
#include "bfin_spi0.h"
//#include "bfin_spi1.h"
#include "bfin_uart0.h"
#include "bfin_timer0.h"
//#include "bfin_timer2.h"
#include "adau1361.h"
#include "bfin_fr32.h"

/* コアクロック(393.216MHz) */
#define BF592_CCLOCK_HZ		(393216000)
/* システムクロック(98.304MHz) */
#define BF592_SCLOCK_HZ		(98304000)

/* UART0ボーレート(115200bps) */
#define UART0_BAUDRATE	(115200)

/* Blackfin PLL MULの値 */
#define BF592_PLL_MUL_VAL (8)
/* Blackfin PLL DIVの値 */
#define BF592_PLL_DIV_VAL (4)

extern void ifx49_init(void);

#endif /* _IFX49_ */
