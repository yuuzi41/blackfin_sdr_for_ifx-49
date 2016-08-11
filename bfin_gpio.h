/*
 * bfin_gpio.h
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
#ifndef _BFIN_GPIO_
#define _BFIN_GPIO_

#define SW0_RELEASE  0
#define SW0_PUSHING  1

extern void bfin_gpio_init(void);

inline static void bfin_led0_on(void)
{
    *pPORTGIO_CLEAR = PG4;
}

inline static void bfin_led0_off(void)
{
    *pPORTGIO_SET = PG4;
}

inline static void bfin_led0_toggle(void)
{
    *pPORTGIO_TOGGLE = PG4;
}

inline static uint8_t bfin_sw0_status(void)
{
    return (*pPORTFIO & PF0) ? SW0_RELEASE : SW0_PUSHING;
}

#endif /* _BFIN_GPIO_ */
