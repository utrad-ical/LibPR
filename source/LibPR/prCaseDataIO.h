
#ifndef PR_CASE_DATA_IO_H

#include "./prFeatTag.h"
#include "./prCandDataSet.h"


PR_CASELIST* PR_NewReadBinaryCases(char** pathes, int num);

int PR_SaveBinaryCaseData(PR_CASEDATA* caseData, char* fileName, char booOutLikelihood);

PR_CASEDATA* PR_ReadBinaryCaseData(PR_CASELIST* caseList, char* fileName);
PR_CASEDATA* PR_InsertReadBinaryCaseDataToCaseList(PR_CASELIST* caseList, char* fileName);
PR_FEATTAGTABLE* PR_ReadFeatTableFromCaseDataFile(char* fileName);


typedef struct _PR_Num_Class_And_Num_Feat_Of_Case_Data
{
	unsigned char cls;
	unsigned int feat;
}
PR_NUMCLASSANDFEATOFCASE;

typedef struct _PR_Input_Case_File_Name_Table
{
	unsigned int numCase;
	unsigned int numClass;
	char** strFileName;
}
PR_INPUTCASEFILENAMETABLE;


PR_CASELIST* PR_NewReadBinaryCaseList(PR_INPUTCASEFILENAMETABLE* table);
void PR_DeleteInputCaseFileNameTable(PR_INPUTCASEFILENAMETABLE* table);

PR_NUMCLASSANDFEATOFCASE* PR_ReadNumClassAndNumFeatFromCaseFile(char* filename);
unsigned char PR_ReadNumClassFromBinaryCaseData(char* fileName);



// For Exparts
int PR_WriteBinaryCaseDataForFILE(FILE* fp, PR_CASEDATA* caseData, char booOutLikelihood);
PR_CASEDATA* PR_ReadBinaryCaseDataInFILE(FILE* fp, PR_CASELIST* caseList);


#define PR_CASE_DATA_IO_H
#endif

