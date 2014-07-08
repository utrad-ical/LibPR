
#include "prCaseDataIO.h"

#define PR_FILE__EXTENSION_TEXT		1
#define PR_FILE__EXTENSION_BINARY	2

PR_INPUTCASEFILENAMETABLE* PR_NewInputCaseFileNameTable(unsigned int numCase, unsigned int numClass);


int PR_CheckAndModifyFileNamesExtensionToLibPR(char* name, int mode);

PR_CASEDATA* PR_ReadBinaryCaseDataOld(PR_CASELIST* caseList, char* fileName, unsigned char numClass);

