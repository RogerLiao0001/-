#define API_KEY ""
#define OPENAI_API_KEY ""
#define API_URL "https://api.openai.com/v1/images/generations"

#define API_URL "https://api.openai.com/v1/images/generations"

#define IMAGE_SIZE "1024x1024"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

typedef struct {
    char character1_describe[1024];
    char character2_describe[1024];
    char character3_describe[1024];
    char scene1_name[256];
    char scene2_name[256];
    char scene3_name[256];
    char scene4_name[256];
    char scene5_name[256];
    char item1_description[256];
    char item2_description[256];
} ExtractedData;

const char* SYSTEM_MESSAGE = 
    "請用以下模板生成一個以使用者要求為主題的toml檔案之完整rpg遊戲劇本。"
    "模板內容完全不要更動，只能在每個{}內新增對應的合理故事情節、對話等內容。(圖片名稱也不要改)"
    "內容包含三個角色(每個角色有三個表情、player(含玩家的inventory(道具))、兩種道具(item)、五個場景、與三種結局(final)，角色名稱、場景等{}內皆需生成內容。\n"
    "輸出的toml要直接可執行，不能包含額外純文字、格式的其他無關之解說文字，不要程式碼區塊。劇本內容要是繁體中文。\n\n"
    "\n"
    "[player]\n"
    "name=\"{}\"\n"
    "inventory=[] #擁有道具(item)，一開始沒有道具。\n\n"
    "[character.1]\n"
    "name=\"{}\"\n"
    "describe=\"{}\"\n"
    "like={} #任意數值，可為好感度、血量等\n"
    "tachie_default=\"C1_1.png\"\n"
    //"1_emote2=\"\"\n"
    //"1_emote3=\"\"\n\n"
    "[character.2]\n"
    "name=\"{}\"\n"
    "describe=\"{}\"\n"
    "tachie_default=\"C2_1.png\"\n"
    //"2_emote2=\"\"\n\n"
    "[character.3]\n"
    "name=\"{}\"\n"
    "describe=\"{}\"\n"
    "tachie_default=\"C3_1.png\"\n"
    //"3_emote2=\"\"\n\n"
    "[item.1]\n"
    "name=\"{}\"\n"
    "description=\"{}\"\n"
    "icon=\"I1.png\"\n\n"
    "[item.2]\n"
    "name=\"{}\"\n"
    "description=\"{}\"\n"
    "icon=\"I2.png\"\n\n"
    "[scene.1]\n"
    "name=\"{}\"\n"
    "background=\"S1.png\"\n\n"
    "[scene.2]\n"
    "name=\"{}\"\n"
    "background=\"S2.png\"\n\n"
    "[scene.3]\n"
    "name=\"{}\"\n"
    "background=\"S3.png\"\n\n"
    "[scene.4]\n"
    "name=\"{}\"\n"
    "background=\"S4.png\"\n\n"
    "[scene.5]\n"
    "name=\"{}\"\n"
    "background=\"S5.png\"\n\n"
    "[event.start]\n"
    "name=\"{}\"\n"
    "scene=\"1\"\n"
    "music=\"llm/bgm1.mp3\"\n"
    "dialogue=\"intro\"\n\n"
    "[dialogue.intro]\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "text3=\"{}\"\n"
    "event=\"2\"\n\n"
    "[event.2]\n"
    "name=\"{}\"\n"
    "scene=\"2\"\n"
    "music=\"llm/bgm2.mp3\"\n"
    "dialogue=\"1\"\n\n"
    "[dialogue.1]\n"
    "character1=\"1\"\n"
    "emote1=\"default\"\n"
    "text1=\"{}\"\n"
    "character2=\"2\"\n"
    "emote2=\"default\"\n"
    "text2=\"{}\"\n"
    "character3=\"{}\"\n"
    "emote3=\"default\"\n"
    "text3=\"{}\"\n"
    "event=\"3\"\n\n"
    "[event.3]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"llm/bgm3.mp3\"\n"
    "dialogue=\"2\"\n\n"
    "[dialogue.2]\n"
    "text1=\"{}\"\n\n "
    "#接下來是選項，因此這裡不用加evet=...或next=..."
    "[dialogue.2.options.1]\n"
    "text=\"{}\"\n"
    "get_item=\"1\" #獲得道具(1或2)\n"
    "next=\"2_1\"\n\n"
    "[dialogue.2.options.2]\n"
    "text=\"{}\"\n"
    "next=\"2_2\"\n\n"
    "[dialogue.2.options.3]\n"
    "text=\"{}\"\n"
    "get_item=\"2\"\n"
    "next=\"2_1\"\n\n"
    "[dialogue.2.options.4]\n"
    "text=\"{}\"\n"
    "event=\"final_1\"\n\n"
    "[dialogue.2_1]\n"
    "character1=\"{}\"\n"
    "emote1=\"default\"\n"
    "text1=\"{}\"\n"
    "next=\"2\"\n\n"
    "[dialogue.2_2]\n"
    "character1=\"{}\"\n"
    "emote1=\"default\"\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "next=\"3\"\n\n"
    "[dialogue.3]\n"
    "text1=\"{}\"\n\n"
    "#接下來是選項，因此這裡不用加evet=...或next=..."
    "[dialogue.3.options.1]\n"
    "text=\"{}\"\n"
    "next=\"2\"\n\n"
    "[dialogue.3.options.2]\n"
    "text=\"{}\"\n"
    "hide_item=\"{}\" #隱藏選項，在有該道具時才會出現\n"
    "remove_item=\"{}\" #移除道具\n"
    "get_item=\"{}\" \n"
    "event=\"4\"\n\n"
    "[event.4]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"llm/bgm4.mp3\"\n"
    "dialogue=\"4\"\n\n"
    "[dialogue.4]\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "text3=\"{}\"\n"
    "next=\"4_1\"\n\n"
    "[dialogue.4_1]\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "text3=\"{}\"\n\n"
    "[dialogue.4_1.options.1]\n"
    "text=\"{}\"\n"
    "next=\"4_2\"\n\n"
    "[dialogue.4_1.options.2]\n"
    "text=\"{}\"\n"
    "next=\"4_3\"\n\n"
    "[dialogue.4_1.options.3]\n"
    "text=\"{}\"\n"
    "next=\"4_4\"\n\n"
    "[dialogue.4_2]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}] #數值更改\n"
    "next=\"5\"\n\n"
    "[dialogue.4_3]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}\"]\n"
    "next=\"5\"\n\n"
    "[dialogue.4_4]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}] \n"
    "next=\"5\"\n\n"
    "[dialogue.5]\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "text3=\"{}\"\n"
    "next=\"5_1\"\n\n"
    "[dialogue.5_1]\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "text3=\"{}\"\n\n"
    "[dialogue.5_1.options.1]\n"
    "text=\"{}\"\n"
    "next=\"5_2\"\n\n"
    "[dialogue.5_1.options.2]\n"
    "text=\"{}\"\n"
    "next=\"5_3\"\n\n"
    "[dialogue.5_1.options.3]\n"
    "text=\"{}\"\n"
    "next=\"5_4\"\n\n"
    "[dialogue.5_2]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}] #數值更改\n"
    "event=\"6\"\n\n"
    "[dialogue.5_3]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}]\n"
    "event=\"6\"\n\n"
    "[dialogue.5_4]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}\"] \n"
    "event=\"6\"\n\n"
    "[event.6]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"llm/bgm5.mp3\"\n"
    "dialogue=\"6\"\n\n"
    "[dialogue.6]\n"
    "text1=\"{}\"\n\n"
    "[dialogue.6.options.1]\n"
    "text=\"{}\"\n"
    "hide_like=[\"{}\"] #隱藏選項，當好感度為該數值時才會出現\n"
    "next=\"final_success\"\n\n"
    "[dialogue.6.options.2]\n"
    "text=\"{}\"\n"
    "event=\"final_fail\"\n\n"
    "[dialogue.final_success]\n"
    "character1=\"{}\"\n"
    "emote1=\"default\"\n"
    "text1=\"{}\"\n"
    "character2=\"{}\"\n"
    "emote2=\"default\"\n"
    "text2=\"{}\"\n"
    "character3=\"{}\"\n"
    "emote3=\"default\"\n"
    "text3=\"{}\"\n\n"
    "[event.final_1]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"success.mp3\"\n"
    "dialogue=\"final_1\"\n\n"
    "[dialogue.final_1]\n"
    "text1=\"{}\"\n\n"
    "[event.final_fail]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"fail.mp3\"\n"
    "dialogue=\"final_fail\"\n\n"
    "[dialogue.final_fail]\n"
    "text1=\"{}\"\n";

