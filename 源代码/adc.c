
#include "config.h"
uint8_t code battIconEmpty[] ={
	0xFF,0x81,0xBD,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,0x3C,
};

uint8_t code battIconLow[] ={
	0xFF,0x81,0xBD,0xBD,0xBD,0xBD,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,0x3C,
};

uint8_t code battIconHigh[] ={
	0xFF,0x81,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0x81,0x81,0x81,0x81,0xFF,0x3C,
};

uint8_t code battIconFull[] ={
	0xFF,0x81,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,0x81,0xFF,0x3C,
};

uint8_t code chargeIcon[] = {
	0x00, 0x10, 0x18, 0x9C, 0x72, 0x30, 0x10, 0x00
};
uint16_t code Vbg_ROM _at_ 0xfdf7;              //60K程序空间的MCU

#define ADC_POWER   0x80                    //ADC电源控制位

uint16_t GetADC(uint8_t ch)
{     
	uint16_t result;
  ADC_CONTR = ADC_POWER | ch | ADC_START;
  _nop_();                                //等待4个NOP
  _nop_();
  _nop_();
  _nop_();
  while (!(ADC_CONTR & ADC_FLAG));        //等待ADC转换完成
  ADC_CONTR &= ~ADC_FLAG;                 //清除ADC标志
  result = ADC_RES;
	result <<= 8;
	result |= ADC_RESL;
  return result;
}

uint16_t adc_get_voltage(uint8_t channel)
{
	 uint32_t average = 0;
	 uint8_t i;
	 ADC_RES = 0;
   GetADC(channel);
   GetADC(channel);
	for(i = 0; i < 8; ++i)
	{
   average += GetADC(channel);
	}
	average = average >> 3;

  average = average *6600/4096;
	return average;
}

void ADCUpdate()
{
	uint16_t battery_voltage = adc_get_voltage(BAT_ADC);
	if(CHARGE == 1)
	{
		if(battery_voltage > 3900)
			oled_draw_bitmap_fast(0, 55, battIconFull,16, 8, 0, 0);
		else if(battery_voltage > 3700)
			oled_draw_bitmap_fast(0, 55, battIconHigh, 16, 8, 0, 0);
		else if(battery_voltage > 3500)
			oled_draw_bitmap_fast(0, 55, battIconHigh, 16, 8, 0, 0);
		else if(battery_voltage > 3400)
			oled_draw_bitmap_fast(0, 55, battIconLow, 16, 8, 0, 0);
		else                           
			oled_draw_bitmap_fast(0, 55, battIconEmpty, 16, 8, 0, 0);
	}
	else
	{
		oled_draw_bitmap_fast(6, 55 ,chargeIcon, 8, 8, 0, 0);
	}
}