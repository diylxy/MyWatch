#include "config.h"

uint8_t volatile currentmode = 0;
uint32_t volatile millis = 0;
uint32_t volatile lastpress_millis = 0;


key_t keyUp = {0,0,0,0,0,1};
key_t keyDown = {0,0,0,0,0,1};
key_t keyOK = {0,0,0,0,0,1};

bit left_key_pressing = 0;
bit right_key_pressing = 0;
bit ignore_next_key = 0;
bit morse_mode = 0;
bit morse_finished = 0;
bit morse_updated = 0;
bit keep_screen_on = 0;
bit terminal_enable_serial = 0;
uint8_t beeping = 0;
volatile bit alarming = 0;
bit resetprotect = 0;

void keyClear()
{
	keyUp.pressed = keyDown.pressed = keyOK.pressed = 0;
	keyUp.longpressed = keyDown.longpressed = keyOK.longpressed = 0;
}

////////////////////////////////////////////
void delay10ms(uint32_t time)
{
	uint32_t oldmillis=millis;
	while(millis - oldmillis < time);
}

void Timer0Init(void)		//10毫秒@24.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xE0;		//设置定时初值
	TH0 = 0xB1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	ET0 = 1;
	TR0 = 1;		//定时器0开始计时
}

void Timer1Init(void)		//5毫秒@24.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xF0;		//设置定时初值
	TH1 = 0xD8;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	ET1 = 1;
	TR1 = 1;		//定时器1开始计时
}




void GPUUpdate(void) interrupt 3
{
	static uint8_t lasttm = 0;
	++lasttm;
	if(lasttm > 2)
	{
		TR1=0;
		lasttm = 0;
		oled_clear_buffer();
		oled_proc_sprites();
		oled_send_buffer();
		TR1=1;
	}
}

void PowerOff()
{
	oled_wr_cmd(0xAE);
	oled_wr_cmd(0xAE);
	oled_wr_cmd(0xAE);
	 ADC_CONTR = 0;
	_nop_();
	_nop_();
	PCON = 0x02;
	_nop_();
	_nop_();
	ADC_CONTR = ADC_POWER;
	oled_wr_cmd(0xAF);
	oled_wr_cmd(0xAF);
	oled_wr_cmd(0xAF);
	if(KEY_OK == 0)keyOK.ignore = 1;
	if(KEY_DOWN == 0)keyDown.ignore = 1;
}

void main(void)
{
	uint8_t test;
	BUZZER = 0;
	Timer0Init();
	WDT_CONTR = 0x24;                           //使能看门狗,溢出时间约为1s
	P2M0 = 0x01;
	ADC_CONTR = ADC_POWER;
	ADCCFG = 0x27;
	IT0 = 1;                                    //使能INT1下降沿中断
  EX0 = 1;                                    //使能INT1中断
	IT1 = 1;                                    //使能INT1下降沿中断
  EX1 = 1;                                    //使能INT1中断
	ES = 1;
	INTCLKO = EX3;                              //使能INT3中断
	EA = 1;	
	keep_screen_on =0;
	flash_init();
	settings_load();
	serial_init();
	serial_set_timeout(100);
	oled_init();
	Timer1Init();
	oled_init_sprites();
	oled_start_refresh();
	rtc_clear_alarm_flag();
	class_update();
	oled_stop_refresh();
	serial1_sendstr("Hello World");

	while(1)
	{
		menu_init();
		menu_add("课程管理");
		menu_add("当前课程");
		menu_add("允许闹钟");
		menu_add("开始倒计时");
		menu_add("音量调节");
		menu_add("灯");
		test = menu_start("主菜单");
		switch (test)
		{
			case 0:
			default:
				TimeMode();
				break;
			case 1:
				class_manager();
				break;
			case 2:
				if(have_class == 1)
				{
						oled_clear_sprites();
						print_class(&current_class);
				}
				else
				{
					oled_sprite_change_gb2312(0,0,0,"暂无课程");
				}
				keyClear();
				while(keyUp.pressed == 0);
				keyUp.pressed = 0;
				keyUp.longpressed = 0;
				break;
			case 3:
				settings.enable_alert = menu_ask_yn(NULL);
				if(settings.enable_alert == 0xff)settings.enable_alert=0;
				settings_save();
				break;
			case 4:
				alert_countdown();
				break;
			case 5:
				settings.volume = menu_ask_num(0,30,NULL);
				settings_save();
				alert();
				break;
			case 6:
				menu_init();
				menu_add("开灯");
				menu_add("关灯");
				menu_add("初始化RF");
				test = menu_start("灯");
				oled_stop_refresh();
				keep_screen_on = 1;
				rf_enable(1);
				delay10ms(10);
				switch (test)
				{
					case 0:break;
					case 1:
						rf_sendstr("on");
						rf_sendstr("on");
						rf_sendstr("on");
						break;
					case 2:
						rf_sendstr("of");
						rf_sendstr("of");
						rf_sendstr("of");
						break;
					case 3:
						rf_set_rfid(LED_RFID);
						rf_set_dvid(LED_DVID);
						rf_set_channel(LED_CHANNEL);
						break;
					default:break;
				}
				delay10ms(20);
				rf_enable(0);
				keep_screen_on = 0;
				break;
		}
	}
}

