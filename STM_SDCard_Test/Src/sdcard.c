#include "sdcard.h"
#include "main.h"
#include <stdio.h>
#include <string.h>



/*********************************************************/
/*                          SD CARD FILE Functions                                */
/*********************************************************/


void structToStr(STATIC_INFO *staticInfo, char* writeBuffer)
{
	sprintf(writeBuffer, "%s\n%s\n%d\n%d\n%d\n%d\n%d\n", staticInfo->WiFi_SSID, staticInfo->WiFi_PW, staticInfo->wificonfig, staticInfo->mqttconfig1, staticInfo->mqttconfig2, staticInfo->sslconfig1, staticInfo->sslconfig2);
	
	//strcpy(writeBuffer+10, (char*)staticInfo->num);
}


void storeWifiToStructure(STATIC_INFO *staticInfo, char** infoVariable)
{
	
	//printf("infoVariable is : \r\n%s\n", *infoVariable);
	
	char *ptr = strtok(*infoVariable, "\n"); 
	strcpy(staticInfo->WiFi_SSID, ptr);
	//printf("%s\n", ptr);          // 자른 문자열 출력
	
  ptr = strtok(NULL, "\n");      // 다음 문자열을 잘라서 포인터를 반환
	strcpy(staticInfo->WiFi_PW, ptr); 
	
	ptr = strtok(NULL, "\n");      // 다음 문자열을 잘라서 포인터를 반환
	staticInfo->wificonfig = atoi(ptr);
	
	ptr = strtok(NULL, "\n");      // 다음 문자열을 잘라서 포인터를 반환
	staticInfo->mqttconfig1 = atoi(ptr);
	
	ptr = strtok(NULL, "\n");      // 다음 문자열을 잘라서 포인터를 반환
	staticInfo->mqttconfig2= atoi(ptr);
	
	ptr = strtok(NULL, "\n");      // 다음 문자열을 잘라서 포인터를 반환
	staticInfo->sslconfig1= atoi(ptr);
	
	ptr = strtok(NULL, "\n");      // 다음 문자열을 잘라서 포인터를 반환
	staticInfo->sslconfig2= atoi(ptr);
	
}	

bool writeInfoToSD(FIL* fp, char *writeFile, char *writeBuffer, STATIC_INFO *staticInfo)
{
	uint8_t result = 0;
	int size = 0;
	
	result = createFile(writeFile, fp);
	printf("write file is %s\r\n", writeFile);
	if(result != FR_OK){
		printf("create result is %d\r\n", result);
	  printf("Create Error: %s : %d \r\n",__FILE__, __LINE__);
	  return false;
	}
	else{
	  printf("File Create Success.\r\n"); 
		
	}

	structToStr(staticInfo, writeBuffer);
	size = writeDataToFile(writeBuffer,fp);
	printf("File write Success, size:  %d \r\n",size);
	
	/* Close file */
	closeFile(fp);
	return true;
}

bool readInfoFromSD(FIL* fp, char *readFile,  char** staticInfo,  UINT* br)
{
	uint8_t result = 0;
	int size = 0;
	
	result = openFile(readFile, fp);		
	if(result != FR_OK){
		printf("File Open Error: %s : %d \r\n",__FILE__, __LINE__);
		return false;
	}
	else{
	 	printf("File Open Success.\r\n");
	}
	
	size = fileSize(fp);
 	printf("file size : %d \n", size);
 	*staticInfo = (char*)malloc(size);
		
	result = readDataFromFile(fp, *staticInfo, size, br);
		
	if(result != FR_OK || br == 0){
	 	printf("f_read error result value is %d \r\n", result);
    printf("f_read Error: %s : %d \r\n",__FILE__, __LINE__);
	  return false;
  }
  else{
  	printf("File Read Success.\r\n");	
  }
	
	
	/* Close file */
	closeFile(fp);
	return true;
	
}


/*********************************************************/
/*                        SD CARD CERT Functions                                */
/*********************************************************/


bool readCertFromSD(FILINFO* finfo, FIL* fp, char *readFile, char** certVariable,  UINT* br)
{
	uint8_t result = 0;
	int size = 0;
	
	result = existFile(readFile, finfo);
	if(result != FR_OK){
		printf("Do not Exist.\r\n");
		return false;
	}
	else{
		printf("File Exist.\r\n");
		//readDataFromFile(fp, readBuffer, btr, br);
	}
		
	result = openFile(readFile, fp);		
	if(result != FR_OK){
		printf("File Open Error: %s : %d \r\n",__FILE__, __LINE__);
		return false;
	}
	else{
		printf("File Open Success.\r\n");
	}
	
	size = fileSize(fp);
	printf("file size : %d \n", size);
	*certVariable = (char*)malloc(size);

	result = readDataFromFile(fp, *certVariable, size, br);

	if(result != FR_OK || br == 0){
		printf("f_read error result value is %d \r\n", result);
    printf("f_read Error: %s : %d \r\n",__FILE__, __LINE__);
		return false;
	}
	else{
		printf("File Read Success.\r\n");	
	}

	/* Close file */
	closeFile(fp);
	return true;
}




/*********************************************************/
/*                     SD CARD Basic Functions                                   */
/*********************************************************/

/* Mount SD Card */
void mountSD(FATFS* fs)
{
  if(f_mount(fs, "", 0) != FR_OK)
	  printf("f_mount Error: %s : %d \r\n",__FILE__, __LINE__);
}

/* Unmount SDCARD */
void unmountSD(void)
{
  if(f_mount(NULL, "", 1) != FR_OK)
	  printf("f_mount Error: %s : %d \r\n",__FILE__, __LINE__);	
}

int fileSize(FIL* fp)
{
	return  f_size(fp);
}

FRESULT existFile(char *fileName, FILINFO* finfo)
{
	return  f_stat(fileName, finfo);
}


FRESULT openFile(char *fileName, FIL* fp)
{
	return  f_open(fp, fileName, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
}

FRESULT createFile(char *fileName, FIL* fp)
{
	return  f_open(fp, fileName, FA_CREATE_NEW | FA_WRITE);
}

	
FRESULT readDataFromFile(FIL* fp, void* readBuffer, UINT btr, UINT* br)
{
	return f_read(fp, readBuffer, btr, br);
}

	
int writeDataToFile(char *str ,FIL* fp)
{
	f_puts(str, fp);
}

void closeFile(FIL* fp)
{
	if(f_close(fp) != FR_OK)
	  printf("f_close Error: %s : %d \r\n",__FILE__, __LINE__);
	
}