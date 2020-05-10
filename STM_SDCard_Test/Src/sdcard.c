#include "sdcard.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
extern int size;

/*********************************************************/
/*                          SD CARD FILE Functions                                */
/*********************************************************/


void structToStr(WiFi_AP *wifiInfo, char* writeBuffer)
{
	memset(writeBuffer, 0, 20);
	strcpy(writeBuffer, wifiInfo->WiFi_SSID);
	strcpy(writeBuffer+sizeof( wifiInfo->WiFi_SSID), wifiInfo->WiFi_PW);
	strcpy(writeBuffer+10, (char*)wifiInfo->num);
	
}


void storeWifiToStructure(WiFi_AP *wifiInfo, void* readBuffer)
{
	
	printf("readbuffer is : %s\n", readBuffer);
	
	char *ptr = strtok(readBuffer, ","); 
	strcpy(wifiInfo->WiFi_SSID, ptr);
	//printf("%s\n", ptr);          // 자른 문자열 출력
	
  ptr = strtok(NULL, ",");      // 다음 문자열을 잘라서 포인터를 반환
	strcpy(wifiInfo->WiFi_PW, ptr);
  //printf("%s\n", ptr);     
	
	ptr = strtok(NULL, ",");      // 다음 문자열을 잘라서 포인터를 반환
	wifiInfo->num = atoi(ptr);
	
	//printf("%s\n", ptr); 
}	

bool readInfoFromSD(FILINFO* finfo, FIL* fp, char *fileName, void* readBuffer,  UINT* br)
{
	uint8_t result = 0;
	uint8_t isExist = 0; // 0: doesn't exist, 1: exists
	
	result = existFile(fileName, finfo);
	if(result != FR_OK){
		printf("Do not Exist.\r\n");
		
		result = createFile(fileName, fp);
		if(result != FR_OK){
	  	printf("Create Error: %s : %d \r\n",__FILE__, __LINE__);
	  	return false;
	  }
	  else{
	  	printf("File Create Success.\r\n");
	  }
		
		result = openFile(fileName, fp);		
		if(result != FR_OK){
			printf("File Open Error: %s : %d \r\n",__FILE__, __LINE__);
			return false;
	  }
	  else{
	  	printf("File Open Success.\r\n");
	  }
		
		
	}
	else{
		printf("File Exist.\r\n");
		//readDataFromFile(fp, readBuffer, btr, br);
		result = openFile(fileName, fp);		
		if(result != FR_OK){
			printf("File Open Error: %s : %d \r\n",__FILE__, __LINE__);
			return false;
	  }
	  else{
	  	printf("File Open Success.\r\n");
	  }
	
	  size = fileSize(fp);
	  printf("file size : %d \n", size);
	  result = readDataFromFile(fp, readBuffer, size, br);
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
	
	
}

/*********************************************************/
/*                        SD CARD CERT Functions                                */
/*********************************************************/

bool readCertFromSD(FILINFO* finfo, FIL* fp, char *fileName, void* readBuffer,  UINT* br)
{
	uint8_t result = 0;
	
	result = existFile(fileName, finfo);
	if(result != FR_OK){
		printf("Do not Exist.\r\n");
		return false;
	}
	else{
		printf("File Exist.\r\n");
		//readDataFromFile(fp, readBuffer, btr, br);
	}
		
	result = openFile(fileName, fp);		
	if(result != FR_OK){
		printf("File Open Error: %s : %d \r\n",__FILE__, __LINE__);
		return false;
	}
	else{
		printf("File Open Success.\r\n");
	}
	
	size = fileSize(fp);
	printf("file size : %d \n", size);
	result = readDataFromFile(fp, readBuffer, size, br);
	if(result != FR_OK || br == 0)
	{
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

void storeCertToVariable(char** certVal, void* readBuffer)
{

	printf("file size : %d \n", size);
	*certVal = (char*)malloc(size);
	strcpy(*certVal, readBuffer);
	//free(certVal);
}	


/*********************************************************/
/*                    SD CARD General Functions                                */
/*********************************************************/

void initBuffer(void* readBuffer, int* size)
{
	*(char*)readBuffer = NULL;
	*size = 0;
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
	return  f_open(fp, fileName, FA_CREATE_NEW);
}

	
FRESULT readDataFromFile(FIL* fp, void* readBuffer, UINT btr, UINT* br)
{
	return f_read(fp, readBuffer, btr, br);
}

	
void writeDataToFile(char *str ,FIL* fp)
{
	f_puts(str, fp);
}

void closeFile(FIL* fp)
{
	if(f_close(fp) != FR_OK)
	  printf("f_close Error: %s : %d \r\n",__FILE__, __LINE__);
	
}