// main.c
#include <stdio.h>
#include <string.h>
#include "llm.h"

int main(void) {
    char input[256];
    char output[4096];

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

        llm(input, output);

        printf("Assistant: %s\n", output);
    }

    return 0;
}
