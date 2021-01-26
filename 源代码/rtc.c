#include "config.h"

struct_time current_time;
unsigned char code DaysInMonth[13]={0,31,28,31,30,31,30,31,31,30,31,30,31}; //二月日期数改成对应的闰年或平年
uint8_t * code week_arr[7] = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};

void delay(uint16_t ms)	//@24.000MHz
{
	unsigned char i, j;
	while(ms--){
		i = 32;
		j = 36;
		do
		{
			while (--j);
		} while (--i);
	}
}
uint8_t bcd_to_dec(uint8_t bcd)
{
	return 10 * (bcd >> 4) + (bcd & 0x0f);
}

uint8_t dec_to_bcd(uint8_t dec)
{
	return ((dec / 10) << 4 ) + (dec % 10);
}

void rtc_read_time(struct_time *tm)
{
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS);   
	delay(1);
  int_iic_write(0);   
  int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS | 0x01);   
	tm -> second = bcd_to_dec(int_iic_read(INTERNAL_IIC_ACK));
  tm -> minute = bcd_to_dec(int_iic_read(INTERNAL_IIC_ACK));
  tm -> hour = bcd_to_dec(int_iic_read(INTERNAL_IIC_ACK));
  tm -> day = bcd_to_dec(int_iic_read(INTERNAL_IIC_ACK));
  tm -> date = bcd_to_dec(int_iic_read(INTERNAL_IIC_ACK));
  tm -> month = bcd_to_dec(int_iic_read(INTERNAL_IIC_ACK));
  tm -> year = bcd_to_dec(int_iic_read(INTERNAL_IIC_NACK));
	tm -> minute_of_day = tm -> minute + tm -> hour * 60;
  int_iic_stop();  
}

void rtc_set_time(struct_time *tm)
{
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS);
  int_iic_write(0x00);
  int_iic_write(dec_to_bcd(tm -> second));
  int_iic_write(dec_to_bcd(tm -> minute));
  int_iic_write(dec_to_bcd(tm -> hour));
  int_iic_write(dec_to_bcd(tm -> day)); 
  int_iic_write(dec_to_bcd(tm -> date)); 
  int_iic_write(dec_to_bcd(tm -> month)); 
  int_iic_write(dec_to_bcd(tm -> year));
  int_iic_stop(); 
}





void rtc_offset(int8_t offset)
{
	int_iic_write_addr(RTC_IIC_ADDRESS, 0x10, int_iic_read_addr(RTC_IIC_ADDRESS, 0x10)+offset);
}

void time_to_str(uint8_t *str, struct_time *tm)
{
	str[0] = '2';
	str[1] = '0';
	str[2] = tm->year / 10 + 0x30;
	str[3] = tm->year % 10 + 0x30;
	str[4] = '-';
	str[5] = tm->month / 10 + 0x30;
	str[6] = tm->month % 10 + 0x30;
	str[7] = '-';
	str[8] = tm->date / 10 + 0x30;
	str[9] = tm->date % 10 + 0x30;
	str[10] = ' ';
	str[11] = tm->day + 0x30;
	str[12] = ' ';
	str[13] = tm->hour / 10 + 0x30;
	str[14] = tm->hour % 10 + 0x30;
	str[15] = ':';
	str[16] = tm->minute / 10 + 0x30;
	str[17] = tm->minute % 10 + 0x30;
	str[18] = ':';
	str[19] = tm->second / 10 + 0x30;
	str[20] = tm->second % 10 + 0x30;
	str[21] = 0;
}


