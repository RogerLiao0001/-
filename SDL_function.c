#include "SDL_function.h"

//視窗寬高
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 810
//640x480
//1080x810
//設定全域變數
SDL_Window *win =NULL;
SDL_Renderer *rend =NULL;
TTF_Font *font = NULL;

//截圖
void screenshot(){
uint32_t format=SDL_PIXELFORMAT_ARGB8888;
SDL_Surface *surface = SDL_CreateRGBSurface(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
SDL_RenderReadPixels(rend, NULL,format, surface->pixels, surface->pitch);
SDL_SaveBMP(surface,"assets/image/screenshot.bmp");
SDL_FreeSurface(surface);
}

//存檔寫入
void write_save(char now_event_str[200],char now_dialogue_str[200],char now_character_str[200],char now_character_image_str[200],char now_character_avatar_str[200],char now_talk_str[200],char now_text_str[200],char now_event_name_str[200],char now_scene_name_str[200],int32_t dialogue_line_which,int32_t text_line_which,int32_t emote_line_which,int32_t ch_line_which,int32_t nowtalk_line_which,int32_t sound_line_which,struct Character *character_list,int32_t num_character,int32_t start_option,char **backpack,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	//printf("%s\n",filename);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"w+"))==NULL){
		printf("file is not open!\n");
		return;
	}
	fprintf(pfile,"now_event_str=%s\n",now_event_str);
	fprintf(pfile,"now_dialogue_str=%s\n",now_dialogue_str);
	fprintf(pfile,"now_character_str=%s\n",now_character_str);
	fprintf(pfile,"now_character_image_str=%s\n",now_character_image_str);
	fprintf(pfile,"now_character_avatar_str=%s\n",now_character_avatar_str);
	fprintf(pfile,"now_talk_str=%s\n",now_talk_str);
	fprintf(pfile,"now_text_str=%s\n",now_text_str);
	fprintf(pfile,"now_event_name_str=%s\n",now_event_name_str);
	fprintf(pfile,"now_scene_name_str=%s\n",now_scene_name_str);
	
	fprintf(pfile,"dialogue_line_which=%d\n",dialogue_line_which);
	fprintf(pfile,"text_line_which=%d\n",text_line_which-1);
	fprintf(pfile,"emote_line_which=%d\n",emote_line_which);
	fprintf(pfile,"ch_line_which=%d\n",ch_line_which);
	fprintf(pfile,"nowtalk_line_which=%d\n",nowtalk_line_which);
	fprintf(pfile,"sound_line_which=%d\n",sound_line_which);
	fprintf(pfile,"start_option=%d\n",start_option);
	for(int32_t i=0;i<num_character;i++){
	fprintf(pfile,"character=%s,like=%d\n",character_list[i].name,character_list[i].like);
	}
	fprintf(pfile,"backpack=");
	for(int32_t i=0;i<9;i++){
		if(strlen(backpack[i])==0){
		fprintf(pfile,"empty,");
		}
		else{
		fprintf(pfile,"%s,",backpack[i]);
		}
	}
	fprintf(pfile,"\n");
	fclose(pfile);	
}
//存檔確認有無
int32_t check_save(struct Character *character_list,int32_t num_character,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	//printf("%s\n",filename);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"r+"))==NULL){
	return 0;
	}
	char line[1000];
	char now_event_str[200]={0};
	char now_dialogue_str[200]={0};
	char now_character_str[200]={0};
	char now_character_image_str[200]={0};
	char now_character_avatar_str[200]={0};
	char now_talk_str[200]={0};
	char now_event_name_str[200]={0};
	char now_scene_name_str[200]={0};
	char dialogue_line_which_str[200]={0};
	char text_line_which_str[200]={0};
	char emote_line_which_str[200]={0};
	char ch_line_which_str[200]={0};
	char nowtalk_line_which_str[200]={0};
	char sound_line_which_str[200]={0};
	char start_option_str[200]={0};
	int32_t dialogue_line_which=0;
	int32_t text_line_which=0;
	int32_t emote_line_which=0;
	int32_t ch_line_which=0;
	int32_t nowtalk_line_which=0;
	int32_t sound_line_which=0;
	int32_t start_option=-1;
	int32_t now_num_character=0;
	while(feof(pfile)==0){
	char character_list_check[200]={0};
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
	sscanf(line,"now_character_avatar_str=%s",now_character_avatar_str);
	sscanf(line,"now_talk_str=%s",now_talk_str);
	sscanf(line,"now_event_name_str=%s",now_event_name_str);
	sscanf(line,"now_scene_name_str=%s",now_scene_name_str);
	
	sscanf(line,"dialogue_line_which=%s",dialogue_line_which_str);
	sscanf(line,"text_line_which=%s",text_line_which_str);
	sscanf(line,"emote_line_which=%s",emote_line_which_str);
	sscanf(line,"ch_line_which=%s",ch_line_which_str);
	sscanf(line,"nowtalk_line_which=%s",nowtalk_line_which_str);
	sscanf(line,"sound_line_which=%s",sound_line_which_str);
	sscanf(line,"start_option=%s",start_option_str);
		if(sscanf(line,"character=%[^,]",character_list_check)==1){
		//printf("%s\n",character_list_check);
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
	nowtalk_line_which=atoi(nowtalk_line_which_str);
	sound_line_which=atoi(sound_line_which_str);
	start_option=atoi(start_option_str);
	//如果有錯誤算無資料
	if(now_event_str[0]==0 || now_dialogue_str[0]==0 || now_event_name_str[0]==0 || now_scene_name_str[0]==0){
	fclose(pfile);	
	return 0;
	}
	
	if(start_option==-1){
	fclose(pfile);	
	return 0;
	}
	if(start_option==1){
		if((emote_line_which-1)+(ch_line_which-1)+(nowtalk_line_which-1)+(text_line_which-1)+(sound_line_which-1)!=dialogue_line_which-1){
		fclose(pfile);	
		return 0;
		}
	}
	else{
		if((emote_line_which-1)+(ch_line_which-1)+(nowtalk_line_which-1)+(text_line_which-1)+(sound_line_which-1)!=dialogue_line_which){
		fclose(pfile);	
		return 0;
		}
	}
	
	
	if(now_num_character!=num_character){
	fclose(pfile);	
	return 0;
	}
	fclose(pfile);	
	return 1;
	
}

//存檔讀取
void read_save(char now_event_str[200],char now_dialogue_str[200],char now_character_str[200],char now_character_image_str[200],char now_character_avatar_str[200],char now_talk_str[200],char now_text_str[200],char now_event_name_str[200],char now_scene_name_str[200],int32_t *dialogue_line_which,int32_t *text_line_which,int32_t *emote_line_which,int32_t *ch_line_which,int32_t *nowtalk_line_which,int32_t *sound_line_which,struct Character *character_list,int32_t num_character,char **backpack,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"r+"))==NULL){
	return;
	}
	char line[1000];
	
	char dialogue_line_which_str[200]={0};
	char text_line_which_str[200]={0};
	char emote_line_which_str[200]={0};
	char ch_line_which_str[200]={0};
	char nowtalk_line_which_str[200]={0};
	char sound_line_which_str[200]={0};
	char like_str[200]={0};
	while(feof(pfile)==0){
	char character_list_check[200]={0};
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
	sscanf(line,"now_character_avatar_str=%s",now_character_avatar_str);
	sscanf(line,"now_talk_str=%s",now_talk_str);
	sscanf(line,"now_text_str=%s",now_text_str);
	sscanf(line,"now_event_name_str=%s",now_event_name_str);
	sscanf(line,"now_scene_name_str=%s",now_scene_name_str);
	
	sscanf(line,"dialogue_line_which=%s",dialogue_line_which_str);
	sscanf(line,"text_line_which=%s",text_line_which_str);
	sscanf(line,"emote_line_which=%s",emote_line_which_str);
	sscanf(line,"ch_line_which=%s",ch_line_which_str);
	sscanf(line,"nowtalk_line_which=%s",nowtalk_line_which_str);
	sscanf(line,"sound_line_which=%s",sound_line_which_str);
		if(sscanf(line,"character=%[^,],like=%s",character_list_check,like_str)==2){
			for(int32_t i=0;i<num_character;i++){
				if(strcmp(character_list_check,character_list[i].name)==0){
				character_list[i].like=atoi(like_str);
				break;
				}
			}
		}
	}
	sscanf(line,"backpack=%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],",backpack[0],backpack[1],backpack[2],backpack[3],backpack[4],backpack[5],backpack[6],backpack[7],backpack[8]);
	for(int32_t i=0;i<9;i++){
		if((strcmp(backpack[i],"empty")==0)){
			for(int32_t j=0;j<5;j++){
			backpack[i][j]=0;
			}
		}
	}
	*dialogue_line_which=atoi(dialogue_line_which_str);
	*text_line_which=atoi(text_line_which_str);
	*emote_line_which=atoi(emote_line_which_str);
	*ch_line_which=atoi(ch_line_which_str);
	*nowtalk_line_which=atoi(nowtalk_line_which_str);
	*sound_line_which=atoi(sound_line_which_str);
	
	
	fclose(pfile);	
}

