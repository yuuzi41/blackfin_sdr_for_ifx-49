/*
 * bfin_uart0.c
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
#include "bf592_rom_func.h"

/* UART0_DLL/UART0_DLHの設定値 */
#define UART0_DIVISOR   (BF592_SCLOCK_HZ/16/UART0_BAUDRATE)

/* UARTの受信バッファサイズ */
#define UART_NUM_RXBUF  64

static uint8_t uart0_rxbuf[UART_NUM_RXBUF];
static int32_t uart0_rxbuf_ridx;
static int32_t uart0_rxbuf_widx;

/* UART受信割り込みのプロトタイプ宣言 */
DECL_INT_HANDLER(uart0_rx_isr);

void bfin_uart0_init(void)
{
    uart0_rxbuf_widx = 0;
    uart0_rxbuf_ridx = 0;
    
    /* UARTイネーブル */
    *pUART0_GCTL = 1;

    /* UART分周比の設定 */
    *pUART0_LCR |= DLAB;
    *pUART0_DLL = UART0_DIVISOR & 0xFF;
    *pUART0_DLH = UART0_DIVISOR >> 8;
    *pUART0_LCR &= DLAB;

    /* UARTモード設定 パリティ無し 8bit 1ストップビット */
    *pUART0_LCR = WLS(8);

    /* UART割込み禁止 */
    *pUART0_IER = 0;

    /* PF11/PF12ピンをUARTに割り振る */
    *pPORTF_MUX &= (PF11 | PF12);   /* PF11,PF12を0にする */
    *pPORTF_FER |= PF11 | PF12;     /* PF11,PF12をUART0に割り振る */

    /* 割り込みハンドラをIVG10に登録 */
    bfin_register_handler(ik_ivg10, uart0_rx_isr);
    /* UART受信割込みを許可 */
    *pUART0_IER = ERBFI;
    *pSIC_IMASK |= IRQ_DMA7;    /* DMA7 = UART RX */
}

void bfin_uart0_putc(char ch)
{
    /* UART0 LSRのTHREが1になるまで待つ。1ならば送信レジスタ空き */
    while(0 == (*pUART0_LSR & THRE)) ssync();
    *pUART0_THR = ch;   /* 一文字送信 */
    ssync();
}

void bfin_uart0_puts(const char *p_buf)
{
    int l_lc;
    int l_len = rom_strlen(p_buf);
    
    for(l_lc = 0; l_lc < l_len; l_lc++)
    {
        bfin_uart0_putc(p_buf[l_lc]);   /* 一文字送信 */
    }
}

int32_t bfin_uart0_read_byte(void)
{
    uint32_t l_reg;
    uint32_t l_widx;
    int32_t  l_rv = -1;

    l_reg = cli();
    l_widx = uart0_rxbuf_widx;
    sti(l_reg);

    if(uart0_rxbuf_ridx != l_widx)
    {
        l_rv = (int32_t)uart0_rxbuf[uart0_rxbuf_ridx];
        uart0_rxbuf_ridx = (uart0_rxbuf_ridx + 1) % UART_NUM_RXBUF;
    }
    
    return l_rv;
}

DECL_INT_HANDLER(uart0_rx_isr)
{
    uart0_rxbuf[uart0_rxbuf_widx] = *pUART0_RBR;
    uart0_rxbuf_widx = (uart0_rxbuf_widx + 1) % UART_NUM_RXBUF;
}

