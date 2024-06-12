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
	//遊戲螢幕長寬
	uint32_t screen_height=640;
	uint32_t screen_width=480;
	
	//數值設定
	//title
	button_rect title_block;
	button_setting_rect(&title_block,80,20,500,100);
	//open_option_block
	button_rect open_option_block;
	button_setting_rect(&open_option_block,260,150,100,40);
	//start_button
	button_rect start_button;
	button_setting_rect(&start_button,255,190,140,40);
	//option_button
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
	//save_data
	button_rect save_data_block;
	button_setting_rect(&save_data_block,70,50,500,45);
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
    		printf("角色好感度有錯誤！\n");
    		return 0;
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
	char now_text_str[1000]={0};
	char now_options_next_str[1000]={0};
	
	//預設的圖片
	toml_table_t *game_default = toml_table_in(category, "game_default");
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
	//load
	toml_datum_t load_bg = toml_string_in(game_default, "load_bg");
	//close
	toml_datum_t close_1 = toml_string_in(game_default, "close_1");
	toml_datum_t close_2 = toml_string_in(game_default, "close_2");
	//預設按鈕
	toml_datum_t default_button_1 = toml_string_in(game_default, "default_button_1");
	toml_datum_t default_button_2 = toml_string_in(game_default, "default_button_2");
	toml_datum_t default_button_3 = toml_string_in(game_default, "default_button_3");
	//開場按鈕
	toml_datum_t open_button_1 = toml_string_in(game_default, "open_button_1");
	toml_datum_t open_button_2 = toml_string_in(game_default, "open_button_2");
	toml_datum_t open_button_3 = toml_string_in(game_default, "open_button_3");
	//背包按鈕
	toml_datum_t backpack_button_1 = toml_string_in(game_default, "backpack_button_1");
	toml_datum_t backpack_button_2 = toml_string_in(game_default, "backpack_button_2");
	toml_datum_t backpack_button_3 = toml_string_in(game_default, "backpack_button_3");
	//儲存按鈕
	toml_datum_t save_button_1 = toml_string_in(game_default, "save_button_1");
	toml_datum_t save_button_2 = toml_string_in(game_default, "save_button_2");
	toml_datum_t save_button_3 = toml_string_in(game_default, "save_button_3");

	
	
	
	//開始畫面
	//open
	toml_datum_t open_bg = toml_string_in(game_default, "open_bg");
	toml_datum_t open_title = toml_string_in(game_default, "open_title");
	
	
	
	
	
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
	//開始遊戲,選項 是1 不是0
	int32_t start_open=1;
	int32_t start_game=0;
	int32_t start_option=0;
	//打開背包,儲存畫面,載入遊戲畫面  是1 不是0
	int32_t open_backpack=0;
	int32_t open_save=0;
	int32_t open_load=0;
	//到對話,text,emote,character第幾個
	int32_t dialogue_line_which=0;
	int32_t text_line_which=1;
	int32_t emote_line_which=1;
	int32_t ch_line_which=1;
	//是不是對話的第一條 是1 不是0 (如果是載入就設為0)
	int32_t first_dialogue_check=1;
	
	//選擇的選項
	int32_t option_select=0;
	//總共有幾個選項
	int32_t options_size=0;
	
	
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
				////開始遊戲選項
				if(start_open==1){
					for(int32_t i=0;i<4;i++){
						if(check_button(mouse_x,mouse_y,&open_option_block,i)){
							//新遊戲
							if(i==0){
							start_game=1;
							start_open=0;
							game_pause=0;
							//一開始事件都是event.start
							strncpy(now_event_str,"start", 1000);
							}
							//加載遊戲
							if(i==1){
							open_load=1;
							}
							//遊戲設定
							if(i==2){
							
							}
							//離開遊戲
							if(i==3){
							quit=1;
							break;
							}
						}
					}
				}
				//載入資料
				for(int32_t i=0;i<8;i++){
					if(check_button(mouse_x,mouse_y,&save_data_block,i) && open_load==1 && load_pause==1){
						if(check_save(character_list,num_character,i)){
						//初始化數值
						init_str(now_event_str);
						init_str(now_event_name_str);
						init_str(now_scene_bg_str);
						init_str(now_scene_name_str);
						init_str(now_dialogue_str);
						init_str(now_character_str);
						init_str(now_character_image_str);
						init_str(now_text_str);
						init_str(now_options_next_str);
						scene_catch=0;
						character_catch=1;
						text_catch=0;
						text_line_which=1;
						emote_line_which=1;
						ch_line_which=1;
						text_pause=0;
						dialogue_line_which=0;
						first_dialogue_check=0;
						start_option=0;
						option_select=0;
						options_size=0;
						//載入
						read_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,
						now_event_name_str,now_scene_name_str,&dialogue_line_which,&text_line_which,
						&emote_line_which,&ch_line_which,character_list,num_character,i);	
						//漸黑特效
						fadeout(NULL,&event);
						start_game=1;
						start_open=0;
						game_pause=0;
						open_load=0;
						}
					}
				}
				////關閉載入
				if(check_button(mouse_x,mouse_y,&close_button,0) && open_load==1){
				open_load=0;
				load_pause=0;
				open_pause=0;
				}
				////打開背包
				if(check_button(mouse_x,mouse_y,&backpack_button,0) && start_game==1 && open_backpack==0){
				game_pause=1;
				open_backpack=1;
				}
				////關閉背包
				if(check_button(mouse_x,mouse_y,&close_button,0) && open_backpack==1){
				game_pause=0;
				text_pause=0;
				open_backpack=0;
				backpack_pause=0;
				//把字往回一串
				text_catch=0;
				text_line_which--;
				}
				////打開儲存畫面
				if(check_button(mouse_x,mouse_y,&save_block,0) && start_game==1 && open_save==0){
				game_pause=1;
				open_save=1;
				}
				
				if(open_save==1){
					for(int32_t i=0;i<8;i++){
						if(check_button(mouse_x,mouse_y,&save_data_block,i)){
						write_save(now_event_str,now_dialogue_str,now_character_str,now_character_image_str,now_event_name_str,
						now_scene_name_str,dialogue_line_which,text_line_which,emote_line_which,ch_line_which,
						character_list,num_character,i);
						save_pause=0;
						}
					}
				}
				
				////關閉儲存畫面
				if(check_button(mouse_x,mouse_y,&close_button,0) && open_save==1){
				game_pause=0;
				text_pause=0;
				open_save=0;
				save_pause=0;
				//把字往回一串
				text_catch=0;
				text_line_which--;
				}
				//如果現在在跑對話就遊戲繼續 打開背包或儲存畫面或設定要擋
				if(text_catch==1 && open_backpack==0 && open_save==0){
				dialogue_line_which++;
				text_pause=0;
				}
				//選項按鈕
				if(start_option==1){
					for(int32_t i=0;i<options_size;i++){
						if(check_button(mouse_x,mouse_y,&option_button,i+1)){
						option_select=i+1;
						game_pause=0;
						}
					}
				}
				break;
			}
		}
		////////跑開始畫面//////////
		if(start_open==1 && open_pause==0){
		//清除畫面
		clear_renderer();
		//渲染場景
		render_scene(open_bg.u.s);
		render_image_block(open_title.u.s,&title_block,0,0);
			for(int32_t i=0;i<4;i++){
			render_image_block(open_button_1.u.s,&open_option_block,0,i*50);	
			}
		render_option_Button("新遊戲",&open_option_block,0,0*50);
		render_option_Button("載入遊戲",&open_option_block,0,1*50);
		render_option_Button("遊戲設定",&open_option_block,0,2*50);
		render_option_Button("離開遊戲",&open_option_block,0,3*50);
		open_pause=1;
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
								now_character_image = toml_string_in(now_character_table, "tachie_default");
								catch_sucess=1;
								}
								//開心
								else if(strncmp(dialogue_line.u.s,"happy",5)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_happy");
								catch_sucess=1;
								}
								//害羞
								else if(strncmp(dialogue_line.u.s,"shy",3)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_shy");
								catch_sucess=1;
								}
								//悲傷
								else if(strncmp(dialogue_line.u.s,"sad",3)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_sad");
								catch_sucess=1;
								}
								//生氣
								else if(strncmp(dialogue_line.u.s,"angry",5)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_angry");
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
					init_str(now_text_str);
					init_str(now_options_next_str);
					scene_catch=0;
					character_catch=0;
					text_catch=0;
					text_line_which=1;
					emote_line_which=1;
					ch_line_which=1;
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
		render_image_block(now_character_image_str,&character_block,0,0);
		}
		//顯示對話背景
		if(text_catch==1 && chatbar.ok){
		render_image_block(chatbar.u.s,&chatbar_block,0,0);
		}
		//渲染儲存按鈕 裡面(它才不一直閃)
		if(save_1.ok && start_option==0){
		render_image_block(save_1.u.s,&save_block,0,0);
		}
		
		//渲染背包按鈕 裡面(它才不一直閃)
		if(backpack_1.ok && start_option==0){
		render_image_block(backpack_1.u.s,&backpack_button,0,0);
		}
		
		//渲染文字
		if(text_catch==1){
		renderText(now_text_str,100,365,1,&event);
		text_pause=1;
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
				//確認是不是隱藏選項
				int32_t notshow=0;
				toml_array_t *hide_array=NULL;
				hide_array=toml_array_in(now_options,"hide");
				if(hide_array!=NULL){
				toml_datum_t hide_name = toml_string_at(hide_array,0);
				toml_datum_t hide_like = toml_int_at(hide_array,1);
					for(int32_t i=0;i<num_character;i++){
						if(strcmp(character_list[i].name,hide_name.u.s)==0){
							if(character_list[i].like<hide_like.u.i){
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
				
				toml_datum_t now_options_text = toml_string_in(now_options, "text");
				toml_datum_t now_options_next = toml_string_in(now_options, "next");
				toml_datum_t now_options_event = toml_string_in(now_options, "event");
					if(option_select==options_size){
						//如果有好感度可以加減
						toml_array_t *like_array=NULL;
						like_array=toml_array_in(now_options,"like");
						if(like_array!=NULL){
						toml_datum_t now_character_name = toml_string_at(like_array,0);
						toml_datum_t now_character_like = toml_int_at(like_array,1);
							for(int32_t i=0;i<num_character;i++){
								if(strcmp(character_list[i].name,now_character_name.u.s)==0){
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
						init_str(now_text_str);
						init_str(now_options_next_str);
						character_catch=0;
						text_catch=0;
						text_line_which=1;
						emote_line_which=1;
						ch_line_which=1;
						text_pause=0;
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
						init_str(now_event_name_str);
						init_str(now_scene_bg_str);
						init_str(now_scene_name_str);
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
						text_pause=0;
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
					render_image_block(default_button_1.u.s,&option_button,0,options_size*50);
					render_option_Button(now_options_text.u.s,&option_button,0,options_size*50);	
				}
				if(start_option==1){
				game_pause=1;
				}
			}
			
		}
		
		//小圖示改變(如果滑鼠在save上面)(外面)
		if(check_button(mouse_x,mouse_y,&save_block,0) && start_game==1 && game_pause==0 && start_option==0){
			if(save_2.ok){
			render_image_block(save_2.u.s,&save_block,0,0);
			}
		}
		else if(game_pause==0 && start_option==0){
			//渲染儲存按鈕
			if(save_1.ok){
			render_image_block(save_1.u.s,&save_block,0,0);
			}
		}
		
		
		
		//小圖示改變(如果滑鼠在backpack上面)(外面)
		if(check_button(mouse_x,mouse_y,&backpack_button,0) && start_game==1 && game_pause==0 && start_option==0){
			if(backpack_2.ok){
			render_image_block(backpack_2.u.s,&backpack_button,0,0);
			}
		}
		else if(game_pause==0 && start_option==0){
			//渲染儲存按鈕
			if(backpack_1.ok){
			render_image_block(backpack_1.u.s,&backpack_button,0,0);
			}
		}
		
		
		//選項按鈕互動(如果滑鼠在option上面)(外面)
		for(int32_t i=0;i<options_size;i++){
			if(check_button(mouse_x,mouse_y,&option_button,i+1) && start_game==1 && start_option==1){
				if(default_button_2.ok){
				render_image_block(default_button_2.u.s,&option_button,0,(i+1)*50);
				}
			}
			else if(start_option==1){
				//渲染儲存按鈕
				if(default_button_3.ok){
				render_image_block(default_button_3.u.s,&option_button,0,(i+1)*50);
				}
			}
		}
		
		//起始按鈕互動(如果滑鼠在open_option上面)(外面)
		for(int32_t i=0;i<4;i++){
			if(check_button(mouse_x,mouse_y,&open_option_block,i) && start_open==1 && open_load==0){
				if(open_button_2.ok){
				render_image_block(open_button_2.u.s,&open_option_block,0,i*50);
				}
			}
			else if(start_open==1 && open_load==0){
				//渲染儲存按鈕
				if(open_button_3.ok){
				render_image_block(open_button_3.u.s,&open_option_block,0,i*50);
				}
			}
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
				
				//印出角色數值
				
				//印出背包格子
				for(int32_t i=0;i<9;i++){
					if(backpack_button_1.ok){
					render_image_block(backpack_button_1.u.s,&item_block,((i%3))*130,(i/3)*130);
					}
				}
				//印出物品
				for(int32_t i=0;i<9;i++){
					if(strlen(backpack[i])!=0){
					toml_table_t *now_item_table = NULL;
					now_item_table = toml_table_in(item_table,backpack[i]);
						if(now_item_table!=NULL){
						toml_datum_t now_item_icon = toml_string_in(now_item_table,"icon");
						toml_datum_t now_item_describe = toml_string_in(now_item_table,"description");
							if(now_item_icon.ok){
							render_image_block(now_item_icon.u.s,&item_block,((i%3))*130,(i/3)*130);
							}
						}
					}
				}
			//更新視窗
			present_renderer();
			backpack_pause=1;
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
			render_image_block(save_button_1.u.s,&save_data_block,0,i*50);
			//渲染存檔文字
			show_save_info(&save_data_block,character_list,num_character,i);
			}
		
			save_pause=1;
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
			
			for(int32_t i=0;i<8;i++){
			render_image_block(save_button_1.u.s,&save_data_block,0,i*50);
			//渲染存檔文字
			show_save_info(&save_data_block,character_list,num_character,i);
			}
		
			load_pause=1;
			}
			
			
			//如果滑鼠在close上面變圖片(外面)
			if(open_backpack==1 || open_save==1 || open_load==1){
				if(check_button(mouse_x,mouse_y,&close_button,0)){
					if(close_2.ok){
					render_image_block(close_2.u.s,&close_button,0,0);
					}
				}
				else{
					//渲染close
					if(close_1.ok){
					render_image_block(close_1.u.s,&close_button,0,0);
					}
				}
				//如果滑鼠在save_data上面變圖片(外面)
				if(open_save==1 || open_load==1){
					for(int32_t i=0;i<8;i++){
						if(check_button(mouse_x,mouse_y,&save_data_block,i)){
							if(save_button_2.ok){
							render_image_block(save_button_2.u.s,&save_data_block,0,i*50);
							}
						}
						else{
							if(save_button_3.ok){
							render_image_block(save_button_3.u.s,&save_data_block,0,i*50);
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