//渲染存檔文字function
void show_save_info(button_rect *save_data_block,struct Character *character_list,int32_t num_character,int32_t save_which){
	char filename[100]={0};
	snprintf(filename,100,"assets/save/save%d.txt",save_which+1);
	//printf("%s\n",filename);
	FILE *pfile=NULL;
	if((pfile=fopen(filename,"r"))==NULL){
	render_option_Button("無資料",save_data_block,0,save_which*SCREEN_HEIGHT*0.08);
	return;
	}
	
	char timestamp[200];
	struct stat fileinfo;
	if(stat(filename,&fileinfo)!=-1){
	//printf("%s",ctime(&fileinfo.st_mtime));
	strncpy(timestamp,ctime(&fileinfo.st_mtime), 200);
	}
	char line[200];
	char now_event_str[200]={0};
	char now_dialogue_str[200]={0};
	char now_character_str[200]={0};
	char now_character_image_str[200]={0};
	char now_character_avatar_str[200]={0};
	char now_talk_str[200]={0};
	char now_event_name_str[200]={0};
	char now_scene_name_str[200]={0};
	char dialogue_line_which_str[200]={0};
	char text_line_which_str[200]={0};
	char emote_line_which_str[200]={0};
	char ch_line_which_str[200]={0};
	char nowtalk_line_which_str[200]={0};
	char sound_line_which_str[200]={0};
	char start_option_str[200]={0};
	int32_t dialogue_line_which=0;
	int32_t text_line_which=0;
	int32_t emote_line_which=0;
	int32_t ch_line_which=0;
	int32_t nowtalk_line_which=0;
	int32_t sound_line_which=0;
	int32_t start_option=-1;
	int32_t now_num_character=0;
	while(feof(pfile)==0){
	char character_list_check[200]={0};
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
	sscanf(line,"now_character_avatar_str=%s",now_character_avatar_str);
	sscanf(line,"now_talk_str=%s",now_talk_str);
	sscanf(line,"now_event_name_str=%s",now_event_name_str);
	sscanf(line,"now_scene_name_str=%s",now_scene_name_str);
	
	sscanf(line,"dialogue_line_which=%s",dialogue_line_which_str);
	sscanf(line,"text_line_which=%s",text_line_which_str);
	sscanf(line,"emote_line_which=%s",emote_line_which_str);
	sscanf(line,"ch_line_which=%s",ch_line_which_str);
	sscanf(line,"nowtalk_line_which=%s",nowtalk_line_which_str);
	sscanf(line,"sound_line_which=%s",sound_line_which_str);
	sscanf(line,"start_option=%s",start_option_str);
		if(sscanf(line,"character=%[^,]",character_list_check)==1){
		//printf("%s\n",character_list_check);
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
	nowtalk_line_which=atoi(nowtalk_line_which_str);
	sound_line_which=atoi(sound_line_which_str);
	start_option=atoi(start_option_str);
	//如果有錯誤算無資料
	if(now_event_str[0]==0 || now_dialogue_str[0]==0 || now_event_name_str[0]==0 || now_scene_name_str[0]==0){
	render_option_Button("無資料",save_data_block,0,save_which*SCREEN_HEIGHT*0.08);
	fclose(pfile);	
	return;
	}
	
	if(start_option==-1){
	render_option_Button("無資料",save_data_block,0,save_which*SCREEN_HEIGHT*0.08);
	fclose(pfile);	
	return;
	}
	
	if(start_option==1){
		if((emote_line_which-1)+(ch_line_which-1)+(nowtalk_line_which-1)+(text_line_which-1)+(sound_line_which-1)!=dialogue_line_which-1){
		render_option_Button("無資料",save_data_block,0,save_which*SCREEN_HEIGHT*0.08);
		fclose(pfile);	
		return;
		}
	}
	else{
		if((emote_line_which-1)+(ch_line_which-1)+(nowtalk_line_which-1)+(text_line_which-1)+(sound_line_which-1)!=dialogue_line_which){
		render_option_Button("無資料",save_data_block,0,save_which*SCREEN_HEIGHT*0.08);
		fclose(pfile);	
		return;
		}
	}
	
	
	if(now_num_character!=num_character){
	render_option_Button("無資料",save_data_block,0,save_which*SCREEN_HEIGHT*0.08);
	fclose(pfile);	
	return;
	}
	
	

	//渲染資訊
	render_saveinfo_Button(now_event_name_str,now_scene_name_str,timestamp,save_data_block,0,save_which*SCREEN_HEIGHT*0.08);
	fclose(pfile);	
}


//渲染場景function
void render_scene(char *path){
	// 渲染按鈕矩形
	SDL_Rect sceneRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	SDL_Texture* newTexture = NULL;
	// 載入圖片
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL){
	//printf("無法載入圖片 %s！ SDL錯誤：%s\n", path, SDL_GetError());
    } 
	else{
	// 將表面轉換成紋理
	newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);
        if (newTexture == NULL){
		printf("無法創建紋理 %s！ SDL錯誤：%s\n", path, SDL_GetError());
        }
    //渲染場景
    SDL_RenderCopy(rend,newTexture,NULL,&sceneRect);
	// 釋放表面記憶體
	SDL_FreeSurface(loadedSurface);
	SDL_DestroyTexture(newTexture);
	}
}

