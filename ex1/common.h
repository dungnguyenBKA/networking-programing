#ifndef _COMMON
#define _COMMON

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MAX_STRING_LENGTH 80
char* getInputString();

void error(char* errMsg);
void errorAndExit(char* errMsg);

#endif