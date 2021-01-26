#include "config.h"



/////////////////////////////////////////
//»æÍ¼Ä£Ê½
float graph_data[128];
uint8_t data_pos = 0;
uint8_t graph_view_x = 0;
int8_t now_y, last_y;
uint8_t now_x;
float scale;
float graph_max, graph_min;
int32_t current_y;
uint8_t get_y(uint8_t x);


void graph_init()
{
	for(now_x = 0; now_x < 128; ++now_x)
	{
		graph_data[now_x] = 0;
	}
	graph_view_x = 0;
	data_pos = 0;
	now_x = 0;
	now_y = 0;
	last_y = 0;
	scale = 0.0f;
	oled_stop_refresh();
	graph_draw();
}

void graph_add_num(float num)
{
	graph_data[data_pos++] = num;
	data_pos &= 0x7f;
	graph_draw();
}

void graph_draw(void)
{
	uint8_t i;
	oled_clear_buffer();
	now_y = 0;
	graph_max = graph_min = graph_data[0];
	for(now_x = 1;now_x < 128; ++now_x)
	{
		if(graph_max < graph_data[now_x])
		{
			graph_max = graph_data[now_x];
		}
		if(graph_min > graph_data[now_x])
		{
			graph_min = graph_data[now_x];
		}
	}
	if(graph_max == graph_min)
	{
		oled_drawhline(0,128,26);
		goto information;
	}
	if(graph_min > 0) scale = 27.0f/(graph_max);
	else if(graph_max < 0) scale = 27.0f/(graph_min);
	else scale = 27.0f/(graph_max - graph_min);
	oled_drawhline(0,128,27);
	last_y = get_y(data_pos);
	++data_pos;
	for(i = 1; i < 128; ++i)
	{
		now_y = get_y(data_pos);
		if(last_y > now_y)
		{
			oled_draw_line(i, now_y, i, last_y);
		}
		else if(last_y == now_y)
		{
			oled_draw_dot(i, now_y);
		}
		else
		{
			oled_draw_line(i, last_y, i, now_y);
		}
		++data_pos;
		data_pos &= 0x7f;
		last_y = now_y;
	}
	information:
	current_y = (int32_t)(graph_data[(graph_view_x + data_pos) & 127] * 1000);
	sprintf(oled_tmpstr, "%bu, %ld.%03ld", graph_view_x, current_y/1000, labs(current_y % 1000));
	oled_draw_6x8(0, 7, oled_tmpstr);
	oled_draw_line(graph_view_x, 0, graph_view_x, 55);
	//oled_draw_line(0, get_y(data_pos), 127, get_y(data_pos));
	oled_send_buffer();
}


uint8_t get_y(uint8_t x)
{
	int8_t result;
	result = graph_data[x] * scale;
	result += 27.0f;
	return 54-result;
}

void graph_move_axis(int8_t offset)
{
	graph_view_x += offset;
	graph_view_x &= 127;
	graph_draw();
}
