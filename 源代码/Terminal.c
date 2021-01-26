#include "config.h"

uint8_t bottomline=0;
bit bigfont = 0;
uint8_t TERMINAL_LINES = 8;
#define COMMAND_COUNT 10
uint8_t *command_func_name[] = 
{
	"iiccheck",
	"iicread",
	"iicwrite",
	"iicread16",
	"iicwrite16",
	"enserial",
	"flashprog",
	"mario",
	"deadline",
	"alarm"
};
uint8_t (*command_func[])(uint8_t, uint8_t [][COMMAND_MAX_ARG_LEN+1]) = 
{
	command_iic_check,
	command_iic_read,
	command_iic_write,
	command_iic_read16,
	command_iic_write16,
	command_enable_serial,
	command_flash_program,
	command_mario,
	command_set_deadline,
	command_alarm,
};

void terminal_init()
{
	oled_clear_sprites();
	
	bottomline=0;	
}

void terminal_set_big_font(bit en)
{
	bigfont = en;
}

void terminal_set_lines(uint8_t lines)
{
	TERMINAL_LINES = lines;
}

uint8_t terminal_get_arr(uint8_t num)
{
	uint8_t result;
	result = bottomline + TERMINAL_LINES+1;
	result -= num;
	return result % (TERMINAL_LINES+1);
}

void terminal_add_bottom(uint8_t *str)
{
	if(bottomline == 0)
	{
		bottomline = TERMINAL_LINES;
	}
	else
	{
		bottomline--;
	}
	oled_stop_refresh();
	oled_sprite_move_direct(bottomline,0,63);
	if(has_gb2312(str) || (bigfont == 1)){
		oled_sprite_change_str_gb2312(bottomline, str);
	}
	else
		oled_sprite_change_str_6x8(bottomline, str);
	
	oled_sprite_move_all_up(oled_sprite_get_height(bottomline));
	oled_start_refresh();
}

void terminal_add_top(uint8_t *str)
{
	oled_stop_refresh();
	oled_sprite_move_direct(bottomline,0,-8);
	if(has_gb2312(str) ||(bigfont == 1))
		oled_sprite_change_gb2312(bottomline, 0, -16, str);
	else
		oled_sprite_change_6x8(bottomline, 0, -8, str);
	
	oled_sprite_move_all_down(oled_sprite_get_height(bottomline));
	oled_start_refresh();
	bottomline++;
	if(bottomline == TERMINAL_LINES+1)bottomline = 0;
} 

void terminal_modify(uint8_t line, uint8_t *str)
{
	oled_stop_refresh();
	if(has_gb2312(str) ||(bigfont == 1))
	{
		oled_sprite_change_str_gb2312(terminal_get_arr(line), str);
	}
	else
		oled_sprite_change_str_6x8(terminal_get_arr(line), str);
	oled_start_refresh();
}

uint8_t proc_command(uint8_t *command)
{
	uint8_t args[COMMAND_MAX_ARGS][COMMAND_MAX_ARG_LEN+1];
	uint8_t arg_pos = 0, arg_str_pos = 0, tmp;
	
	tmp = *(command++);
	while(tmp != '\n' && tmp)
	{
		if(tmp == ' ')
		{
			args[arg_pos][arg_str_pos] = 0;
			if(arg_str_pos != 0)
			{
				arg_pos++;
				if(arg_pos == COMMAND_MAX_ARGS)
				{
					return COMMAND_ARG_ERROR;
				}
			}
			arg_str_pos = 0;
		}
		else
		{
			args[arg_pos][arg_str_pos++] = tmp;
			if(arg_str_pos == COMMAND_MAX_ARG_LEN)
			{
				return COMMAND_ARG_ERROR;
			}
		}
		tmp = *(command++);
	}
	args[arg_pos][arg_str_pos] = 0;
	for(tmp = 0; tmp < COMMAND_COUNT; ++tmp)
	{
		if(strcmp(args[0], command_func_name[tmp])==0)
		{
			return (*command_func[tmp])(arg_pos, args);
		}
	}
	return COMMAND_NOT_FOUND;
}

void terminal_add_chr(uint8_t chr)
{
	morse_input_str[strPointer++] = chr;
	if(strPointer == MAX_INPUT_STR)strPointer = 0;
	morse_input_str[strPointer] = 0;
	morse_updated = 1;
	if(chr == '\n')
	{
		strPointer = 0;
		morse_finished = 1;
	}
}
void TerminalMode()
{
	uint16_t len;
	uint8_t result;
	terminal_init();
	delay10ms(30);
	morse_mode = 1;
	terminal_enable_serial = 1;
	terminal_add_bottom("STC Terminal");
	terminal_add_bottom("");
	terminal_add_bottom("");
	oled_inverse(0, 63-8, 5, 63);
	oled_enable_inverse(1);
	while(1)
	{
		if(morse_updated == 1)
		{
			morse_updated = 0;
			len = strlen(morse_input_str);
			if(len > 20)
			{
				terminal_modify(0, &(morse_input_str[len-20]));
			}
			else
			{
				terminal_modify(0, morse_input_str);
			}
			oled_inverse(oled_sprite_get_width(terminal_get_arr(0)), 63-7, oled_sprite_get_width(terminal_get_arr(0))+5, 63);
		}
		if(morse_finished == 1)
		{
			morse_finished = 0;
			oled_inverse(0, 64, 4, 64);
			morse_mode = 0;
			keyClear();
			result = proc_command(morse_input_str);
			if(result == COMMAND_NOT_FOUND)terminal_add_bottom("ÕÒ²»µ½ÃüÁî.");
			if(result == COMMAND_ARG_ERROR)terminal_add_bottom("²ÎÊý´íÎó.");
			if(result == COMMAND_ERROR)terminal_add_bottom("Î´Öª´íÎó.");
			if(result == COMMAND_QUIT_TERMINAL)break;
			terminal_add_bottom("");
			morse_mode = 1;
			oled_inverse(0, 63-8, 5, 63);
		}
		if(morse_mode == 0)
		{
			break;
		}
	}
	keyClear();
	oled_clear_sprites();
	oled_inverse(0, 64, 4, 64);
	delay10ms(30);
	return;	
}

uint8_t isint(uint8_t *str)
{
	uint8_t chr;
	if(*str == 'm')str++;
	while(chr=*str++)
	{
		if(chr - 0x30 > 9)
		{
			return 0;
		}
	}
	return 1;
}

int16_t getint(uint8_t *str)
{
	int16_t result=0;
	uint8_t chr;
	bit minus = 0;
	if(*str == 'm')
	{
		str++;
		minus = 1;
	}
	while(chr = *str++)
	{
		result *= 10;
		result += chr - 0x30;
	}
	if(minus)result *= -1;
	return result;
}

uint8_t gethex8(uint8_t *str)
{
	uint8_t tmp;
	uint8_t result;
	
	tmp = str[0];
	if(tmp >= 'a' && tmp <= 'f')
	{
		tmp = tmp - 'a'+ 0x0a;
	}
	else if(tmp >= '0' && tmp <= '9')
	{
		tmp -= '0';
	}
	else
	{
		return 0;
	}
	result = tmp;
	result <<= 4;
	
	tmp = str[1];
	if(tmp >= 'a' && tmp <= 'f')
	{
		tmp = tmp - 'a'+0x0a;
	}
	else if(tmp >= '0' && tmp <= '9')
	{
		tmp -= '0';
	}
	else
	{
		return 0;
	}
	result += tmp;

	return result;
}

uint8_t command_enable_serial(COMMAND_ARGS)
{
	terminal_enable_serial=1;
	return COMMAND_OK;
}