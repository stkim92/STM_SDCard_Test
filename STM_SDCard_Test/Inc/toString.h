#ifndef __TOSTRING_H
#define __TOSTRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


void addIntValToString(uint8_t *value, char* writeBuffer, char** pCurrent);
void addStrValToString(char *value, char* writeBuffer, char** pCurrent);

#endif