// 其餘部分保持不變
struct MemoryStruct {
    char *memory;
    size_t size;
};


#include <string.h>

void extract_between(const char *source, const char *start, const char *end, char *result, size_t max_size) {
    const char *start_ptr = strstr(source, start);
    if (start_ptr) {
        start_ptr += strlen(start);
        const char *end_ptr = strstr(start_ptr, end);
        if (end_ptr) {
            size_t length = end_ptr - start_ptr;
            if (length < max_size) {
                strncpy(result, start_ptr, length);
                result[length] = '\0';
            }
        }
    }
}

void extract_script_data(const char *script, ExtractedData *data) {
    char temp[1024];

    extract_between(script, "character.1]\n", "\nlike", temp, sizeof(temp));
    snprintf(data->character1_describe, sizeof(data->character1_describe), "80年代手繪電玩風格，全白背景%s", temp);
    
    extract_between(script, "character.2]\n", "\ntachie_default", temp, sizeof(temp));
    snprintf(data->character2_describe, sizeof(data->character2_describe), "80年代手繪電玩風格，全白背景%s", temp);
    
    extract_between(script, "character.3]\n", "\ntachie_default", temp, sizeof(temp));
    snprintf(data->character3_describe, sizeof(data->character3_describe), "80年代手繪電玩風格%s", temp);

    extract_between(script, "scene.1]\nname=\"", "\"\nbackground=\"", temp, sizeof(temp));
    snprintf(data->scene1_name, sizeof(data->scene1_name), "80年代手繪電玩風格%s", temp);
    
    extract_between(script, "scene.2]\nname=\"", "\"\nbackground=\"", temp, sizeof(temp));
    snprintf(data->scene2_name, sizeof(data->scene2_name), "80年代手繪電玩風格%s", temp);
    
    extract_between(script, "scene.3]\nname=\"", "\"\nbackground=\"", temp, sizeof(temp));
    snprintf(data->scene3_name, sizeof(data->scene3_name), "80年代手繪電玩風格%s", temp);
    
    extract_between(script, "scene.4]\nname=\"", "\"\nbackground=\"", temp, sizeof(temp));
    snprintf(data->scene4_name, sizeof(data->scene4_name), "80年代手繪電玩風格%s", temp);
    
    extract_between(script, "scene.5]\nname=\"", "\"\nbackground=\"", temp, sizeof(temp));
    snprintf(data->scene5_name, sizeof(data->scene5_name), "80年代手繪電玩風格%s", temp);

    extract_between(script, "item.1]\n", "\nicon=\"I1.png", temp, sizeof(temp));
    snprintf(data->item1_description, sizeof(data->item1_description), "80年代手繪電玩風格%s", temp);
    
    extract_between(script, "item.2]\n", "\nicon=\"I2.png", temp, sizeof(temp));
    snprintf(data->item2_description, sizeof(data->item2_description), "80年代手繪電玩風格，全白背景%s", temp);
}





