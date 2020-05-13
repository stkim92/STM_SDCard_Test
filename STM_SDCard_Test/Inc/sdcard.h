#ifndef __SDCARD_H
#define __SDCARD_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include "main.h"
#include "ff.h"
	
/*
void init_flash(FLASH_EraseInitTypeDef  *flashStruct);
bool common_write_flash(uint32_t _addr, void * _val, int _len, VAR_TYPE _var_type);
bool write_flash(FLASH_INFO flash_info, void * value);
char* read_string_from_flash(FLASH_INFO flash_info);
int read_int_from_flash(FLASH_INFO flash_info);
bool writeSSID2Flash(char *_str);
bool writeNUM2Flash(int _val);
*/


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

void storeWifiToStructure(STATIC_INFO *staticInfo, char** infoVariable);

//void storeWifiToStructure1(staticinfo_union *wifiInfo, char** infoVariable);

void structToStr(STATIC_INFO *staticInfo, char* writeBuffer);


bool readCertFromSD(FILINFO* finfo, FIL* fp, char *readFile, char** certFile,  UINT* br);

bool readInfoFromSD(FIL* fp, char *readFile,  char** staticInfo,  UINT* br);
bool writeInfoToSD(FIL* fp, char *writeFile, char *writeBuffer, STATIC_INFO *staticInfo);

int fileSize(FIL* fp);
	

void mountSD(FATFS* fs);
void unmountSD(void);
FRESULT existFile(char *fileName, FILINFO* finfo);
FRESULT openFile(char *fileName, FIL* fp);
FRESULT createFile(char *fileName, FIL* fp);
int writeDataToFile(char *str ,FIL* fp);
FRESULT readDataFromFile(FIL* fp, void* readBuffer, UINT btr, UINT* br);
void closeFile(FIL* fp);

#endif