#ifndef __SETTINGS_H__
#define __SETTINGS_H__


typedef struct 
{
	uint8_t empty;
	uint8_t stop_year;
	uint8_t stop_month;
	uint8_t stop_date;
	uint8_t enable_alert;
	uint8_t volume;
}setting_t;

extern setting_t settings;

void settings_load();
void settings_default();
void settings_save();


#endif