void Timer0_isr() interrupt 1
{
	millis++;
	WDT_CONTR |= 0x10;                      //清看门狗,否则系统复位
	if(beeping != 0)
	{
		P_SW2 |= 0x80;
		PWM0T1 -= PWM0T1 /15; //输出低电平
		if(PWM0T1 == 0)
		{
			beeping = 0;
			PWMCR = 0;
			BUZZER = 0;
		}
		else if(PWM0T1 < 50)
		{
			PWM0T1--;
		}
		P_SW2 &= 0x7f; 
	}
	if(keep_screen_on == 1)
	{
		lastpress_millis = millis;
	}
	else if(millis - lastpress_millis >= AUTO_SLEEP_TIME)
	{
		PowerOff();
	}
	if(morse_mode == 1)
	{
		morseUpdate();
	}

/////////////////////////////////////按键处理	
	if(KEY_UP == 0)
	{
		if(keyUp.last_state == 1)
		{
			keyUp.last_state = 0;
			keyUp.press_time=0;
		}
		else
		{
			lastpress_millis = millis;
			keyUp.press_time++;
			keyUp.pressing = 1;
			if(keyUp.press_time == KEY_LONG_PRESS_TIME)
			{
				keyUp.longpressed = 1;
			}
		}
	}
	else
	{
		if(keyUp.last_state == 0)
		{
			keyUp.pressing = 0;
			keyUp.last_state = 1;
			keyUp.pressed = 1;
			if((keyUp.press_time >= KEY_LONG_PRESS_TIME) && (keyUp.longpressed == 0))
			{
				keyUp.pressed = 0;
			}
			if(keyUp.ignore == 1)
			{
				keyUp.ignore = 0;
				keyUp.pressed = keyUp.longpressed = 0;
			}
		}
	}
	if(KEY_DOWN == 0)
	{
		if(keyDown.last_state == 1)
		{
			keyDown.last_state = 0;
			keyDown.press_time=0;
		}
		else
		{
			lastpress_millis = millis;
			keyDown.press_time++;
			keyDown.pressing = 1;
			if(keyDown.press_time == KEY_LONG_PRESS_TIME)
			{
				keyDown.longpressed = 1;
			}
		}
	}
	else
	{
		if(keyDown.last_state == 0)
		{
			keyDown.pressing = 0;
			keyDown.last_state = 1;
			keyDown.pressed = 1;
			if((keyDown.press_time >= KEY_LONG_PRESS_TIME) && (keyDown.longpressed == 0))
			{
				keyDown.pressed = 0;
			}
			if(keyDown.ignore == 1)
			{
				keyDown.ignore = 0;
				keyDown.pressed = keyDown.longpressed = 0;
			}
		}
	}
	if(KEY_OK == 0)
	{
		if(keyOK.last_state == 1)
		{
			keyOK.last_state = 0;
			keyOK.press_time=0;
		}
		else
		{
			lastpress_millis = millis;
			keyOK.press_time++;
			keyOK.pressing = 1;
			if(keyOK.press_time == KEY_LONG_PRESS_TIME)
			{
				keyOK.longpressed = 1;
			}
		}
		if(keyOK.press_time >= KEY_RESTART_TIME)
		{
			if(resetprotect == 0)
			{
				IAP_CONTR = 0x60;
				_nop_();
			}
			else
			{
				beep(SOUND_ERROR_FREQ);
			}
		}
	}
	else
	{
		if(keyOK.last_state == 0)
		{
			keyOK.pressing = 0;
			keyOK.last_state = 1;
			keyOK.pressed = 1;
			if((keyOK.press_time >= KEY_LONG_PRESS_TIME) && (keyOK.longpressed == 0))
			{
				keyOK.pressed = 0;
			}
			if(keyOK.ignore == 1)
			{
				keyOK.ignore = 0;
				keyOK.pressed = keyOK.longpressed = 0;
			}
		}
	}
	
////////////////////////////////////////////

}

void INT0_Isr() interrupt 0
{
	lastpress_millis = millis;
}


void INT1_Isr() interrupt 2
{
	lastpress_millis = millis;
}



void UART1_Isr() interrupt 4
{
	static uint8_t count = 0;
	uint8_t last_chr;
	uint8_t chr;
	lastpress_millis = millis;
    if (TI)
    {
        //TI = 0;                                 //清中断标志
    }
    if (RI)
    {
			chr = SBUF;
			RI = 0;                                 //清中断标志	
			if(terminal_enable_serial == 1)
			{
        
				terminal_add_chr(chr);
			}
			if(chr == 0x7f)
			{
				count ++;
				if(count == 100)
				{
					IAP_CONTR = 0x60;
				}
			}
			else
			{
				last_chr = chr;
				count = 0;
			}
    }
}

void INT3_Isr() interrupt 11
{
	lastpress_millis = millis;
	alarming = 1;
}

