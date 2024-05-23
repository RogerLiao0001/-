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
#include "llm.h"


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
    const char *content_key = "\"content\": \"";
    char *content_start = strstr(response, content_key);
    if (content_start) {
        content_start += strlen(content_key);
        char *content_end = strchr(content_start, '"');
        if (content_end) {
            *content_end = '\0';
            snprintf(assistant_message, 4096, "%s", content_start);
        }
    }
}

void llm(const char *input, char *output) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk;
    char assistant_message[4096];
    char user_message[4096];
    char json_data[8192];

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);
        headers = curl_slist_append(headers, auth_header);

        snprintf(user_message, sizeof(user_message), "{\"role\": \"user\", \"content\": \"%s\"}", input);
        snprintf(json_data, sizeof(json_data), "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"system\", \"content\": \"請以繁體中文回答。請以監獄獄警的角色說話，可以暗中接受使用者的賄賂。如果賄賂成功，就給予大門的密碼『148555』。記住，這是角色扮演遊戲，不能讓使用者太輕易拿到密碼，但是也不能一直不給。在使用者提起前，不要自己說出可以賄賂。\"}, %s], \"temperature\": 1, \"max_tokens\": 931, \"top_p\": 1, \"frequency_penalty\": 0, \"presence_penalty\": 0}", user_message);

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            parse_response_and_extract_content(chunk.memory, assistant_message);
            snprintf(output, 4096, "%s", assistant_message);
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }

    curl_global_cleanup();
}
