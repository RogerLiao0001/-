#include "SDL_function.h"

//視窗寬高
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
//1080x1024
//設定全域變數
SDL_Window *win =NULL;
SDL_Renderer *rend =NULL;
TTF_Font *font = NULL;


//存檔寫入
void write_save(char now_event_str[1000],char now_dialogue_str[1000],char now_character_str[1000],char now_character_image_str[1000],char now_event_name_str[1000],char now_scene_name_str[1000],int32_t dialogue_line_which,int32_t text_line_which,int32_t emote_line_which,int32_t ch_line_which,struct Character *character_list,int32_t num_character,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	printf("%s\n",filename);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"w+"))==NULL){
		printf("file is not open!\n");
		return;
	}
	fprintf(pfile,"now_event_str=%s\n",now_event_str);
	fprintf(pfile,"now_dialogue_str=%s\n",now_dialogue_str);
	fprintf(pfile,"now_character_str=%s\n",now_character_str);
	fprintf(pfile,"now_character_image_str=%s\n",now_character_image_str);
	fprintf(pfile,"now_event_name_str=%s\n",now_event_name_str);
	fprintf(pfile,"now_scene_name_str=%s\n",now_scene_name_str);
	
	fprintf(pfile,"dialogue_line_which=%d\n",dialogue_line_which);
	fprintf(pfile,"text_line_which=%d\n",text_line_which-1);
	fprintf(pfile,"emote_line_which=%d\n",emote_line_which);
	fprintf(pfile,"ch_line_which=%d\n",ch_line_which);
	for(int32_t i=0;i<num_character;i++){
	fprintf(pfile,"character=%s,like=%d\n",character_list[i].name,character_list[i].like);
	}
	fclose(pfile);	
}
//存檔確認有無
int32_t check_save(struct Character *character_list,int32_t num_character,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	printf("%s\n",filename);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"r+"))==NULL){
	return 0;
	}
	char line[1000];
	char now_event_str[1000]={0};
	char now_dialogue_str[1000]={0};
	char now_character_str[1000]={0};
	char now_character_image_str[1000]={0};
	char now_event_name_str[1000]={0};
	char now_scene_name_str[1000]={0};
	char dialogue_line_which_str[1000]={0};
	char text_line_which_str[1000]={0};
	char emote_line_which_str[1000]={0};
	char ch_line_which_str[1000]={0};
	int32_t dialogue_line_which=0;
	int32_t text_line_which=0;
	int32_t emote_line_which=0;
	int32_t ch_line_which=0;
	int32_t now_num_character=0;
	while(feof(pfile)==0){
	char character_list_check[1000]={0};
	//要再次檢查！！！
	if(fgets(line,sizeof(line),pfile)==NULL){
	break;
	}
	
	//記得換行符要換掉\r\n
	if(line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]=0;
	}

	sscanf(line,"now_event_str=%s",now_event_str);
	sscanf(line,"now_dialogue_str=%s",now_dialogue_str);
	sscanf(line,"now_character_str=%s",now_character_str);
	sscanf(line,"now_character_image_str=%s",now_character_image_str);
	sscanf(line,"now_event_name_str=%s",now_event_name_str);
	sscanf(line,"now_scene_name_str=%s",now_scene_name_str);
	
	sscanf(line,"dialogue_line_which=%s",dialogue_line_which_str);
	sscanf(line,"text_line_which=%s",text_line_which_str);
	sscanf(line,"emote_line_which=%s",emote_line_which_str);
	sscanf(line,"ch_line_which=%s",ch_line_which_str);
		if(sscanf(line,"character=%[^,]",character_list_check)==1){
		printf("%s\n",character_list_check);
			for(int32_t i=0;i<num_character;i++){
				if(strcmp(character_list_check,character_list[i].name)==0){
				now_num_character++;
				break;
				}
			}
		}
	}
	dialogue_line_which=atoi(dialogue_line_which_str);
	text_line_which=atoi(text_line_which_str);
	emote_line_which=atoi(emote_line_which_str);
	ch_line_which=atoi(ch_line_which_str);
	//如果有錯誤算無資料
	if(now_event_str[0]==0 || now_dialogue_str[0]==0 || now_character_str[0]==0 || now_character_image_str[0]==0 || now_event_name_str[0]==0 || now_scene_name_str[0]==0){
	fclose(pfile);	
	return 0;
	}
	
	if((emote_line_which-1)+(ch_line_which-1)+text_line_which-1!=dialogue_line_which){
	fclose(pfile);	
	return 0;
	}
	
	if(now_num_character!=num_character){
	fclose(pfile);	
	return 0;
	}
	return 1;
	fclose(pfile);	
}

