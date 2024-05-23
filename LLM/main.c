// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llm.h"

int main(void) {
    char input[256];
    char output[4096];
    char *messages[1024];  // 假設最多保存1024條訊息
    int message_count = 1;

    // 初始化系統訊息
    messages[0] = strdup("{\"role\": \"system\", \"content\": \"請以繁體中文回答。請以監獄獄警的角色說話，可以暗中接受使用者的賄賂。如果賄賂成功，就給予大門的密碼『148555』。記住，這是角色扮演遊戲，不能讓使用者太輕易拿到密碼，但是也不能一直不給。在使用者提起前，不要自己說出可以賄賂。\"}");

    while (1) {
        printf("User: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // 去除換行符
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        llm(messages, &message_count, input, output);

        printf("Assistant: %s\n", output);
    }

    // 釋放記憶體
    for (int i = 0; i < message_count; i++) {
        free(messages[i]);
    }

    return 0;
}
