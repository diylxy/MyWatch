#include "config.h"

#define CLASS_TYPE_TOTAL 13
uint8_t* code classtype[CLASS_TYPE_TOTAL+1] =
{
	"Math",
	"语文",
	"英语",
	"政治",
	"历史",
	"地理",
	"生物",
	"物理",
	"化学",
	"自习",
	"P.E.",
	"班会",
	"音美",
	NULL
};

uint8_t* code class_manager_menu[] = 
{
	"新建课程",
	"Delete",
	"格式化",
	"更新RTC",
	"Serial",
	""
};
uint8_t * code class_alarm_type[] = 
{
	"响一声",
	"倒计时",
	"静音",
};
class_t tmpclass;
class_t current_class;
uint8_t have_class = 0;
void rtc_enable_alarm(bit enable)
{
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS);
  int_iic_write(0x0e);
	if(enable == 1)
	{
		int_iic_write(0x55);
	}
	else
	{
		int_iic_write(0x54);
	}
	int_iic_stop();
}

uint8_t rtc_alarm_enabled()
{
	uint8_t enable;
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS);
	int_iic_write(0x0e);
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS+1);
  enable = int_iic_read(INTERNAL_IIC_ACK);
	int_iic_stop();
	if((enable&0x01) == 0x01)
		return 1;
	return 0;
}

uint16_t rtc_get_alarm()
{
	uint16_t result = 0xff;
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS);
	int_iic_write(0x08);
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS | 0x01);  
	result = int_iic_read(INTERNAL_IIC_ACK);
	result |= ((uint16_t)int_iic_read(INTERNAL_IIC_ACK))<<8;
	int_iic_stop();
	return result;
}
void rtc_set_alarm(uint16_t time, uint8_t ring_before)
{
	int_iic_start();   
  int_iic_write(RTC_IIC_ADDRESS);
	int_iic_write(0x07);
	if(ring_before == 0)
	{
		int_iic_write(0);
		ring_before = 0;
	}
	else
	{
		ring_before %= 60;
		int_iic_write(dec_to_bcd(60-(ring_before%60)));
		ring_before = 1;
	}
	if(time == 0)
	{
		int_iic_write(0);
		int_iic_write(0);
	}
	else
	{
		int_iic_write(dec_to_bcd((time-ring_before)%60));
		int_iic_write(dec_to_bcd((time-ring_before)/60));
	}
	int_iic_write(0x80);
	int_iic_stop();
	rtc_enable_alarm(1);
}

void rtc_clear_alarm_flag()															//清空RTC状态寄存器
{
	int_iic_write_addr(RTC_IIC_ADDRESS, 0x0f, 0x00);
}



///////////////////////////////////////////////////////////////////
/////////////////////////课程管理///////////////////////////////////
///////////////////////////////////////////////////////////////////

void print_class(class_t *cls)
{
	oled_inverse(0,0,0,0);
	//if(cls->type > CLASS_TYPE_TOTAL)return;
	oled_sprite_change_gb2312(0,0,0,classtype[cls->type]);
	sprintf(oled_tmpstr, "%02bu:%02bu-%02bu:%02bu", (uint8_t)(cls->begin_min/60),  (uint8_t)(cls->begin_min%60),  (uint8_t)(cls->over_min/60),  (uint8_t)(cls->over_min%60));
	oled_sprite_change_gb2312(1,0,15,oled_tmpstr);
	oled_sprite_change_gb2312(2,0,31,class_alarm_type[cls->ring]);
	oled_sprite_change_gb2312(3,64,0,week_arr[cls->week-1]);
}

void class_write(uint8_t classnum, class_t *cls)
{
	uint32_t addr;
	addr = classnum;
	addr *= 4096;
	addr += FLASH_ALARM_START;
	flash_erase_sector(addr);
	flash_write(addr, sizeof(class_t), (uint8_t *)cls);
	
}
void class_read(uint8_t classnum, class_t *cls)
{
	uint32_t addr;
	addr = classnum;
	addr *= 4096;
	addr += FLASH_ALARM_START;
	flash_read(addr, sizeof(class_t),(uint8_t *)cls);
}
void class_erase(uint8_t classnum)
{
	uint32_t addr;
	addr = classnum;
	addr *= 4096;
	addr += FLASH_ALARM_START;
	flash_erase_sector(addr);
}