//螢幕變黑一點
void darker(char *path){
//渲染按鈕矩形
SDL_Rect Rect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);
render_scene(path);
SDL_SetRenderDrawColor(rend,0,0,0,100);
SDL_RenderFillRect(rend,&Rect);
SDL_RenderPresent(rend);
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


//渲染結局文字function
void render_end_text(const char *text,SDL_Event *event){
	SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend,0,0,0,255);
	SDL_Rect Rect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	SDL_RenderFillRect(rend,&Rect);
	SDL_RenderPresent(rend);
	SDL_Color textColor = {255,255,255}; // 白色文字顏色
	//加載字體
	TTF_Font *endfont = TTF_OpenFont("zpix.ttf",SCREEN_WIDTH*0.05);
	// 渲染按鈕文本
	for(int32_t i=0;i<50;i++){
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(endfont,text,textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend,textSurface);
	SDL_Rect textRect ={(SCREEN_WIDTH-textSurface->w)/2,(SCREEN_HEIGHT-textSurface->h)/2-SCREEN_HEIGHT*0.05,textSurface->w,textSurface->h};
	SDL_RenderCopy(rend,textTexture,NULL,&textRect);
	SDL_RenderPresent(rend);
	SDL_Delay(50);
	// 釋放表面和紋理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    while(SDL_PollEvent(event));
	}
}

