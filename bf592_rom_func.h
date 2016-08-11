/*
 * bf592_rom_func.h
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

#ifndef _BF592_ROM_FUNC_
#define _BF592_ROM_FUNC_

/* ADSP-BF592 Rev.0.2用ROMに格納されている関数
 * 注意：他のリビジョンでは動作しません
 * また、sin_fr16など、データテーブル参照する関数は動作しません
 */

#define rom_strstr(a, b) ((char* (*)(const char *, const char *))0xFFA11440)((a), (b))
#define rom_strspn(a, b) ((size_t (*)(const char *, const char *))0xFFA11418)((a), (b))
#define rom_strrchr(a, b) ((char* (*)(const char *, int))0xFFA11400)((a), (b))
#define rom_strpbrk(a, b) ((char* (*)(const char *, const char *))0xFFA113D8)((a), (b))
#define rom_strncpy(a, b, c) ((char* (*)(char *, const char *, size_t))0xFFA11348)((a), (b), (c))
#define rom_strncmp	0xFFA11310
#define rom_strncat	0xFFA112D8
#define rom_strlen(a) ((size_t (*)(const char *))0xFFA112B8)(a)
#define rom_strcspn	0xFFA11294
#define rom_strcpy	0xFFA11284
#define rom_strcmp	0xFFA11210
#define rom_strchr	0xFFA111F8
#define rom_strcat	0xFFA111C4
#define rom_sqrtf	0xFFA1B378
#define rom_sqrtd	0xFFA1B278
#define rom_sqrt_fr16	0xFFA1B1F8
#define rom_sinhd	0xFFA1AFAC
#define rom_sinhf	0xFFA1AE3C
#define rom_sind	0xFFA1AB80
//#define rom_sin_fr16(a) ((int16_t (*)(int16_t))0xFFA1AB30)(a)
#define rom_sinf  ((float (*)(float))0xFFA1A9E4)(x)
#define rom_copysignf	0xFFA1A9D4
#define rom_copysign_fr16	0xFFA1A9C8
#define rom_rsqrtf	0xFFA1A920
#define rom_rsqrtd	0xFFA1A80C
#define rom_float32_to_int32	0xFFA1A784
#define rom_rmsd	0xFFA1A728
#define rom_rms_fr16	0xFFA1A67C
#define rom_rmsf	0xFFA1A660
#define rom_rfft_fr16	0xFFA1A3F0
#define rom__rfft2d_fr16	0xFFA1A110
#define rom_powd	0xFFA19A44
#define rom_powf	0xFFA196E8
#define rom_polard	0xFFA19690
#define rom_polarf	0xFFA1965C
#define rom_polar_fr16	0xFFA195DC
#define rom_normd	0xFFA1956C
#define rom_normf	0xFFA19520
#define rom_multu64_32x32	0xFFA194F8
#define rom_mult64_32x32	0xFFA194D0
#define rom_mu_expand	0xFFA19448
#define rom_mu_compress	0xFFA193DC
#define rom_memset(a, b, c) ((void* (*)(void *, int, size_t))0xFFA110B4)((a), (b), (c))
#define rom_memmove(a, b, c) ((void* (*)(void *, const void*, size_t))0xFFA11040)((a), (b), (c))
#define rom_memcpy_l1	0xFFA10CF4
#define rom_memcpy	0xFFA10FF4
#define rom_memcmp	0xFFA10F98
#define rom_memchr	0xFFA10F78
#define rom_meansf	0xFFA19238
#define rom_meand	0xFFA191F0
#define rom_mean_fr16	0xFFA191C4
#define rom_longjmp	0xFFA10ED8
#define rom_logd	0xFFA18D74
#define rom_logf	0xFFA18C60
#define rom_log10d	0xFFA18A68
#define rom_log10f	0xFFA18948
#define rom_lldiv	0xFFA18928
#define rom_llabs	0xFFA10EB0
#define rom_ldexpd	0xFFA18854
#define rom_l1_memcpy	0xFFA10B20
#define rom_isnand	0xFFA18830
#define rom_isnan	0xFFA18820
#define rom_isinfd	0xFFA187FC
#define rom_isinf	0xFFA187EC
#define rom_iirdf1_fr16	0xFFA18770
#define rom_iir_fr16	0xFFA1866C
#define rom_ifft_fr16	0xFFA183F8
#define rom_ifft2d_fr16	0xFFA18118
#define rom_frexpd	0xFFA17EBC
#define rom_frexpf	0xFFA17E8C
#define rom_fr16_to_float	0xFFA11628
#define rom_fr16_to_fr32	0xFFA11624
#define rom_fmodf	0xFFA17A2C
#define rom_fminf	0xFFA17A0C
#define rom_fmind	0xFFA179E4
#define rom_fmaxf	0xFFA179C0
#define rom_fmaxd	0xFFA17994
#define rom_floord	0xFFA17734
#define rom_floorf	0xFFA176D4
#define rom_float_to_fr16	0xFFA115D8
#define rom_float_to_fr32	0xFFA11590
#define rom_fir_interp_fr16	0xFFA17518
#define rom_fir_fr16	0xFFA173BC
#define rom_fir_decima_fr16	0xFFA172EC
#define rom_fclipf	0xFFA1728C
#define rom_fclipd	0xFFA17224
#define rom_fabsd	0xFFA171F8
#define rom_expd	0xFFA16F54
#define rom_expf	0xFFA16E00
#define rom_ldiv	0xFFA16B1C
#define rom_div	0xFFA16B1C
#define rom_fr32_to_fr16	0xFFA1158C
#define rom_fr32_to_float	0xFFA11558
#define rom_csubd	0xFFA16350
#define rom_csub_fr16	0xFFA16348
#define rom_crosscorrd	0xFFA1625C
#define rom_crosscorr_fr16	0xFFA161E4
#define rom_crosscorrf	0xFFA1617C
#define rom_llcountones	0xFFA15ECC
#define rom_lcountones	0xFFA15EC4
#define rom_countones	0xFFA15EC4
#define rom_cotd	0xFFA15C50
#define rom_cotf	0xFFA15AC0
#define rom_coshd	0xFFA159F8
#define rom_coshf	0xFFA15954
#define rom_cosd	0xFFA156D4
#define rom_cos_fr16	0xFFA15684
#define rom_cosf	0xFFA15528
#define rom_copysignd	0xFFA154E0
#define rom_convolve_fr16	0xFFA15404
#define rom_conv2d_fr16	0xFFA15344
#define rom_conv2d3x3_fr16	0xFFA15174
#define rom_conjd	0xFFA15150
#define rom_conj_fr16	0xFFA15144
#define rom_coeff_iirdf1_fr16	0xFFA15050
#define rom_cmltd	0xFFA14F50
#define rom_cmlt_fr16	0xFFA14F44
#define rom_cmatmmltd	0xFFA14E1C
#define rom_cmatmmlt_fr16	0xFFA14DA8
#define rom_cmatmmltf	0xFFA14CC8
#define rom_llclip	0xFFA14C60
#define rom_cfir_fr16	0xFFA14BC4
#define rom_cfftf_fr16	0xFFA149BC
#define rom_cfft_fr16	0xFFA14748
#define rom_cfft2d_fr16	0xFFA14468
#define rom_cexpd	0xFFA1442C
#define rom_cexpf	0xFFA14408
#define rom_ceild	0xFFA14334
#define rom_ceilf	0xFFA142D8
#define rom_cdivd	0xFFA140DC
#define rom_cdiv_fr16	0xFFA1407C
#define rom_cdivf	0xFFA13F5C
#define rom_cartesiand	0xFFA13F24
#define rom_cartesianf	0xFFA13EFC
#define rom_cartesian_fr16	0xFFA13ED8
#define rom_caddd	0xFFA13E94
#define rom_cadd_fr16	0xFFA13E8C
#define rom_cabsd	0xFFA13DC4
#define rom_cabsf	0xFFA13D1C
#define rom_cabs_fr16	0xFFA13CB0
#define rom_autocorrd	0xFFA13C08
#define rom_autocorr_fr16	0xFFA13B88
#define rom_autocorrf	0xFFA13B18
#define rom_atand	0xFFA136C0
#define rom_atan_fr16	0xFFA1365C
#define rom_atanf	0xFFA13524
#define rom_atan2d	0xFFA13224
#define rom_atan2_fr16	0xFFA1317C
#define rom_atan2f	0xFFA12F9C
#define rom_asind	0xFFA12D2C
#define rom_asin_fr16	0xFFA12C70
#define rom_asinf	0xFFA12B10
#define rom_argd	0xFFA12AC8
#define rom_argf	0xFFA12AB4
#define rom_arg_fr16	0xFFA12A7C
#define rom_alogf	0xFFA12A6C
#define rom_alogd	0xFFA12A5C
#define rom_alog10f	0xFFA12A40
#define rom_alog10d	0xFFA12A20
#define rom_acosd	0xFFA127B4
#define rom_acos_fr16	0xFFA126F8
#define rom_acosf	0xFFA12578
#define rom_abs_fr16	0xFFA12560
#define rom_a_expand	0xFFA12524
#define rom_a_compress	0xFFA124D4
#define rom_memmove2	0xFFA114E0
#define rom_memcpy2	0xFFA1148C

#endif /* _BF592_ROM_FUNC_ */
