#ifndef __SDCARD_H
#define __SDCARD_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include "main.h"
#include "ff.h"

typedef struct
{
	char WiFi_SSID[32];
	char WiFi_PW[32];
	uint8_t wificonfig;
	uint8_t mqttconfig1;
	uint8_t mqttconfig2;
	uint8_t sslconfig1;
	uint8_t sslconfig2;	
}STATIC_INFO;

void testFunc(void);
	
void storeStaticInfoToStruct(STATIC_INFO *staticInfo, char** infoVariable);
void structToStr(STATIC_INFO *staticInfo, char* writeBuffer, char** pCurrent);

bool readInfoFromSD(char *readFile,  char** staticInfo);
bool writeInfoToSD(char *writeFile, char *writeBuffer);

void addIntValToWriteStr(uint8_t *value, char* writeBuffer, char** pCurrent);
void addStrValToWriteStr(char *value, char* writeBuffer, char** pCurrent);




void mountSD(FATFS* fs);
void unmountSD(void);
int fileSize(FIL* fp);

FRESULT existFile(char *fileName, FILINFO* finfo);
FRESULT openFile(char *fileName, FIL* fp);
FRESULT createFile(char *fileName, FIL* fp);

int writeDataToFile(char *str ,FIL* fp);
FRESULT readDataFromFile(FIL* fp, void* readBuffer, UINT btr, UINT* br);

void closeFile(FIL* fp);

#endif