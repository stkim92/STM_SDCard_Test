#include "sdcard.h"
#include "main.h"
#include <stdio.h>
#include <string.h>


/*********************************************************/
/*                          TEST Function                                  */
/*********************************************************/

void testFunc(void)
{
	FATFS fs;
	FILINFO finfo;


  char *staticInfoFileName = "static4.txt";
  char *staticInfoStr;
  char writeBuffer[69]={0,};
  char *pCurrent;

  char *firstCertName = "91375cab42-private.pem.key";
  char *firstCert;

  char *secondCertName = "testtest.key";
  char *secondCert;


  STATIC_INFO staticInfo = {"SSID1", "PW1", 112, 1, 2, 3, 4};

	/* Mount SD Card */
	mountSD(&fs);

	printf("------------------------------------------\r\n");
  printf("		   SPI SD Card INFO File Read/Write Test \r\n");
  printf("------------------------------------------\r\n");
	

	
	int result = existFile(staticInfoFileName, &finfo);
	if(result != FR_OK){
		printf("Do not Exist.\r\n");
		structToStr(&staticInfo, writeBuffer, &pCurrent);
		writeInfoToSD(staticInfoFileName, writeBuffer);
	}
	else{
		printf("Do Exist.\r\n");
		readInfoFromSD(staticInfoFileName,  &staticInfoStr);
		storeStaticInfoToStruct(&staticInfo, &staticInfoStr);
	}
	
	printf("[Static Info Print]\r\n");
	printf("SSID: %s\n", staticInfo.WiFi_SSID);
	printf("PW: %s\n", staticInfo.WiFi_PW);
	printf("wificonfig: %d\n", staticInfo.wificonfig);
	printf("mqttconfig1: %d\n", staticInfo.mqttconfig1);
	printf("mqttconfig2: %d\n", staticInfo.mqttconfig2);
	printf("sslconfig1: %d\n", staticInfo.sslconfig1);
	printf("sslconfig2: %d\n", staticInfo.sslconfig2);
	
	
  printf("------------------------------------------\r\n");
  printf("		   SPI SD Card CERT File Read Test \r\n");
  printf("------------------------------------------\r\n");

	/* Read Certification from SD Card */
	result = existFile(staticInfoFileName, &finfo);
	if(result != FR_OK){
		printf("Do not Exist.\r\n");
	}
	else{
		printf("Do Exist.\r\n");
		readInfoFromSD(firstCertName,  &firstCert);
	  printf("[firstCert]\r\n%s\r\n", firstCert);
	}
	
	result = existFile(staticInfoFileName, &finfo);
	if(result != FR_OK){
		printf("Do not Exist.\r\n");
	}
	else{
		printf("Do Exist.\r\n");
		readInfoFromSD(secondCertName, &secondCert);
	  printf("[secondCertName]\r\n%s\r\n", secondCert);
	}
	
	/* Free Cert  */
  free(firstCert);
	free(secondCertName);
	

  /* Unmount SDCARD */
  unmountSD();
}

/*********************************************************/
/*                          SKP Static Info Parsing                                  */
/*********************************************************/

void structToStr(STATIC_INFO *staticInfo, char* writeBuffer, char** pCurrent)
{
  *pCurrent = writeBuffer;	
	addStrValToWriteStr(staticInfo->WiFi_SSID, writeBuffer, pCurrent);
	addStrValToWriteStr(staticInfo->WiFi_PW, writeBuffer, pCurrent);
	addIntValToWriteStr(&staticInfo->wificonfig, writeBuffer, pCurrent);
	addIntValToWriteStr(&staticInfo->mqttconfig1, writeBuffer, pCurrent);
	addIntValToWriteStr(&staticInfo->mqttconfig2, writeBuffer, pCurrent);
	addIntValToWriteStr(&staticInfo->sslconfig1, writeBuffer, pCurrent);
	addIntValToWriteStr(&staticInfo->sslconfig2, writeBuffer, pCurrent);
}



void storeStaticInfoToStruct(STATIC_INFO *staticInfo, char** infoVariable)
{
	
	//printf("infoVariable is : \r\n%s\n", *infoVariable);
	
	char *ptr = strtok(*infoVariable, "\n"); 
	strcpy(staticInfo->WiFi_SSID, ptr);
	//printf("%s\n", ptr);          // 자른 문자열 출력
	
  ptr = strtok(NULL, "\n");      
	strcpy(staticInfo->WiFi_PW, ptr); 
	
	ptr = strtok(NULL, "\n");     
	staticInfo->wificonfig = atoi(ptr);
	
	ptr = strtok(NULL, "\n");     
	staticInfo->mqttconfig1 = atoi(ptr);
	
	ptr = strtok(NULL, "\n");    
	staticInfo->mqttconfig2= atoi(ptr);
	
	ptr = strtok(NULL, "\n");      
	staticInfo->sslconfig1= atoi(ptr);
	
	ptr = strtok(NULL, "\n");      
	staticInfo->sslconfig2= atoi(ptr);
	
}	


/*********************************************************/
/*                               General Functions                                    */
/*********************************************************/


void addIntValToWriteStr(uint8_t *value, char* writeBuffer, char** pCurrent)
{
	
	*pCurrent += sprintf(*pCurrent,"%d\n", *value );
	
	printf("[writeBuffer]\n\%s", writeBuffer);
	
	//strcpy(writeBuffer+10, (char*)staticInfo->num);
}

void addStrValToWriteStr(char *value, char* writeBuffer, char** pCurrent)
{
	
	*pCurrent += sprintf(*pCurrent,"%s\n", value );
	
	printf("[writeBuffer]\n %s", writeBuffer);
	
	//strcpy(writeBuffer+10, (char*)staticInfo->num);
}

/*********************************************************/
/*                     SD CARD Basic Functions                                   */
/*********************************************************/


bool writeInfoToSD(char *writeFile, char *writeBuffer)
{
	FIL fp;
	uint8_t result = 0;
	int size = 0;
	
	result = createFile(writeFile, &fp);
	printf("write file is %s\r\n", writeFile);
	if(result != FR_OK){
		printf("create result is %d\r\n", result);
	  printf("Create Error: %s : %d \r\n",__FILE__, __LINE__);
	  return false;
	}
	else{
	  printf("File Create Success.\r\n"); 
		
	}
	size = writeDataToFile(writeBuffer,&fp);
	printf("File write Success, size:  %d \r\n",size);
	
	/* Close file */
	closeFile(&fp);
	return true;
}

bool readInfoFromSD(char *readFile,  char** staticInfo)
{
	uint8_t result = 0;
	int size = 0;
	FIL fp;
	uint32_t  bytesRead;
	

	result = openFile(readFile, &fp);		
	if(result != FR_OK){
		printf("File Open Error: %s : %d \r\n",__FILE__, __LINE__);
		return false;
	}
	else{
	 	printf("File Open Success.\r\n");
	}
	
	size = fileSize(&fp);
 	printf("file size : %d \n", size);
 	*staticInfo = (char*)malloc(size);
		
	result = readDataFromFile(&fp, *staticInfo, size, &bytesRead);
		
	if(result != FR_OK || bytesRead == 0){
	 	printf("f_read error result value is %d \r\n", result);
    printf("f_read Error: %s : %d \r\n",__FILE__, __LINE__);
	  return false;
  }
  else{
  	printf("File Read Success.\r\n");	
  }
	
	closeFile(&fp);
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