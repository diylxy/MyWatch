#ifndef __MENU_H__
#define __MENU_H__



extern void menu_init();
extern uint16_t menu_start(uint8_t *title);
extern void menu_add(uint8_t *str);
extern void menu_roll(bit direction);
extern void menu_end();
extern void menu_inverse_refresh();

uint8_t menu_ask_yn(uint8_t *tip);
uint8_t menu_ask_num(uint8_t start, uint8_t end, uint8_t *tip);
void pop_msg(uint8_t *tip);
void req_password();


#endif