uint16_t date_countdown(struct_time *time)
{
	int16_t calc = 0;
	uint8_t i;
	if(settings.stop_year > time->year)
	{
		calc +=( settings.stop_year - time->year ) * 365;
		for(i = time->year; i < settings.stop_year; ++i)
		{
			if(i % 4 == 0) calc++;
		}
	}
	if(time->month < settings.stop_month )
	{
		for(i = time->month ; i < settings.stop_month ; ++i)
		{
			calc += DaysInMonth[i];
			if(i == 2)
			{
				if(time->year % 4 == 0) calc++;
			}
		}
	}
	else if(time->month > settings.stop_month )
	{
		for(i =settings.stop_month ; i < time->month ; ++i)
		{
			calc -= DaysInMonth[i];
			if(i == 2)
			{
				if(time->year % 4 == 0) calc--;
			}
		}
	}
	calc += settings.stop_date;
	calc -= time->date;
	if(calc < 0)return 0;
	return calc;
}
//////////////////////////////////////////////////////////////////////////////


#define TIME_POS_X 1
#define TIME_POS_Y 20
#define TICKER_GAP 4

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t *bitmap;
	uint8_t w;
	uint8_t h;
	uint8_t offsetY;
	uint8_t val;
	uint8_t maxVal;
	bool moving;
} tickerData_t;

uint8_t code midFont[][57] ={
	{
		0xFE,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0xFE,
		0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
		0x7F,0xFF,0xFF,0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F
	},
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x3C,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00
	},
	{
		0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0xFE,
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0x7E,0x3F,0x1F,0x0F,0x07,0x03,0x01,
		0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xF7,0xF3,0xF1,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0
	},
	{
		0xFE,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0xFE,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x1C,0x1C,0x3E,0x3E,0x7E,0x7F,0xF7,0xF7,0xF7,0xE3,0xE3,
		0x7F,0xFF,0xFF,0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F
	},
	{
		0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF0,0xFC,0xFF,0x7F,0x1F,0x0F,0x03,0x01,0x00,0x00,0x00,0x00,
		0x80,0xC0,0xF0,0xFC,0xFE,0xFF,0x3F,0x0F,0x03,0x01,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
		0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F
	},
	{
		0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
		0x1F,0x1F,0x1F,0x1F,0x1F,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0xFE,0xFE,0xFE,0xFE,0xFC,
		0x7F,0xFF,0xFF,0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F
	},
	{
		0xFE,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x7F,0x7F,0x7F,0x7F,0x7E,
		0xFF,0xFF,0xFF,0xFF,0xFF,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0xFC,0xFC,0xFC,0xFC,0xF8,
		0x7F,0xFF,0xFF,0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F
	},
	{
		0x7F,0x7F,0x7F,0x7F,0x7F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xCF,0xFF,0xFF,0xFF,0xFF,0x1F,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF0,0xFC,0xFF,0xFF,0x3F,0x0F,0x03,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFE,0xFF,0xFF,0x1F,0x07,0x01,0x00,0x00,0x00,0x00,0x00
	},
	{
		0xFE,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0xFE,
		0xC3,0xE7,0xE7,0xF7,0xFF,0x7E,0x3E,0x3E,0x3C,0x1C,0x3C,0x3E,0x3E,0x7E,0xFF,0xF7,0xE7,0xE7,0xC3,
		0x7F,0xFF,0xFF,0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F
	},
	{
		0xFE,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0xFE,
		0x1F,0x3F,0x3F,0x3F,0x3F,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0xFF,0xFF,0xFF,0xFF,0xFF,
		0x7E,0xFE,0xFE,0xFE,0xFE,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F
	}
};
uint8_t code small2Font[][22] ={
	{
		0xFE,0xFF,0xFF,0x0F,0x07,0x07,0x07,0x0F,0xFF,0xFF,0xFE,
		0x7F,0xFF,0xFF,0xF0,0xE0,0xE0,0xE0,0xF0,0xFF,0xFF,0x7F,
	},
	{
		0x00,0x00,0x18,0x1C,0x1E,0xFF,0xFF,0xFF,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,
	},
	{
		0x1E,0x1F,0x1F,0x07,0x07,0x87,0xC7,0xE7,0xFF,0x7F,0x3E,
		0xF0,0xF8,0xFC,0xFE,0xEF,0xE7,0xE3,0xE1,0xE0,0xE0,0xE0,
	},
	{
		0x0E,0x0F,0x0F,0x07,0x07,0x87,0xC7,0xEF,0xFF,0x7F,0x3E,
		0x70,0xF0,0xF0,0xE0,0xE0,0xE1,0xE3,0xF7,0xFF,0xFE,0x7C,
	},
	{
		0xC0,0xE0,0xF0,0x78,0x3C,0x1E,0xFF,0xFF,0xFF,0x00,0x00,
		0x1F,0x1F,0x1F,0x1C,0x1C,0x1C,0xFF,0xFF,0xFF,0x1C,0x1C,
	},
	{
		0xFF,0xFF,0xFF,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0x87,0x07,
		0x71,0xF1,0xF1,0xE1,0xE1,0xE1,0xE1,0xF3,0xFF,0x7F,0x3F,
	},
	{
		0xFE,0xFF,0xFF,0x8F,0x87,0x87,0x87,0x87,0x8F,0x8F,0x0E,
		0x7F,0xFF,0xFF,0xE3,0xE3,0xE3,0xE3,0xE3,0xFF,0xFF,0x7F,
	},
	{
		0x0F,0x0F,0x0F,0x07,0x07,0x07,0x87,0xC7,0xFF,0xFF,0x7F,
		0xE0,0xF0,0x78,0x3C,0x1E,0x0F,0x07,0x03,0x01,0x00,0x00,
	},
	{
		0x7E,0xFF,0xFF,0xC7,0x87,0x87,0x87,0xC7,0xFF,0xFF,0x7E,
		0x7E,0xFF,0xFF,0xE3,0xE1,0xE1,0xE1,0xE3,0xFF,0xFF,0x7E,
	},
	{
		0xFE,0xFF,0xFF,0xC7,0xC7,0xC7,0xC7,0xC7,0xFF,0xFF,0xFE,
		0x78,0xF9,0xF9,0xE1,0xE1,0xE1,0xE1,0xE1,0xFF,0xFF,0x7F,
	}
};
uint8_t code colon[] ={
	0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0x00,
	0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0x00,
	0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x00
};
#define div10(val) val / 10
#define mod10(val) val % 10

