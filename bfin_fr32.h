/*
 * bfin_fract.h
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

#ifndef _BFIN_FRACT_
#define _BFIN_FRACT_

/* 固定小数点の小数点ビット数 */
#ifndef FIXED_POINT_DECIMAL
#define FIXED_POINT_DECIMAL (24)
#endif

#if FIXED_POINT_DECIMAL < 16
#error Must be more than 16
#endif


/* float to fract */
#define FLOAT_TO_FR32(x, y)   ((int32_t)((1U << y) * (x)))
#define FLOAT_TO_Q31(x)       FLOAT_TO_FR32(x, 31)
#define FLOAT_TO_Q24(x)       FLOAT_TO_FR32(x, 24)
#define FLOAT_TO_Q16(x)       FLOAT_TO_FR32(x, 16)
#define FLOAT_TO_Q15(x)       FLOAT_TO_FR32(x, 15)

/* float to fract16 */
#define FLOAT_TO_FR16(x)   ((int16_t)((1U << 32768) * (x)))

#ifdef __ECC__

/* 乗算 */
inline static int32_t mult_fr32_q(int32_t _a, int32_t _b, int32_t _q)
{
    return (int32_t)(((long long)_a * (long long)_b) >> _q);
}

#define mult_fr32(_a, _b) mult_fr32_q(_a, _b, FIXED_POINT_DECIMAL)

#else /* __ECC__ */

/* 乗算 */
inline static int32_t mult_fr32_q(int32_t _a, int32_t _b, int32_t _q)
{
    if(_q > 16)
    {
        int32_t l_rv;
        asm(
            "A1 = %1.L * %2.L (FU);"
            "A1 = A1 >> 16;"
            "A0 = %1.H * %2.H, A1 += %1.H * %2.L (M);"
            "A1 += %2.H * %1.L (M);"
            "A1 = A1 >>> 16 - %3;"
            "A0 = A0 << %3 - 1;"
            "%0 = (A0 += A1);"
            : "=d"(l_rv) : "d"(_a), "d"(_b), "n"(32-_q) : "A0", "A1");
        return l_rv;
    }
    else
    {
        return (int32_t)(((long long)_a * (long long)_b) >> _q);
    }
}

#define mult_fr32(_a, _b) mult_fr32_q(_a, _b, FIXED_POINT_DECIMAL)

#endif /* __ECC__ */

/* 除算 */
inline static int32_t div_fr32(int32_t _a, int32_t _b)
{
    return ((long long)_a << FIXED_POINT_DECIMAL) / _b;
}


/* 加算（飽和対策用） */
inline static int32_t add_fr32(int32_t _a, int32_t _b)
{
    int32_t l_rv;
    asm("%0 = %1 + %2(S);" : "=d"(l_rv) : "d"(_a), "d"(_b));
    return l_rv;
}

/* 減算（飽和対策用） */
inline static int32_t sub_fr32(int32_t _a, int32_t _b)
{
    int32_t l_rv;
    asm("%0 = %1 - %2(S);" : "=d"(l_rv) : "d"(_a), "d"(_b));
    return l_rv;
}

#ifndef __ECC__

/* MAX */
inline static int32_t max_fr32(int32_t _a, int32_t _b)
{
    int32_t l_rv;
    asm("%0 = MAX(%1, %2);" : "=d"(l_rv) : "d"(_a), "d"(_b));
    return l_rv;
}

/* MIN */
inline static int32_t min_fr32(int32_t _a, int32_t _b)
{
    int32_t l_rv;
    asm("%0 = MIN(%1, %2);" : "=d"(l_rv) : "d"(_a), "d"(_b));
    return l_rv;
}

/* ABS */
inline static int32_t abs_fr32(int32_t _a)
{
    int32_t l_rv;
    asm("%0 = ABS %1;" : "=d"(l_rv) : "d"(_a));
    return l_rv;
}
#endif /* __ECC__ */

/* 右ビットシフト（飽和対策用） */
inline static int32_t shr_fr32(int32_t _a, int16_t _b)
{
    int32_t l_rv;
    asm("%0 = ASHIFT %1 BY %2.L(S);" : "=d"(l_rv) : "d"(_a), "d"(-_b));
    return l_rv;
}

/* 左ビットシフト（飽和対策用） */
inline static int32_t shl_fr32(int32_t _a, int16_t _b)
{
    int32_t l_rv;
    asm("%0 = ASHIFT %1 BY %2.L(S);" : "=d"(l_rv) : "d"(_a), "d"(_b));
    return l_rv;
}

#endif /* _BFIN_FRACT_ */
