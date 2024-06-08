#define API_KEY "sk-proj-j6vjT9Y1Og8lN3jFl5XrT3BlbkFJUbZuUwLDIHfKG3NzSXlM"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

const char* SYSTEM_MESSAGE = 
    "請用以下模板生成一個以使用者要求為主題的toml檔案之完整rpg遊戲劇本。"
    "模板內容完全不要更動，只能在每個{}內新增對應的合理故事情節、對話等內容。"
    "內容包含三個角色(每個角色有三個表情、player(含玩家的inventory(道具))、兩種道具(item)、五個場景、與三種結局(final)，角色名稱、場景等{}內皆需生成內容。\n"
    "輸出的toml要直接可執行，不能包含額外純文字、格式的其他無關之解說文字。劇本內容要是繁體中文。\n\n"
    "```toml\n"
    "[player]\n"
    "name=\"{}\"\n"
    "inventory=[] #擁有道具(item)，一開始沒有道具。\n\n"
    "[character.1]\n"
    "name=\"{}\"\n"
    "describe=\"{}\"\n"
    "like={} #任意數值，可為好感度、血量等\n"
    "1_emote1=\"C1_1.png\"\n"
    "1_emote2=\"C1_2.png\"\n"
    "1_emote3=\"C1_3.png\"\n\n"
    "[character.2]\n"
    "name=\"{}\"\n"
    "2_describe=\"{}\"\n"
    "2_emote1=\"C2_1.png\"\n"
    "2_emote2=\"C2_2.png\"\n\n"
    "[character.3]\n"
    "name=\"{}\"\n"
    "describe=\"{}\"\n"
    "3_emote1=\"C3_1.png\"\n"
    "3_emote2=\"C3_2.png\"\n\n"
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
    "[event.1]\n"
    "name=\"{}\"\n"
    "scene=\"1\"\n"
    "music=\"\"\n"
    "dialogue=\"intro\"\n\n"
    "[dialogue.intro]\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "text3=\"{}\"\n"
    "event=\"2\"\n\n"
    "[event.2]\n"
    "name=\"{}\"\n"
    "scene=\"2\"\n"
    "music=\"\"\n"
    "dialogue=\"1\"\n\n"
    "[dialogue.1]\n"
    "character1=\"1\"\n"
    "emote1=\"{}\"\n"
    "text1=\"{}\"\n"
    "character2=\"2\"\n"
    "emote2=\"{}\"\n"
    "text2=\"{}\"\n"
    "character3=\"{}\"\n"
    "emote3=\"{}\"\n"
    "text3=\"{}\"\n"
    "character4=\"{}\"\n"
    "emote4=\"{}\"\n"
    "text4=\"{}\"\n"
    "event=\"3\"\n\n"
    "[event.3]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"\"\n"
    "dialogue=\"2\"\n\n"
    "[dialogue.2]\n"
    "text1=\"{}\"\n\n"
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
    "emote1=\"{}\"\n"
    "text1=\"{}\"\n"
    "next=\"2\"\n\n"
    "[dialogue.2_2]\n"
    "character1=\"{}\"\n"
    "emote1=\"{}\"\n"
    "text1=\"{}\"\n"
    "nowtalk=\"\" #切換成旁白講話\n"
    "text2=\"{}\"\n"
    "next=\"3\"\n\n"
    "[dialogue.3]\n"
    "text1=\"{}\"\n\n"
    "[dialogue.storage_entry.3.1]\n"
    "text=\"{}\"\n"
    "next=\"2\"\n\n"
    "[dialogue.storage_entry.3.2]\n"
    "text=\"{}\"\n"
    "hide_item=\"{}\" #隱藏選項，在有該道具時才會出現\n"
    "remove_item=\"{}\" #移除道具\n"
    "get_item=\"{}\" \n"
    "event=\"4\"\n\n"
    "[event.4]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"\"\n"
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
    "event=\"5\"\n\n"
    "[dialogue.4_3]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}]\n"
    "event=\"5\"\n\n"
    "[dialogue.4_4]\n"
    "text1=\"{}\"\n"
    "like=[\"1\",\"-{}] \n"
    "event=\"5\"\n\n"
    "[dialogue.5]\n"
    "text1=\"{}\"\n"
    "text2=\"{}\"\n"
    "text3=\"{}\"\n"
    "next=\"4_1\"\n\n"
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
    "like=[\"1\",\"-{}] \n"
    "event=\"6\"\n\n"
    "[event.6]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"\"\n"
    "dialogue=\"6\"\n\n"
    "[dialogue.shore]\n"
    "text1=\"{}\"\n\n"
    "[dialogue.shore.options.1]\n"
    "text=\"{}\"\n"
    "hide_like=[\"{}\"] #隱藏選項，當好感度為該數值時才會出現\n"
    "next=\"final_success\"\n\n"
    "[dialogue.shore.options.2]\n"
    "text=\"{}\"\n"
    "next=\"final_fail1\"\n\n"
    "[dialogue.final_success]\n"
    "character1=\"{}\"\n"
    "emote1=\"{}\"\n"
    "text1=\"{}\"\n"
    "character2=\"{}\"\n"
    "emote2=\"{}\"\n"
    "text2=\"{}\"\n"
    "character1=\"{}\"\n"
    "emote3=\"{}\"\n"
    "text3=\"{}\"\n"
    "character2=\"{}\"\n"
    "emote4=\"{}\"\n"
    "text4=\"{}\"\n\n"
    "[event.final_1]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"\"\n"
    "dialogue=\"final_1\"\n\n"
    "[dialogue.final_1]\n"
    "text1=\"{}\"\n\n"
    "[event.final_fail]\n"
    "name=\"{}\"\n"
    "scene=\"{}\"\n"
    "music=\"\"\n"
    "dialogue=\"final_fail\"\n\n"
    "[dialogue.final_fail]\n"
    "text1=\"{}\"\n```";

// 其餘部分保持不變
struct MemoryStruct {
    char *memory;
    size_t size;
};

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

void chat_with_gpt_function(const char *input, char *output) {
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
        cJSON_AddStringToObject(root, "model", "gpt-3.5-turbo-16k");

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

        cJSON_AddNumberToObject(root, "temperature", 0.3);
        cJSON_AddNumberToObject(root, "max_tokens", 14384);
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
            //printf("Full API Response: %s\n", chunk.memory);
            parse_response_and_extract_content(chunk.memory, assistant_message);
            strncpy(output, assistant_message, 16384 - 1);
            output[16384 - 1] = '\0';  // Ensure null-termination

            // Write the AI response to a file
            FILE *fp = fopen("ai_script.toml", "w");
            if (fp != NULL) {
                fputs(assistant_message, fp);
                fclose(fp);
                printf("Script generated successfully\n");
            } else {
                printf("Error writing to file\n");
            }
        }

        free(json_string);
        cJSON_Delete(root);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }

    curl_global_cleanup();
}

int main() {
    char user_input[1024];
    char api_output[16384] = {0};

    printf("您的故事劇本想要什麼主題？ ");
    fgets(user_input, sizeof(user_input), stdin);
    user_input[strcspn(user_input, "\n")] = 0;

    chat_with_gpt_function(user_input, api_output);

    return 0;
}