//存檔讀取
void read_save(char now_event_str[1000],char now_dialogue_str[1000],char now_character_str[1000],char now_character_image_str[1000],char now_event_name_str[1000],char now_scene_name_str[1000],int32_t *dialogue_line_which,int32_t *text_line_which,int32_t *emote_line_which,int32_t *ch_line_which,struct Character *character_list,int32_t num_character,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"r+"))==NULL){
	return;
	}
	char line[1000];
	
	char dialogue_line_which_str[1000]={0};
	char text_line_which_str[1000]={0};
	char emote_line_which_str[1000]={0};
	char ch_line_which_str[1000]={0};
	char like_str[1000]={0};
	while(feof(pfile)==0){
	char character_list_check[1000]={0};
	int32_t like;
	//要再次檢查！！！
	if(fgets(line,sizeof(line),pfile)==NULL){
	break;
	}
	
	//記得換行符要換掉\r\n
	if(line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]=0;
	}

	sscanf(line,"now_event_str=%s",now_event_str);
	sscanf(line,"now_dialogue_str=%s",now_dialogue_str);
	sscanf(line,"now_character_str=%s",now_character_str);
	sscanf(line,"now_character_image_str=%s",now_character_image_str);
	sscanf(line,"now_event_name_str=%s",now_event_name_str);
	sscanf(line,"now_scene_name_str=%s",now_scene_name_str);
	
	sscanf(line,"dialogue_line_which=%s",dialogue_line_which_str);
	sscanf(line,"text_line_which=%s",text_line_which_str);
	sscanf(line,"emote_line_which=%s",emote_line_which_str);
	sscanf(line,"ch_line_which=%s",ch_line_which_str);
		if(sscanf(line,"character=%[^,],like=%s",character_list_check,like_str)==2){
			for(int32_t i=0;i<num_character;i++){
				if(strcmp(character_list_check,character_list[i].name)==0){
				character_list[i].like=atoi(like_str);
				break;
				}
			}
		}
	}
	*dialogue_line_which=atoi(dialogue_line_which_str);
	*text_line_which=atoi(text_line_which_str);
	*emote_line_which=atoi(emote_line_which_str);
	*ch_line_which=atoi(ch_line_which_str);
	
	fclose(pfile);	
}

//渲染存檔文字function
void show_save_info(button_rect *save_data_block,struct Character *character_list,int32_t num_character,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	printf("%s\n",filename);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"r"))==NULL){
	render_option_Button("無資料",save_data_block,0,save_which*50);
	return;
	}
	
	char timestamp[1000];
	struct stat fileinfo;
	if(stat(filename,&fileinfo)!=-1){
	printf("%s",ctime(&fileinfo.st_mtime));
	strncpy(timestamp,ctime(&fileinfo.st_mtime), 1000);
	}
	char line[1000];
	char now_event_str[1000]={0};
	char now_dialogue_str[1000]={0};
	char now_character_str[1000]={0};
	char now_character_image_str[1000]={0};
	char now_event_name_str[1000]={0};
	char now_scene_name_str[1000]={0};
	char dialogue_line_which_str[1000]={0};
	char text_line_which_str[1000]={0};
	char emote_line_which_str[1000]={0};
	char ch_line_which_str[1000]={0};
	int32_t dialogue_line_which=0;
	int32_t text_line_which=0;
	int32_t emote_line_which=0;
	int32_t ch_line_which=0;
	int32_t now_num_character=0;
	while(feof(pfile)==0){
	char character_list_check[1000]={0};
	//要再次檢查！！！
	if(fgets(line,sizeof(line),pfile)==NULL){
	break;
	}
	
	//記得換行符要換掉\r\n
	if(line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]=0;
	}

	sscanf(line,"now_event_str=%s",now_event_str);
	sscanf(line,"now_dialogue_str=%s",now_dialogue_str);
	sscanf(line,"now_character_str=%s",now_character_str);
	sscanf(line,"now_character_image_str=%s",now_character_image_str);
	sscanf(line,"now_event_name_str=%s",now_event_name_str);
	sscanf(line,"now_scene_name_str=%s",now_scene_name_str);
	
	sscanf(line,"dialogue_line_which=%s",dialogue_line_which_str);
	sscanf(line,"text_line_which=%s",text_line_which_str);
	sscanf(line,"emote_line_which=%s",emote_line_which_str);
	sscanf(line,"ch_line_which=%s",ch_line_which_str);
		if(sscanf(line,"character=%[^,]",character_list_check)==1){
		printf("%s\n",character_list_check);
			for(int32_t i=0;i<num_character;i++){
				if(strcmp(character_list_check,character_list[i].name)==0){
				now_num_character++;
				break;
				}
			}
		}
	}
	dialogue_line_which=atoi(dialogue_line_which_str);
	text_line_which=atoi(text_line_which_str);
	emote_line_which=atoi(emote_line_which_str);
	ch_line_which=atoi(ch_line_which_str);
	//如果有錯誤算無資料
	if(now_event_str[0]==0 || now_dialogue_str[0]==0 || now_character_str[0]==0 || now_character_image_str[0]==0 || now_event_name_str[0]==0 || now_scene_name_str[0]==0){
	render_option_Button("無資料",save_data_block,0,save_which*50);
	fclose(pfile);	
	return;
	}
	
	if((emote_line_which-1)+(ch_line_which-1)+text_line_which-1!=dialogue_line_which){
	render_option_Button("無資料",save_data_block,0,save_which*50);
	fclose(pfile);	
	return;
	}
	
	if(now_num_character!=num_character){
	render_option_Button("無資料",save_data_block,0,save_which*50);
	fclose(pfile);	
	return;
	}
	
	

	//渲染資訊
	render_saveinfo_Button(now_event_name_str,now_scene_name_str,timestamp,save_data_block,0,save_which*50);
	fclose(pfile);	
}

