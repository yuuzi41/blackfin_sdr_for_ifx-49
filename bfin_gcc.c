/*
 * bfin_gcc.c
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

/* 割り込みハンドラを登録 */
void bfin_register_handler(interrupt_kind kind, ex_handler_fn_isr fn)
{
    uint32_t l_mask;

    if((ik_emulation > kind) || (kind > ik_ivg15)) return;

    *((ex_handler_fn_isr*)&(((uint32_t*)EVT0)[(int32_t)kind])) = fn;
    l_mask  = cli();
    l_mask |= 1u << (int32_t)kind;
    sti(l_mask);
}
