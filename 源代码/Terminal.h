#ifndef __TERMINAL_H__
#define __TERMINAL_H__

uint8_t command_print(uint8_t argc, uint8_t argv [][COMMAND_MAX_ARG_LEN+1]);


uint8_t terminal_get_arr(uint8_t num);
void terminal_init();
void terminal_set_big_font(bit en);
void terminal_set_lines(uint8_t lines);
void terminal_add_top(uint8_t *str);
void terminal_add_bottom(uint8_t *str);
void terminal_modify(uint8_t line, uint8_t *str);
uint8_t proc_command(uint8_t *command);
void terminal_add_chr(uint8_t chr);
void TerminalMode();
uint8_t command_enable_serial(COMMAND_ARGS);

uint8_t isint(uint8_t *str);
int16_t getint(uint8_t *str);
uint8_t gethex8(uint8_t *str);

#endif