//載入存檔

//渲染場景function
void render_scene(char *path){
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
    //渲染場景
    SDL_RenderCopy(rend,newTexture,NULL,NULL);
	// 釋放表面記憶體
	SDL_FreeSurface(loadedSurface);
	SDL_DestroyTexture(newTexture);
	}
}

//漸出特效function
void fadeout(char *path,SDL_Event *event){
	// 渲染按鈕矩形
	SDL_Rect Rect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	// 逐漸增加 alpha 值，實現漸黑效果
	for (int32_t alpha=0; alpha<=255;alpha+=10){
		if(alpha>0 && path!=NULL){
		render_scene(path);
		}
    SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend,0,0,0,alpha);
	SDL_RenderFillRect(rend,&Rect);
	SDL_RenderPresent(rend);
	SDL_Delay(50); // 延遲一段時間，以控制漸變速度
    }
    while(SDL_PollEvent(event));
}

//漸入特效function
void fadein(char *path,SDL_Event *event){
	// 渲染按鈕矩形
	SDL_Rect Rect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};   
    // 逐漸增加 alpha 值，實現漸黑效果
    for (int32_t alpha=255; alpha>=0;alpha-=10){  
    if(alpha<255){
    render_scene(path);
    }
    SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend,0,0,0,alpha);
	SDL_RenderFillRect(rend,&Rect);
	SDL_RenderPresent(rend);
	SDL_Delay(50); // 延遲一段時間，以控制漸變速度
    }
    while(SDL_PollEvent(event));
}



//按鈕設定數值
void button_setting_rect(button_rect *button,int32_t rect_x,int32_t rect_y,int32_t rect_w,int32_t rect_h){
button->x=rect_x;
button->y=rect_y;
button->w=rect_w;
button->h=rect_h;
}


//渲染文字按鈕function
void render_text_Button(const char *text,button_rect *button,SDL_Color *bgColor,SDL_Color *textColor){
	if(bgColor!=NULL){
	// 渲染按鈕矩形
	SDL_Rect buttonRect={button->x,button->y,button->w,button->h};
	SDL_SetRenderDrawColor(rend, bgColor->r, bgColor->g, bgColor->b, bgColor->a);
	SDL_RenderFillRect(rend,&buttonRect);
	}
	// 渲染按鈕文本
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font,text,*textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend,textSurface);
	SDL_Rect textRect ={button->x+(button->w-textSurface->w)/2,button->y+(button->h-textSurface->h)/2,textSurface->w,textSurface->h};
	SDL_RenderCopy(rend,textTexture,NULL,&textRect);
	// 釋放表面和紋理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


