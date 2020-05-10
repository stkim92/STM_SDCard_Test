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

typedef struct CertVal_t
{
	char *cert1;
	char *cert2;
	char *cert3;
}CertVal;

typedef struct
{
	char WiFi_SSID[32];
	char WiFi_PW[32];
	int num;
}WiFi_AP;

void storeWifiToStructure(WiFi_AP *wifiInfo, void* readBuffer);
void structToStr(WiFi_AP *wifiInfo, char* writeBuffer);

bool readCertFromSD(FILINFO* finfo, FIL* fp, char *fileName, void* readBuffer,  UINT* br);
bool readInfoFromSD(FILINFO* finfo, FIL* fp, char *fileName, void* readBuffer,  UINT* br);

int fileSize(FIL* fp);
void storeCertToVariable(char** certVal, void* readBuffer);
void initBuffer(void* readBuffer, int* size);
	

void mountSD(FATFS* fs);
void unmountSD(void);
FRESULT existFile(char *fileName, FILINFO* finfo);
FRESULT openFile(char *fileName, FIL* fp);
FRESULT createFile(char *fileName, FIL* fp);
void writeDataToFile(char *str ,FIL* fp);
FRESULT readDataFromFile(FIL* fp, void* readBuffer, UINT btr, UINT* br);
void closeFile(FIL* fp);

#endif