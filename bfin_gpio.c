/*
 * bfin_gpio.c
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

void bfin_gpio_init(void)
{
    /* LED0 */
    *pPORTG_FER   &= PG4;
    *pPORTGIO_SET  = PG4;
    *pPORTGIO_DIR |= PG4;

    /* SW0 */
    *pPORTF_FER    &= PF0;
    *pPORTFIO_DIR  &= PF0;
    *pPORTFIO_INEN |= PF0;
}

