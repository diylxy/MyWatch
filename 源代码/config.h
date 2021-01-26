#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <STC8.H>
#include <intrins.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint16_t;
typedef signed int int16_t;
typedef unsigned long uint32_t;
typedef signed long int32_t;

typedef struct {
	uint8_t pressed;
	uint8_t longpressed;
	uint8_t pressing;
	uint8_t ignore;
	uint32_t press_time;
	uint8_t last_state;
} key_t;

extern key_t keyUp;
extern key_t keyDown;
extern key_t keyOK;


#define OLED_DATA P0
sbit OLED_CS = P2^3;
sbit OLED_DC = P2^1;
sbit OLED_RST = P2^2;
sbit OLED_WR = P2^6;
sbit OLED_RD = P2^7;

sbit INT_SCL = P2^5;
sbit INT_SDA = P2^4;

sbit FLASH_CS = P1^2;

#define BAT_ADC 7

sbit KEY_OK = P3^2;
sbit KEY_UP = P3^4;
sbit KEY_DOWN = P3^3;

sbit CHARGE = P4^1;
sbit INT_DS3231 = P3^7;

sbit BUZZER = P2^0;

sbit JDY_CS = P3^5;
sbit JDY_SET = P4^2;
/*
#define MODE_TIME 0
#define MODE_TIME_SET 1
#define MODE_TIME_SET_CONFIRM 2
*/
#define LED_RFID 0x8899
#define LED_DVID 0x1122
#define LED_CHANNEL 1

extern uint8_t currentmode;
extern uint32_t millis;
extern uint32_t lastpress_millis;
extern bit left_key_pressing;
extern bit right_key_pressing;
extern bit ignore_next_key;
extern bit morse_mode;
extern bit morse_finished;
extern bit morse_updated;
extern bit keep_screen_on;
extern bit terminal_enable_serial;
extern bit resetprotect;
extern uint8_t beeping;
extern volatile bit alarming;

void keyClear();
void delay10ms(uint32_t time);
#define MENU_PASSWORD "lxy"

typedef uint8_t bool;
#define true 1
#define false 0
//////////////////////////////////////////////////////////////////////////////////
#define AUTO_SLEEP_TIME 600
#define KEY_LONG_PRESS_TIME 100
#define KEY_RESTART_TIME 500
//////////////////////////////////////////////////////////////////////////////////
#define COMMAND_MAX_ARGS 10
#define COMMAND_MAX_ARG_LEN 15

#define COMMAND_ARG_ERROR 1
#define COMMAND_NOT_FOUND 2
#define COMMAND_ERROR 3
#define COMMAND_QUIT_TERMINAL 4
#define COMMAND_OK 0

//COMMAND参数列表
#define COMMAND_ARGS uint8_t argc, uint8_t argv [][COMMAND_MAX_ARG_LEN+1]
//////////////////////////////////////////////////////////////////////////////////
#define FLASH_GB2312_START 0
#define FLASH_SETTINGS_START 0x40000
#define FLASH_ALARM_START 0x41000
#define FLASH_VIDEO_START 0x140000
///////////////////////////////////////////////////////////////////////////////////


#include "settings.h"
#include "oled.h"
#include "flash.h"
#include "rtc.h"
#include "adc.h"
#include "internal_iic.h"
#include "Terminal.h"
#include "morse.h"
#include "alarm.h"
#include "music.h"
#include "menu.h"
#include "serial.h"
#endif