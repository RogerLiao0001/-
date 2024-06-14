/*//載入場景function
SDL_Texture *loadTexture(char *path){
	SDL_Texture* newTexture = NULL;
	// 載入圖片
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL){
	printf("無法載入圖片 %s！ SDL錯誤：%s\n", path, SDL_GetError());
    } 
	else{
	// 將表面轉換成紋理
	newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);
        if (newTexture == NULL){
		printf("無法創建紋理 %s！ SDL錯誤：%s\n", path, SDL_GetError());
        }
	// 釋放表面記憶體
	SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}*/




/*if(scene==1){
					//檢查背包按鈕
					if(click_button(mouse_x,mouse_y,&backpack_button)){
					printf("背包開啟了！\n");
					scene=2;
					}
				}
				if(scene==2){
					//檢查關閉背包按鈕
					if(click_button(mouse_x,mouse_y,&close_backpack_button)) {
					printf("背包關閉了！\n");
					scene=1;
					}
				}*/






/*//scene 0
		if(scene==0){
		//渲染場景
		render_scene("Tux.jpg");
		//渲染開始按鈕
		render_text_Button("開始遊戲",&start_button,&bgColor,&textColor);
			if(text_appear==0){
			renderText("我雞巴很大",100,400,1);
			text_appear=1;
			}
			else{
			renderText("我雞巴很大",100,400,0);
			}
		}
		//scene 1
		if(scene==1){
		//渲染場景
		render_scene("a.jpg");
		//渲染角色
		render_character("character.png");
		//渲染背包按鈕
		render_picture_Button("character.png",&backpack_button);
		}
		//scene 2(backpack)
		if(scene==2){
		render_scene("backpack.png");
		render_text_Button("關閉背包",&close_backpack_button,&bgColor,&textColor);
		}*/
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		// 遍歷 "dialogue" 表中的每個鍵值對並逐行顯示
		const char *key=NULL;
		key=toml_key_in(now_dialogue_array, dialogue_line_which);
		printf("%d",dialogue_line_which);
		printf("%s",key);
		if(key!=NULL && strncmp(key,"options",7)!=0 && start_option==0){
			toml_datum_t dialogue_line = toml_string_in(now_dialogue_array, key);
			if (dialogue_line.ok){
			printf("%s: %s\n", key, dialogue_line.u.s);
				//抓角色
				if(strncmp(key,"character",9)==0){
				//printf("%s\n", now_character);
				toml_table_t *now_character_table = toml_table_in(character_table, dialogue_line.u.s);
				toml_datum_t now_character_tachie = toml_string_in(now_character_table, "tachie");
					//clear
					for(int32_t i=0;i<1000;i++){
					now_character_image[i]=0;
					}
				strncpy(now_character_image,now_character_tachie.u.s, 1000);
				dialogue_line_which++;
				printf("%s\n", now_character_tachie.u.s);
				}
				//渲染角色
				render_character(now_character_image);
					
				//抓text
				if(strncmp(key,"text",4)==0){
				strncpy(now_text,dialogue_line.u.s, 1000);
					//渲染文字
					if(text_appear==0){
					renderText(now_text,100,400,1,&event);
					text_appear=1;
					}
					else{
					renderText(now_text,100,400,0,&event);
					}	
				}
				//抓next
				if(strncmp(key,"next",4)==0){
					for(int32_t i=0;i<1000;i++){
					now_dialogue[i]=0;
					}
				strncpy(now_dialogue,dialogue_line.u.s, 1000);
				first_dialogue_check=1;
				dialogue_line_which=0;
				}	
			}
		free(dialogue_line.u.s);
		}
		else{
		start_option=1;
		}
		
		/*//抓option
		if(start_option==1){
				 // 從 "dialogue.happy.options" 表中獲取內容
				toml_array_t *options_array = toml_array_in(now_dialogue_array, "options");
				test++;
				printf("\ntest=%d\n",test);
				if (!options_array) {
					fprintf(stderr, "Options array not found in dialogue.happy\n");
					toml_free(category);
					return 0;
				}
				// 遍歷 "dialogue.happy.options" 表中的每個元素並顯示
				int32_t options_size = toml_array_nelem(options_array);
				printf("options_size:%d\n",options_size);
				for(int i = 0; i < options_size; i++){
					toml_table_t *option = toml_table_at(options_array, i);
					if(option!=0){
						toml_datum_t options_text = toml_string_in(option, "text");
						printf("%s\n",options_text.u.s);
						toml_datum_t options_next = toml_string_in(option, "next");
						if (options_text.ok && options_next.ok) {
						    render_option_Button(options_text.u.s,&option_button,&bgColor,&textColor,i);
						} else {
						    fprintf(stderr, "Failed to get option at index %d\n", i);
						}
						toml_free(option);
					}
					else{
					fprintf(stderr, "Failed to get option table at index %d\n", i);
					}
				}
		}*/
		
		
		
		
		
		
		
		
		
		
		
		
		
		/*//抓對話
		toml_datum_t first_dialogue;
		toml_table_t *now_dialogue_array=NULL;
		if(first_dialogue_check==0){
		first_dialogue = toml_string_in(now_event_table, "dialogue");
		now_dialogue_array = toml_table_in(dialogue_table, first_dialogue.u.s);
		}
		else{
		printf("%s\n",now_dialogue);
		now_dialogue_array = toml_table_in(dialogue_table,now_dialogue);
		}
		char line[1000];
		fseek(fp,0,SEEK_SET);
		while(feof(fp)==0){
			if(fgets(line,sizeof(line),fp)==NULL){
			break;
			}
			//printf("%s",line);
			//記得換行符要換掉\r\n
			if(line[strlen(line)-1]=='\n'){
			line[strlen(line)-2]=0;
			}
			char which_dialogue[100]={0};
			if(sscanf(line,"[dialogue.%s]",which_dialogue)==1){
				if(strcmp(first_dialogue.u.s,which_dialogue)==0){
				now_pointer=fp;
				break;
				}
			}
		}
		if(fgets(line,sizeof(line),now_pointer)==NULL){
		break;
		}
		printf("%s",line);
		//記得換行符要換掉\r\n
		if(line[strlen(line)-1]=='\n'){
		line[strlen(line)-2]=0;
		}
		
		//抓角色
		if(sscanf(line,"character=\"%s\"",now_character)==1){
		//printf("%s\n", now_character);
		toml_table_t *now_character_table = toml_table_in(character_table,now_character);
		toml_datum_t now_character_tachie = toml_string_in(now_character_table, "tachie");
			//clear
			for(int32_t i=0;i<1000;i++){
			now_character_image[i]=0;
			}
		strncpy(now_character_image,now_character_tachie.u.s, 1000);
			if(fgets(line,sizeof(line),now_pointer)==NULL){
			break;
			}
			//printf("%s",line);
			//記得換行符要換掉\r\n
			if(line[strlen(line)-1]=='\n'){
			line[strlen(line)-2]=0;
			}
		//printf("%s\n", now_character_tachie.u.s);
		}
		//渲染角色
		//render_character(now_character_image);*/
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
						//開始選項
				
				
				if(start_option==1 && end_option==0){
				// 從現在dialogue表中獲取options
				toml_array_t *all_options = toml_array_in(now_dialogue_table, "options");
					if (!all_options) {
						printf("Options array not found in dialogue.happy\n");
						toml_free(category);
						return 0;
					}
				// 遍歷 "dialogue.happy.options" 表中的每個元素並顯示
				options_size = toml_array_nelem(all_options);
				printf("options_size:%d\n",options_size);
				option_array=realloc(option_array,options_size*sizeof(options_struct));
					for(int32_t i = 0; i < options_size; i++){
					toml_table_t *option = toml_table_at(all_options, i);
						if(option!=0){
						toml_datum_t options_text = toml_string_in(option, "text");
						printf("option.text=%s\n",options_text.u.s);
						toml_datum_t options_next = toml_string_in(option, "next");
							if (options_text.ok && options_next.ok) {
							//option.text
							for(int32_t i=0;i<1000;i++){
							option_array[i].text[i]=0;
							}
							strncpy(option_array[i].text,options_text.u.s, 1000);
							//option.next
							for(int32_t i=0;i<1000;i++){
							option_array[i].next[i]=0;
							}
							strncpy(option_array[i].text,options_next.u.s, 1000);
							} else {
								printf("Failed to get option at index %d\n", i);
							}
							toml_free(option);
						}
						else{
						printf("Failed to get option table at index %d\n", i);
						}
					}
					
				end_option=1;
				}
				
				for(int32_t i = 0; i < options_size; i++){
				printf("option.text=%s\n",option_array[i].text);
				printf("option.next=%s\n",option_array[i].next);
				}
				
				
				
				
				
			//鍵盤按下
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_SPACE){
				printf("鍵盤被點擊了！\n");
                }
                break;
				
					//一個
					if(options_size>=1){
						if(click_button(mouse_x,mouse_y,&option_button,1)){
						option_select=1;
						}
					}
					//兩個
					if(options_size>=2){
						if (click_button(mouse_x,mouse_y,&option_button,2)) {
						option_select=2;
						}
					}
					//三個
					if(options_size>=3){
						if (click_button(mouse_x,mouse_y,&option_button,3)) {
						option_select=3;
						printf("now\n");
						}
					}
					
					
					
					
		//文字慢慢跑出來的效果
		for(int32_t i=0;i<textLength;i++){
		printf("%d\n",text[i]);
		// 依次渲染文字
		char currentText[i+2];
		strncpy(currentText, text, i+1);
		currentText[i+1] = '\0';
		
		SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, currentText, textColor); 
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rend, textSurface); 
		SDL_Rect destRect = {x, y, textSurface->w, textSurface->h}; 
		    
		
		// 渲染文字紋理
		SDL_RenderCopy(rend, textTexture, NULL, &destRect);

		// 釋放表面和紋理
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		
		// 更新視窗
		SDL_RenderPresent(rend);

		// 延遲一段時間，模擬逐字顯示效果
		SDL_Delay(1); 
		}
		while(SDL_PollEvent(event));
		
		
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, textColor); 
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rend, textSurface); 
	SDL_Rect destRect = {x, y, textSurface->w, textSurface->h}; 
	// 渲染文字紋理
	SDL_RenderCopy(rend, textTexture, NULL, &destRect);
	// 釋放表面和紋理
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	// 更新視窗
	SDL_RenderPresent(rend);
	while(SDL_PollEvent(event));			
				
				
				
				
	/*//渲染選項方塊function
void render_option_block(const char *path,button_rect *button,int32_t offset_y){
	// 渲染按鈕矩形
	SDL_Rect buttonRect={button->x,button->y+(offset_y*50),button->w,button->h};
	SDL_Texture* newTexture = NULL;
	// 載入圖片
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL){
	printf("無法載入圖片 %s！ SDL錯誤：%s\n", path, SDL_GetError());
	} 
	else{
	// 將表面轉換成紋理
	newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);
		if (newTexture == NULL){
		printf("無法創建紋理 %s！ SDL錯誤：%s\n", path, SDL_GetError());
		}
	//渲染圖片按鈕
    SDL_RenderCopy(rend,newTexture,NULL,&buttonRect);
	// 釋放表面和紋理
	SDL_FreeSurface(loadedSurface);
	SDL_DestroyTexture(newTexture);
	}
}

//渲染物品方塊function
void render_item_block(const char *path,button_rect *button,int32_t offset_x,int32_t offset_y){
	// 渲染按鈕矩形
	SDL_Rect buttonRect={button->x+(offset_x*130),button->y+(offset_y*130),button->w,button->h};
	SDL_Texture* newTexture = NULL;
	// 載入圖片
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL){
	printf("無法載入圖片 %s！ SDL錯誤：%s\n", path, SDL_GetError());
	} 
	else{
	// 將表面轉換成紋理
	newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);
		if (newTexture == NULL){
		printf("無法創建紋理 %s！ SDL錯誤：%s\n", path, SDL_GetError());
		}
	//渲染圖片按鈕
    SDL_RenderCopy(rend,newTexture,NULL,&buttonRect);
	// 釋放表面和紋理
	SDL_FreeSurface(loadedSurface);
	SDL_DestroyTexture(newTexture);
	}
}*/			
				
				
				
				
		//title
	button_rect title_block;
	button_setting_rect(&title_block,(screen_width/2)*0.33,screen_height*0.07,screen_width*0.7,screen_height*0.12);
	//open_option_block
	button_rect open_option_block;
	button_setting_rect(&open_option_block,screen_width*0.42,screen_height*0.25,screen_width*0.156,screen_height*0.083);			
		
		
		
		
				//起始按鈕互動(如果滑鼠在open_option上面)(外面)
		for(int32_t i=0;i<4;i++){
			if(check_button(mouse_x,mouse_y,&open_option_block,i*screen_height*0.1) && start_open==1 && open_load==0){
				if(open_button_2.ok){
				render_image_block(open_button_2.u.s,&open_option_block,0,i*screen_height*0.1);
				}
			}
			else if(start_open==1 && open_load==0){
				//渲染儲存按鈕
				if(open_button_3.ok){
				render_image_block(open_button_3.u.s,&open_option_block,0,i*screen_height*0.1);
				}
			}
		}
		
		
		
		
		
										/*//預設表情
								if(strncmp(dialogue_line.u.s,"default",7)==0){
								now_character_image = toml_string_in(now_character_table,now_tachie_str);
								now_character_avatar = toml_string_in(now_character_table,now_avatar_str);
								catch_sucess=1;
								}
								else if(strncmp(dialogue_line.u.s,"emote_1",7)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_emote_1");
								now_character_avatar = toml_string_in(now_character_table, "avatar_emote_1");
								catch_sucess=1;
								}
								else if(strncmp(dialogue_line.u.s,"emote_2",7)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_emote_2");
								now_character_avatar = toml_string_in(now_character_table, "avatar_emote_2");
								catch_sucess=1;
								}
								else if(strncmp(dialogue_line.u.s,"emote_3",7)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_emote_3");
								now_character_avatar = toml_string_in(now_character_table, "avatar_emote_3");
								catch_sucess=1;
								}
								else if(strncmp(dialogue_line.u.s,"emote_4",7)==0){
								now_character_image = toml_string_in(now_character_table, "tachie_emote_4");
								now_character_avatar = toml_string_in(now_character_table, "avatar_emote_4");
								catch_sucess=1;
								}*/
								
								
								
								/*if(catch_sucess==1){
								
								}
								else{
								printf("toml格式錯誤，沒有抓到表情！\n");
								dialogue_line_which++;
								//return 0;
								}*/
		
		
				/*//llm開始
		if(start_llm==1){
		int32_t change=0;
		if(llm_first_time==1){
		//清除畫面
		clear_renderer();
		//渲染場景
		render_scene(open_bg.u.s);
		llm_first_time=0;
		}
			while (SDL_PollEvent(&event)){
				if(event.type==SDL_QUIT){
				quit=1;
				}
				else if(event.type==SDL_TEXTINPUT){
					if(strlen(llm_my_text)<999){
					strcat(llm_my_text,event.text.text);
					change=1;
					}
				}
				else if(event.type==SDL_KEYDOWN){
					if(event.key.keysym.sym==SDLK_BACKSPACE && strlen(llm_my_text)>0){
					llm_my_text[strlen(llm_my_text)-1]='\0';
					change=1;
					}
					if(event.key.keysym.sym==SDLK_RETURN){
						//跑llm
						//印出llm的文字
						if(strlen(llm_machine_text)>0){
						init_str(llm_my_text);
						render_llm_Text(llm_machine_text,100,100,&event);
						}
					}
				}
			}
			if(strlen(llm_my_text)>0 && change==1){
			change=0;
			//清除畫面
			clear_renderer();
			//渲染場景
			render_scene(open_bg.u.s);
			render_llm_Text(llm_my_text,100,100,&event);
			}
		}*/
		
		
				
				
				
				