#define FONT_COLON_WIDTH 6
#define FONT_COLON_HEIGHT 24

#define FONT_SMALL2_WIDTH 11
#define FONT_SMALL2_HEIGHT 16

#define MIDFONT_WIDTH 19
#define MIDFONT_HEIGHT 24
#define NOINVERT 0

uint8_t * code week_eng[] = {
	"ERR",
	"Mon",
	"Tue",
	"Wed",
	"Thr",
	"Feb",
	"Sat",
	"Sun",
};
uint8_t code smallFontAlarm[] ={
	0x06, 0x7B, 0x85, 0xBC, 0xA4, 0x95, 0x7B, 0x06
};

static void drawTickerNum(tickerData_t *dat)
{
	uint8_t arraySize = (dat->w * dat->h) / 8;
	uint8_t yPos = dat->offsetY;
	const uint8_t *bitmap = &dat->bitmap[dat->val * arraySize];
	uint8_t x = dat->x;
	uint8_t y = dat->y;

	if (!dat->moving || yPos == 0 || yPos == 255)
		oled_draw_bitmap_fast(x, y, bitmap, dat->w, dat->h, NOINVERT, 0);
	else
	{
		uint8_t prev = dat->val - 1;
		if (prev == 255)
			prev = dat->maxVal;

		oled_draw_bitmap_fast(x, y, bitmap, dat->w, dat->h, NOINVERT, yPos - dat->h - TICKER_GAP);
		oled_draw_bitmap_fast(x, y, &dat->bitmap[prev * arraySize], dat->w, dat->h, NOINVERT, yPos);
	}
}

