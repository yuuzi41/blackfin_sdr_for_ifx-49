/*
 * adau1361.c
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
#include "bf592_rom_func.h"

/* SPORT1バッファリング数(トリプルバッファ) */
#define NUM_SPORT1_BUFFERS      (3)

/* SPORT1 RX DMA割り込みのプロトタイプ宣言 */
DECL_INT_HANDLER(sport1_rx_isr);

/* SPORT1受信バッファ(トリプルバッファ,2チャンネル分) */
static int32_t s_sport1_rxbuf[NUM_CHANNEL][NUM_SPORT1_BUFFERS][NUM_SAMPLES];
/* SPORT1送信バッファ(トリプルバッファ,2チャンネル分) */
static int32_t s_sport1_txbuf[NUM_CHANNEL][NUM_SPORT1_BUFFERS][NUM_SAMPLES];
/* DMAがどのバッファ使用をしているか */
static volatile int32_t s_sport1_use_bufidx;

/* DMA用ディスクリプタ */
struct dma_large_desc_t
{
    uint32_t next_addr;
    uint32_t addr;
    uint16_t cfg;
    uint16_t xcnt;
    int16_t  xmod;
    uint16_t ycnt;
    int16_t  ymod;
};

static struct dma_large_desc_t s_sport1_rxdma_desc[NUM_SPORT1_BUFFERS];
static struct dma_large_desc_t s_sport1_txdma_desc[NUM_SPORT1_BUFFERS];

/* データ転送完了フラグ */
static volatile int32_t s_data_incoming;

/* ADAU1361 SPI初期化データ列 */
static const uint8_t s_audio_init_data[] =
{
    // R0: Clock Control
    0x40, 0x00, 0x00,       // 0000-0000
    // R0: Clock Control
    0x40, 0x00, 0x07,       // 0000-0111
    // R2: Digital Microphone/Jack Detection Control
    0x40, 0x08, 0x00,
    // R3: Record Power Management
    0x40, 0x09, 0x74,       // 0111-0100
    // R4: Record Mixer Left (Mixer 1) Control 0
    0x40, 0x0A, 0x5B,       // 0101-1011
    // R5: Record Mixer Left (Mixer 1) Control 1
    0x40, 0x0B, 0x00,       // 0000-1000
    // R6: Record Mixer Right (Mixer 2) Control 0
    0x40, 0x0C, 0x5B,       // 0101-1011
    // R7: Record Mixer Right (Mixer 2) Control 1
    0x40, 0x0D, 0x00,       // 0000-0000
    // R8: Left Differential Input Volume Control
    0x40, 0x0E, 0x00,
    // R9: Right Differential Input Volume Control
    0x40, 0x0F, 0x00,
    // R10: Record Microphone Bias Control
    0x40, 0x10, 0x00,
    // R11: ALC Control 0
    0x40, 0x11, 0x03,       // 0000-0011
    // R12: ALC Control 1
    0x40, 0x12, 0x00,
    // R13: ALC Control 2
    0x40, 0x13, 0xF0,       // 0100-0000
    // R14: ALC Control 3
    0x40, 0x14, 0x64,       // 0110-0100

    // R15: Serial Port Control 0
    0x40, 0x15, 0x01,       // 0000-0001
    // R16: Serial Port Control 1
    0x40, 0x16, 0x00,
    // R17: Converter Control 0
    0x40, 0x17, 0x00,
    // R18: Converter Control 1
    0x40, 0x18, 0x00,
    // R19: ADC Control
    0x40, 0x19, 0x63,       // 0110-0011
    // R20: Left Input Digital Volume
    0x40, 0x1A, 0x00,
    // R21: Right Input Digital Volume
    0x40, 0x1B, 0x00,
    // R22: Playback Mixer Left (Mixer 3) Control 0
    0x40, 0x1C, 0x21,       // 0010-0001
    // R23: Playback Mixer Left (Mixer 3) Control 1
    0x40, 0x1D, 0x00,
    // R24: Playback Mixer Right (Mixer 4) Control 0
    0x40, 0x1E, 0x41,       // 0100-0001
    // R25: Playback Mixer Right (Mixer 4) Control 1
    0x40, 0x1F, 0x00,
    // R26: Playback L/R Mixer Left (Mixer 5) Line Output Control
    0x40, 0x20, 0x05,       // 0000-0101
    // R27: Playback L/R Mixer Right (Mixer 6) Line Output Control
    0x40, 0x21, 0x11,       // 0001-0001
    // R28: Playback L/R Mixer Mono Output
    0x40, 0x22, 0x00,
    // R29: Playback Headphone Left Volume Control
    0x40, 0x23, 0xEF,       // 1110-1111
    // R30: Playback Headphone Right Volume Control
    0x40, 0x24, 0xEF,       // 1110-1111
    // R31: Playback Line Output Left Volume Control
    0x40, 0x25, 0xEE,       // 1110-1110
    // R32: Playback Line Output Right Volume Control
    0x40, 0x26, 0xEE,       // 1110-1110
    // R33: Playback Mono Output Control
    0x40, 0x27, 0x00,
    // R34: Playback Pop/Click Suppression
    0x40, 0x28, 0x00,
    // R35: Playback Power Management
    0x40, 0x29, 0xAB,       // 1010-1011
    // R36: DAC Control 0
    0x40, 0x2A, 0x03,       // 0000-0011
    // R37: DAC Control 1
    0x40, 0x2B, 0x00,
    // R38: DAC Control 2
    0x40, 0x2C, 0x00,

    // R39: Serial Port Pad Control
    0x40, 0x2D, 0xAA,       // 1010-1010
    // R40: Control Port Pad Control 0
    0x40, 0x2F, 0xAA,       // 1010-1010
    // R41: Control Port Pad Control 1
    0x40, 0x30, 0x00,
    // R42: Jack Detect Pin Control
    0x40, 0x31, 0x08,       // 0000-1000
    // R67: Dejitter Control
    0x40, 0x36, 0x03,
};

