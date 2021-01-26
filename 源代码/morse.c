#include "config.h"

unsigned char* code MorseTable[] = {
	".-",
	"-...",
	"-.-.",
	"-..",
	".",
	"..-.",
	"--.",
	"....",
	"..",
	".---",
	"-.-",
	".-..",
	"--",
	"-.",
	"---",
	".--.",
	"--.-",
	".-.",
	"...",
	"-",
	"..-",
	"...-",
	".--",
	"-..-",
	"-.--",
	"--..",
	"-----",
	".----",
	"..---",
	"...--",
	"....-",
	".....",
	"-....",
	"--...",
	"---..",
	"----."
};
unsigned char code AsciiTable[] = "abcdefghijklmnopqrstuvwxyz0123456789";


unsigned char morse_input_str[MAX_INPUT_STR] = {0};				//ÊäÈëµÄ×Ö·û´®
unsigned char morse_input_chr[MAX_INPUT_CHR] = {0};								//ÊäÈëµÄ×Ö·û
bit lastState = 1;
uint8_t strPointer = 0;									//×Ö·û´®Ö¸Õë
uint8_t chrPointer = 0;										//×Ö·ûÖ¸Õë

uint32_t key_release_time = 0;
#define MORSE_KEY keyOK
#define MORSE_KEY_PIN KEY_OK
#define MORSE_DEL_KEY keyUp
void handleStrOverflow()
{
	
}

void handleNotFound()
{
	
}

void findMorse()
{
	unsigned char i;
	morse_input_chr[chrPointer] = 0;
	for(i = 0; i < MORSE_TOTAL; ++i)
	{
		if(strcmp(morse_input_chr, MorseTable[i]) == 0)
		{
			morse_input_str[strPointer++] = AsciiTable[i];
			if(strPointer == MAX_INPUT_STR)
			{
				morse_input_str[strPointer] = 0;
				handleStrOverflow();
				strPointer = 0;
			}
			chrPointer = 0;
			morse_input_str[strPointer] = 0;
			morse_updated = 1;
			return;
		}
	}
	handleNotFound();
	chrPointer = 0;
}


void morseUpdate()
{
	if(MORSE_KEY_PIN == 0)
	{
		key_release_time = 0;
		if(MORSE_KEY.press_time == KEY_LONG_PRESS_TIME)
		{
			morse_finished = 1;
			strPointer = 0;
			return;
		}
	}
	else		//KEY == 1
	{
		if(key_release_time == 1)
		{
			if(MORSE_KEY.press_time == 0)
			{
				
			}
			else if(MORSE_KEY.press_time < DA_TIME)			//.
			{
				morse_input_chr[chrPointer++] = '.';
			}
			else if(MORSE_KEY.press_time < KEY_LONG_PRESS_TIME)								//-
			{
				morse_input_chr[chrPointer++] = '-';
			}
			if(chrPointer == MAX_INPUT_CHR)
			{
				chrPointer = 0;
			}
		}
		else if(key_release_time == CHR_TIME && chrPointer != 0)
		{
			morse_input_chr[chrPointer] = 0;
			findMorse();
			chrPointer = 0;
			return;
		}
		else if(key_release_time == SPACE_TIME)
		{
			morse_input_str[strPointer++] = ' ';
			morse_input_str[strPointer] = 0;
			if(strPointer == MAX_INPUT_STR)strPointer = 0;
			morse_updated = 1;
		}
		key_release_time++;
	}
	if(MORSE_DEL_KEY.pressed == 1)
	{
		MORSE_DEL_KEY.pressed = 0;
		if(strPointer != 0)
		{
			strPointer--;
			morse_input_str[strPointer] = 0;
			morse_updated = 1;
		}
	}
	else if(MORSE_DEL_KEY.longpressed == 1)
	{
		MORSE_DEL_KEY.longpressed = 0;
		morse_mode = 0;
	}
}