void TimeMode_page1()
{
	static uint8_t yPos;
	static uint8_t yPos_secs;
	static bool moving = false;
	static bool moving2[5];
	tickerData_t dat;
	static uint8_t hour2;
	static uint8_t mins;
	static uint8_t secs;
	uint16_t tmp16;
	if (current_time.second != secs)
	{
		yPos = 0;
		yPos_secs = 0;
		moving = true;

		moving2[0] = div10(current_time.hour) != div10(hour2);
		moving2[1] = mod10(current_time.hour) != mod10(hour2);
		moving2[2] = div10(current_time.minute) != div10(mins);
		moving2[3] = mod10(current_time.minute) != mod10(mins);
		moving2[4] = div10(current_time.second) != div10(secs);

		hour2 = current_time.hour;
		mins = current_time.minute;
		secs = current_time.second;
	}

	if (moving)
	{
		if (yPos <= 3)
			yPos++;
		else if (yPos <= 6)
			yPos += 3;
		else if (yPos <= 16)
			yPos += 5;
		else if (yPos <= 22)
			yPos += 3;
		else if (yPos <= 24 + TICKER_GAP)
			yPos++;

		if (yPos >= MIDFONT_HEIGHT + TICKER_GAP)
			yPos = 255;

		if (yPos_secs <= 1)
			yPos_secs++;
		else if (yPos_secs <= 13)
			yPos_secs += 3;
		else if (yPos_secs <= 16 + TICKER_GAP)
			yPos_secs++;

		if (yPos_secs >= FONT_SMALL2_HEIGHT + TICKER_GAP)
			yPos_secs = 255;

		if (yPos_secs > FONT_SMALL2_HEIGHT + TICKER_GAP && yPos > MIDFONT_HEIGHT + TICKER_GAP)
		{
			yPos = 0;
			yPos_secs = 0;
			moving = false;
			memset(moving2, false, sizeof(moving2));
		}
	}

	// Seconds
	dat.x = 104;
	dat.y = 28;
	dat.bitmap = (uint8_t *)&small2Font;
	dat.w = FONT_SMALL2_WIDTH;
	dat.h = FONT_SMALL2_HEIGHT;
	dat.offsetY = yPos_secs;
	dat.val = div10(current_time.second);
	dat.maxVal = 5;
	dat.moving = moving2[4];
	drawTickerNum(&dat);

	dat.x = 116;
	dat.val = mod10(current_time.second);
	dat.maxVal = 9;
	dat.moving = moving;
	drawTickerNum(&dat);

	// Set new font dat for hours and minutes
	dat.y = TIME_POS_Y;
	dat.w = MIDFONT_WIDTH;
	dat.h = MIDFONT_HEIGHT;
	dat.bitmap = (uint8_t *)&midFont;
	dat.offsetY = yPos;

	// Minutes
	dat.x = 60;
	dat.val = div10(current_time.minute);
	dat.maxVal = 5;
	dat.moving = moving2[2];
	drawTickerNum(&dat);

	dat.x = 83;
	dat.val = mod10(current_time.minute);
	dat.maxVal = 9;
	dat.moving = moving2[3];
	drawTickerNum(&dat);

	// Hours
	dat.x = 1;
	dat.val = div10(current_time.hour);
	dat.maxVal = 5;
	dat.moving = moving2[0];
	drawTickerNum(&dat);

	dat.x = 24;
	dat.val = mod10(current_time.hour);
	dat.maxVal = 9;
	dat.moving = moving2[1];
	drawTickerNum(&dat);

	oled_draw_bitmap_fast(TIME_POS_X + 46 + 2, TIME_POS_Y, colon, FONT_COLON_WIDTH, FONT_COLON_HEIGHT, NOINVERT, 0);
	
	//日期
	sprintf(oled_tmpstr, "20%02bu-%02bu-%02bu %s", current_time.year, current_time.month, current_time.date, week_eng[current_time.day]);
	oled_draw_6x8(21, 0, oled_tmpstr);
	
	//闹钟
	tmp16 = rtc_get_alarm();
	oled_draw_bitmap_fast(24,55,smallFontAlarm, 8,8,0,0);
	sprintf(oled_tmpstr, "%02bx:%02bx", (uint8_t)(tmp16>>8), (uint8_t)(tmp16&0xff));
	oled_draw_6x8(34, 7, oled_tmpstr);
	if(current_class.ring == MUTE || settings.enable_alert == 0)
		oled_draw_6x8(64,7,"[M]");
	else if(current_class.ring == COUNTDOWN)
		oled_draw_6x8(64,7,"[CD]");
	
	//电池
	ADCUpdate();
}

