#include "common.h"

char* getInputString() {
    char* des = malloc(sizeof(char) * MAX_STRING_LENGTH);
    fgets(des, MAX_STRING_LENGTH, stdin);
    if(des != NULL) {
        des[strlen(des)-1] = 0;
    }
    return des;
}

void error(char* errMsg) {
    if(errMsg == NULL || strlen(errMsg) == 0) {
        printf("An error orcur\n");
    } else {
        printf("%s\n", errMsg);
    }
}

void errorAndExit(char* errMsg) {
    error(errMsg);
    exit(0);
}