#ifndef __OLED_H__
#define __OLED_H__

#define OLED_MAX_SPIRITS 9
#define OLED_ANIMATION_SPEED 2
#define OLED_X_MAX 128
#define OLED_Y_MAX 64

#define oled_stop_refresh() TR1=0
#define oled_start_refresh() TR1=1


typedef struct{
	uint8_t enable;
	uint8_t buffer[256];
	int8_t x;
	int8_t y;
	int8_t now_x;
	int8_t now_y;
	int8_t w;
	int8_t h;
	uint8_t gooff;
	uint8_t speed;
} struct_oled_sprite;

typedef struct{
	uint8_t enable;
	int8_t x;
	int8_t y;
	int8_t x_end;
	int8_t y_end;
	int8_t now_x;
	int8_t now_y;
	int8_t now_x_end;
	int8_t now_y_end;
} struct_oled_inverse;


int16_t max(int16_t a ,int16_t b);
int16_t min(int16_t a ,int16_t b);
void oled_wr_cmd(uint8_t cmd);
void oled_wr_dat(uint8_t dat);
void oled_send_buffer();
void oled_clear_buffer();
void oled_init_sprites();
void oled_clear_sprites();
void oled_remove_sprite(uint8_t num);
void oled_init();  //Initial_M096128x64_ssd1306

int8_t oled_sprite_find();
int8_t oled_sprite_change_6x8(uint8_t sprite_num, int8_t x, int8_t y, uint8_t str[]);
void oled_inverse(int8_t x, int8_t y, int8_t x_end, int8_t y_end);
void oled_enable_inverse(uint8_t enable);
void oled_proc_sprites();

uint8_t oled_sprite_get_height(uint8_t num);
uint8_t oled_sprite_get_width(uint8_t num);
void oled_sprite_move(uint8_t num, int8_t x, int8_t y);
void oled_sprite_move_direct(uint8_t num, int8_t x, int8_t y);
void oled_sprite_move_all_up(uint8_t distance);
void oled_sprite_move_all_down(uint8_t distance);
void oled_sprite_change_str_6x8(uint8_t num, uint8_t* str);
void oled_sprite_change_pic(uint8_t sprite_num, int8_t x, int8_t y, uint8_t w, uint8_t h, uint8_t *dat);
////////////////////////////////////////////////////
void oled_draw_6x8(uint8_t x, uint8_t page, uint8_t* str);
void oled_draw_dot(uint8_t x, uint8_t y);
void oled_drawvline(uint8_t x, uint8_t y_start, uint8_t y_end);
void oled_drawhline(uint8_t x0, uint8_t x1, uint8_t y);
void oled_draw_line(unsigned char x0 , unsigned char y0 , unsigned char x1 , unsigned char y1);
void oled_fill_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
/////////////////////////////////////////////////////////////
bit has_gb2312(uint8_t *str);
void oled_sprite_change_gb2312(uint8_t num, int8_t x, int8_t y, uint8_t *str);
void oled_sprite_change_str_gb2312(uint8_t num, uint8_t *str);
void oled_draw_bitmap_fast(uint8_t x, uint8_t yy, uint8_t* bitmap, uint8_t w, uint8_t h, uint8_t invert, uint8_t offsetY);
///////////////////////////////////////////////////////////////
extern uint8_t oled_tmpstr[50];
extern uint8_t code oled_font_64x32[];
#endif