//////////////////////////////////////////////////////////////////////////////
void TimeMode()
{
	static uint32_t lastmillis = 0;
	static set_position = 0;
	static int8_t change;
	static uint8_t page = 0;
	while(1)
	{
		timemode:
		keep_screen_on = 0;
		if(alarming == 1)
		{
			alarming = 0;
			rtc_clear_alarm_flag();
			if(settings.enable_alert != 0)
			{
				if(current_class.ring == ONCE)
				{
					alert();
				}
				else if(current_class.ring == COUNTDOWN)
				{
					alert_countdown();
				}
			}
			class_update();
		}
		if(page == 0)
		{
			oled_stop_refresh();
			oled_clear_buffer();
			rtc_read_time(&current_time);
			TimeMode_page1();
			oled_send_buffer();
		}
		else if(page == 1)
		{
			print_class(&current_class);
			sprintf(oled_tmpstr, "倒计时:%u天", date_countdown(&current_time));
			oled_sprite_change_gb2312(4, 0, 64-16, oled_tmpstr);
		}
		if(keyUp.pressed)
		{
			keyUp.pressed = 0;
			if(page == 1)
			{
				oled_clear_sprites();
				delay10ms(30);
				page = 0;																		//时间
			}
		}
		if(keyDown.pressed)
		{
			keyDown.pressed = 0;
			oled_start_refresh();
			page = 1;																		//课程
		}
		if(keyDown.longpressed)
		{
			keyDown.longpressed = 0;
			TerminalMode();
		}
		if(keyOK.pressed)
		{
			keyOK.pressed = 0;
			oled_start_refresh();
			return;
		}
		if(keyUp.longpressed)
		{
			keyUp.longpressed = 0;
			oled_enable_inverse(1);
			oled_stop_refresh();
			oled_inverse(0,0,23,7);
			oled_clear_sprites();
			oled_start_refresh();
			rtc_read_time(&current_time);
			time_to_str(&oled_tmpstr, &current_time);
			oled_sprite_change_6x8(0,0,0,oled_tmpstr);
			oled_start_refresh();
			delay10ms(50);
			while(1)
			{
				if(millis - lastmillis > 30)
				{
					lastmillis = millis;
					if(keyOK.longpressed)
					{
						keyOK.longpressed = 0;
						if(set_position != 0)set_position--;
					}
					if(keyOK.pressed)
					{
						keyOK.pressed = 0;
						set_position++;
						if(set_position == 7)
						{
							set_position = 0;
							time_to_str(oled_tmpstr, &current_time);
							oled_stop_refresh();
							oled_inverse(20,45,108,47);
							while(KEY_DOWN == 0 || KEY_UP == 0 || KEY_OK == 0);
							oled_sprite_change_6x8(3,30,0,"Confirm...");
							oled_sprite_change_6x8(0,0,35,oled_tmpstr);
							oled_start_refresh();
							delay10ms(20);
							while(1)
							{
								if(KEY_OK == 0)
								{
									keyOK.ignore = 1;
									oled_stop_refresh();
									oled_clear_sprites();
									rtc_set_time(&current_time);
									oled_start_refresh();
									goto timemode;
								}
								else if(KEY_DOWN == 0)
								{
									keyDown.ignore = 1;
									oled_stop_refresh();
									oled_clear_sprites();
									oled_start_refresh();
									goto timemode;
								}
								else if(KEY_UP == 0)
								{
									keyUp.ignore = 1;
									break;
								}
							}
						}
					}
					if(keyUp.pressing == 1)
					{
						keyUp.ignore = 1;
						change = 1;
					}
					else if(keyDown.pressing == 1)
					{
						keyDown.ignore = 1;
						change = -1;
					}
					else
					{
						change = 0;
					}
					
					switch(set_position)
					{
						case 0:				//year
							current_time.year += change;
							if(current_time.year > 99)current_time.year = 0;
							oled_inverse(0,0,23,7);
							break;
						case 1:
							current_time.month += change;
							if(current_time.month > 12)current_time.month = 1;
							if(current_time.month == 0)current_time.month = 12;
							oled_inverse(29,0,41,7);
							break;
						case 2:
							current_time.date += change;
							if(current_time.date > 31)current_time.date = 1;
							if(current_time.date == 0)current_time.date = 30;
							oled_inverse(47, 0, 59, 7);
							break;
						case 3:
							current_time.day += change;
							if(current_time.day > 7) current_time.day = 1;
							oled_inverse(65, 0, 71, 7);
							break;
						case 4:
							current_time.hour += change;
							if(current_time.hour > 23)current_time.hour = 0;
							oled_inverse(77, 0, 89, 7);
							break;
						case 5:
							current_time.minute += change;
							if(current_time.minute > 60)current_time.minute = 0;
							oled_inverse(95,0,107,7);
							break;
						case 6:
							current_time.second += change;
							if(current_time.second > 60) current_time.second = 0;
							oled_inverse(115, 0, 127, 7);
							break;
					}
					time_to_str(&oled_tmpstr, &current_time);
					oled_stop_refresh();
					oled_sprite_change_6x8(0,0,0,oled_tmpstr);
					oled_start_refresh();
				}
			}
		}
	}
}


