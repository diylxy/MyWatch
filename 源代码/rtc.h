#ifndef __RTC_H__
#define __RTC_H__
#define RTC_IIC_ADDRESS 0xD0
typedef struct
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint16_t minute_of_day;
}struct_time;

extern struct_time current_time;
extern uint8_t * code week_arr[7];

uint8_t dec_to_bcd(uint8_t dec);
uint8_t bcd_to_dec(uint8_t bcd);
void delay(uint16_t ms);	//@24.000MHz
void TimeUpdate();
void rtc_reset();
void rtc_read_time(struct_time *tm);
void rtc_set_time(struct_time *tm);
void TimeMode();

void alert_countdown();
uint8_t command_set_deadline(COMMAND_ARGS);
#endif