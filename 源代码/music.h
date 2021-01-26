#ifndef __MUSIC_H__
#define __MUSIC_H__

#define SOUND_TIP_FREQ 2000
#define SOUND_ERROR_FREQ 400
void alert();
void beep(uint16_t freq);
uint8_t command_mario(COMMAND_ARGS);

#endif