//渲染選項按鈕function
void render_option_Button(const char *text,button_rect *button,int32_t offset_x,int32_t offset_y){
	SDL_Color textColor = {0, 0, 0}; // 黑色文字顏色

	// 渲染按鈕文本
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font,text,textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend,textSurface);
	SDL_Rect textRect ={button->x+(button->w-textSurface->w)/2+offset_x,button->y+(button->h-textSurface->h)/2+offset_y,textSurface->w,textSurface->h};
	SDL_RenderCopy(rend,textTexture,NULL,&textRect);
	// 釋放表面和紋理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

//渲染儲存資訊按鈕function
void render_saveinfo_Button(char now_event_name_str[1000],char now_scene_name_str[1000],char timestamp[1000],button_rect *button,int32_t offset_x,int32_t offset_y){
	//黑色文字顏色
	SDL_Color textColor = {0, 0, 0}; 
	char currentText[500]={0};
	//給事件名字分配15格
	int32_t event_name_length=15;
	//給地點名字分配15格
	int32_t scene_name_length=15;
	//時間秀出年/月/日/ 時:分:秒
	int32_t year=0;
	char month_str[10]={0};
	int32_t month=0;
	int32_t day=0;
	int32_t hour=0;
	int32_t min=0;
	int32_t sec=0;
	//判斷event有多少中文英文或數字
	int32_t chinese=0;
	int32_t normal=0;
	for(int32_t i=0;i<strlen(now_event_name_str);i++){
		//判斷是是英文或數字還是中文
		if(now_event_name_str[i]>=0 && now_event_name_str[i]<=127){
		normal++;
		}
		else{
		//如果不是英文或數字就抓三個
		chinese++;
		i+=2;
		}
	}
	if(chinese+(normal*2)>=event_name_length){
	strncpy(currentText,now_event_name_str,chinese*3+normal-6);
	strcat(currentText,"...");
	}
	else{
	char space=' ';
	event_name_length-=chinese + normal;
	strcpy(currentText,now_event_name_str);
		for(int32_t i=0;i<event_name_length;i++){
		strcat(currentText," ");
		}
	}
	
	//判斷scene有多少中文英文或數字
	chinese=0;
	normal=0;
	for(int32_t i=0;i<strlen(now_scene_name_str);i++){
		//判斷是是英文或數字還是中文
		if(now_scene_name_str[i]>=0 && now_scene_name_str[i]<=127){
		normal++;
		}
		else{
		//如果不是英文或數字就抓三個
		chinese++;
		i+=2;
		}
	}
	if(chinese+(normal*2)>=scene_name_length){
	strncat(currentText,now_scene_name_str,chinese*3+normal-6);
	strcat(currentText,"...");
	}
	else{
	char space=' ';
	scene_name_length-=chinese + normal;
	strcat(currentText,now_scene_name_str);
		for(int32_t i=0;i<scene_name_length;i++){
		strcat(currentText," ");
		}
	}
	
	printf("%s\n",currentText);
	
	//抓日期
	sscanf(timestamp,"%*3[^ ] %3[^ ] %d %d:%d:%d %d",month_str,&day,&hour,&min,&sec,&year);
	//抓月份
	if(strcmp(month_str,"Jan")==0){
	month=1;
	}
	else if(strcmp(month_str,"Feb")==0){
	month=2;
	}
	else if(strcmp(month_str,"Mar")==0){
	month=3;
	}
	else if(strcmp(month_str,"Apr")==0){
	month=4;
	}
	else if(strcmp(month_str,"May")==0){
	month=5;
	}
	else if(strcmp(month_str,"Jun")==0){
	month=6;
	}
	else if(strcmp(month_str,"Jul")==0){
	month=7;
	}
	else if(strcmp(month_str,"Aug")==0){
	month=8;
	}
	else if(strcmp(month_str,"Sep")==0){
	month=9;
	}
	else if(strcmp(month_str,"Oct")==0){
	month=10;
	}
	else if(strcmp(month_str,"Nov")==0){
	month=11;
	}
	else if(strcmp(month_str,"Dec")==0){
	month=12;
	}
	char date[30]={0};
	sprintf(date,"%04d/%02d/%02d %02d:%02d:%02d",year,month,day,hour,min,sec);
	strcat(currentText,date);
	// 依次渲染文字
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, currentText, textColor); 
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
	SDL_Rect destRect = {button->x+20,button->y+(button->h-textSurface->h)/2+offset_y,textSurface->w,textSurface->h}; 
	// 渲染文字紋理
	SDL_RenderCopy(rend, textTexture, NULL, &destRect);
	// 釋放表面和紋理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


