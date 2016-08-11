/*
 * bfin_pll.c
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

#include <ifx-49.h>

void bfin_pll_init(void)
{
    /* CODECにクロックを供給 */
    /* VR_CTLを設定 */
    /* EXTCLK_OE = 1, EXTCLK_SEL = 0 */
    /* 注意：CP2114を使う場合は、CLOCKを出力しないでください */
    *pVR_CTL = EXTCLK_OE | HIBERNATEB;

    *pSIC_IWR = IWR_ENABLE(0);              /* PLLのみIWRを許す */
    *pPLL_DIV = BF592_PLL_DIV_VAL;
    *pPLL_CTL = BF592_PLL_MUL_VAL << 9;
    asm("CLI R0; CSYNC; IDLE; STI R0;" : : :"R0");
    *pSIC_IWR = IWR_ENABLE_ALL;
}

/* 指定されたマイクロ秒だけ待機 */
void bfin_us_delay(uint32_t us)
{
    uint32_t l_start;
    uint32_t l_now;
    uint32_t l_stop = us * (uint32_t)(BF592_CCLOCK_HZ / 1000000);

    asm("%0 = CYCLES;" : "=d"(l_start));
    while(1)
    {
        asm volatile("%0 = CYCLES;" : "=d"(l_now));
        if((l_now - l_start) >= l_stop)
            break;
    }
}

