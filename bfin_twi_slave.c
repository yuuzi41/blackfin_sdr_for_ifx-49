#include <cdefBF592-A.h>
#include <builtins.h>
#include <sys/exception.h>
#include <stdint.h>
#include <string.h>
#ifdef __ECC__
#include "bfin_vdsp.h"
#else /* __ECC__ */
#include "bfin_gcc.h"
#endif /* __ECC__ */
#include "bfin_twi_slave.h"

#define TWI_COUNT(x) (DCNT & ((x) << 6))

#define TWI_SLAVE_RECV_BUF_SIZE     (16)

typedef struct
{
    uint8_t  slave_recv_buf[TWI_SLAVE_RECV_BUF_SIZE];
    int16_t  slave_recv_count;
    uint8_t  (*slave_xmit_callback)(void);
    void     (*slave_recv_callback)(uint8_t* p_buf, int16_t count);
} twi_xfer_param;

static twi_xfer_param sf_twi_xfer_param;

DECL_INT_HANDLER(s_bfin_twi_slave_isr);

void bfin_twi_slave_init(uint8_t slave_addr, uint8_t (*callback_xmit)(void), void (*callback_recv)(uint8_t* p_buf, int16_t count))
{
    memset(&sf_twi_xfer_param, 0, sizeof(sf_twi_xfer_param));
    sf_twi_xfer_param.slave_xmit_callback = callback_xmit;
    sf_twi_xfer_param.slave_recv_callback = callback_recv;
    sf_twi_xfer_param.slave_recv_count = 0;
    
    *pTWI_CONTROL = 0;
    *pTWI_MASTER_CTL = 0;
    *pTWI_SLAVE_CTL = 0;
    ssync();
//  *pTWI_CONTROL = 10U;
//  *pTWI_CLKDIV  = CLKHI(12) | CLKLOW(13);
    *pTWI_INT_MASK = RCVSERV | XMTSERV | SOVF | SINIT | SCOMP | SERR;
    *pTWI_INT_STAT = *pTWI_INT_STAT;
    *pTWI_FIFO_CTL = XMTFLUSH | RCVFLUSH;
    ssync();
    *pTWI_FIFO_CTL = 0;
    
    /* 割り込みハンドラをIVG12に登録 */
    bfin_register_handler(ik_ivg12, s_bfin_twi_slave_isr);
    *pSIC_IMASK |= IRQ_TWI;

    *pTWI_CONTROL |= TWI_ENA;
    *pTWI_SLAVE_ADDR = slave_addr;
    *pTWI_SLAVE_CTL = STDVAL | SEN;
}

static void s_bfin_twi_data_recv(void)
{
    uint8_t l_data;

    // TWI receive
    while((*pTWI_FIFO_STAT & RCVSTAT) != RCV_EMPTY)
    {
        if(sf_twi_xfer_param.slave_recv_count < TWI_SLAVE_RECV_BUF_SIZE)
        {
            sf_twi_xfer_param.slave_recv_buf[sf_twi_xfer_param.slave_recv_count] = *pTWI_RCV_DATA8;
            sf_twi_xfer_param.slave_recv_count++;
        }
        else
        {
            // the buffer is full
            l_data = *pTWI_RCV_DATA8; // dummy read
        }
    }
}

DECL_INT_HANDLER(s_bfin_twi_slave_isr)
{
    uint16_t l_twi_stat = *pTWI_INT_STAT;

    // Salve
    if((l_twi_stat & SINIT) != 0)
    {
        // Slave mode transfering.
        sf_twi_xfer_param.slave_recv_count = 0;
        *pTWI_INT_STAT = SINIT;
    }
    if((l_twi_stat & RCVSERV) != 0)
    {
        s_bfin_twi_data_recv();
        *pTWI_INT_STAT = RCVSERV;
    }
    else if((l_twi_stat & XMTSERV) != 0)
    {
        // TWI transmit
        while((*pTWI_FIFO_STAT & XMTSTAT) != XMT_FULL)
        {
            *pTWI_XMT_DATA8 = sf_twi_xfer_param.slave_xmit_callback();
        }
        *pTWI_INT_STAT = XMTSERV;
    }
    else if((l_twi_stat & SERR) != 0u)
    {
        // Clear FIFO
        *pTWI_FIFO_CTL = XMTFLUSH | RCVFLUSH;
        *pTWI_FIFO_CTL = 0;

        *pTWI_INT_STAT = SERR;
    }
    else if((l_twi_stat & SOVF) != 0u)
    {
        // Clear RX FIFO
        *pTWI_FIFO_CTL = RCVFLUSH;
        *pTWI_FIFO_CTL = 0;
        *pTWI_INT_STAT = SOVF;
    }
    else if((l_twi_stat & SCOMP) != 0u)
    {
        s_bfin_twi_data_recv();
        if(sf_twi_xfer_param.slave_recv_count > 0)
        {
            // Slave mode callback
            sf_twi_xfer_param.slave_recv_callback(sf_twi_xfer_param.slave_recv_buf, sf_twi_xfer_param.slave_recv_count);
        }
        // Clear FIFO
        *pTWI_FIFO_CTL = XMTFLUSH;
        *pTWI_FIFO_CTL = 0;

        *pTWI_INT_STAT = SCOMP;
    }
    ssync();
}

