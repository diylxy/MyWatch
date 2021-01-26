#ifndef __ALARM_H__
#define __ALARM_H__

typedef enum{
	ONCE=0,
	COUNTDOWN=1,
	MUTE=2
}ringtype_t;
typedef struct
{
	uint8_t week;
	uint8_t type;
	uint16_t begin_min;
	uint16_t over_min;
	ringtype_t ring;
	uint8_t ring_before;
}class_t;

extern class_t current_class;
extern uint8_t have_class;

void rtc_enable_alarm(bit enable);
uint8_t rtc_alarm_enabled();
uint16_t rtc_get_alarm();
void rtc_set_alarm(uint16_t time, uint8_t ring_before);
void rtc_clear_alarm_flag();	//Çå¿ÕRTC×´Ì¬¼Ä´æÆ÷

void print_class(class_t *cls);
void class_write(uint8_t classnum, class_t *cls);
void class_read(uint8_t classnum, class_t *cls);
void class_erase(uint8_t classnum);
uint8_t classequ(class_t *cls1, class_t *cls2);
uint8_t class_add(class_t *cls);
uint8_t class_remove(class_t *cls);
uint8_t class_get_day(uint8_t week, uint8_t classnum, class_t *cls);
void class_update();
uint8_t menu_ask_class_type();
void class_manager();

uint8_t command_alarm(COMMAND_ARGS);

#endif