/* SPI通信開始(PF4ピンを'L'に変更) */
static void bfin_spi0_codec_assert(void)
{
    *pPORTFIO_CLEAR = PF4;
    ssync();

    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
}

/* SPI通信終了(PF4ピンを'H'に変更) */
static void bfin_spi0_codec_deassert(void)
{
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();

    *pPORTFIO_SET = PF4;
    ssync();

    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
}

static void spi0_codec_write(uint8_t reg_addr_hi, uint8_t reg_addr_low, uint8_t reg_data)
{
    bfin_spi0_codec_assert();
    bfin_spi0_xfer(0x00);
    bfin_spi0_xfer(reg_addr_hi);
    bfin_spi0_xfer(reg_addr_low);
    bfin_spi0_xfer(reg_data);
    bfin_spi0_codec_deassert();
}

static uint8_t spi0_codec_read(uint8_t reg_addr_hi, uint8_t reg_addr_low)
{
    uint8_t l_rv;
    
    bfin_spi0_codec_assert();
    bfin_spi0_xfer(0x01);
    bfin_spi0_xfer(reg_addr_hi);
    bfin_spi0_xfer(reg_addr_low);
    l_rv = bfin_spi0_xfer(0x00);
    bfin_spi0_codec_deassert();

    return l_rv;
}

