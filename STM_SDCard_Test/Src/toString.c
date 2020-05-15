#include "toString.h"
#include "main.h"
#include <stdio.h>
#include <string.h>


/*********************************************************/
/*           ADD Int/Char* Value to char* Functions                               */
/*********************************************************/


void addIntValToString(uint8_t *value, char* writeBuffer, char** pCurrent)
{
	
	*pCurrent += sprintf(*pCurrent,"%d\n", *value );
	
	printf("[writeBuffer]\n\%s", writeBuffer);
	
	//strcpy(writeBuffer+10, (char*)staticInfo->num);
}

void addStrValToString(char *value, char* writeBuffer, char** pCurrent)
{
	
	*pCurrent += sprintf(*pCurrent,"%s\n", value );
	
	printf("[writeBuffer]\n %s", writeBuffer);
	
	//strcpy(writeBuffer+10, (char*)staticInfo->num);
}
