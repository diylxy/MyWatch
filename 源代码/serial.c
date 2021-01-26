#include "config.h"

uint16_t timeout = 0;
bit en = 1;
volatile bit busy = 0;
volatile bit ri2 = 0;
volatile uint8_t tmps2buf;
void serial_init()		//9600bps@24.000MHz
{
	//串口1
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	PS = 1;
	IP2 = 1;
	PT0 =1 ;// PT1= 1;
	//串口2
	S2CON = 0x50;
	P1M0 |= 0x02;
	P3M0 |= 0x02;
	P1M1 = P3M1 = 0;
	P10 = P30 = P11 = P31 = 1;IE2 = 0x01;
}
void serial_set_timeout(uint16_t _timeout)
{
	timeout = _timeout;
}
void serial1_sendstr(uint8_t *str)
{
	TI = 0;
	while(*str)
	{
		SBUF = *str;
		while(TI == 0);
		TI = 0;
		++str;
	}
}
void serial2_send(char dat)
{
	if(en == 1)return;
	while (busy);
  busy = 1;
  S2BUF = dat;
}
char serial2_read()
{
	uint32_t start = millis;
//	ri2 = 0;
	while(ri2 == 0)
	{
		if((millis - start > timeout) && (timeout != 0))return 1;
	}
	ri2 = 0;
	return tmps2buf;
}
void serial2_sendstr(uint8_t *str)
{
	S2CON &= ~0x02;
	while(*str)
	{
		serial2_send(*str);
		++str;
	}
}
bit serial1_getdata(uint8_t *buffer, uint16_t size)
{
	uint32_t start = millis;
	uint16_t i = 0;
	RI = 0;
	while(size)
	{
		while(RI == 0)
		{
			if((millis - start > timeout) && timeout != 0)return 1;
		}
		RI = 0;
		*(buffer + i) = serial2_read();
		++i;
		--size;
	}
	return 0;
}
bit serial2_getdata(uint8_t *buffer, uint16_t size)
{
	uint32_t start = millis;
	uint16_t i = 0;
	S2CON &= ~0x01;
	while(size)
	{
		*(buffer + i) = serial2_read();
		++i;
		--size;
	}
	return 0;
}
void rf_set_rfid(uint16_t rfid)
{
	JDY_SET = 0;
	delay10ms(5);
	sprintf(oled_tmpstr, "AT+RFID%04x\r\n", rfid);
	serial2_sendstr(oled_tmpstr);
	rf_read();
	delay10ms(5);
	JDY_SET = 1;
}
void rf_set_dvid(uint16_t dvid)
{
	JDY_SET = 0;
	delay10ms(5);
	sprintf(oled_tmpstr, "AT+DVID%04x\r\n", dvid);
	serial2_sendstr(oled_tmpstr);
	rf_read();
	delay10ms(5);
	JDY_SET = 1;
}
void rf_set_power(uint8_t power)
{
	if(power > 9)power = 9;
	JDY_SET = 0;
	delay10ms(5);
	sprintf(oled_tmpstr, "AT+POWE%bd\r\n", power);
	serial2_sendstr(oled_tmpstr);
	rf_read();
	delay10ms(5);
	JDY_SET = 1;
}

void rf_set_channel(int8_t channel)
{
	if(channel < 0)return;
	JDY_SET = 0;
	delay10ms(5);
	sprintf(oled_tmpstr, "AT+RFC%03bd\r\n", channel);
	serial2_sendstr(oled_tmpstr);
	rf_read();
	delay10ms(5);
	JDY_SET = 1;
}


void rf_enable(bit enable)
{
	/*
	if(enable)
	{
		IE2 |= 0x01;
	}
	else
	{
		IE2 &= 0xfe;
	}*/
	en = JDY_CS = !enable;
	rf_read();
	delay10ms(5);
}


void Uart2Isr() interrupt 8
{
    if (S2CON & 0x02)
    {
        S2CON &= ~0x02;
        busy = 0;
    }
		if (S2CON & 0x01)
    {
        S2CON &= ~0x01;
				ri2 = 1;
        tmps2buf = S2BUF;
    }
}