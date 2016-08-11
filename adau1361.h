/*
 * adau1361.h
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
#ifndef _ADAU1361_
#define _ADAU1361_

/* 1チャンネル当たりの1回のサンプルサイズ */
#define NUM_SAMPLES	        	(128)
/* チャンネル数(ステレオ) */
#define NUM_CHANNEL		        (2)

/* サンプリングレート */
#define SAMPLING_RATE           (48000)

extern void adau1361_init(void);
extern int32_t adau1361_data_check(void);
extern void adau1361_enable(void);
extern void codec_audio_process(const int32_t* p_rxbuf[2], int32_t* p_txbuf[2]);

#endif /* _ADAU1361_ */
