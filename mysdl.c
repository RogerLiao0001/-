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
#include "toml_function.h"

//初始化字串
void init_str(char str[1000]);

int main(int32_t argc,char *argv[]){
	
	///////////////////處理SDL///////////////////
	//初始化
	if(initialize()==0){
	return 0;
	}
	//數值設定
	//start
	button_rect start_button;
	button_setting_rect(&start_button,255,190,140,40);
	//option
	button_rect option_button;
	button_setting_rect(&option_button,180,40,300,40);
	//backpack
	button_rect backpack_button;
	button_setting_rect(&backpack_button,550,50,40,40);
	//close_backpack
	button_rect close_button;
	button_setting_rect(&close_button,0,0,40,40);
	//character
	button_rect character_block;
	button_setting_rect(&character_block,220,170,200,500);
	//save
	button_rect save_block;
	button_setting_rect(&save_block,530,430,40,40);
	//chatbar
	button_rect chatbar_block;
	button_setting_rect(&chatbar_block,80,350,500,100);
	//item icon
	button_rect item_block;
	button_setting_rect(&item_block,40,40,100,100);
	
	
	///////////////////處理toml///////////////////
	//打開劇情檔案.toml
	FILE *fp;
	char errbuf[200];
	fp = fopen("test.toml", "r");
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
	printf("name: %s\n", player_name.u.s);
	//玩家初始背包10格
	char *backpack[9];
	//初始化背包
    for (int i = 0; i <9; i++) {
    //分配100個字元的空間
	backpack[i]=calloc(100, sizeof(char));
    }
	//把一開始的背包存入
	toml_array_t *inventory_arr = toml_array_in(player, "inventory");
	for(int32_t i=0; i<9; i++){
	toml_datum_t item = toml_string_at(inventory_arr, i);
		if (item.ok){
		strncpy(backpack[i],item.u.s, 100);
		free(item.u.s);
		}
	}
	printf("Inventory Items:\n");
	for(int32_t i=0;i<9;i++){
	printf("- %s\n", backpack[i]);
	}
	
	/////讀取進來的字串/////
	char now_event_str[1000]={0};
	char now_dialogue_str[1000]={0};
	char now_scene_bg_str[1000]={0};
	char now_character_str[1000]={0};
	char now_character_image_str[1000]={0};
	char now_text_str[1000]={0};
	char now_options_next_str[1000]={0};
	//預設的字串
	char open_image_str[1000]={0};
	char backpack_image_str[1000]={0};
	char text_bg_str[1000]={0};
	char button_bg_str[1000]={0};
	//一開始事件都是event.start
	strncpy(now_event_str,"start", 1000);
	
	
	//預設的圖片
	toml_table_t *game_default = toml_table_in(category, "game_default");
	//backpack
	toml_datum_t backpack_0 = toml_string_in(game_default, "backpack_0");
	toml_datum_t backpack_1 = toml_string_in(game_default, "backpack_1");
	toml_datum_t backpack_2 = toml_string_in(game_default, "backpack_2");
	//chatbar
	toml_datum_t chatbar = toml_string_in(game_default, "chatbar");
	//button
	toml_datum_t button_1 = toml_string_in(game_default, "button_1");
	toml_datum_t button_2 = toml_string_in(game_default, "button_2");
	toml_datum_t button_3 = toml_string_in(game_default, "button_3");
	//save
	toml_datum_t save_1 = toml_string_in(game_default, "save_1");
	toml_datum_t save_2 = toml_string_in(game_default, "save_2");
	//close
	toml_datum_t close_1 = toml_string_in(game_default, "close_1");
	toml_datum_t close_2 = toml_string_in(game_default, "close_2");
	
	
	
	
	
	/////控制遊戲進行的變數/////
	//關閉遊戲
	SDL_Event event; 
	int32_t quit=0;
	//有沒有抓到scene 有1 沒有0
	int32_t scene_catch=0;
	//有沒有抓到character 有1 沒有0
	int32_t character_catch=0;
	//有沒有抓到text 0沒有 1有
	int32_t text_catch=0;
	//主遊戲暫停(打開背包,設定,儲存)
	int32_t game_stop=0;
	int32_t text_stop=0;
	//暫停視窗
	int32_t backpack_stop=0;
	int32_t save_stop=0;
	//到對話的第幾條
	int32_t dialogue_line_which=0;
	//到text第幾個
	int32_t text_line_which=1;
	//到emote第幾個
	int32_t emote_line_which=1;
	//到character第幾個
	int32_t ch_line_which=1;
	//是不是對話的第一條 是1 不是0
	int32_t first_dialogue_check=1;
	//開始選項 是1 不是0
	int32_t start_option=0;
	//選擇的選項
	int32_t option_select=0;
	//打開背包  是1 不是0
	int32_t open_backpack=0;
	//打開儲存畫面  是1 不是0
	int32_t open_save=0;
	//int32_t test=0;
	//總共有幾個選項
	int32_t options_size=0;
	/*//清除特定區域
	SDL_Rect clip_text={100,365,500,1};*/
	//主事件迴圈
	while (!quit) {
	//獲取滑鼠位置
	int32_t mouse_x,mouse_y;
	SDL_GetMouseState(&mouse_x,&mouse_y);
		//處理事件
		while (SDL_PollEvent(&event)){
			switch (event.type){ 
			//按下離開按鍵離開遊戲 在看看要不要自動存檔
			case SDL_QUIT: 
				quit=1; 
				break; 
			//滑鼠點擊
			case SDL_MOUSEBUTTONDOWN:
				//打開背包
				if(check_button(mouse_x,mouse_y,&backpack_button,0)){
				game_stop=1;
				open_backpack=1;
				}
				//關閉背包
				if(check_button(mouse_x,mouse_y,&close_button,0) && open_backpack==1){
				game_stop=0;
				text_stop=0;
				open_backpack=0;
				backpack_stop=0;
				//把字往回一串
				text_catch=0;
				text_line_which--;
				}
				
				//打開儲存畫面
				if(check_button(mouse_x,mouse_y,&save_block,0)){
				game_stop=1;
				open_save=1;
				}
				//關閉儲存畫面
				if(check_button(mouse_x,mouse_y,&close_button,0) && open_save==1){
				game_stop=0;
				text_stop=0;
				open_save=0;
				save_stop=0;
				//把字往回一串
				text_catch=0;
				text_line_which--;
				}
				
				
				//如果現在在跑對話就遊戲繼續 打開背包或儲存畫面或設定要擋
				if(text_catch==1 && open_backpack==0 && open_save==0){
				dialogue_line_which++;
				text_stop=0;
				}
				//選項按鈕
				if(start_option==1){
					for(int32_t i=0;i<options_size;i++){
						if(check_button(mouse_x,mouse_y,&option_button,i+1)){
						option_select=i+1;
						game_stop=0;
						}
					}
				}
				break;
			}
		}
		
		////////跑對話和選項//////////
		//如果遊戲沒有暫停
		if(game_stop==0 && text_stop==0){
		
		//清除畫面
		clear_renderer();
		
		
		//配置現在事件event
		toml_table_t *now_event_table = toml_table_in(event_table, now_event_str);
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
		//讀入scene.background
		//如果場景沒抓過才要抓 抓完設置為有抓過
		if(scene_catch==0){
		toml_datum_t now_scene_bg = toml_string_in(now_scene_table, "background");
			//如果有抓到背景
			if(now_scene_bg.ok){
			//把現在background存下來
			init_str(now_scene_bg_str);
			strncpy(now_scene_bg_str,now_scene_bg.u.s, 1000);
			//漸入
			fadein(now_scene_bg_str,&event);
			}
			else{
			printf("現在場景沒有background\n");
			return 0;
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
			}
			else{
			printf("現在事件沒有第一個對話\n");
			return 0;
			}
		}
		else{
		now_dialogue_table = toml_table_in(dialogue_table, now_dialogue_str);
		}
		
		//如果沒有開始選項
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
							int32_t catch_sucess=0;
								//預設表情
								if(strncmp(dialogue_line.u.s,"default",7)==0){
								now_character_image = toml_string_in(now_character_table, "default");
								catch_sucess=1;
								}
								//開心
								else if(strncmp(dialogue_line.u.s,"happy",5)==0){
								now_character_image = toml_string_in(now_character_table, "happy");
								catch_sucess=1;
								}
								//害羞
								else if(strncmp(dialogue_line.u.s,"shy",3)==0){
								now_character_image = toml_string_in(now_character_table, "shy");
								catch_sucess=1;
								}
								//悲傷
								else if(strncmp(dialogue_line.u.s,"sad",3)==0){
								now_character_image = toml_string_in(now_character_table, "sad");
								catch_sucess=1;
								}
								//生氣
								else if(strncmp(dialogue_line.u.s,"angry",5)==0){
								now_character_image = toml_string_in(now_character_table, "angry");
								catch_sucess=1;
								}
								//清除
								else if(strncmp(dialogue_line.u.s,"clear",5)==0){
								init_str(now_character_image_str);
								//對話換下一條
								dialogue_line_which++;
								//不是在文字
								text_catch=0;
								continue;
								}
								if(catch_sucess==1){
								//把現在人物存下來
								init_str(now_character_image_str);
								strncpy(now_character_image_str,now_character_image.u.s, 1000);
								//對話換下一條
								dialogue_line_which++;
								//不是在文字
								text_catch=0;
								}
								else{
								printf("toml格式錯誤，沒有抓到表情！\n");
								return 0;
								}
							}
							else{
							printf("toml格式錯誤，沒有抓到角色！\n");
							return 0;
							}
						}
						else{
						printf("表情順序有問題\n");
						return 0;
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
					init_str(now_text_str);
					init_str(now_options_next_str);
					character_catch=0;
					text_catch=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
					text_stop=0;
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
					init_str(now_scene_bg_str);
					init_str(now_dialogue_str);
					init_str(now_character_str);
					init_str(now_character_image_str);
					init_str(now_text_str);
					init_str(now_options_next_str);
					scene_catch=0;
					character_catch=0;
					text_catch=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
					text_stop=0;
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
		render_image_block(now_character_image_str,&character_block);
		}
		//顯示對話背景
		if(text_catch==1 && chatbar.ok){
		render_image_block(chatbar.u.s,&chatbar_block);
		}
		//渲染儲存按鈕 裡面(它才不一直閃)
		if(save_1.ok && start_option==0){
		render_image_block(save_1.u.s,&save_block);
		}
		
		//渲染背包按鈕 裡面(它才不一直閃)
		if(backpack_1.ok && start_option==0){
		render_image_block(backpack_1.u.s,&backpack_button);
		}
		
		//渲染文字
		if(text_catch==1){
		renderText(now_text_str,100,365,1,&event);
		text_stop=1;
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
			sprintf(options_id,"%d",options_size);
			toml_table_t *now_options = toml_table_in(now_options_table,options_id);
			toml_datum_t now_options_text = toml_string_in(now_options, "text");
			toml_datum_t now_options_next = toml_string_in(now_options, "next");
			toml_datum_t now_options_event = toml_string_in(now_options, "event");
				if(option_select==options_size){
					//如果是next
					if(now_options_next.ok){
					//重置
					init_str(now_dialogue_str);
					init_str(now_character_str);
					init_str(now_character_image_str);
					init_str(now_text_str);
					init_str(now_options_next_str);
					character_catch=0;
					text_catch=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
					text_stop=0;
					dialogue_line_which=0;
					first_dialogue_check=0;
					start_option=0;
					option_select=0;
					options_size=0;
					//抓下一個對話的名稱
					strncpy(now_dialogue_str,now_options_next.u.s, 1000);
					continue;
					}
					//如果是event
					else if(now_options_event.ok){
					//漸黑特效
					fadeout(now_scene_bg_str,&event);
					init_str(now_event_str);
					init_str(now_scene_bg_str);
					init_str(now_dialogue_str);
					init_str(now_character_str);
					init_str(now_character_image_str);
					init_str(now_text_str);
					init_str(now_options_next_str);
					scene_catch=0;
					character_catch=0;
					text_catch=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
					text_stop=0;
					dialogue_line_which=0;
					first_dialogue_check=1;
					start_option=0;
					option_select=0;
					options_size=0;
					//抓下一個事件的名稱
					strncpy(now_event_str,now_options_event.u.s, 1000);
					continue;
					}
					else{
					printf("資料有誤！\n");
					return 0;
					}
				}
				//printf("%s\n",now_options_text.u.s);
				render_option_block(button_1.u.s,&option_button,options_size);
				render_option_Button(now_options_text.u.s,&option_button,options_size);	
			}
			if(start_option==1){
			game_stop=1;
			}
		}
		
		
		}
		//如果滑鼠在save上面變圖片(外面)
		if(check_button(mouse_x,mouse_y,&save_block,0) && game_stop==0 && start_option==0){
			if(save_2.ok){
			render_image_block(save_2.u.s,&save_block);
			}
		}
		else if(game_stop==0 && start_option==0){
			//渲染儲存按鈕
			if(save_1.ok){
			render_image_block(save_1.u.s,&save_block);
			}
		}
		
		
		
		//如果滑鼠在backpack上面變圖片(外面)
		if(check_button(mouse_x,mouse_y,&backpack_button,0) && game_stop==0 && start_option==0){
			if(backpack_2.ok){
			render_image_block(backpack_2.u.s,&backpack_button);
			}
		}
		else if(game_stop==0 && start_option==0){
			//渲染儲存按鈕
			if(backpack_1.ok){
			render_image_block(backpack_1.u.s,&backpack_button);
			}
		}
		
		
		//如果滑鼠在option上面變圖片(外面)
		for(int32_t i=0;i<options_size;i++){
			if(check_button(mouse_x,mouse_y,&option_button,i+1) && start_option==1){
				if(button_2.ok){
				render_option_block(button_2.u.s,&option_button,i+1);
				}
			}
			else if(start_option==1){
				//渲染儲存按鈕
				if(button_1.ok){
				render_option_block(button_3.u.s,&option_button,i+1);
				}
			}
		}
					
					
					
					
			////////開啟背包(只跑過一次就stop)//////////
			if(open_backpack==1 && backpack_stop==0){
			//清除畫面
			clear_renderer();
				//渲染場景
				if(backpack_0.ok){
				render_scene(backpack_0.u.s);
				}
				//印出玩家數值
				
				//印出角色數值
			
				//印出背包
				for(int32_t i=0;i<9;i++){
					if(strlen(backpack[i])!=0){
					toml_table_t *now_item_table = NULL;
					now_item_table = toml_table_in(item_table,backpack[i]);
						if(now_item_table!=NULL){
						toml_datum_t now_item_icon = toml_string_in(now_item_table,"icon");
						toml_datum_t now_item_describe = toml_string_in(now_item_table,"description");
							if(now_item_icon.ok){
							render_item_block(now_item_icon.u.s,&item_block,i,i/3);
							}
						}
					}
				}
			//更新視窗
			present_renderer();
			backpack_stop=1;
			}
			
			if(open_backpack==1){
			//如果滑鼠在close上面變圖片(外面)
				if(check_button(mouse_x,mouse_y,&close_button,0)){
					if(backpack_2.ok){
					render_image_block(backpack_2.u.s,&close_button);
					}
				}
				else{
					//渲染close
					if(backpack_1.ok){
					render_image_block(backpack_1.u.s,&close_button);
					}
				}
			}
				
			
			////////開啟儲存畫面(只跑過一次就stop)//////////
			if(open_save==1 && save_stop==0){
			//清除畫面
			clear_renderer();
				//渲染場景
				if(backpack_0.ok){
				render_scene(backpack_0.u.s);
				}
			//更新視窗
			present_renderer();
			save_stop=1;
			}
			
			if(open_save==1){
			//如果滑鼠在close上面變圖片(外面)
				if(check_button(mouse_x,mouse_y,&close_button,0)){
					if(backpack_2.ok){
					render_image_block(backpack_2.u.s,&close_button);
					}
				}
				else{
					//渲染close
					if(backpack_1.ok){
					render_image_block(backpack_1.u.s,&close_button);
					}
				}
			}
		
		
		//更新視窗
		present_renderer();
		
		//60 fps 
		SDL_Delay(1000 / 60); 
	}

	
	//關閉TTF和SDL
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



