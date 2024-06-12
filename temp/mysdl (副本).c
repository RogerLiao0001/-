#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_function.h"

#include "toml.h"

//初始化字串
void init_str(char str[1000]);

int main(int32_t argc,char *argv[]){
	//遊戲螢幕長寬
	uint32_t screen_width=1080;
	uint32_t screen_height=810;
	//4:3
	//640x480
	//1080x810
	
	//數值設定
	//title
	button_rect title_block;
	button_setting_rect(&title_block,(screen_width/2)*0.33,screen_height*0.07,screen_width*0.7,screen_height*0.12);
	//open_option_block
	button_rect open_option_block;
	button_setting_rect(&open_option_block,screen_width*0.42,screen_height*0.25,screen_width*0.156,screen_height*0.083);
	//option_button
	button_rect option_button;
	button_setting_rect(&option_button,screen_width*0.165,screen_height*0.015,screen_width*0.67,screen_height*0.076);
	//backpack
	button_rect backpack_button;
	button_setting_rect(&backpack_button,screen_width*0.86,screen_height*0.1,screen_height*0.07,screen_height*0.07);
	//close_backpack
	button_rect close_button;
	button_setting_rect(&close_button,0,0,screen_height*0.07,screen_height*0.07);
	//character
	button_rect character_block;
	button_setting_rect(&character_block,screen_width*0.36,screen_height*0.2,screen_width*0.3,screen_height*0.8);
	//character_avatar
	button_rect character_avatar_block;
	button_setting_rect(&character_avatar_block,screen_width*0.28,screen_height*0.587,screen_height*0.07,screen_height*0.07);
	//save
	button_rect save_block;
	button_setting_rect(&save_block,screen_width*0.86,screen_height*0.2,screen_height*0.07,screen_height*0.07);
	//save_data
	button_rect save_data_block;
	button_setting_rect(&save_data_block,screen_width*0.08,screen_height*0.07,screen_width*0.86,screen_height*0.076);
	//chatbar
	button_rect chatbar_block;
	button_setting_rect(&chatbar_block,screen_width*0.08,screen_height*0.65,screen_width*0.86,screen_height*0.25);
	//nowtalk
	button_rect nowtalk_block;
	button_setting_rect(&nowtalk_block,screen_width*0.09,screen_height*0.587,screen_width*0.18,screen_height*0.05);
	//item icon bg
	button_rect item_bg_block;
	button_setting_rect(&item_bg_block,screen_width*0.058,screen_height*0.083,screen_height*0.2,screen_height*0.2);
	//item icon
	button_rect item_block;
	button_setting_rect(&item_block,screen_width*0.078,screen_height*0.115,screen_height*0.14,screen_height*0.14);
	
	//hide
	button_rect hide_block;
	button_setting_rect(&hide_block,screen_width*0.86,screen_height*0.4,screen_height*0.07,screen_height*0.07);
	//settings
	button_rect settings_block;
	button_setting_rect(&settings_block,screen_width*0.86,screen_height*0.3,screen_height*0.07,screen_height*0.07);
	
	
	///////////////////處理toml///////////////////
	//打開圖片設定toml
	FILE *fp_default;
	char errbuf_default[200];
	fp_default = fopen("assets/default.toml", "r");
	toml_table_t *default_category = toml_parse_file(fp_default, errbuf_default, sizeof(errbuf_default));
	fclose(fp_default);
	
	//打開劇情檔案.toml
	FILE *fp;
	char errbuf[200];
	fp = fopen("script.toml", "r");
	//讀取全部資料
	toml_table_t *category = toml_parse_file(fp, errbuf, sizeof(errbuf));
	fclose(fp);
	//全部事件event_table
	toml_table_t *event_table = toml_table_in(category, "event");
	//全部角色character_table
	toml_table_t *character_table = toml_table_in(category, "character");
	//全部東西item_table
	toml_table_t *item_table = toml_table_in(category, "item");
	//全部場景scene_table
	toml_table_t *scene_table = toml_table_in(category, "scene");
	//全部場景dialogue_table
	toml_table_t *dialogue_table = toml_table_in(category, "dialogue");
	
	
	//玩家
	toml_table_t *player = toml_table_in(category, "player");
	//玩家名稱
	toml_datum_t player_name = toml_string_in(player, "name");
	//玩家頭像
	toml_datum_t player_avatar = toml_string_in(player, "avatar");
	//玩家介紹
	toml_datum_t player_describe = toml_string_in(player, "describe");
	//printf("name: %s\n", player_name.u.s);
	//玩家初始背包10格
	char *backpack[9];
	//初始化背包
    for (int i=0;i<9;i++){
    //分配100個字元的空間
	backpack[i]=calloc(100, sizeof(char));
    }
	//把一開始的背包存入
	toml_array_t *inventory_arr=NULL;
	inventory_arr=toml_array_in(player, "inventory");
	//重製背包
	//如果有inventory
	if(inventory_arr!=NULL){
		int32_t item_position=0;
		for(int32_t i=0;i<9;i++){
		toml_datum_t item = toml_string_at(inventory_arr, i);
			if(item.ok){
			//確認有沒有這一個物品
			toml_table_t *check_item=NULL;
			check_item=toml_table_in(item_table,item.u.s);
				if(check_item!=NULL){
				strncpy(backpack[item_position],item.u.s, 100);
				item_position++;
				free(item.u.s);
				}
			}
		}
	}
	
	
	
	//抓取角色資料
    const char *character_key;
    //獲取有幾個character
    int32_t num_character=0;
    while ((character_key = toml_key_in(character_table, num_character++)));
    num_character--;
    //創建struct存各項數值
    struct Character character_list[num_character];
    //初始化
    for(int32_t i=0;i<num_character;i++){
    	for(int32_t j=0;j<1000;j++){
    	character_list[i].name[j]=0;
		}
    }
    for(int32_t i=0;i<num_character;i++){
    character_key=toml_key_in(character_table,i);
    	if(character_key!=NULL){
    	toml_table_t *now_character_table = toml_table_in(character_table,character_key);
    	toml_datum_t now_character_name = toml_string_in(now_character_table,"name");
    	toml_datum_t now_character_like = toml_int_in(now_character_table,"like");
    		if(now_character_name.ok){
    		strncpy(character_list[i].name,now_character_name.u.s, 1000);
    		}
    		else{
    		printf("角色名稱有錯誤！\n");
    		return 0;
    		}
    		if(now_character_like.ok){
    		character_list[i].like=now_character_like.u.i;
    		}
    		else{
    		character_list[i].like=0;
    		}
    	} 
	}
	
	
    
	
	/////讀取進來的字串/////
	char now_event_str[1000]={0};
	char now_event_name_str[1000]={0};
	char now_dialogue_str[1000]={0};
	char now_scene_bg_str[1000]={0};
	char now_scene_name_str[1000]={0};
	char now_character_str[1000]={0};
	char now_character_image_str[1000]={0};
	char now_character_avatar_str[1000]={0};
	char now_event_music_str[1000]={0};
	char now_talk_str[1000]={0};
	char now_text_str[1000]={0};
	char now_options_next_str[1000]={0};
	//音效
	Mix_Chunk *open_music=NULL;
	Mix_Chunk *music1=NULL;
	Mix_Chunk *sound1=NULL;
	Mix_Chunk *dialogue_sound=NULL;
	int32_t open_music_channel=-1;
	int32_t music_channel=-1;
	int32_t dialogue_sound_channel=-1;
	
	//預設的圖片
	toml_table_t *game_default = toml_table_in(default_category, "game_default");
	//game_name
	toml_datum_t game_name = toml_string_in(game_default, "game_name");
	//backpack
	toml_datum_t backpack_bg = toml_string_in(game_default, "backpack_bg");
	toml_datum_t backpack_1 = toml_string_in(game_default, "backpack_1");
	toml_datum_t backpack_2 = toml_string_in(game_default, "backpack_2");
	//chatbar
	toml_datum_t chatbar = toml_string_in(game_default, "chatbar");
	//save
	toml_datum_t save_bg = toml_string_in(game_default, "save_bg");
	toml_datum_t save_1 = toml_string_in(game_default, "save_1");
	toml_datum_t save_2 = toml_string_in(game_default, "save_2");
	//hide
	toml_datum_t hide_1 = toml_string_in(game_default, "hide_1");
	toml_datum_t hide_2 = toml_string_in(game_default, "hide_2");
	//settings
	toml_datum_t settings_bg = toml_string_in(game_default, "settings_bg");
	toml_datum_t settings_1 = toml_string_in(game_default, "settings_1");
	toml_datum_t settings_2 = toml_string_in(game_default, "settings_2");
	//load
	toml_datum_t load_bg = toml_string_in(game_default, "load_bg");
	//close
	toml_datum_t close_1 = toml_string_in(game_default, "close_1");
	toml_datum_t close_2 = toml_string_in(game_default, "close_2");
	//預設按鈕
	toml_datum_t default_button_1 = toml_string_in(game_default, "default_button_1");
	toml_datum_t default_button_2 = toml_string_in(game_default, "default_button_2");
	//開場按鈕
	toml_datum_t open_button_1 = toml_string_in(game_default, "open_button_1");
	toml_datum_t open_button_2 = toml_string_in(game_default, "open_button_2");
	//背包按鈕
	toml_datum_t backpack_button_1 = toml_string_in(game_default, "backpack_button_1");
	toml_datum_t backpack_button_2 = toml_string_in(game_default, "backpack_button_2");
	//儲存按鈕
	toml_datum_t save_button_1 = toml_string_in(game_default, "save_button_1");
	toml_datum_t save_button_2 = toml_string_in(game_default, "save_button_2");
	//遺失
	toml_datum_t missing_image = toml_string_in(game_default, "missing_image");
	
	//按按鈕音樂
	toml_datum_t button_sound = toml_string_in(game_default, "button_sound");
	
	///////////////////處理SDL///////////////////
	//初始化
	if(game_name.ok){
		if(initialize(game_name.u.s)==0){
		return 0;
		}
	}
	else{
		if(initialize("game")==0){
		return 0;
		}
	}
	
	
	
	//如果有抓到場景音樂
	if(button_sound.ok){
	// 加載音效文件
	sound1=loadSound(button_sound.u.s);
	}

	//開始畫面
	//open
	toml_datum_t open_bg = toml_string_in(game_default, "open_bg");
	toml_datum_t open_title = toml_string_in(game_default, "open_title");
	toml_datum_t open_music_str = toml_string_in(game_default, "open_music");
	
	
	
	
	/////控制遊戲進行的變數/////
	//關閉遊戲
	SDL_Event event; 
	int32_t quit=0;
	//有沒有抓到scene,character,text 有1 沒有0
	int32_t scene_catch=0;
	int32_t character_catch=0;
	int32_t text_catch=0;
	//開場畫面暫停
	int32_t open_pause=0;
	//主遊戲暫停(打開背包,設定,儲存)
	int32_t game_pause=1;
	int32_t text_pause=0;
	//暫停視窗
	int32_t backpack_pause=0;
	int32_t save_pause=0;
	int32_t load_pause=0;
	int32_t settings_pause=0;
	int32_t OpeningSettings_pause=0;
	//開始遊戲,選項 是1 不是0
	int32_t start_open=1;
	int32_t start_game=0;
	int32_t start_option=0;
	int32_t start_llm=0;
	char llm_my_text[1000]={0};
	char llm_machine_text[1000]={0};
	int32_t llm_first_time=1;
	//打開背包,儲存畫面,載入遊戲畫面,設定  是1 不是0 //按下hide //按下現在角色介紹
	int32_t open_backpack=0;
	int32_t open_save=0;
	int32_t open_load=0;
	int32_t open_settings=0;
	int32_t open_OpeningSettings=0;
	int32_t press_hide=0;
	int32_t press_CharacterDiscribe=0;
	//到對話,text,emote,character,nowtalk第幾個
	int32_t dialogue_line_which=0;
	int32_t text_line_which=1;
	int32_t emote_line_which=1;
	int32_t ch_line_which=1;
	int32_t nowtalk_line_which=1;
	int32_t sound_line_which=1;
	//是不是對話的第一條 是1 不是0 (如果是載入就設為0)
	int32_t first_dialogue_check=1;
	//物品選擇
	int32_t item_select=-1;
	//選擇的選項
	int32_t option_select=0;
	//總共有幾個選項
	int32_t options_size=0;
	//有圖案改變就放出截圖
	int32_t icon_Change=0;
	//放一次開頭音樂
	int32_t play_open_music_check=0;
	//聲音大小
	double sound_rate=1;
	
	
	SDL_StartTextInput();
	//主事件迴圈
	while (!quit) {
	//獲取滑鼠位置
	int32_t mouse_x,mouse_y;
	SDL_GetMouseState(&mouse_x,&mouse_y);
		//處理事件
		if(start_llm==0){
			while (SDL_PollEvent(&event)){
				switch (event.type){ 
				//按下離開按鍵離開遊戲 在看看要不要自動存檔
				case SDL_QUIT:
					if(start_game==1 && (text_catch==1 || start_option==1)){
					write_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,now_character_avatar_str,
					now_talk_str,now_text_str,now_event_name_str,now_scene_name_str,dialogue_line_which,text_line_which,
					emote_line_which,ch_line_which,nowtalk_line_which,sound_line_which,character_list,num_character,
					start_option,backpack,0);
					}
				
					quit=1; 
					break; 
				//滑鼠點擊
				case SDL_MOUSEBUTTONDOWN:
				
					//開頭(選項)
					if(start_open==1){
						for(int32_t i=0;i<4;i++){
							if(check_button(mouse_x,mouse_y,&open_option_block,0,i*screen_height*0.1) && open_OpeningSettings==0 && open_load==0){
							//按鈕音效
							Mix_PlayChannel(-1,sound1,0);
							
								//新遊戲
								if(i==0){
								start_game=1;
								start_open=0;
								game_pause=0;
								//重製背包
								for(int32_t i=0;i<9;i++){
									for(int32_t j=0;j<100;j++){
									backpack[i][j]=0;
									}
								}
								//如果有inventory
								if(inventory_arr!=NULL){
									int32_t item_position=0;
									for(int32_t i=0;i<9;i++){
									toml_datum_t item = toml_string_at(inventory_arr, i);
										if(item.ok){
										//確認有沒有這一個物品
										toml_table_t *check_item=NULL;
										check_item=toml_table_in(item_table,item.u.s);
											if(check_item!=NULL){
											strncpy(backpack[item_position],item.u.s, 100);
											item_position++;
											free(item.u.s);
											}
										}
									}
								}
								if(open_music_channel>=0){
								Mix_HaltChannel(open_music_channel);
								open_music_channel=-1;
								}
								play_open_music_check=0;
								//一開始事件都是event.start
								strncpy(now_event_str,"start", 1000);
								}
								//載入(開啟)
								if(i==1){
								open_load=1;
								}
								//遊戲設定(開啟)
								if(i==2){
								open_OpeningSettings=1;
								}
								//離開遊戲
								if(i==3){
								quit=1;
								break;
								}
							}
						}
					}
					
					////載入(選項)////
					for(int32_t i=0;i<8;i++){
						if(check_button(mouse_x,mouse_y,&save_data_block,0,i*screen_height*0.08) && open_load==1 && load_pause==1){
						//按鈕音效
						Mix_PlayChannel(-1,sound1,0);
							if(check_save(character_list,num_character,i)){
								if(open_music_channel>=0){
								Mix_HaltChannel(open_music_channel);
								open_music_channel=-1;
								}
							play_open_music_check=0;
							//初始化數值
							init_str(now_event_str);
							init_str(now_event_name_str);
							init_str(now_scene_bg_str);
							init_str(now_scene_name_str);
							init_str(now_dialogue_str);
							init_str(now_character_str);
							init_str(now_character_image_str);
							init_str(now_character_avatar_str);
							init_str(now_talk_str);
							init_str(now_text_str);
							init_str(now_options_next_str);
							scene_catch=0;
							character_catch=1;
							text_catch=0;
							text_line_which=1;
							emote_line_which=1;
							ch_line_which=1;
							nowtalk_line_which=1;
							sound_line_which=1;
							text_pause=0;
							dialogue_line_which=0;
							first_dialogue_check=0;
							start_option=0;
							option_select=0;
							options_size=0;
							//載入
							read_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,
							now_character_avatar_str,now_talk_str,now_text_str,now_event_name_str,now_scene_name_str,
							&dialogue_line_which,&text_line_which,&emote_line_which,&ch_line_which,&nowtalk_line_which,
							&sound_line_which,character_list,num_character,backpack,i);
							if(sound1!=NULL){
							Mix_VolumeChunk(sound1,MIX_MAX_VOLUME*sound_rate);
							}
							for(int32_t j=0;j<9;j++){
								if(strlen(backpack[j])!=0){
								//確認有沒有這一個物品
								toml_table_t *check_item=NULL;
								check_item=toml_table_in(item_table,backpack[j]);
									if(check_item==NULL){
										for(int32_t k=0;k<100;k++){
										backpack[j][k]=0;
										}
									}
								}
							}
							//漸黑特效
							fadeout(NULL,&event);
							start_game=1;
							start_open=0;
							game_pause=0;
							open_load=0;
							}
						}
					}
					
					////載入(關閉)
					if(check_button(mouse_x,mouse_y,&close_button,0,0) && open_load==1){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					open_load=0;
					load_pause=0;
					open_pause=0;
					}
					
					//開頭設定(選項)
					if(open_OpeningSettings==1){
						//設定調整音量  (有新增聲音放在這裡)
						for(int32_t i=-2;i<3;i++){
							if(check_button(mouse_x,mouse_y,&open_option_block,screen_width*0.2*i,screen_height*-0.1) && open_OpeningSettings==1){
								if(i==-2){
									if(open_music!=NULL){
									Mix_VolumeChunk(open_music,0);
									}
									if(music1!=NULL){
									Mix_VolumeChunk(music1,0);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,0);
									}
									sound_rate=0;
								}
								if(i==-1){
									if(open_music!=NULL){
									Mix_VolumeChunk(open_music,MIX_MAX_VOLUME*0.25);
									}
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME*0.25);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME*0.25);
									}
									sound_rate=0.25;
								}
								if(i==0){
									if(open_music!=NULL){
									Mix_VolumeChunk(open_music,MIX_MAX_VOLUME*0.5);
									}
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME*0.5);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME*0.5);
									}
									sound_rate=0.5;
								}
								if(i==1){
									if(open_music!=NULL){
									Mix_VolumeChunk(open_music,MIX_MAX_VOLUME*0.75);
									}
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME*0.75);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME*0.75);
									}
									sound_rate=0.75;
								}
								if(i==2){
									if(open_music!=NULL){
									Mix_VolumeChunk(open_music,MIX_MAX_VOLUME);
									}
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME);
									}
									sound_rate=1;
								}
							Mix_PlayChannel(-1,sound1,0);
							}
						}
					}
					////開頭設定(關閉)
					if(check_button(mouse_x,mouse_y,&close_button,0,0) && open_OpeningSettings==1){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					open_OpeningSettings=0;
					OpeningSettings_pause=0;
					open_pause=0;
					}
					
					//選項(選項)
					if(start_option==1 && press_hide==0 && open_backpack==0 && open_save==0  && open_settings==0 && press_CharacterDiscribe==0){
						for(int32_t i=0;i<options_size;i++){
							if(check_button(mouse_x,mouse_y,&option_button,0,(i+1)*screen_height*0.09)){
							//按鈕音效
							option_select=i+1;
							game_pause=0;
							}
						}
					}
					
					
					////背包(開啟)
					if(check_button(mouse_x,mouse_y,&backpack_button,0,0) && start_game==1 && open_backpack==0  && open_save==0 && press_hide==0 && open_settings==0 && press_CharacterDiscribe==0){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					game_pause=1;
					open_backpack=1;
					}
					//背包(選項)
					if(open_backpack==1){
						for(int32_t i=0;i<9;i++){
							if(check_button(mouse_x,mouse_y,&item_bg_block,(i%3)*screen_width*0.21,
							(i/3)*screen_height*0.25)){
							//按鈕音效
							Mix_PlayChannel(-1,sound1,0);
							item_select=i;
							backpack_pause=0;
							break;
							}
						}
					}
					////背包(關閉)
					if(check_button(mouse_x,mouse_y,&close_button,0,0) && open_backpack==1){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					game_pause=0;
					text_pause=0;
					open_backpack=0;
					backpack_pause=0;
						if(start_option==0){
						//把字往回一串
						text_catch=0;
						text_line_which--;
						}
					}
					
					
					
					////儲存(選項)
					if(open_save==1){
						for(int32_t i=0;i<8;i++){
							if(check_button(mouse_x,mouse_y,&save_data_block,0,i*screen_height*0.08)){
							//按鈕音效
							Mix_PlayChannel(-1,sound1,0);
							write_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,
							now_character_avatar_str,now_talk_str,now_text_str,now_event_name_str,now_scene_name_str,
							dialogue_line_which,text_line_which,emote_line_which,ch_line_which,nowtalk_line_which,
							sound_line_which,character_list,num_character,start_option,backpack,i);
							save_pause=0;
							}
						}
					}
					
					////儲存(開啟)
					if(check_button(mouse_x,mouse_y,&save_block,0,0) && start_game==1 && open_backpack==0 && open_save==0 && press_hide==0 && open_settings==0 && press_CharacterDiscribe==0){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					game_pause=1;
					open_save=1;
					}
					
					////儲存(關閉)
					if(check_button(mouse_x,mouse_y,&close_button,0,0) && open_save==1){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					game_pause=0;
					text_pause=0;
					open_save=0;
					save_pause=0;
						if(start_option==0){
						//把字往回一串
						text_catch=0;
						text_line_which--;
						}
					}
					
					////設定(開啟)
					if(check_button(mouse_x,mouse_y,&settings_block,0,0) && start_game==1 && open_backpack==0 && open_save==0 && press_hide==0 && open_settings==0 && press_CharacterDiscribe==0){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					game_pause=1;
					open_settings=1;
					}
					
					
					//設定(選項)
					if(open_settings==1){
						//設定調整音量  (有新增聲音放在這裡)
						for(int32_t i=-2;i<3;i++){
							if(check_button(mouse_x,mouse_y,&open_option_block,screen_width*0.2*i,screen_height*-0.1) && open_settings==1){
								if(i==-2){
									if(music1!=NULL){
									Mix_VolumeChunk(music1,0);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,0);
									}
									sound_rate=0;
								}
								if(i==-1){
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME*0.25);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME*0.25);
									}
									sound_rate=0.25;
								}
								if(i==0){
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME*0.5);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME*0.5);
									}
									sound_rate=0.5;
								}
								if(i==1){
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME*0.75);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME*0.75);
									}
									sound_rate=0.75;
								}
								if(i==2){
									if(music1!=NULL){
									Mix_VolumeChunk(music1,MIX_MAX_VOLUME);
									}
									if(sound1!=NULL){
									Mix_VolumeChunk(sound1,MIX_MAX_VOLUME);
									}
									sound_rate=1;
								}
							Mix_PlayChannel(-1,sound1,0);
							}
						}
						for(int32_t i=0;i<3;i++){
							if(check_button(mouse_x,mouse_y,&open_option_block,0,i*screen_height*0.1)){
							//按鈕音效
							Mix_PlayChannel(-1,sound1,0);
								//設定(關閉)
								if(i==0){
								//按鈕音效
								Mix_PlayChannel(-1,sound1,0);
								game_pause=0;
								text_pause=0;
								open_settings=0;
								settings_pause=0;
									if(start_option==0){
									//把字往回一串
									text_catch=0;
									text_line_which--;
									}
								}
								//儲存並返回標題
								if(i==1){
								write_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,
								now_character_avatar_str,now_talk_str,now_text_str,now_event_name_str,now_scene_name_str,
								dialogue_line_which,text_line_which,emote_line_which,ch_line_which,nowtalk_line_which,
								sound_line_which,character_list,num_character,start_option,backpack,0);
								//漸黑特效
								fadeout(NULL,&event);
								start_game=0;
								start_open=1;
								open_pause=0;
								//初始化數值
								init_str(now_event_str);
								init_str(now_event_name_str);
								init_str(now_scene_bg_str);
								init_str(now_scene_name_str);
								init_str(now_dialogue_str);
								init_str(now_character_str);
								init_str(now_character_image_str);
								init_str(now_character_avatar_str);
								init_str(now_talk_str);
								init_str(now_text_str);
								init_str(now_options_next_str);
								scene_catch=0;
								character_catch=0;
								text_catch=0;
								open_pause=0;
								game_pause=1;
								text_pause=0;
								backpack_pause=0;
								save_pause=0;
								load_pause=0;
								settings_pause=0;
								start_open=1;
								start_game=0;
								start_option=0;
								start_llm=0;
								llm_first_time=1;
								open_backpack=0;
								open_save=0;
								open_load=0;
								open_settings=0;
								press_hide=0;
								press_CharacterDiscribe=0;
								dialogue_line_which=0;
								text_line_which=1;
								emote_line_which=1;
								ch_line_which=1;
								nowtalk_line_which=1;
								sound_line_which=1;
								first_dialogue_check=1;
								item_select=-1;
								option_select=0;
								options_size=0;
								icon_Change=0;
								//重製背包
								for(int32_t i=0;i<9;i++){
									for(int32_t j=0;j<100;j++){
									backpack[i][j]=0;
									}
								}
								Mix_HaltChannel(music_channel);
								}
								//儲存並離開遊戲
								if(i==2){
								write_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,
								now_character_avatar_str,now_talk_str,now_text_str,now_event_name_str,now_scene_name_str,
								dialogue_line_which,text_line_which,emote_line_which,ch_line_which,
								nowtalk_line_which,sound_line_which,character_list,num_character,
								start_option,backpack,0);
								quit=1;
								break;
								}
							}
						}
					}
					
					////隱藏(開啟)
					if(check_button(mouse_x,mouse_y,&hide_block,0,0) && press_hide==0 && open_backpack==0 && open_save==0 && open_settings==0 && press_CharacterDiscribe==0){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					//渲染場景
					render_scene(now_scene_bg_str);
					//渲染角色
					if(strlen(now_character_image_str)!=0){
					render_image_block(now_character_image_str,&character_block,0,0,255);
					}
					press_hide=1;
					text_pause=1;
						if(start_option==0){
						//把字往回一串
						text_catch=0;
						text_line_which--;
						}
					}
					
					//隱藏(關閉)
					else if(press_hide==1){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					press_hide=0;
					text_pause=0;
					}
					
					////角色介紹(開啟)
					if(check_button(mouse_x,mouse_y,&character_avatar_block,0,0) && press_hide==0 && open_backpack==0 && open_save==0 && open_settings==0 && press_CharacterDiscribe==0 && strlen(now_character_str)>0 && strlen(now_character_avatar_str)>0 && start_option==0){
					//按鈕音效
					Mix_PlayChannel(-1,sound1,0);
					darker("assets/image/screenshot.bmp");
					//渲染角色介紹框
					if(chatbar.ok){
					render_image_block(chatbar.u.s,&chatbar_block,0,0,255);
					}
					//渲染角色介紹
					toml_table_t *now_character_table = toml_table_in(character_table,now_character_str);
					toml_datum_t now_character_describe;
					now_character_describe = toml_string_in(now_character_table, "describe");
					if(now_character_describe.ok){
					renderStaticText(now_character_describe.u.s,screen_width*0.12,screen_height*0.687,1,&event);
					}
								
								
					press_CharacterDiscribe=1;
					text_pause=1;
					//把字往回一串
					text_catch=0;
					text_line_which--;
					}
					
					//角色介紹(關閉)
					else if(press_CharacterDiscribe==1){
					press_CharacterDiscribe=0;
					text_pause=0;
					}
					
					
					//如果現在在跑對話就遊戲繼續 打開背包或儲存畫面或設定要擋
					if(text_catch==1 && press_hide==0 && open_backpack==0 && open_save==0 && open_settings==0 && press_CharacterDiscribe==0){
					dialogue_line_which++;
					text_pause=0;
					}
					
					break;
				}
			}
		}

			
			
			
		////////跑開始畫面//////////
		if(start_open==1 && open_pause==0){
			if(play_open_music_check==0){
				//如果有抓到場景音樂
				if(open_music_str.ok){
				// 加載音效文件
				open_music=loadSound(open_music_str.u.s);
				}
				if(open_music!=NULL){
				Mix_VolumeChunk(open_music,MIX_MAX_VOLUME*sound_rate);
				open_music_channel=Mix_PlayChannel(-1,open_music,0);
				}
			play_open_music_check=1;	
			}
			
		//清除畫面
		clear_renderer();
		//渲染場景
		render_scene(open_bg.u.s);
		render_image_block(open_title.u.s,&title_block,0,0,255);
			for(int32_t i=0;i<4;i++){
			render_image_block(open_button_1.u.s,&open_option_block,0,i*screen_height*0.1,255);	
			}
		render_option_Button("新遊戲",&open_option_block,0,0*screen_height*0.1);
		render_option_Button("載入遊戲",&open_option_block,0,1*screen_height*0.1);
		render_option_Button("遊戲設定",&open_option_block,0,2*screen_height*0.1);
		render_option_Button("離開遊戲",&open_option_block,0,3*screen_height*0.1);
		open_pause=1;
		//截圖
		screenshot();
		}
		
		////////跑對話和選項//////////
		//如果遊戲沒有暫停
		if(start_game==1 && game_pause==0 && text_pause==0){
		
		//清除畫面
		clear_renderer();
		
		
		//配置現在事件event
		toml_table_t *now_event_table = toml_table_in(event_table, now_event_str);
		//讀取事件名稱
		toml_datum_t now_event_name = toml_string_in(now_event_table, "name");
		if(now_event_name.ok){
		init_str(now_event_name_str);
		strncpy(now_event_name_str,now_event_name.u.s, 1000);
		}
			
		//讀入現在場景名稱event.scene
		toml_datum_t now_scene = toml_string_in(now_event_table, "scene");
		//配置現在場景scene
		toml_table_t *now_scene_table=NULL;
		if(now_scene.ok){
		now_scene_table = toml_table_in(scene_table, now_scene.u.s);
		}
		else{
		printf("沒讀到現在scene\n");
		return 0;
		}
		
		//讀入scene.name
		toml_datum_t now_scene_name = toml_string_in(now_scene_table, "name");
		if(now_scene_name.ok){
		init_str(now_scene_name_str);
		strncpy(now_scene_name_str,now_scene_name.u.s, 1000);
		}
		//讀入scene.background
		//如果場景沒抓過才要抓 抓完設置為有抓過 抓場景音樂如果有
		if(scene_catch==0){
		toml_datum_t now_scene_bg = toml_string_in(now_scene_table, "background");
			//如果有抓到背景
			if(now_scene_bg.ok){
			//抓事件場景音樂
			toml_datum_t now_event_music = toml_string_in(now_event_table, "music");
			//如果有抓到場景音樂
			if(now_event_music.ok){
			//如果名稱一樣就繼續播放
				if(strcmp(now_event_music_str,now_event_music.u.s)!=0){
				Mix_HaltChannel(music_channel);
				strncpy(now_event_music_str,now_event_music.u.s,1000);
					// 加載音效文件
					music1=loadSound(now_event_music.u.s);
					if(music1!=NULL){
					// 播放音效 -1無限巡迴
					Mix_VolumeChunk(music1,MIX_MAX_VOLUME*sound_rate);
					music_channel=Mix_PlayChannel(-1,music1,-1);
					}
					else{
					music_channel=-1;
					}
				}
			}
			
			//把現在background存下來
			init_str(now_scene_bg_str);
			strncpy(now_scene_bg_str,now_scene_bg.u.s, 1000);
			//漸入
			fadein(now_scene_bg_str,&event);
			}
			else{
			//遺失圖像
			init_str(now_scene_bg_str);
				if(missing_image.ok){
				strncpy(now_scene_bg_str,missing_image.u.s,1000);
				}
			//漸入
			fadein(now_scene_bg_str,&event);	
			}
		scene_catch=1;
		}
		
		//渲染場景
		render_scene(now_scene_bg_str);
		
		//dialogue
		//配置現在對話
		toml_table_t *now_dialogue_table=NULL;
		//如果是現在第一個對話才是用事件抓到的對話
		if(first_dialogue_check==1){
		toml_datum_t now_dialogue = toml_string_in(now_event_table, "dialogue");
			if(now_dialogue.ok){
			now_dialogue_table = toml_table_in(dialogue_table, now_dialogue.u.s);
			strncpy(now_dialogue_str,now_dialogue.u.s, 1000);
			}
			else{
			printf("現在事件沒有第一個對話\n");
			return 0;
			}
		first_dialogue_check=0;
		}
		else{
		now_dialogue_table = toml_table_in(dialogue_table, now_dialogue_str);
		}
		
		//如果沒到選擇就抓這些東西
		if(start_option==0){
			//依序去抓現在對話裡面的key (key="value")
			const char *key=NULL;
			key=toml_key_in(now_dialogue_table, dialogue_line_which);
			//printf("line:%d\n",dialogue_line_which);
			//檢查有沒有key
			if(key!=NULL){
				toml_datum_t dialogue_line = toml_string_in(now_dialogue_table, key);
				//printf("%s",dialogue_line.u.s);
				//檢查有沒有value
				if(dialogue_line.ok){
				
					//抓角色
					if(strncmp(key,"character",9)==0){
					//用來比對是不是現在要出現的character
					int32_t key_ch_which=0;
					//抓character第幾行
					sscanf(key,"character%d",&key_ch_which);
						if(key_ch_which==ch_line_which){
						ch_line_which++;
						//把現在人物存下來
						init_str(now_character_str);
						init_str(now_character_image_str);
						init_str(now_character_avatar_str);
						strncpy(now_character_str,dialogue_line.u.s, 1000);
						//對話換下一條
						dialogue_line_which++;
						//不是在文字
						text_catch=0;
						character_catch=1;
						}
						else{
						printf("角色順序有問題\n");
						return 0;
						}
					}
					//抓表情
					else if(strncmp(key,"emote",5)==0){
					//用來比對是不是現在要出現的emote
					int32_t key_emote_which=0;
					//抓emote第幾行
					sscanf(key,"emote%d",&key_emote_which);
						if(key_emote_which==emote_line_which){
						emote_line_which++;
							if(character_catch==1){
							toml_table_t *now_character_table = toml_table_in(character_table,now_character_str);
							toml_datum_t now_character_image;
							toml_datum_t now_character_avatar;
							//int32_t catch_sucess=0;
							//清除
							if(strncmp(dialogue_line.u.s,"clear",5)==0){
							init_str(now_character_image_str);
							init_str(now_character_avatar_str);
							//對話換下一條
							dialogue_line_which++;
							//不是在文字
							text_catch=0;
							continue;
							}
							
							//支援表情
							char now_tachie_str[100]={0};
							char now_avatar_str[100]={0};
							snprintf(now_tachie_str,sizeof(now_tachie_str),"tachie_%s",dialogue_line.u.s);
							snprintf(now_avatar_str,sizeof(now_avatar_str),"avatar_%s",dialogue_line.u.s);
							//printf("%s\n",now_tachie_str);
							//printf("%s\n",now_avatar_str);
							now_character_image = toml_string_in(now_character_table,now_tachie_str);
							now_character_avatar = toml_string_in(now_character_table,now_avatar_str);
								init_str(now_character_image_str);
								init_str(now_character_avatar_str);
								//把現在人物存下來
								if(now_character_image.ok){
								strncpy(now_character_image_str,now_character_image.u.s,1000);
								}
								else{
									if(missing_image.ok){
									strncpy(now_character_image_str,missing_image.u.s,1000);
									}
								}
								if(now_character_avatar.ok){
								strncpy(now_character_avatar_str,now_character_avatar.u.s,1000);
								}
								else{
									if(missing_image.ok){
									strncpy(now_character_avatar_str,missing_image.u.s,1000);
									}
								}
							//對話換下一條
							dialogue_line_which++;
							//不是在文字
							text_catch=0;
							}
							else{
							printf("toml格式錯誤，沒有抓到角色，不能放表情！\n");
							dialogue_line_which++;
							//return 0;
							}
						}
						else{
						printf("表情順序錯誤\n");
						return 0;
						}
					}
					//抓現在講話的是誰
					else if(strncmp(key,"nowtalk",7)==0){
					//用來比對是不是現在要出現的character
					int32_t key_ch_which=0;
					//抓character第幾行
					sscanf(key,"nowtalk%d",&key_ch_which);
						if(key_ch_which==nowtalk_line_which){
						nowtalk_line_which++;
						//把現在人物存下來
						init_str(now_talk_str);
						strncpy(now_talk_str,dialogue_line.u.s, 1000);
						//對話換下一條
						dialogue_line_which++;
						//不是在文字
						text_catch=0;
						}
						else{
						printf("角色順序有問題\n");
						return 0;
						}
					}
					//放不出來 要檔上一個 上一個近來咖調 用一個變數確認
					//抓聲音
					else if(strncmp(key,"sound",5)==0){
					int32_t key_sound_which=0;
					//抓character第幾行
					sscanf(key,"sound%d",&key_sound_which);
						if(key_sound_which==sound_line_which){
						sound_line_which++;
							if(dialogue_sound_channel>=0){
							Mix_HaltChannel(dialogue_sound_channel);
							dialogue_sound_channel=-1;
							}
						// 加載音效文件
						dialogue_sound=loadSound(dialogue_line.u.s);
							if(dialogue_sound!=NULL){
							dialogue_sound_channel=Mix_PlayChannel(-1,dialogue_sound,0);
							}
						
						//對話換下一條
						dialogue_line_which++;
						//不是在文字
						text_catch=0;
						}
					}
					//抓文字
					else if(strncmp(key,"text",4)==0){
					/*//清除特定畫面
					clear_special_renderer(&clip_text);*/
					//用來比對是不是現在要出現的text
					int32_t key_text_which=0;
					//抓文字第幾行
					sscanf(key,"text%d",&key_text_which);
						if(key_text_which==text_line_which){
						text_line_which++;
							//自動儲存
							if(text_line_which==2){
							write_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,
							now_character_avatar_str,now_talk_str,now_text_str,now_event_name_str,now_scene_name_str,
							dialogue_line_which,text_line_which,emote_line_which,ch_line_which,nowtalk_line_which,
							sound_line_which,character_list,num_character,start_option,backpack,0);
							}
						//把現在文字抓下來
						init_str(now_text_str);
						strncpy(now_text_str,dialogue_line.u.s, 1000);
						//是在文字
						text_catch=1;
						}
						else{
						printf("文字順序有問題\n");
						return 0;
						}
					//printf("成功抓到文字%s",dialogue_line.u.s);
					}
					//抓下一個對話
					else if(strncmp(key,"next",4)==0){
					//重置
					init_str(now_dialogue_str);
					init_str(now_character_str);
					init_str(now_character_image_str);
					init_str(now_character_avatar_str);
					init_str(now_talk_str);
					init_str(now_text_str);
					init_str(now_options_next_str);
					character_catch=0;
					text_catch=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
					nowtalk_line_which=1;
					sound_line_which=1;
					text_pause=0;
					dialogue_line_which=0;
					first_dialogue_check=0;
					start_option=0;
					option_select=0;
					options_size=0;
					//抓下一個對話的名稱
					strncpy(now_dialogue_str,dialogue_line.u.s, 1000);
					continue;
					}
					//抓下一個事件
					else if(strncmp(key,"event",5)==0){
					//重置
					init_str(now_event_str);
					init_str(now_event_name_str);
					init_str(now_scene_bg_str);
					init_str(now_scene_name_str);
					init_str(now_dialogue_str);
					init_str(now_character_str);
					init_str(now_character_image_str);
					init_str(now_character_avatar_str);
					init_str(now_talk_str);
					init_str(now_text_str);
					init_str(now_options_next_str);
					scene_catch=0;
					character_catch=0;
					text_catch=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
					nowtalk_line_which=1;
					sound_line_which=1;
					text_pause=0;
					dialogue_line_which=0;
					first_dialogue_check=1;
					start_option=0;
					option_select=0;
					options_size=0;
					//抓下一個事件的名稱
					strncpy(now_event_str,dialogue_line.u.s, 1000);
					//漸黑特效
					fadeout(now_scene_bg_str,&event);
					continue; 
					}
					else if(strncmp(key,"end",3)==0){
					//漸黑特效
					Mix_HaltChannel(music_channel);
					fadeout(now_scene_bg_str,&event);
					render_end_text(dialogue_line.u.s);
					//回到標題
					start_game=0;
					start_open=1;
					open_pause=0;
					//初始化數值
					init_str(now_event_str);
					init_str(now_event_name_str);
					init_str(now_scene_bg_str);
					init_str(now_scene_name_str);
					init_str(now_dialogue_str);
					init_str(now_character_str);
					init_str(now_character_image_str);
					init_str(now_character_avatar_str);
					init_str(now_talk_str);
					init_str(now_text_str);
					init_str(now_options_next_str);
					scene_catch=0;
					character_catch=0;
					text_catch=0;
					open_pause=0;
					game_pause=1;
					text_pause=0;
					backpack_pause=0;
					save_pause=0;
					load_pause=0;
					settings_pause=0;
					start_open=1;
					start_game=0;
					start_option=0;
					start_llm=0;
					llm_first_time=1;
					open_backpack=0;
					open_save=0;
					open_load=0;
					open_settings=0;
					press_hide=0;
					press_CharacterDiscribe=0;
					dialogue_line_which=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
					nowtalk_line_which=1;
					sound_line_which=1;
					first_dialogue_check=1;
					item_select=-1;
					option_select=0;
					options_size=0;
					icon_Change=0;
					continue; 
					}
				}
				else{
				//key不是text就結束對話
				text_catch=0;
				//如果有抓到key可是不是以上的key就進入選項
				start_option=1;
				}
			}
			else{
			//沒有抓到key也沒有跳到其他對話、選項、事件，資料有誤
			printf("沒下個資料了！\n");
			return 0;
			}
		}
		
		//渲染角色
		if(strlen(now_character_image_str)!=0){
		render_image_block(now_character_image_str,&character_block,0,0,255);
		}
		//渲染角色頭像
		if(strlen(now_character_avatar_str)!=0){
		render_image_block(now_character_avatar_str,&character_avatar_block,0,0,255);
		}
		//顯示對話背景
		if((text_catch==1 || start_option==1) && chatbar.ok){
		render_image_block(chatbar.u.s,&chatbar_block,0,0,240);
		}
		
		//顯示現在講話是誰
		if(strlen(now_talk_str)!=0){
		render_image_block(chatbar.u.s,&nowtalk_block,0,0,240);
		render_option_Button(now_talk_str,&nowtalk_block,0,0);
		}
		
		//渲染儲存按鈕 裡面(它才不一直閃)
		if(save_1.ok){
		render_image_block(save_1.u.s,&save_block,0,0,255);
		}
		
		//渲染隱藏對話按鈕 裡面(它才不一直閃)
		if(hide_1.ok){
		render_image_block(hide_1.u.s,&hide_block,0,0,255);
		}
		
		//渲染設定按鈕 裡面(它才不一直閃)
		if(settings_1.ok){
		render_image_block(settings_1.u.s,&settings_block,0,0,255);
		}
		
		//渲染背包按鈕 裡面(它才不一直閃)
		if(backpack_1.ok){
		render_image_block(backpack_1.u.s,&backpack_button,0,0,255);
		}
		
		//渲染文字
		if(text_catch==1){
		renderText(now_text_str,screen_width*0.12,screen_height*0.687,1,&event);
		text_pause=1;
		}
		
		//截圖
		screenshot();
		
		//渲染上次文字
		if(start_option==1){
		renderStaticText(now_text_str,screen_width*0.12,screen_height*0.687,1,&event);
		}
		
		
				
			//開始選項		/////要把文字抓下來！		
			if(start_option==1){
			toml_table_t *now_options_table = toml_table_in(now_dialogue_table,"options");
			const char *key;
			int options_many = 0;
			options_size=0;
				while((key = toml_key_in(now_options_table, options_many++))){
				char options_id[100]={0};
				options_size++;
				sprintf(options_id,"%d",options_many);	
				toml_table_t *now_options = toml_table_in(now_options_table,options_id);
				//確認是不是隱藏選項(好感度)
				int32_t notshow=0;
				//大於才顯示
				toml_array_t *hide_like_bigger_array=NULL;
				hide_like_bigger_array=toml_array_in(now_options,"hide_like_bigger");
				if(hide_like_bigger_array!=NULL){
				toml_datum_t hide_like_name = toml_string_at(hide_like_bigger_array,0);
				toml_datum_t hide_like_value = toml_int_at(hide_like_bigger_array,1);
					for(int32_t i=0;i<num_character;i++){
					const char *now_character_key=toml_key_in(character_table,i);
						if(strcmp(now_character_key,hide_like_name.u.s)==0){
							if(character_list[i].like<=hide_like_value.u.i){
							options_size--;
							notshow=1;
							}
						break;
						}
					}	
				}
				if(notshow==1){
				continue;
				}
				//小於才顯示
				toml_array_t *hide_like_smaller_array=NULL;
				hide_like_smaller_array=toml_array_in(now_options,"hide_like_smaller");
				if(hide_like_smaller_array!=NULL){
				toml_datum_t hide_like_name = toml_string_at(hide_like_smaller_array,0);
				toml_datum_t hide_like_value = toml_int_at(hide_like_smaller_array,1);
					for(int32_t i=0;i<num_character;i++){
					const char *now_character_key=toml_key_in(character_table,i);
						if(strcmp(now_character_key,hide_like_name.u.s)==0){
							if(character_list[i].like>=hide_like_value.u.i){
							options_size--;
							notshow=1;
							}
						break;
						}
					}	
				}
				if(notshow==1){
				continue;
				}
				//確認是不是隱藏選項(道具)
				toml_datum_t hide_item_name = toml_string_in(now_options, "hide_item");
				if(hide_item_name.ok){
				int32_t hide_item_check=0;
					for(int32_t i=0;i<9;i++){
						if(strcmp(backpack[i],hide_item_name.u.s)==0){
						hide_item_check=1;
						break;
						}
					}
					//如果有這個物品就顯示
					if(hide_item_check!=1){
					options_size--;
					notshow=1;
					}
				}
				if(notshow==1){
				continue;
				}
				
				toml_datum_t now_options_text = toml_string_in(now_options, "text");
				toml_datum_t now_options_next = toml_string_in(now_options, "next");
				toml_datum_t now_options_event = toml_string_in(now_options, "event");
					if(option_select==options_size){
						//刪除道具
						toml_datum_t remove_item_name = toml_string_in(now_options, "remove_item");
						if(remove_item_name.ok){
						//確認有沒有這一個物品
						toml_table_t *check_item=NULL;
						check_item=toml_table_in(item_table,remove_item_name.u.s);
							if(check_item!=NULL){
								for(int32_t i=0;i<9;i++){
									if(strcmp(backpack[i],remove_item_name.u.s)==0){
										for(int32_t j=0;j<100;j++){
										backpack[i][j]=0;
										}
									break;
									}
								}
							}
						}
						//如果有道具可以加到道具
						toml_datum_t get_item_name = toml_string_in(now_options, "get_item");
						if(get_item_name.ok){
						//確認有沒有這一個物品
						toml_table_t *check_item=NULL;
						check_item=toml_table_in(item_table,get_item_name.u.s);
							if(check_item!=NULL){
								for(int32_t i=0;i<9;i++){
									if(backpack[i][0]==0){
									strncpy(backpack[i],get_item_name.u.s, 100);
									break;
									}
								}
							}
						}
						//如果有好感度可以加減
						toml_array_t *like_array=NULL;
						like_array=toml_array_in(now_options,"like");
						if(like_array!=NULL){
						toml_datum_t now_character_name = toml_string_at(like_array,0);
						toml_datum_t now_character_like = toml_int_at(like_array,1);
							for(int32_t i=0;i<num_character;i++){
							const char *now_character_key=toml_key_in(character_table,i);
								if(strcmp(now_character_key,now_character_name.u.s)==0){
								character_list[i].like+=now_character_like.u.i;
								/*//test
								for(int32_t i=0;i<num_character;i++){
								printf("%s\n",character_list[i].name);
								printf("%d\n",character_list[i].like);
								}*/
								break;
								}
							}	
						}
						//如果是next
						if(now_options_next.ok){
						//重置
						init_str(now_dialogue_str);
						init_str(now_character_str);
						init_str(now_character_image_str);
						init_str(now_character_avatar_str);
						init_str(now_talk_str);
						init_str(now_text_str);
						init_str(now_options_next_str);
						character_catch=0;
						text_catch=0;
						text_line_which=1;
						emote_line_which=1;
						ch_line_which=1;
						nowtalk_line_which=1;
						sound_line_which=1;
						text_pause=0;
						dialogue_line_which=0;
						first_dialogue_check=0;
						start_option=0;
						option_select=0;
						options_size=0;
						//抓下一個對話的名稱
						strncpy(now_dialogue_str,now_options_next.u.s, 1000);
						break;
						}
						//如果是event
						else if(now_options_event.ok){
						//漸黑特效
						fadeout(now_scene_bg_str,&event);
						init_str(now_event_str);
						init_str(now_event_name_str);
						init_str(now_scene_bg_str);
						init_str(now_scene_name_str);
						init_str(now_dialogue_str);
						init_str(now_character_str);
						init_str(now_character_image_str);
						init_str(now_character_avatar_str);
						init_str(now_talk_str);
						init_str(now_text_str);
						init_str(now_options_next_str);
						scene_catch=0;
						character_catch=0;
						text_catch=0;
						text_line_which=1;
						emote_line_which=1;
						ch_line_which=1;
						nowtalk_line_which=1;
						sound_line_which=1;
						text_pause=0;
						dialogue_line_which=0;
						first_dialogue_check=1;
						start_option=0;
						option_select=0;
						options_size=0;
						//抓下一個事件的名稱
						strncpy(now_event_str,now_options_event.u.s, 1000);
						break;
						}
						else{
						printf("資料有誤！\n");
						return 0;
						}
					}
					//printf("%s\n",now_options_text.u.s);
					if(option_select==0){
					render_image_block(default_button_1.u.s,&option_button,0,options_size*screen_height*0.09,255);
					render_option_Button(now_options_text.u.s,&option_button,0,options_size*screen_height*0.09);
					//截圖
					screenshot();
					}	
				}
				if(start_option==1){
				game_pause=1;
				}
			}
			
		}
		
		if(icon_Change==1 && press_hide==0 && press_CharacterDiscribe==0){
		//放出截圖
		render_scene("assets/image/screenshot.bmp");
		icon_Change=0;
		}
		
		//小圖示改變(如果滑鼠在save上面)(外面)
		if(check_button(mouse_x,mouse_y,&save_block,0,0) && start_game==1 && press_hide==0 && open_backpack==0 && open_save==0  && open_settings==0 && press_CharacterDiscribe==0 && (game_pause==0 || start_option==1)){
			if(save_2.ok){
			render_image_block(save_2.u.s,&save_block,0,0,255);
			icon_Change=1;
			}
		}
		
		//小圖示改變(如果滑鼠在hide上面)(外面)
		if(check_button(mouse_x,mouse_y,&hide_block,0,0) && start_game==1 && press_hide==0 && open_backpack==0 && open_save==0  && open_settings==0 && press_CharacterDiscribe==0 && (game_pause==0 || start_option==1)){
			if(hide_2.ok){
			render_image_block(hide_2.u.s,&hide_block,0,0,255);
			icon_Change=1;
			}
		}
		
		//小圖示改變(如果滑鼠在settings上面)(外面)
		if(check_button(mouse_x,mouse_y,&settings_block,0,0) && start_game==1 && press_hide==0 && open_backpack==0 && open_save==0 && open_settings==0 && press_CharacterDiscribe==0 && (game_pause==0 || start_option==1)){
			if(settings_2.ok){
			render_image_block(settings_2.u.s,&settings_block,0,0,255);
			icon_Change=1;
			}
		}
		
		//小圖示改變(如果滑鼠在backpack上面)(外面)
		if(check_button(mouse_x,mouse_y,&backpack_button,0,0) && start_game==1 && press_hide==0 && open_backpack==0 && open_save==0 && open_settings==0 && press_CharacterDiscribe==0 && (game_pause==0 || start_option==1)){
			if(backpack_2.ok){
			render_image_block(backpack_2.u.s,&backpack_button,0,0,255);
			icon_Change=1;
			}
		}
		
		
		
		
		//選項按鈕互動(如果滑鼠在option上面)(外面)
		for(int32_t i=0;i<options_size;i++){
			if(check_button(mouse_x,mouse_y,&option_button,0,(i+1)*screen_height*0.09) && start_game==1 && start_option==1 && press_hide==0 && press_CharacterDiscribe==0 && open_backpack==0 && open_save==0  && open_settings==0){
				if(default_button_2.ok){
				render_image_block(default_button_2.u.s,&option_button,0,(i+1)*screen_height*0.09,255);
				icon_Change=1;
				}
			}
		}
		
		
		//起始按鈕互動(如果滑鼠在open_option上面)(外面)
		for(int32_t i=0;i<4;i++){
			if(check_button(mouse_x,mouse_y,&open_option_block,0,i*screen_height*0.1) && start_open==1 && open_load==0 && open_OpeningSettings==0){
				if(open_button_2.ok){
				render_image_block(open_button_2.u.s,&open_option_block,0,i*screen_height*0.1,255);
				icon_Change=1;
				}
			}
		}
		
		//設定按鈕互動(如果滑鼠在open_option上面)(外面)
		for(int32_t i=0;i<3;i++){
			if(check_button(mouse_x,mouse_y,&open_option_block,0,i*screen_height*0.1) && open_settings==1){
				if(open_button_2.ok){
				render_image_block(open_button_2.u.s,&open_option_block,0,i*screen_height*0.1,255);
				icon_Change=1;
				}
			}
		}
		//設定調整音量
		for(int32_t i=-2;i<3;i++){
			if(check_button(mouse_x,mouse_y,&open_option_block,screen_width*0.2*i,screen_height*-0.1) && (open_settings==1 || open_OpeningSettings==1)){
				if(open_button_2.ok){
				render_image_block(open_button_2.u.s,&open_option_block,screen_width*0.2*i,screen_height*-0.1,255);	
				icon_Change=1;
				}
			}
		}
				
			////////跑設定畫面//////////
			if(open_settings==1 && settings_pause==0){
			//清除畫面
			clear_renderer();
			//渲染場景
			darker("assets/image/screenshot.bmp");
			//調整音量
			render_image_block(chatbar.u.s,&nowtalk_block,screen_width*-0.05,screen_height*-0.5,255);
			render_option_Button("音量調整",&nowtalk_block,screen_width*-0.05,screen_height*-0.5);
				for(int32_t i=-2;i<3;i++){
				render_image_block(open_button_1.u.s,&open_option_block,screen_width*0.2*i,screen_height*-0.1,255);	
				}
			render_option_Button("0%",&open_option_block,2*screen_width*-0.2,screen_height*-0.1);
			render_option_Button("25%",&open_option_block,1*screen_width*-0.2,screen_height*-0.1);
			render_option_Button("50%",&open_option_block,0*screen_width*0.2,screen_height*-0.1);
			render_option_Button("75%",&open_option_block,1*screen_width*0.2,screen_height*-0.1);
			render_option_Button("100%",&open_option_block,2*screen_width*0.2,screen_height*-0.1);
			
				for(int32_t i=0;i<3;i++){
				render_image_block(open_button_1.u.s,&open_option_block,0,i*screen_height*0.1,255);	
				}
			render_option_Button("返回遊戲",&open_option_block,0,0*screen_height*0.1);
			render_option_Button("回到標題",&open_option_block,0,1*screen_height*0.1);
			render_option_Button("離開遊戲",&open_option_block,0,2*screen_height*0.1);
			settings_pause=1;
			//截圖
			screenshot();
			}			
					
					
			////////開啟背包(只跑過一次就pause)//////////
			if(open_backpack==1 && backpack_pause==0){
			//清除畫面
			clear_renderer();
				//渲染場景
				if(backpack_bg.ok){
				render_scene(backpack_bg.u.s);
				}
				//印出玩家數值
				render_option_Button("玩家設定",&item_block,screen_width*0.682,screen_height*-0.1);
				if(player_avatar.ok){
				render_image_block(player_avatar.u.s,&item_block,screen_width*0.68,0,255);
				}
				if(player_name.ok){
				render_option_Button(player_name.u.s,&item_block,screen_width*0.682,screen_height*0.1);
				}
				if(player_describe.ok){
				//render_option_Button(player_describe.u.s,&item_block,screen_width*0.682,screen_height*0.15);
				render_discribe_Text(player_describe.u.s,&item_block,screen_width*0.682,screen_height*0.15);
				}
				//印出角色數值
				
				//印出背包格子
				for(int32_t i=0;i<9;i++){
					if(backpack_button_1.ok){
					render_image_block(backpack_button_1.u.s,&item_bg_block,((i%3))*screen_width*0.21,(i/3)*screen_height*0.25,255);
					}
				}
				//印出物品
				for(int32_t i=0;i<9;i++){
				toml_datum_t now_item_icon;
				toml_datum_t now_item_describe;
					if(strlen(backpack[i])!=0){
					toml_table_t *now_item_table = NULL;
					now_item_table = toml_table_in(item_table,backpack[i]);
						if(now_item_table!=NULL){
						now_item_icon = toml_string_in(now_item_table,"icon");
						now_item_describe = toml_string_in(now_item_table,"description");
							if(now_item_icon.ok){
							render_image_block(now_item_icon.u.s,&item_block,((i%3))*screen_width*0.21,
							(i/3)*screen_height*0.25,255);
							}
							else{
							render_image_block(missing_image.u.s,&item_block,((i%3))*screen_width*0.21,
							(i/3)*screen_height*0.25,255);
							}
						}
						if(item_select==i){
							render_image_block(now_item_icon.u.s,&item_block,screen_width*0.68,
							screen_height*0.3,255);
							
							////////////////////////////////////wwwwwwwww
							toml_table_t *check_item=NULL;
							check_item=toml_table_in(item_table,backpack[i]);
							toml_datum_t item_name = toml_string_in(check_item, "name");
							if(item_name.ok){
							render_option_Button(item_name.u.s,&item_block,screen_width*0.682,
							screen_height*0.42);
							}
							
							if(now_item_describe.ok){
							render_discribe_Text(now_item_describe.u.s,&item_block,screen_width*0.682,
							screen_height*0.5);
							}
						item_select=-1;
						}
					}
					
				}
			//更新視窗
			present_renderer();
			backpack_pause=1;
			//截圖
			screenshot();
			}
			
			////////開啟儲存畫面(只跑過一次就pause)//////////
			if(open_save==1 && save_pause==0){
			//清除畫面
			clear_renderer();
				//渲染場景
				if(save_bg.ok){
				render_scene(save_bg.u.s);
				}
			//更新視窗
			present_renderer();
			
			for(int32_t i=0;i<8;i++){
			render_image_block(save_button_1.u.s,&save_data_block,0,i*screen_height*0.08,255);
			//渲染欄位
			render_option_Button("事件/場景/時間",
			&save_data_block,screen_width*-0.055,screen_height*-0.07);
			render_option_Button("自動",&open_option_block,screen_width*(-0.46),screen_height*(-0.185));
			//渲染存檔文字
			show_save_info(&save_data_block,character_list,num_character,i);
			}
			save_pause=1;
			//截圖
			screenshot();
			}
			
			////////開啟載入畫面(只跑過一次就pause)//////////
			if(open_load==1 && load_pause==0){
			//清除畫面
			clear_renderer();
				//渲染場景
				if(load_bg.ok){
				render_scene(load_bg.u.s);
				}
			//更新視窗
			present_renderer();
			//渲染欄位
			render_option_Button("事件/場景/時間",
			&save_data_block,screen_width*-0.05,screen_height*-0.07);
			render_option_Button("自動",&open_option_block,screen_width*(-0.46),screen_height*(-0.185));
			for(int32_t i=0;i<8;i++){
			render_image_block(save_button_1.u.s,&save_data_block,0,i*screen_height*0.08,255);
			//渲染存檔文字
			show_save_info(&save_data_block,character_list,num_character,i);
			}
			load_pause=1;
			//截圖
			screenshot();
			}
			
			////////開啟開頭設定畫面(只跑過一次就pause)//////////
			if(open_OpeningSettings==1 && OpeningSettings_pause==0){
			//清除畫面
			clear_renderer();
				//渲染場景
				if(settings_bg.ok){
				render_scene(settings_bg.u.s);
				}
			//更新視窗
			present_renderer();
			//調整音量
			render_image_block(chatbar.u.s,&nowtalk_block,screen_width*-0.05,screen_height*-0.5,255);
			render_option_Button("音量調整",&nowtalk_block,screen_width*-0.05,screen_height*-0.5);
				for(int32_t i=-2;i<3;i++){
				render_image_block(open_button_1.u.s,&open_option_block,screen_width*0.2*i,screen_height*-0.1,255);	
				}
			render_option_Button("0%",&open_option_block,2*screen_width*-0.2,screen_height*-0.1);
			render_option_Button("25%",&open_option_block,1*screen_width*-0.2,screen_height*-0.1);
			render_option_Button("50%",&open_option_block,0*screen_width*0.2,screen_height*-0.1);
			render_option_Button("75%",&open_option_block,1*screen_width*0.2,screen_height*-0.1);
			render_option_Button("100%",&open_option_block,2*screen_width*0.2,screen_height*-0.1);
			OpeningSettings_pause=1;
			//截圖
			screenshot();
			}
			
			
			//如果滑鼠在close上面變圖片(外面)
			if(open_backpack==1 || open_save==1 || open_load==1 || open_OpeningSettings==1){
				if(check_button(mouse_x,mouse_y,&close_button,0,0)){
					if(close_2.ok){
					render_image_block(close_2.u.s,&close_button,0,0,255);
					}
				}
				else{
					//渲染close
					if(close_1.ok){
					render_image_block(close_1.u.s,&close_button,0,0,255);
					}
				}
				//背包物品點選
				if(open_backpack==1){
					for(int32_t i=0;i<9;i++){
						if(check_button(mouse_x,mouse_y,&item_bg_block,(i%3)*screen_width*0.21,
						(i/3)*screen_height*0.25)){
							if(backpack_button_2.ok){
							render_image_block(backpack_button_2.u.s,&item_bg_block,(i%3)*screen_width*0.21,
							(i/3)*screen_height*0.25,255);
							icon_Change=1;
							}
						}
					}
				}
				//如果滑鼠在save_data上面變圖片(外面)
				if(open_save==1 || open_load==1){
					for(int32_t i=0;i<8;i++){
						if(check_button(mouse_x,mouse_y,&save_data_block,0,i*screen_height*0.08)){
							if(save_button_2.ok){
							render_image_block(save_button_2.u.s,&save_data_block,0,i*screen_height*0.08,255);
							icon_Change=1;
							}
						}
					}
				}
			}
				
			
			
			
			
		
		
		//更新視窗
		present_renderer();
		
		//60 fps 
		SDL_Delay(1000 / 60); 
	}

	
	//關閉TTF和SDL和sdl_mixer
	Mix_FreeChunk(open_music);
	Mix_FreeChunk(music1);
	Mix_FreeChunk(sound1);
	Mix_CloseAudio();
	close_SDL();
    TTF_Quit();
    SDL_Quit();
    
    //釋放table
    free(player_name.u.s);
	toml_free(category);
	return 0;
}


//初始化字串
void init_str(char str[1000]){
	for(int32_t i=0;i<1000;i++){
	str[i]=0;
	}
}



