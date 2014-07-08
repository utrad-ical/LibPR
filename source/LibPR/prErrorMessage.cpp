

#include "../LibPR.h"


int PR_ErrorMessageInt(char* str)
{
	printf("%s",str);
	if((strchr(str,'\n'))==NULL)	printf("\n");
	return PR_AVEND;
}

void* PR_ErrorMessageNULL(char* str)
{
	printf("%s",str);
	if((strchr(str,'\n'))==NULL)	printf("\n");
	return NULL;
}

void PR_ErrorMessageExit(char* str)
{
	printf("%s",str);
	if((strchr(str,'\n'))==NULL)	printf("\n");
	printf("exit from this application...");
	getchar();	
	exit(1);
}