//渲染儲存資訊按鈕function
void render_saveinfo_Button(char now_event_name_str[200],char now_scene_name_str[200],char timestamp[200],button_rect *button,int32_t offset_x,int32_t offset_y){
	//黑色文字顏色
	SDL_Color textColor = {0, 0, 0}; 
	char currentText[500]={0};
	//給事件名字分配15格
	int32_t event_name_length=18;
	//給地點名字分配15格
	int32_t scene_name_length=30;
	//時間秀出年/月/日/ 時:分:秒
	int32_t year=0;
	char month_str[10]={0};
	int32_t month=0;
	int32_t day=0;
	int32_t hour=0;
	int32_t min=0;
	int32_t sec=0;
	
	/*//判斷event有多少中文英文或數字
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
	event_name_length-=(chinese + normal);
	strcpy(currentText,now_event_name_str);
		for(int32_t i=0;i<event_name_length*2;i++){
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
	}*/
	
	//printf("%s\n",currentText);
	
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
	//strcat(currentText,date);
	snprintf(currentText,500,"%s/%s/%s",now_event_name_str,now_scene_name_str,date);
	// 依次渲染文字
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, currentText, textColor); 
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
	SDL_Rect destRect = {button->x+(button->w-textSurface->w)/2+offset_x,button->y+(button->h-textSurface->h)/2+offset_y,textSurface->w,textSurface->h}; 
	// 渲染文字紋理
	SDL_RenderCopy(rend, textTexture, NULL, &destRect);
	// 釋放表面和紋理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