static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void parse_response_and_extract_content(const char *response, char *assistant_message) {
    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON parsing error: %s\n", error_ptr);
        }
        return;
    }

    cJSON *choices = cJSON_GetObjectItemCaseSensitive(json, "choices");
    if (cJSON_IsArray(choices) && cJSON_GetArraySize(choices) > 0) {
        cJSON *first_choice = cJSON_GetArrayItem(choices, 0);
        cJSON *message = cJSON_GetObjectItemCaseSensitive(first_choice, "message");
        cJSON *content = cJSON_GetObjectItemCaseSensitive(message, "content");
        if (cJSON_IsString(content) && (content->valuestring != NULL)) {
            strncpy(assistant_message, content->valuestring, 16384 - 1);
            assistant_message[16384 - 1] = '\0';  // Ensure null-termination
        }
    }

    cJSON_Delete(json);
}

void chat_with_gpt_function(const char *input, char *output, ExtractedData *data) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk;
    char assistant_message[16384] = {0};

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);
        headers = curl_slist_append(headers, auth_header);

        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "model", "gpt-4o");//gpt-3.5-turbo-16k

        cJSON *messages = cJSON_AddArrayToObject(root, "messages");

        cJSON *system_message = cJSON_CreateObject();
        cJSON_AddStringToObject(system_message, "role", "system");
        cJSON_AddStringToObject(system_message, "content", SYSTEM_MESSAGE);
        cJSON_AddItemToArray(messages, system_message);

        cJSON *user_message = cJSON_CreateObject();
        cJSON_AddStringToObject(user_message, "role", "user");
        cJSON_AddStringToObject(user_message, "content", input);
        cJSON_AddItemToArray(messages, user_message);

        cJSON *assistant_init = cJSON_CreateObject();
        cJSON_AddStringToObject(assistant_init, "role", "assistant");
        cJSON_AddStringToObject(assistant_init, "content", "您的劇本想要什麼主題?");
        cJSON_AddItemToArray(messages, assistant_init);

        cJSON_AddNumberToObject(root, "temperature", 0.15);
        cJSON_AddNumberToObject(root, "max_tokens", 3500);//14384
        cJSON_AddNumberToObject(root, "top_p", 1);
        cJSON_AddNumberToObject(root, "frequency_penalty", 0);
        cJSON_AddNumberToObject(root, "presence_penalty", 0);

        char *json_string = cJSON_PrintUnformatted(root);

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("%s\n",chunk.memory);
            parse_response_and_extract_content(chunk.memory, assistant_message);
            strncpy(output, assistant_message, 16384 - 1);
            output[16384 - 1] = '\0';  // 確保字串結尾是 null

            // 寫入 AI 回應到文件
            FILE *fp = fopen("script.toml", "w");
            if (fp != NULL) {
                fputs(assistant_message, fp);
                fclose(fp);
                printf("Script generated successfully\n");
            } else {
                printf("Error writing to file\n");
            }

            // 提取資料
            extract_script_data(assistant_message, data);
        }

        free(json_string);
        cJSON_Delete(root);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }

    curl_global_cleanup();
}


