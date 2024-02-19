#include "../headers/util.h"
#include <stdlib.h>
#include <string.h>



char **string_split(char *str, char pattern) {

    int len = strlen(str);
    char buf[200];
    char **list = (char **) malloc(sizeof(char *) * 6);
    int list_c = 0;

    for(int i = 0, j = 0; i < len; i++) {
        if(str[i] != pattern) {
            buf[j++] = str[i];
        } else {
            buf[j] = '\0';
            j = 0;
            list[list_c++] = (char *) malloc(strlen(buf) + 1);
            strcpy(list[list_c], buf);
        }
        if (str[i] == '\n' || str[i] == '\0') {
            buf[j] = '\0';
            list[list_c++] = (char *) malloc(strlen(buf) + 1);
            strcpy(list[list_c], buf);
            break;
        }
        if (list_c == 5) {
            return list;
        }
    }
    return list;
}

int string_starts_with(char *str, char *pattern) {
    int count = 0;
    int str_len = strlen(str);
    for(int i = 0; i < str_len; i++) {
        if (str[i] != pattern[i]) {
            break;
        }
        count++;
    }
    return count;
}

char *string_remove_all(char *str, char symbol) {
    int len = strlen(str);
    char buf[len];

    for(int i = 0, j = 0; i < len; i++) {
        if(str[i] != symbol) {
            buf[j++] = str[i];
        }
        if (str[i] == '\n' || str[i] == '\0') {
            buf[j] = '\0';
            break;
        }

    }
    strcpy(str, buf);
    return str;
}