uint8_t classequ(class_t *cls1, class_t *cls2)
{
	uint8_t i;
	for(i=1; i<sizeof(class_t); ++i)
	{
		if(*((uint8_t*)cls1 + i) != *((uint8_t*)cls2 + i))
		{
			return 1;
		}
	}
	return 0;
}

uint8_t class_add(class_t *cls)
{
	uint8_t i;
	for(i=0; i!= 0xff; ++i)
	{
		class_read(i, &tmpclass);
		if(tmpclass.week == 0xff)
		{
			class_write(i, cls);
			return 0;
		}
	}
	return 1;
}

uint8_t class_remove(class_t *cls)
{
	uint8_t i;
	for(i=0; i!= 0xff; ++i)
	{
		class_read(i, &tmpclass);
		if(classequ(&tmpclass,cls) == 0)
		{
			class_erase(i);
			return 0;
		}
	}
	return 1;
}

uint8_t class_get_day(uint8_t week, uint8_t classnum, class_t *cls)
{
	uint8_t i;
	for(i=0; i<255; ++i)
	{
		class_read(i, &tmpclass);
		if(tmpclass.week == week)
		{
			if(classnum == 0)
			{
				class_read(i, cls);
				return i;
			}
			classnum--;
		}
	}
	return 0xff;
}


void class_update()
{
	int16_t min_delta = 32767;
	uint16_t minutenow;
	uint8_t i=0;
	int16_t tmp16;
	class_t tmp;
	rtc_read_time(&current_time);
	minutenow = current_time.hour;
	minutenow *= 60;
	minutenow += current_time.minute;
	minutenow += 2;
	while(class_get_day(current_time.day, i, &tmp)!= 0xff)
	{
		if((tmp.over_min) > minutenow)
		{
			tmp16 = tmp.over_min - minutenow;
			if(tmp16 < min_delta)
			{
				min_delta = tmp16;
				class_get_day(current_time.day, i, &current_class);
			}
		}
		++i;
	}
	rtc_enable_alarm(1);	
	if(min_delta < 24 * 60)
	{
		have_class = 1;
		if(current_class.begin_min > minutenow)
		{
			rtc_set_alarm(current_class.begin_min, current_class.ring_before);
		}
		else
		{
			rtc_set_alarm(current_class.over_min, current_class.ring_before);
		}
		return;
	}
	rtc_set_alarm(0, 0);
	current_class.ring = MUTE;
	have_class = 0;
	return;
}

uint8_t menu_ask_class_type()
{
	uint8_t i = 0;
	menu_init();
	while(classtype[i] != NULL)
	{
		menu_add(classtype[i]);
		++i;
	}
	return menu_start("请选择科目")-1;
}

