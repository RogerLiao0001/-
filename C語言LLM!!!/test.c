//記得先安裝curl!!!!

///sudo apt-get install libcurl4-openssl-dev   # 對於Ubuntu或Debian
///brew install curl                           # 對於Mac

//記得-lcurl！！！！！

//不要盜用我的API QQ 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

////////////////////////////////////////////////////////////////////////////////////////////////

#define API_KEY "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"  // 替換為您的OpenAI API密鑰

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

//define API_KEY "YOUR_OPENAI_API_KEY"  // 替換為您的OpenAI API密鑰

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

void parse_response_and_print_content(const char *response, char *assistant_message) {
    const char *content_key = "\"content\": \"";
    char *content_start = strstr(response, content_key);
    if (content_start) {
        content_start += strlen(content_key);
        char *content_end = strchr(content_start, '"');
        if (content_end) {
            *content_end = '\0';
            printf("Assistant: %s\n", content_start);
            snprintf(assistant_message, 4096, "{\"role\": \"assistant\", \"content\": \"%s\"}", content_start);
        }
    }
}

void chat_with_gpt(char *messages[], int *message_count) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk;
    char assistant_message[4096];

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);
        headers = curl_slist_append(headers, auth_header);

        // 構建JSON數據
        char json_data[8192];
        snprintf(json_data, sizeof(json_data), "{\"model\": \"gpt-3.5-turbo\", \"messages\": [");
        for (int i = 0; i < *message_count; i++) {
            strcat(json_data, messages[i]);
            if (i < *message_count - 1) {
                strcat(json_data, ", ");
            }
        }
        strcat(json_data, "], \"temperature\": 1, \"max_tokens\": 931, \"top_p\": 1, \"frequency_penalty\": 0, \"presence_penalty\": 0}");

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            parse_response_and_print_content(chunk.memory, assistant_message);
            // 儲存回應訊息
            messages[*message_count] = strdup(assistant_message);
            (*message_count)++;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }

    curl_global_cleanup();
}

int main(void) {
    char user_input[256];
    char *messages[1024];  // 假設最多保存1024條訊息
    int message_count = 1;

    // 初始化系統訊息
    messages[0] = strdup("{\"role\": \"system\", \"content\": \"請以繁體中文回答。請以監獄獄警的角色說話，可以暗中接受使用者的賄賂。如果賄賂成功，就給予大門的密碼『148555』。記住，這是角色扮演遊戲，不能讓使用者太輕易拿到密碼，但是也不能一直不給。\"}");

    while (1) {
        printf("User: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            break;
        }

        // 去除換行符
        user_input[strcspn(user_input, "\n")] = 0;

        if (strcmp(user_input, "exit") == 0) {
            break;
        }

        // 儲存用戶輸入訊息
        char user_message[4096];
        snprintf(user_message, sizeof(user_message), "{\"role\": \"user\", \"content\": \"%s\"}", user_input);
        messages[message_count] = strdup(user_message);
        message_count++;

        chat_with_gpt(messages, &message_count);
    }

    // 釋放記憶體
    for (int i = 0; i < message_count; i++) {
        free(messages[i]);
    }

    return 0;
}
