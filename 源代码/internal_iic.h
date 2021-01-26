#ifndef __INTERNAL_IIC_H__
#define __INTERNAL_IIC_H__

#define INTERNAL_IIC_ACK 0
#define INTERNAL_IIC_NACK 1


void int_iic_start();        /* --------- Initiate int_iic_start condition ---------- */   
void int_iic_stop();     /* ---------- Initiate int_iic_stop condition ----------- */   
uint8_t int_iic_write(uint8_t d);     /* ----------------------------- */   
uint8_t int_iic_read(uint8_t ack);  /* ----------------------------------- */   
uint8_t int_iic_read_addr(uint8_t devaddr, uint8_t addr);
void int_iic_write_addr(uint8_t devaddr, uint8_t addr, uint8_t dat);
uint8_t int_iic_check_device(uint8_t addr);
uint8_t command_iic_check(COMMAND_ARGS);
uint8_t command_iic_read(COMMAND_ARGS);
uint8_t command_iic_write(COMMAND_ARGS);
uint8_t command_iic_read16(COMMAND_ARGS);
uint8_t command_iic_write16(COMMAND_ARGS);


#endif