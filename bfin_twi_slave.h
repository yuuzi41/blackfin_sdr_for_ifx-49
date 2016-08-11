#ifndef _BFIN_TWI_SLAVE_H_
#define _BFIN_TWI_SLAVE_H_

extern void bfin_twi_slave_init(uint8_t slave_addr, uint8_t (*callback_xmit)(void), void (*callback_recv)(uint8_t* p_buf, int16_t count));

#endif /* _BFIN_TWI_SLAVE_H_ */
