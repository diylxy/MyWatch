#ifndef __FLASH_H__
#define __FLASH_H__


void flash_init();
bit flash_chk_busy();
void flash_chip_erase();
void flash_read(uint32_t addr, uint32_t size, uint8_t *buffer);
void flash_write(uint32_t addr, uint32_t size, uint8_t *buffer);
void flash_erase_sector(uint32_t addr);
uint8_t command_flash_program(COMMAND_ARGS);

#endif