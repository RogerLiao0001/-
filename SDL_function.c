#include "SDL_function.h"

//視窗寬高
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
//1080x1024
//設定全域變數
SDL_Window *win =NULL;
SDL_Renderer *rend =NULL;
TTF_Font *font = NULL;



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

//漸黑特效function
void fadeout(char *path,SDL_Event *event){
	// 渲染按鈕矩形
	SDL_Rect Rect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	// 逐漸增加 alpha 值，實現漸黑效果
    for (int32_t alpha=0; alpha<=255;alpha+=10){
    if(alpha>0){
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




//渲染圖片方塊function
void render_image_block(const char *path,button_rect *button){
	// 渲染按鈕矩形
	SDL_Rect buttonRect={button->x,button->y,button->w,button->h};
	
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
void render_option_Button(const char *text,button_rect *button,int32_t offset_y){
	SDL_Color textColor = {0, 0, 0}; // 黑色文字顏色

	// 渲染按鈕文本
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font,text,textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend,textSurface);
	SDL_Rect textRect ={button->x+(button->w-textSurface->w)/2,button->y+(button->h-textSurface->h)/2+(offset_y*50),textSurface->w,textSurface->h};
	SDL_RenderCopy(rend,textTexture,NULL,&textRect);
	// 釋放表面和紋理
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


//渲染選項方塊function
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
}




// 檢查點擊位置是否在按鈕上
int32_t check_button(int32_t mouse_x, int32_t mouse_y,button_rect *button,int32_t offset_y){
   /* SDL_Color bgColor = {0xFF, 0x80, 0xFF, 0xFF};
    // 渲染按鈕矩形
	SDL_Rect buttonRect={button->x,button->y+(offset_y*50),button->w,button->h};
	SDL_SetRenderDrawColor(rend, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect(rend,&buttonRect);
	SDL_RenderPresent(rend);*/
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



