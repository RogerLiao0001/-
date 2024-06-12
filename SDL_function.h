#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "toml.h"


typedef struct _button{
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
}button_rect;

typedef struct _options{
	char text[1000];
	char next[1000];
}options_struct;

//渲染場景function
void render_scene(char *path);

//渲染角色function
void render_character(char *path);

//渲染圖片方塊function
void render_image_block(const char *path,button_rect *button);


//渲染物品方塊function
void render_item_block(const char *path,button_rect *button,int32_t offset_x,int32_t offset_y);

//按鈕設定數值
void button_setting_rect(button_rect *button,int32_t rect_x,int32_t rect_y,int32_t rect_w,int32_t rect_h);

//渲染文字按鈕function
void render_text_Button(const char *text,button_rect *button,SDL_Color *bgColor,SDL_Color *textColor);

//渲染選項按鈕function
void render_option_Button(const char *text,button_rect *button,int32_t offset_y);

//渲染選項方塊function
void render_option_block(const char *path,button_rect *button,int32_t offset_y);

//渲染圖片按鈕function
void render_picture_Button(const char *path,button_rect *button);

//檢查點擊位置是否在按鈕上
int32_t check_button(int32_t mouse_x, int32_t mouse_y,button_rect *button,int32_t offset_y);

//渲染文字function
void renderText(const char *text, int32_t x, int32_t y,int32_t delay_put,SDL_Event *event);

//初始化東西function
int32_t initialize();

//清除畫面
void clear_renderer();

//更新視窗
void present_renderer();

void close_SDL();