// 用於保存圖片的內存結構
struct MemoryStru {
    char *memory;
    size_t size;
};

// 內存寫入函數
static size_t WriteMemoryCall(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStru *mem = (struct MemoryStru *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// 創建JSON請求數據
char* create_json_request(const char* prompt, int n) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "model", "dall-e-3");
    cJSON_AddStringToObject(json, "prompt", prompt);
    cJSON_AddNumberToObject(json, "n", n);
    cJSON_AddStringToObject(json, "size", IMAGE_SIZE);

    char *json_data = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_data;
}

// 發送請求並獲取回應
char* send_request(const char* json_data) {
    CURL *curl;
    CURLcode res;
    struct MemoryStru chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[128];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);
        headers = curl_slist_append(headers, auth_header);

        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        printf("Response received: %s\n", chunk.memory);
    }
    return chunk.memory;
}


// 下載圖片並保存到本地
void download_image(const char* url, const char* filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename, "wb");
        if (fp == NULL) {
            fprintf(stderr, "Could not open file for writing: %s\n", filename);
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }
}
#include <ctype.h>

void sanitize_string(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src == '"' || *src == '\\') {
            src++; // skip this character
        } else if (isprint((unsigned char) *src)) {
            *dst++ = *src;
            src++;
        } else {
            src++; // skip non-printable characters
        }
    }
    *dst = '\0';
}


