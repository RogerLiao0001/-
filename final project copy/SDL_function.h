#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "toml.h"


typedef struct _button{
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
}button_rect;

struct Character{
	char name[1000];
	int32_t like;
};

//截圖
void screenshot();

//存檔寫入
void write_save(char now_event_str[1000],char now_dialogue_str[1000],char now_character_str[1000],char now_character_image_str[1000],char now_character_avatar_str[1000],char now_talk_str[1000],char now_text_str[1000],char now_event_name_str[1000],char now_scene_name_str[1000],int32_t dialogue_line_which,int32_t text_line_which,int32_t emote_line_which,int32_t ch_line_which,int32_t nowtalk_line_which,int32_t sound_line_which,struct Character *character_list,int32_t num_character,int32_t start_option,char **backpack,int32_t save_which);

//存檔確認有無
int32_t check_save(struct Character *character_list,int32_t num_character,int32_t save_which);

//存檔讀取
void read_save(char now_event_str[1000],char now_dialogue_str[1000],char now_character_str[1000],char now_character_image_str[1000],char now_character_avatar_str[1000],char now_talk_str[1000],char now_text_str[1000],char now_event_name_str[1000],char now_scene_name_str[1000],int32_t *dialogue_line_which,int32_t *text_line_which,int32_t *emote_line_which,int32_t *ch_line_which,int32_t *nowtalk_line_which,int32_t *sound_line_which,struct Character *character_list,int32_t num_character,char **backpack,int32_t save_which);

//渲染存檔文字function
void show_save_info(button_rect *save_data_block,struct Character *character_list,int32_t num_character,int32_t save_which);

//渲染場景function
void render_scene(char *path);

//螢幕變黑一點
void darker(char *path);

//漸出特效function
void fadeout(char *path,SDL_Event *event);

//漸入特效function
void fadein(char *path,SDL_Event *event);

//渲染角色function
void render_character(char *path);

//渲染圖片方塊function
void render_image_block(const char *path,button_rect *button,int32_t offset_x,int32_t offset_y,uint8_t alpha);

//按鈕設定數值
void button_setting_rect(button_rect *button,int32_t rect_x,int32_t rect_y,int32_t rect_w,int32_t rect_h);

//渲染文字按鈕function
void render_text_Button(const char *text,button_rect *button,SDL_Color *bgColor,SDL_Color *textColor);

//渲染選項按鈕function
void render_option_Button(const char *text,button_rect *button,int32_t offset_x,int32_t offset_y);

//渲染儲存資訊按鈕function
void render_saveinfo_Button(char now_event_name_str[1000],char now_scene_name_str[1000],char timestamp[1000],button_rect *button,int32_t offset_x,int32_t offset_y);

//渲染圖片按鈕function
void render_picture_Button(const char *path,button_rect *button);

//檢查點擊位置是否在按鈕上
int32_t check_button(int32_t mouse_x, int32_t mouse_y,button_rect *button,int32_t offset_x,int32_t offset_y);

//渲染文字function
void renderText(const char *text, int32_t x, int32_t y,int32_t delay_put,SDL_Event *event);

//渲染結局文字function
void render_end_text(const char *text);

//渲染靜止對話function
void renderStaticText(const char *text, int32_t x, int32_t y,int32_t delay_put,SDL_Event *event);

//渲染物品描述function
void render_discribe_Text(const char *text,button_rect *button,int32_t offset_x,int32_t offset_y);
//void render_discribe_Text(const char *text, int32_t x, int32_t y,int32_t delay_put,SDL_Event *event);

//渲染llm text function
void render_llm_Text(const char *text, int32_t x, int32_t y,SDL_Event *event);

//加載音效
Mix_Chunk *loadSound(const char *path);

//初始化東西function
int32_t initialize();

//清除畫面
void clear_renderer();

//更新視窗
void present_renderer();

void close_SDL();