//渲染圖片方塊function
void render_image_block(const char *path,button_rect *button,int32_t offset_x,int32_t offset_y){
	// 渲染按鈕矩形
	SDL_Rect buttonRect={button->x + offset_x,button->y + offset_y,button->w,button->h};
	
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

// 檢查點擊位置是否在按鈕上
int32_t check_button(int32_t mouse_x, int32_t mouse_y,button_rect *button,int32_t offset_y){
    // 檢查點擊位置是否在按鈕範圍內
    if (mouse_x >= button->x && mouse_x <= button->x+button->w && mouse_y >= button->y+(offset_y*50) && mouse_y <= button->y+(offset_y*50)+button->h){
	return 1;
    }
    return 0;
}


//渲染對話function
void renderText(const char *text, int32_t x, int32_t y,int32_t delay_put,SDL_Event *event){
    SDL_Color textColor = {0, 0, 0}; // 黑色文字顏色
    int32_t textLength=strlen(text);
    int32_t chinese_many=0;
    int32_t normal_many=0;
    int32_t last_normal=0;
    int32_t last_chinese=0;
    int32_t changeline=0;
    if(delay_put==1){
		//文字慢慢跑出來的效果
		for(int32_t i=0;i<textLength;i++){
		char currentText[4];
		//判斷是是英文或數字還是中文
		if(text[i]>=0 && text[i]<=127){
		currentText[0]=text[i];
		currentText[1]='\0';
		last_normal=1;
		}
		else{
		//如果不是英文或數字就抓三個
		currentText[0]=text[i];
		currentText[1]=text[i+1];
		currentText[2]=text[i+2];
		currentText[3]='\0';
		i+=2;
		last_chinese=1;
		}
		// 依次渲染文字
		SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, currentText, textColor); 
		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
		SDL_Rect destRect = {x+(15*chinese_many)+(9*normal_many),y+(changeline*15),textSurface->w,textSurface->h}; 
		// 渲染文字紋理
		SDL_RenderCopy(rend, textTexture, NULL, &destRect);
		// 釋放表面和紋理
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		// 更新視窗
		SDL_RenderPresent(rend);
		// 延遲一段時間，模擬逐字顯示效果
		SDL_Delay(2);
			//判斷上一個是什麼
			if(last_chinese==1){
			chinese_many++;
			last_chinese=0;
			}
			if(last_normal==1){
			normal_many++;
			last_normal=0;
			}
			//看要不要換行
			if(((15*chinese_many)+(9*normal_many))/456>=1){
			chinese_many=0;
			normal_many=0;
			changeline++;
			}
		}
	while(SDL_PollEvent(event));
	}
}


//初始化東西function
int32_t initialize(){
	//初始化SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
	printf("錯誤初始化 SDL: %s\n", SDL_GetError());
	return 0;
	} 
	//初始化文字TTF
	if(TTF_Init()==-1){
	printf("初始化TTF錯誤 SDL: %s\n", TTF_GetError());
	return 0;
	}
	//創建視窗
	win = SDL_CreateWindow("test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if(win==NULL){
	printf("視窗建立失敗 SDL: %s\n", SDL_GetError());
	return 0;
	}
	//創建渲染器
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if(rend==NULL){
	printf("視窗建立失敗 SDL: %s\n", SDL_GetError());
	return 0;
	}
	//加載字體
	font = TTF_OpenFont("chinese.ttf",15);
	if(font==NULL){
	printf("無法加載字體 SDL: %s\n", TTF_GetError());
	return 0;
	}
	return 1;
}


//清除畫面
void clear_renderer(){
SDL_RenderClear(rend);
}

/*//清除特定畫面
void clear_special_renderer(SDL_Rect *clipRect){
SDL_RenderSetClipRect(rend,clipRect);
SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
SDL_RenderClear(rend);
// 重置裁剪矩形以允許在整個視窗中渲染
SDL_RenderSetClipRect(rend, NULL);
}*/
//更新視窗
void present_renderer(){
SDL_RenderPresent(rend);
}

//關閉SDL
void close_SDL(){
	//清除TTF 
	TTF_CloseFont(font);
	//清除renderer 
	SDL_DestroyRenderer(rend); 
	//清除window 
	SDL_DestroyWindow(win); 
}



