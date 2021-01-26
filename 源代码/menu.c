#include "config.h"

uint8_t node_buffer[512];

uint16_t node_total;
uint16_t node_top;
uint16_t node_selected;
uint8_t *node_top_ptr;

uint8_t *get_ptr(uint8_t num)
{
	uint16_t i;
	if(num == 0)
	{
		return node_buffer;
	}
	for(i = 0; i < 512; ++i)
	{
		if(node_buffer[i] == 0)
		{
			num--;
			if(num == 0)
			{
				return node_buffer+i+1;
			}
		}
	}
	return node_buffer;
}

void menu_init()
{
	node_total = 1;
	memcpy(node_buffer, "[返回]", 7);
	node_top_ptr = node_buffer+7;
	node_top = 0;
	node_selected = 0;
}

uint16_t menu_start(uint8_t *title)
{
	uint8_t i;
	terminal_set_lines(6);
	terminal_init();
	terminal_set_big_font(1);
	if(title != NULL)
	{
		oled_sprite_change_str_gb2312(0, title);
		i = oled_sprite_get_width(0);
		oled_sprite_move(0, (127-i)/2, 24);
		oled_inverse((127-i)/2,24,(127-i)/2 + i-1, 24+16);
		oled_enable_inverse(1);
		delay10ms(50);
	}
	for(i=4; i>0; --i)
	{
		if(i > node_total)
		{
			terminal_add_top("");
			continue;
		}
		terminal_add_top(get_ptr(i-1));
	}
	while((KEY_UP == 0) || (KEY_DOWN==0));
	delay10ms(2);
	keyClear();
	menu_inverse_refresh();
	while(1)
	{
		if(keyUp.pressing)
		{
			keyUp.pressed = 0;
			keyUp.ignore = 1;
			if(node_selected == 0)
			{
				while(node_selected != node_total-1)
				{
					menu_roll(1);
					delay10ms(5);
				}
			}
			else
			{
				menu_roll(0);
				delay10ms(15);
			}
		}
		if(keyDown.pressing)
		{
			keyDown.pressed = 0;
			keyDown.ignore = 1;
			if(node_selected == node_total-1)
			{
				while(node_selected != 0)
				{
					menu_roll(0);
					delay10ms(5);
				}
			}
			else
			{
				menu_roll(1);
				delay10ms(15);
			}
		}
		if(keyOK.pressed)
		{
			keyOK.pressed = 0;
			menu_end();
			return node_selected;
		}
		if(keyOK.longpressed)
		{
			keyOK.longpressed = 0;
			menu_end();
			return 0;
		}
	}
}
void menu_add(uint8_t *str)
{
	if(node_top_ptr - node_buffer == 512)return;
	while(*str)
	{
		*(node_top_ptr++)=*str++;
		if(node_top_ptr - node_buffer == 512)
		{
			*node_top_ptr = 0;
			return;
		}
	}
	*(node_top_ptr++)=*str;
	node_total++;
}

void menu_roll(bit direction)
{
	if(direction)
	{
		
		if(node_selected == node_total-1)
		{
			return;																				//已经到最底端
		}
		node_selected++;
		if(node_selected - node_top == 4)
		{
			//向下，且超出显示范围
			node_top++;
			terminal_add_bottom(get_ptr(node_top+3));
		}
		else
		{
			menu_inverse_refresh();
		}
	}
	else
	{
		if(node_selected == 0)return;
		node_selected--;
		if((node_selected - node_top) == -1)
		{
			node_top--;
			terminal_add_top(get_ptr(node_top));
		}
		else
		{
			menu_inverse_refresh();
		}
	}
}

void menu_end()
{
	terminal_set_big_font(0);
	oled_clear_sprites();
	oled_enable_inverse(0);
	terminal_set_lines(8);
}

void menu_inverse_refresh()
{
	uint8_t y_start = node_selected - node_top;

	y_start *= 16;
	y_start -= 1;
	oled_inverse(0, y_start, 127, y_start+16);
	oled_enable_inverse(1);
}



///////////////////////////////////////////////////////////
////////////////////////标准对话框//////////////////////////
///////////////////////////////////////////////////////////

uint8_t menu_ask_yn(uint8_t *tip)
{
	menu_init();
	menu_add("否");
	menu_add("是");
	return menu_start(tip)-1;
}

uint8_t menu_ask_num(uint8_t start, uint8_t end, uint8_t *tip)
{
	uint8_t str[5] = {0};
	uint8_t i;
	menu_init();
	for(i=start; i<end; ++i)
	{
		sprintf(str, "%bu", i);
		menu_add(str);
	}
	i=menu_start(tip);
	if(i==0)return 0xff;
	return i-1+start;
}

void pop_msg(uint8_t *tip)
{
	int8_t i;
	i=oled_sprite_find();
	if(i == -1)return;
	oled_sprite_change_gb2312(i,0,0, tip);
	oled_sprite_move(i, (127-oled_sprite_get_width(0))/2, 24);
	beep(SOUND_TIP_FREQ);
	delay10ms(80);
	oled_sprite_move(i, 0, 65);
	delay10ms(30);
	oled_remove_sprite(i);
}

void req_password()
{
	morse_mode = 1;
	resetprotect = 1;
	oled_sprite_change_gb2312(0,0,0, "请输入密码");
	oled_sprite_move(0, (127-oled_sprite_get_width(0))/2, 24);
	oled_enable_inverse(1);
	while(1)
	{	
		morse_mode = 1;
		oled_inverse(20+2,40,108-2,42);
		delay10ms(100);
		oled_inverse(20,42,108,44);
		if(morse_finished == 1)
		{
			morse_finished = 0;
			if(strcmp(morse_input_str, MENU_PASSWORD) == 0)
				break;
		}
		delay10ms(100);
	}
	oled_inverse(20+20,0,108-20,0);
	morse_mode = 0;
	resetprotect=0;
	delay10ms(50);
}
