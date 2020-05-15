#ifndef __GENERAL_SDCARD_H
#define __GENERAL_SDCARD_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include "main.h"
#include "ff.h"
bool readInfoFromSD(char *readFile,  char** staticInfo);
bool writeInfoToSD(char *writeFile, char *writeBuffer);

void mountSD(FATFS* fs);
void unmountSD(void);
int fileSize(FIL* fp);

FRESULT existFile(char *fileName, FILINFO* finfo);
FRESULT openFile(char *fileName, FIL* fp);
FRESULT createFile(char *fileName, FIL* fp);
FRESULT readDataFromFile(FIL* fp, void* readBuffer, UINT btr, UINT* br);

int writeDataToFile(char *str ,FIL* fp);
void closeFile(FIL* fp);

#endif