static void sport1_init(void)
{
    /* ポートの設定 */
    /* CODEC : SPORT1 */
    *pPORTF_MUX &= (PF1 | PF2 | PF3 | PF5 | PF6 | PF7);
    *pPORTF_FER |= PF1 | PF2 | PF3 | PF5 | PF6 | PF7;

    /* SPORT1 RXの設定 */
    /* 外部クロック、外部SYNC、MSB First */
    /* 24ビットデータ、 ステレオ */
    *pSPORT1_RCR1 = RFSR | RCKFE;
    *pSPORT1_RCR2 = SLEN(31) | RSFSE;
    /* SPORT1 TXの設定 */
    /* 外部クロック、外部SYNC、MSB First */
    /* 24ビットデータ、 ステレオ */
    *pSPORT1_TCR1 = TFSR | TCKFE;
    *pSPORT1_TCR2 = SLEN(31) | TSFSE;

    /* DMA3(SPORT1 RX)の設定 */
    s_sport1_rxdma_desc[0].next_addr = (uint32_t)&s_sport1_rxdma_desc[1];
    s_sport1_rxdma_desc[0].addr      = (uint32_t)&s_sport1_rxbuf[0][0][0];
    s_sport1_rxdma_desc[0].cfg       = FLOW_LARGE | NDSIZE_9 | DI_EN | DMA2D | WDSIZE_32 | WNR | DMAEN;
    s_sport1_rxdma_desc[0].xcnt      = 2;
    s_sport1_rxdma_desc[0].xmod      = (int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS; // = s_sport1_rxbuf[1][0][0]
    s_sport1_rxdma_desc[0].ycnt      = NUM_SAMPLES;
    s_sport1_rxdma_desc[0].ymod      = -((int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS) + (int16_t)sizeof(int32_t);

    s_sport1_rxdma_desc[1].next_addr = (uint32_t)&s_sport1_rxdma_desc[2];
    s_sport1_rxdma_desc[1].addr      = (uint32_t)&s_sport1_rxbuf[0][1][0];
    s_sport1_rxdma_desc[1].cfg       = FLOW_LARGE | NDSIZE_9 | DI_EN | DMA2D | WDSIZE_32 | WNR | DMAEN;
    s_sport1_rxdma_desc[1].xcnt      = 2;
    s_sport1_rxdma_desc[1].xmod      = (int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS; // = s_sport1_rxbuf[1][1][0]
    s_sport1_rxdma_desc[1].ycnt      = NUM_SAMPLES;
    s_sport1_rxdma_desc[1].ymod      = -((int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS) + (int16_t)sizeof(int32_t);

    s_sport1_rxdma_desc[2].next_addr = (uint32_t)&s_sport1_rxdma_desc[0];
    s_sport1_rxdma_desc[2].addr      = (uint32_t)&s_sport1_rxbuf[0][2][0];
    s_sport1_rxdma_desc[2].cfg       = FLOW_LARGE | NDSIZE_9 | DI_EN | DMA2D | WDSIZE_32 | WNR | DMAEN;
    s_sport1_rxdma_desc[2].xcnt      = 2;
    s_sport1_rxdma_desc[2].xmod      = (int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS; // = s_sport1_rxbuf[1][2][0]
    s_sport1_rxdma_desc[2].ycnt      = NUM_SAMPLES;
    s_sport1_rxdma_desc[2].ymod      = -((int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS) + (int16_t)sizeof(int32_t);
    
    *pDMA3_PERIPHERAL_MAP = 0x3000;
    *pDMA3_NEXT_DESC_PTR = &s_sport1_rxdma_desc[0];
    *pDMA3_CONFIG = FLOW_LARGE | NDSIZE_9 | WNR;
    *pDMA3_X_COUNT = 0;
    *pDMA3_IRQ_STATUS = DMA_DONE | DMA_ERR;
    
    /* DMA4(SPORT1 TX)の設定 */
    s_sport1_txdma_desc[0].next_addr = (uint32_t)&s_sport1_txdma_desc[1];
    s_sport1_txdma_desc[0].addr      = (uint32_t)&s_sport1_txbuf[0][0][0];
    s_sport1_txdma_desc[0].cfg       = FLOW_LARGE | NDSIZE_9 | DI_EN | DMA2D | WDSIZE_32 | DMAEN;
    s_sport1_txdma_desc[0].xcnt      = 2;
    s_sport1_txdma_desc[0].xmod      = (int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS; // = s_sport_rxbuf[1][0][0]
    s_sport1_txdma_desc[0].ycnt      = NUM_SAMPLES;
    s_sport1_txdma_desc[0].ymod      = -((int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS) + (int16_t)sizeof(int32_t);

    s_sport1_txdma_desc[1].next_addr = (uint32_t)&s_sport1_txdma_desc[2];
    s_sport1_txdma_desc[1].addr      = (uint32_t)&s_sport1_txbuf[0][1][0];
    s_sport1_txdma_desc[1].cfg       = FLOW_LARGE | NDSIZE_9 | DI_EN | DMA2D | WDSIZE_32 | DMAEN;
    s_sport1_txdma_desc[1].xcnt      = 2;
    s_sport1_txdma_desc[1].xmod      = (int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS; // = s_sport_txbuf[1][1][0]
    s_sport1_txdma_desc[1].ycnt      = NUM_SAMPLES;
    s_sport1_txdma_desc[1].ymod      = -((int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS) + (int16_t)sizeof(int32_t);

    s_sport1_txdma_desc[2].next_addr = (uint32_t)&s_sport1_txdma_desc[0];
    s_sport1_txdma_desc[2].addr      = (uint32_t)&s_sport1_txbuf[0][2][0];
    s_sport1_txdma_desc[2].cfg       = FLOW_LARGE | NDSIZE_9 | DI_EN | DMA2D | WDSIZE_32 | DMAEN;
    s_sport1_txdma_desc[2].xcnt      = 2;
    s_sport1_txdma_desc[2].xmod      = (int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS; // = s_sport_txbuf[1][2][0]
    s_sport1_txdma_desc[2].ycnt      = NUM_SAMPLES;
    s_sport1_txdma_desc[2].ymod      = -((int16_t)sizeof(int32_t) * NUM_SAMPLES * NUM_SPORT1_BUFFERS) + (int16_t)sizeof(int32_t);

    *pDMA4_PERIPHERAL_MAP = 0x4000;
    *pDMA4_NEXT_DESC_PTR = &s_sport1_txdma_desc[0];
    *pDMA4_CONFIG = FLOW_LARGE | NDSIZE_9;
    *pDMA4_X_COUNT = 0;
    *pDMA4_IRQ_STATUS = DMA_DONE | DMA_ERR;
    ssync();

    /* 割り込みハンドラをIVG9に登録 */
    bfin_register_handler(ik_ivg9, sport1_rx_isr);
    *pSIC_IMASK |= IRQ_DMA3;

    /* DMAを有効 */
    *pDMA3_IRQ_STATUS = DMA_DONE;
    *pDMA3_CONFIG |= DMAEN;
    *pDMA4_CONFIG |= DMAEN;
}

void adau1361_init(void)
{
    uint32_t l_lc;
    uint8_t  l_data;

    /* SPI0_CS# */
    *pPORTF_FER   &= PF4;
    *pPORTFIO_DIR |= PF4;
    *pPORTFIO_SET  = PF4;

    /* SPORTバスをADAU1361マスタに切り替える */
    *pPORTGIO_DIR |= PG12;
    *pPORTGIO_SET = PG12;

    /* CODEC安定のため100ms待機 */
    bfin_us_delay(100 * 1000);

    /* バッファクリア */
    rom_memset(s_sport1_rxbuf, 0 , sizeof(s_sport1_rxbuf));
    rom_memset(s_sport1_txbuf, 0 , sizeof(s_sport1_txbuf));
    s_sport1_use_bufidx = NUM_SPORT1_BUFFERS - 1;
    s_data_incoming = 0;

    /* SPIモードに移行 */
    for(l_lc = 0U; l_lc < 3U; l_lc++)
    {
        bfin_spi0_codec_assert();
        bfin_us_delay(1000);
        bfin_spi0_codec_deassert();
        bfin_us_delay(1000);
    }

    /* ADAU1361クロック停止 */
    spi0_codec_write(s_audio_init_data[0], s_audio_init_data[1], s_audio_init_data[2]);

    /* ADAU1361初期化 */
    for(l_lc = 3U; l_lc < sizeof(s_audio_init_data); l_lc += 3)
    {
#if 1
        /* ADAU1361レジスタ設定 */
        spi0_codec_write(s_audio_init_data[l_lc + 0], s_audio_init_data[l_lc + 1], s_audio_init_data[l_lc + 2]);
#else
        while(1)
        {
            /* ADAU1361レジスタ設定 */
            spi0_codec_write(s_audio_init_data[l_lc + 0], s_audio_init_data[l_lc + 1], s_audio_init_data[l_lc + 2]);
            bfin_us_delay(1000);
            /* ADAU1361レジスタリードバック */
            l_data = spi0_codec_read(s_audio_init_data[l_lc + 0], s_audio_init_data[l_lc + 1]);
            if(l_data == s_audio_init_data[l_lc + 2])
            {
                break;
            }
        }
#endif
    }

    sport1_init();
}

void adau1361_enable(void)
{
    /* SPORT1を有効 */
    *pSPORT1_TCR1 |= TSPEN;
    *pSPORT1_RCR1 |= RSPEN;
    ssync();
}

int32_t adau1361_data_check(void)
{
    int32_t l_dmabuf1_idx;
    int32_t l_lc;
    const int32_t* lp_rx_buf[2];
    int32_t* lp_tx_buf[2];

    if(0 != s_data_incoming)
    {
        /* フラグクリア */
        s_data_incoming = 0;
        /* 使用してよいバッファを取得 */
        l_dmabuf1_idx = s_sport1_use_bufidx;

        lp_rx_buf[0] = s_sport1_rxbuf[0][l_dmabuf1_idx];
        lp_rx_buf[1] = s_sport1_rxbuf[1][l_dmabuf1_idx];
        lp_tx_buf[0] = s_sport1_txbuf[0][l_dmabuf1_idx];
        lp_tx_buf[1] = s_sport1_txbuf[1][l_dmabuf1_idx];

        for(l_lc = 0; l_lc < NUM_SAMPLES; l_lc++)
        {
            asm("%0 = %1 >>> %2;" : "=d"(s_sport1_rxbuf[0][l_dmabuf1_idx][l_lc]) : "d"(s_sport1_rxbuf[0][l_dmabuf1_idx][l_lc]), "n"(31 - FIXED_POINT_DECIMAL));
            asm("%0 = %1 >>> %2;" : "=d"(s_sport1_rxbuf[1][l_dmabuf1_idx][l_lc]) : "d"(s_sport1_rxbuf[1][l_dmabuf1_idx][l_lc]), "n"(31 - FIXED_POINT_DECIMAL));
        }

        codec_audio_process(lp_rx_buf, lp_tx_buf);

        for(l_lc = 0; l_lc < NUM_SAMPLES; l_lc++)
        {
            asm("%0 = %1 << %2(S);" : "=d"(s_sport1_txbuf[0][l_dmabuf1_idx][l_lc]) : "d"(s_sport1_txbuf[0][l_dmabuf1_idx][l_lc]), "n"(31 - FIXED_POINT_DECIMAL));
            asm("%0 = %1 << %2(S);" : "=d"(s_sport1_txbuf[1][l_dmabuf1_idx][l_lc]) : "d"(s_sport1_txbuf[1][l_dmabuf1_idx][l_lc]), "n"(31 - FIXED_POINT_DECIMAL));
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

/* SPORT1 RX DMA割り込みハンドラ */
DECL_INT_HANDLER(sport1_rx_isr)
{
    *pDMA3_IRQ_STATUS = DMA_DONE;
    ssync();

    /* オーディオコーデックからデータ到着 */
    s_data_incoming = 1;
    /* DMAが使用するバッファを変更 */
    s_sport1_use_bufidx = (s_sport1_use_bufidx + 1U) % NUM_SPORT1_BUFFERS;
}
