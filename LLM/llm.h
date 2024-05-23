#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void llm(char *messages[], int *message_count, const char *input, char *output) ;

