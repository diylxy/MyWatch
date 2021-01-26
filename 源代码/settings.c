#include "config.h"

setting_t settings;
void settings_load()
{
	flash_read(FLASH_SETTINGS_START, sizeof(settings), (uint8_t *)&settings);
	if(settings.empty == 0xff)
	{
		settings_default();						//…Ë÷√Œ™ø’
		settings_save();
	}
	if(settings.volume == 0xff)settings.volume = 25;
}

void settings_save()
{
	flash_erase_sector(FLASH_SETTINGS_START);
	flash_write(FLASH_SETTINGS_START, sizeof(settings), (uint8_t *)&settings);
}


void settings_default()
{
	settings.empty = 0;
	settings.stop_year = 0;
	settings.stop_month = 1;
	settings.stop_date = 1;
	settings.enable_alert = 0;
	settings.volume = 2;
}