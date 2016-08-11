/*
 * ifx-49.c
 *
 * Copyright(C) 2014 Shinya Kaneko
 *
 * Last Update: 2014/06/28
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

#include <ifx-49.h>

void ifx49_init(void)
{
    /* PLL設定 */
    bfin_pll_init();

    /* GPIO初期化 */
    bfin_gpio_init();
    bfin_led0_on();

    /* SPI0初期化 */
    bfin_spi0_init();

	/* ADAU1361初期化 */
    adau1361_init();

    /* UART0初期化 */
    bfin_uart0_init();

    /* CP2114のRESET解除(UART用) */
    *pPORTGIO_DIR |= PG11;
    *pPORTGIO_CLEAR = PG11;
    ssync();
    bfin_us_delay(500 * 1000);
    *pPORTGIO_DIR |= PG11;
    *pPORTGIO_SET = PG11;
}
