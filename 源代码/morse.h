#ifndef __MORSE_H__
#define __MORSE_H__

#define MORSE_TOTAL 36
#define MAX_INPUT_STR 50
#define MAX_INPUT_CHR 6
#define DA_TIME 20
#define CHR_TIME 40
#define LONG_TIME KEY_LONG_PRESS_TIME
#define SPACE_TIME 200
extern unsigned char morse_input_str[MAX_INPUT_STR];
extern uint8_t strPointer;
void morseUpdate();
#endif