//渲染圖片方塊function
void render_image_block(const char *path,button_rect *button,int32_t offset_x,int32_t offset_y,uint8_t alpha){
	// 渲染按鈕矩形
	SDL_Rect buttonRect={button->x + offset_x,button->y + offset_y,button->w,button->h};
	
	SDL_Texture* newTexture = NULL;
	// 載入圖片
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL){
	//printf("無法載入圖片 %s！ SDL錯誤：%s\n", path, SDL_GetError());
	} 
	else{
	// 將表面轉換成紋理
	newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);
	SDL_SetTextureAlphaMod(newTexture, alpha);
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
int32_t check_button(int32_t mouse_x, int32_t mouse_y,button_rect *button,int32_t offset_x,int32_t offset_y){
    // 檢查點擊位置是否在按鈕範圍內
    if (mouse_x >= button->x+offset_x && mouse_x <= button->x+offset_x+button->w && mouse_y >= button->y+offset_y && mouse_y <= button->y+offset_y+button->h){
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
		SDL_Rect destRect = {x+(SCREEN_WIDTH*0.022*chinese_many)+(SCREEN_WIDTH*0.013*normal_many),y+(changeline*SCREEN_HEIGHT*0.04),textSurface->w,textSurface->h}; 
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
			if(((SCREEN_WIDTH*0.022*chinese_many)+(SCREEN_WIDTH*0.013*normal_many))/(SCREEN_WIDTH*0.76)>=1){
			chinese_many=0;
			normal_many=0;
			changeline++;
			}
		}
	while(SDL_PollEvent(event));
	}
}

//渲染靜止對話function
void renderStaticText(const char *text, int32_t x, int32_t y,int32_t delay_put,SDL_Event *event){
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
		SDL_Rect destRect = {x+(SCREEN_WIDTH*0.022*chinese_many)+(SCREEN_WIDTH*0.013*normal_many),y+(changeline*SCREEN_HEIGHT*0.04),textSurface->w,textSurface->h}; 
		// 渲染文字紋理
		SDL_RenderCopy(rend, textTexture, NULL, &destRect);
		// 釋放表面和紋理
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		// 更新視窗
		SDL_RenderPresent(rend);
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
			if(((SCREEN_WIDTH*0.022*chinese_many)+(SCREEN_WIDTH*0.013*normal_many))/(SCREEN_WIDTH*0.77)>=1){
			chinese_many=0;
			normal_many=0;
			changeline++;
			}
		}
	while(SDL_PollEvent(event));
	}
}

//渲染物品描述function
void render_discribe_Text(const char *text,button_rect *button,int32_t offset_x,int32_t offset_y){
	SDL_Color textColor = {0,0,0}; // 黑色文字顏色

	// 渲染按鈕文本
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font,text,textColor,290);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend,textSurface);
	SDL_Rect textRect ={button->x+(button->w-textSurface->w)/2+offset_x,button->y+(button->h-textSurface->h)/2+offset_y,textSurface->w,textSurface->h};
	SDL_RenderCopy(rend,textTexture,NULL,&textRect);
	// 釋放表面和紋理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}



//渲染llm text function
/*void render_llm_Text(const char *text, int32_t x, int32_t y,SDL_Event *event){
    SDL_Color textColor = {0, 0, 0}; // 黑色文字顏色
	int32_t now_line=1;
    char line[1000]={0};
    SDL_Surface *textSurface=NULL;
    SDL_Texture *textTexture=NULL;
    for(int32_t i=0;i<strlen(text);i+=48){
    strncpy(line,text+i,48);
    now_line++;
    // 依次渲染文字
	textSurface = TTF_RenderText_Blended(font,line,textColor); 
	textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
	SDL_Rect destRect = {x,y+now_line*17,textSurface->w,textSurface->h}; 
	// 渲染文字紋理
	SDL_RenderCopy(rend, textTexture, NULL, &destRect);
    }
   
		// 釋放表面和紋理
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		// 更新視窗
		SDL_RenderPresent(rend);
}*/



//加載音效
Mix_Chunk *loadSound(const char *path){
	Mix_Chunk *sound = Mix_LoadWAV(path);
	/*if(sound==NULL){
	printf("Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError());
	}*/
	return sound;
}


//初始化東西function
int32_t initialize(char *name){
	//初始化SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
	printf("錯誤初始化 SDL: %s\n", SDL_GetError());
	return 0;
	}
	//初始化音效SDL_mixer
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, 2, 4096) <0) {
        printf("初始化mixer錯誤 SDL:%s\n", Mix_GetError());
        return 0;
    }
	//初始化文字TTF
	if(TTF_Init()==-1){
	printf("初始化TTF錯誤 SDL: %s\n", TTF_GetError());
	return 0;
	}
	//創建視窗
	win = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
	font = TTF_OpenFont("zpix.ttf",SCREEN_WIDTH*0.023);
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