void remove_invalid_json_chars(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src >= 32 && *src <= 126) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

void clean_string(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        // 過濾掉無效字符，只保留可見字符和基本標點
        if ((*src >= 32 && *src <= 126) && *src != '"' && *src != '\\') {
            *dst++ = *src;
        } else if (*src == '"') {
            // 將雙引號替換為空格，以保持語句連貫
            *dst++ = ' ';
        }
        src++;
    }
    *dst = '\0';
}

#include <ctype.h>

void extract_and_format_description(const char *input, char *output, size_t max_size) {
    const char *name_start = strstr(input, "name=\"");
    const char *describe_start = strstr(input, "describe=\"");
    char name[256] = {0};
    char describe[256] = {0};

    if (name_start) {
        name_start += strlen("name=\"");
        const char *name_end = strchr(name_start, '"');
        if (name_end) {
            size_t name_length = name_end - name_start;
            if (name_length < sizeof(name)) {
                strncpy(name, name_start, name_length);
                name[name_length] = '\0';
            }
        }
    }

    if (describe_start) {
        describe_start += strlen("describe=\"");
        const char *describe_end = strchr(describe_start, '"');
        if (describe_end) {
            size_t describe_length = describe_end - describe_start;
            if (describe_length < sizeof(describe)) {
                strncpy(describe, describe_start, describe_length);
                describe[describe_length] = '\0';
            }
        }
    }

    snprintf(output, max_size, "80年代手繪電玩風格，全白背景 %s %s", name, describe);
}

void generate_and_save_image(const char* prompt, const char* size, const char* filename) {
    char formatted_prompt[2048];
    extract_and_format_description(prompt, formatted_prompt, sizeof(formatted_prompt)); // 提取並格式化描述

    char json_data[4096];
    snprintf(json_data, sizeof(json_data),
             "{\"model\": \"dall-e-3\", \"prompt\": \"%s\", \"n\": 1, \"size\": \"%s\"}",
             formatted_prompt, size);

    printf("Generating image with prompt: %s\n", formatted_prompt);
    printf("Request JSON: %s\n", json_data);

    char* response = send_request(json_data);
    if (response) {
        printf("API Response: %s\n", response);
        cJSON *json = cJSON_Parse(response);
        if (json) {
            cJSON *data = cJSON_GetObjectItemCaseSensitive(json, "data");
            if (cJSON_IsArray(data)) {
                cJSON *item = cJSON_GetArrayItem(data, 0);
                cJSON *url = cJSON_GetObjectItemCaseSensitive(item, "url");
                if (cJSON_IsString(url) && (url->valuestring != NULL)) {
                    printf("Downloading image from URL: %s\n", url->valuestring);
                    download_image(url->valuestring, filename);
                    printf("Image saved as: %s\n", filename);
                } else {
                    printf("Failed to extract URL from response.\n");
                }
            } else {
                printf("Invalid response format: 'data' is not an array.\n");
            }
            cJSON_Delete(json);
        } else {
            printf("Failed to parse JSON response.\n");
        }
        free(response);
    } else {
        printf("No response received from API.\n");
    }
}

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

// 用於保存圖片的內存結構

#include <stdio.h>
#include <stdlib.h>

#define API_KEY2 "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX2lkIjoiNTQ4N2VlMGItMzc4NS00ZGUzLThmMTgtZGY4NDgyY2I5MTQwIiwidHlwZSI6ImFwaV90b2tlbiJ9.mnBw3Au1fR-_l8kXwp0RwFXuvW4iYBQkWINdqnEjmGQ" // 請替換成您的API KEY

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "cJSON.h"


struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

int download_image2(const char *url, const char *output_filename) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return 1;
    }

    FILE *fp = fopen(output_filename, "wb");
    if (!fp) {
        fprintf(stderr, "fopen() failed\n");
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        fclose(fp);
        curl_easy_cleanup(curl);
        return 1;
    }

    fclose(fp);
    curl_easy_cleanup(curl);
    return 0;
}

