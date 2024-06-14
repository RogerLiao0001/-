#include "toml.h"
#include <errno.h>
#include <stdio.h>
#include<stdint.h>
#include <stdlib.h>
#include <string.h>

toml_table_t *event_table=NULL;
toml_table_t *character_table=NULL;





//處理
//抓對話的function

//抓角色的function



int main(){	
	//打開劇情檔案
	FILE *fp;
	char errbuf[200];
	fp = fopen("test.toml", "r");
	//讀取全部資料
	toml_table_t *category = toml_parse_file(fp, errbuf, sizeof(errbuf));
	fclose(fp);
	
	//玩家
	toml_table_t *player = toml_table_in(category, "player");
	//玩家名稱
	toml_datum_t name = toml_string_in(player, "name");
	printf("name: %s\n", name.u.s);
	//玩家初始背包10格
	char *backpack[10];
	//初始化背包
    for (int i = 0; i <10; i++) {
    //分配100個字元的空間
	backpack[i]=calloc(100, sizeof(char));
    }
	//把一開始的背包存入
	toml_array_t *inventory_arr = toml_array_in(player, "inventory");
	//int32_t inventory_array_size = toml_array_nelem(inventory_arr);
	for(int32_t i = 0; i < 10; i++){
	toml_datum_t item = toml_string_at(inventory_arr, i);
		if (item.ok){
		strncpy(backpack[i],item.u.s, 100);
		free(item.u.s);
		}
	}
	printf("Inventory Items:\n");
	for(int32_t i=0;i<10;i++){
	printf("- %s\n", backpack[i]);
	}
	
	
	//全部事件event_table
	event_table = toml_table_in(category, "event");
	//全部角色character_table
	character_table = toml_table_in(category, "character");
	//全部東西item_table
	toml_table_t *item_table = toml_table_in(category, "item");
	//全部場景scene_table
	toml_table_t *scene_table = toml_table_in(category, "scene");
	//全部場景dialogue_table
	toml_table_t *dialogue_table = toml_table_in(category, "dialogue");
	
	
	
	
	
	
	
	//起始事件event.start
	toml_table_t *event_start = toml_table_in(event_table, "start");
	
	//起始場景
	toml_datum_t start_scene = toml_string_in(event_start, "scene");
	toml_table_t *start_scene_key = toml_table_in(scene_table, start_scene.u.s);
	
	toml_datum_t start_scene_name = toml_string_in(start_scene_key, "name");
	toml_datum_t start_scene_bg = toml_string_in(start_scene_key, "background");
	
	//起始對話
	toml_datum_t start_dialogue = toml_string_in(event_start, "dialogue");
	toml_table_t *start_dialogue_key = toml_table_in(dialogue_table, start_dialogue.u.s);
	//抓對話內容 要一個一個往下抓

    // 遍歷 "dialogue" 表中的每個鍵值對並逐行顯示
    while(1){
    const char *key;
    int32_t i = 0;
		while((key = toml_key_in(start_dialogue_key, i++))!=NULL){
		    toml_datum_t dialogue_line = toml_string_in(start_dialogue_key, key);
		    if (dialogue_line.ok) {
			printf("%s: %s\n", key, dialogue_line.u.s);
			//抓角色
			
			
			//抓text
			//if(sscanf(dialogue_line.u.s,"character=%s",now_character)==1){
			
			
			//抓next
			
			free(dialogue_line.u.s);
		    } 
		    else {
			fprintf(stderr, "Failed to get dialogue line for key: %s\n", key);
		    }
		}
	break;
    }
	
	
	printf("start_scene_name: %s\n", start_scene_name.u.s);
	printf("start_scene_bg: %s\n", start_scene_bg.u.s);
	printf("scene: %s\n", start_scene.u.s);
	printf("dialogue: %s\n", start_dialogue.u.s);

	
	/*//抓character
	while(1){
	int32_t i=0;
	i++;
    const char* key = toml_key_in(category, i);
		if (key==NULL){
		break;
		}
	toml_table_t *server = toml_table_in(category, key);
	
	toml_datum_t host = toml_string_in(server, "text");
	
	printf("host: %s\n", host.u.s);
	
    printf("key %d: %s\n", i, key);
	}*/
	
	
	free(name.u.s);
	toml_free(category);
	
}