void alert_countdown()
{
	uint8_t second_last = 99;
	uint8_t i = 0;
	terminal_add_bottom("倒计时");
	terminal_add_bottom("现在时间");
	rtc_read_time(&current_time);
	time_to_str(oled_tmpstr, &current_time);
	terminal_add_bottom(oled_tmpstr);
	keyDown.pressed = keyUp.pressed = 0;
	keep_screen_on = 1;
	terminal_add_bottom("开始！");
	alert();
	while(1)
	{
		if(second_last != bcd_to_dec(int_iic_read_addr(RTC_IIC_ADDRESS, 0)))
		{
			second_last = bcd_to_dec(int_iic_read_addr(RTC_IIC_ADDRESS, 0));
			sprintf(oled_tmpstr, "还有%bu秒", (60-second_last));
			terminal_modify(0, oled_tmpstr);
			if(60 - second_last == 6)break;
		}
		if(keyUp.pressed == 1)
		{
			keyDown.pressed = keyUp.pressed = 0;
			terminal_modify(0, "中断");
			keep_screen_on = 0;
			return ;
		}
		delay10ms(2);
	}
	oled_clear_sprites();
	beep(800);
	while(1)
	{
		if(second_last != bcd_to_dec(int_iic_read_addr(RTC_IIC_ADDRESS, 0)))
		{
			second_last = bcd_to_dec(int_iic_read_addr(RTC_IIC_ADDRESS, 0));
			if(second_last == 0)break;
			oled_remove_sprite(i);
			i = (i + 1) & 1;
			oled_sprite_change_pic(i, 47, 0, 32, 64, oled_font_64x32 + ((60 - (uint16_t)(second_last)))*256);
			beep(800);
		}
		if(keyUp.pressed == 1)
		{
			keyDown.pressed = keyUp.pressed = 0;
			terminal_modify(0, "中断");
			keep_screen_on = 0;
			return ;
		}
		delay10ms(2);
	}
	beep(1600);
	keep_screen_on = 0;
	oled_clear_sprites();
	delay10ms(25);
	return;
}

uint8_t command_set_deadline(COMMAND_ARGS)
{
	if(argc != 3)return COMMAND_ARG_ERROR;
	
	settings.stop_year = getint(argv[1]);
	settings.stop_month = getint(argv[2]);
	settings.stop_date = getint(argv[3]);
	settings_save();
	terminal_add_bottom("已修改倒计时");
	return COMMAND_OK;
}