void remove_background(const char *input_image_name, const char *output_image_name) {
    CURL *hnd = curl_easy_init();
    if (!hnd) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return;
    }

    struct string response;
    init_string(&response);

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, "https://api.edenai.run/v2/image/background_removal");
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "accept: application/json");
    headers = curl_slist_append(headers, "content-type: multipart/form-data");
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "authorization: Bearer %s", API_KEY2);
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    struct curl_httppost *form = NULL;
    struct curl_httppost *lastptr = NULL;
    curl_formadd(&form, &lastptr, CURLFORM_COPYNAME, "providers", CURLFORM_COPYCONTENTS, "microsoft", CURLFORM_END);
    curl_formadd(&form, &lastptr, CURLFORM_COPYNAME, "file", CURLFORM_FILE, input_image_name, CURLFORM_END);
    curl_easy_setopt(hnd, CURLOPT_HTTPPOST, form);

    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
        return;
    }

    cJSON *json = cJSON_Parse(response.ptr);
    if (!json) {
        fprintf(stderr, "cJSON_Parse() failed\n");
        return;
    }

    cJSON *microsoft = cJSON_GetObjectItem(json, "microsoft");
    if (microsoft) {
        cJSON *image_resource_url = cJSON_GetObjectItem(microsoft, "image_resource_url");
        if (image_resource_url) {
            printf("Image Resource URL: %s\n", image_resource_url->valuestring);
            if (download_image2(image_resource_url->valuestring, output_image_name) != 0) {
                fprintf(stderr, "Failed to download the image from the URL\n");
            }
        } else {
            fprintf(stderr, "No image_resource_url found\n");
        }
    } else {
        fprintf(stderr, "No microsoft object found\n");
    }

    cJSON_Delete(json);
    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);
    curl_formfree(form);
    free(response.ptr);
}


int main() {
    char user_input[1024];
    char api_output[16384] = {0};
    ExtractedData data = {0};

    printf("您的故事劇本想要什麼主題？ ");
    fgets(user_input, sizeof(user_input), stdin);
    user_input[strcspn(user_input, "\n")] = 0;

    printf("User input received: %s\n", user_input);

    chat_with_gpt_function(user_input, api_output, &data);

    printf("角色1描述: %s\n", data.character1_describe);
    printf("角色2描述: %s\n", data.character2_describe);
    printf("角色3描述: %s\n", data.character3_describe);
    printf("場景1名稱: %s\n", data.scene1_name);
    printf("場景2名稱: %s\n", data.scene2_name);
    printf("場景3名稱: %s\n", data.scene3_name);
    printf("場景4名稱: %s\n", data.scene4_name);
    printf("場景5名稱: %s\n", data.scene5_name);
    printf("物品1描述: %s\n", data.item1_description);
    printf("物品2描述: %s\n", data.item2_description);

    // 生成圖片並保存
    printf("Generating images...\n");

    //generate_and_save_image(data.character1_describe, "1024x1792", "C1.png");
    //generate_and_save_image(data.character2_describe, "1024x1792", "C2.png");
    //generate_and_save_image(data.character3_describe, "1024x1792", "C3.png");
    //generate_and_save_image(data.scene1_name, "1792x1024", "S1.png");
    //generate_and_save_image(data.scene2_name, "1792x1024", "S2.png");
    //generate_and_save_image(data.scene3_name, "1792x1024", "S3.png");
    //generate_and_save_image(data.scene4_name, "1792x1024", "S4.png");
    //generate_and_save_image(data.scene5_name, "1792x1024", "S5.png");
    //generate_and_save_image(data.item1_description, "1024x1024", "I1.png");
    //generate_and_save_image(data.item2_description, "1024x1024", "I2.png");

    // 處理角色和物品圖片
    printf("Processing character and item images...\n");

    //remove_background("C1.png", "C1_1.png");
    //remove_background("C2.png", "C2_1.png");
    //remove_background("C3.png", "C3_1.png");

    printf("All images processed and saved successfully.\n");

    return 0;
}