void class_send(class_t* cls)
{
	uint8_t i;
	for(i = 0; i < sizeof(class_t); ++i)
	{
		SBUF = *(((uint8_t *)cls)+i);
		while(TI == 0);
		TI = 0;
	}
}
void class_manager()
{
	uint8_t result;
	uint8_t week;
	class_t tmp;
	uint8_t tmp_buf, max_cls;
	//req_password();
	while(1)
	{
		menu_init();
		menu_add(class_manager_menu[0]);
		menu_add(class_manager_menu[1]);
		menu_add(class_manager_menu[2]);
		menu_add(class_manager_menu[3]);
		menu_add(class_manager_menu[4]);
		result = menu_start(NULL);
		switch(result)
		{
			case 0:return;break;
			case 1:																//新增
				week_jmp:
				result = menu_ask_num(1,8,"请选择星期");
				if(result == 0xff)break;
				tmp.week = result;
				
				type:
				result = menu_ask_class_type();
				if(result == 0xff)goto week_jmp;
				tmp.type = result;
				
				start_h:
				result = menu_ask_num(0,24,"上课时间(时)");
				if(result == 0xff)goto type;
				tmp.begin_min = result;
				tmp.begin_min *= 60;
				result = menu_ask_num(0,60,"上课时间(分)");
				if(result == 0xff)goto start_h;
				tmp.begin_min += result;
			
				over_h:
				result = menu_ask_num(0,24,"下课时间(时)");
				if(result == 0xff)goto start_h;
				tmp.over_min = result;
				tmp.over_min *= 60;
				result = menu_ask_num(0,60,"下课时间(分)");
				if(result == 0xff)goto over_h;
				tmp.over_min += result;
				alert:
				menu_init();
				menu_add("响一声");
				menu_add("倒计时");
				menu_add("静音");
				result = menu_start("设置提醒");
				result--;
				if(result == 0xff)goto over_h;
				tmp.ring = result;
				if(result != MUTE)
				{
					result = menu_ask_num(0,60,NULL);
					if(result == 0xff)goto alert;
					tmp.ring_before = result;
				}
				else
				{
					tmp.ring_before = 0;
				}
				oled_clear_sprites();
				print_class(&tmp);
				delay10ms(100);
				pop_msg("是否保存？");
				result = menu_ask_yn(NULL);
				if(result == 0xff)goto alert;
				if(result == 1)
				{
					if(class_add(&tmp) == 0)
					{
						pop_msg("保存成功");
					}
					else
					{
						pop_msg("保存失败");
					}
				}
				class_update();
				break;
			case 2:																		//删除
				result = menu_ask_num(1,8,"请选择星期");
				if(result == 0xff)break;
				week = result;
				result = 0;
				reselect_class:
				menu_init();
				while(class_get_day(week, result, &tmp) != 0xff)
				{
					menu_add(classtype[tmp.type]);
					result++;
				}
				result = menu_start("课程表");
				if(result == 0)break;
				result--;
				class_get_day(week, result, &tmp);
				oled_clear_sprites();
				print_class(&tmp);
				keyUp.pressed = 0;
				keyDown.pressed = 0;
				while(keyUp.pressed == 0);
				result = menu_ask_yn("是否删除");
				if(result == 0xff)break;
				else if(result == 1)
				{
					class_remove(&tmp);
				}
				else if(result == 0)goto reselect_class;
				class_update();
				break;
			case 3:
				req_password();
				result = menu_ask_yn("确认？");
				if(result == 1)
				{
					keep_screen_on = 1;
					for(result = 0; result != 255; ++result)
					{
						class_erase(result);
					}
					keep_screen_on = 0;
					pop_msg("成功");
				}
				break;
			case 4:
				class_update();
				pop_msg("RTC 已更新");
				break;
			case 5:
				oled_sprite_change_gb2312(0,0,0, "按下上键退出");
				ES = 0;
				keep_screen_on = 1;
				delay10ms(50);
				oled_stop_refresh();
				while(KEY_UP == 1)
				{
					if(RI == 1)
					{
						RI = 0;
						tmp_buf = SBUF;
						if(tmp_buf == 'r')
						{
							for(tmp_buf = 0; tmp_buf != 0xff; ++tmp_buf)
							{
								class_read(tmp_buf, &tmp);
								class_send(&tmp);
							}
						}
						else if(tmp_buf == 'w')
						{
							SBUF = 'o';
							while(TI == 0);
							TI = 0;
							while(RI == 0);
							RI = 0;
							max_cls = SBUF;
							for(result = 0; result != max_cls; ++result)
							{
								for(tmp_buf = 0; tmp_buf < sizeof(class_t); ++tmp_buf)
								{
									while(RI == 0);
									RI = 0;
									*(((uint8_t *)&tmp)+tmp_buf) = SBUF;
								}
								class_write(result, &tmp);
								SBUF = 'o';
								while(TI == 0);
								TI = 0;
							}
						}
					}
				}
				oled_start_refresh();
				ES = 1;
				class_update();
				keep_screen_on = 0;
				keyClear();
			default:
				break;
		}
	}
}

uint8_t command_alarm(COMMAND_ARGS)
{
	uint8_t hour; 
	uint8_t minute;
	if(argc != 2)return COMMAND_ARG_ERROR;
	
	hour =getint(argv[1]); 
	minute =getint(argv[2]);
	
	rtc_set_alarm(hour*60+minute, 0);
	current_class.ring = ONCE;
	terminal_add_bottom("闹钟已设置");
	return COMMAND_OK;
}