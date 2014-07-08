////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prFeatureData_temp.h : temporaly mode 
//			main coder: M. Nemoto
//
//			update history
//			2009.12.14	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_EFATURE_META_TEMP
#define PR_EFATURE_META_TEMP


#include <stdio.h>
#include "./prFeatTag.h"
#include "./prTableInputFile.h"


#define _NAME_TEMP_CAND_ID	".id"
#define _NAME_TEMP_CLASS	".class"
#define _NAME_TEMP_COST		".cost"
#define _NAME_TEMP_FEAT		".feat"
#define _NAME_TEMP_LHOD		".lhod"


typedef struct _PR_DATA_TEMPORAL
{
	FILE* candidateID;		// unsigned int
	FILE* classLabel;		// unsigned char
	FILE* cost;				// double
//	FILE** feature;			// float, tempFeature[i] = temporary of i-th feature
	FILE* feature;			// float, tempFeature[i] = temporary of i-th feature
	FILE** likelihood;		// double, tempLikelihood[c] = temporary of ilikelihood for class-c
}
PR_DATATEMPORAL;

typedef struct _PR_CASE_DATA_TEMPORAL
{
	unsigned int idCase;
	unsigned int numCandidate;
	unsigned int* numCandidateInClass;
	
	void* rootTable;
	void* specific;
	char namebase[32];
	
//	PR_DATATEMPORAL* temp;
}
PR_CASEDATATEMP;

typedef struct _PR_ARRAY_CASE_DATA_TEMPORAL
{
	unsigned int numCandidate;
	unsigned int* numCandidateInClass;
	unsigned int numCase;
	unsigned char numClass;

	PR_FEATTAGTABLE* tableFeature;
	PR_CASEDATATEMP** arrayCase;

	PR_DATATEMPORAL* temp;
}
PR_ARRAYCASEDATATEMP;

PR_ARRAYCASEDATATEMP*
PR_NewArrayCaseDataTemp(unsigned int numCase, unsigned char numClass, unsigned int numFeature);

void PR_DeleteCaseDataTemp(PR_CASEDATATEMP* data);
void PR_DeleteArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* ary);

PR_ARRAYCASEDATATEMP* PR_NewReadArrayCaseDataTemp(PR_TABLEINPUTFILE* tableFile);
PR_ARRAYCASEDATATEMP* PR_NewArrayCaseDataTempWithoutArrayCase(unsigned char numClass, unsigned int numFeature);

/*
int PR_OpenTempFilesInCaseDataTemp(PR_CASEDATATEMP* dataTemp);
int PR_CloseTempFilesInCaseDataTemp(PR_CASEDATATEMP* dataTemp);
int PR_RemoveTempFilesInCaseDataTemp(PR_CASEDATATEMP* dataTemp);
*/

int PR_CountNumCandidateInClassOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* data);



/*
float* PR_ExtractArrayOfOneFeature(PR_ARRAYCASEDATATEMP* tableMeta, unsigned int idxFeature);
unsigned char* PR_ExtractArrayOfClassLabel(PR_ARRAYCASEDATATEMP* tableMeta);
double* PR_ExtractArrayOfCost(PR_ARRAYCASEDATATEMP* tableMeta);
double* PR_ExtractArrayOfLikelihood(PR_ARRAYCASEDATATEMP* tableMeta, unsigned char classLabel);
unsigned int* PR_ExtractArrayOfCandidateID(PR_ARRAYCASEDATATEMP* tableMeta);
*/

float* PR_ExtractFullArrayOfOneFeature(PR_ARRAYCASEDATATEMP* tableMeta, unsigned int idxFeature);
unsigned char* PR_ExtractFullArrayOfClassLabel(PR_ARRAYCASEDATATEMP* tableMeta);
double* PR_ExtractFullArrayOfCost(PR_ARRAYCASEDATATEMP* tableMeta);
double* PR_ExtractFullArrayOfLikelihood(PR_ARRAYCASEDATATEMP* tableMeta, unsigned char classLabel);
unsigned int* PR_ExtractFullArrayOfCandidateID(PR_ARRAYCASEDATATEMP* tableMeta);

float* PR_ExtractOneCaseArrayOfOneFeature(PR_CASEDATATEMP* data, unsigned int idxFeature);
float* PR_ExtractOneCaseArrayOfFullFeatures(PR_CASEDATATEMP* data);
unsigned char* PR_ExtractOneCaseArrayOfClassLabel(PR_CASEDATATEMP* data);
double* PR_ExtractOneCaseArrayOfCost(PR_CASEDATATEMP* data);
double* PR_ExtractOneCaseArrayOfLikelihood(PR_CASEDATATEMP* data, unsigned char classLabel);
unsigned int* PR_ExtractOneCaseArrayOfCandidateID(PR_CASEDATATEMP* data);


int PR_RestructCaseDataByNewFeatTableTemp(PR_ARRAYCASEDATATEMP* dataTemp, PR_FEATTAGTABLE* tableNewFeature);
int PR_BandPassForOneFeatureTemp(PR_ARRAYCASEDATATEMP* data, unsigned int idFeat, float min, float max);

int PR_ReplaceClassLabelInArrayClassDataTemp(PR_ARRAYCASEDATATEMP* dataTemp, unsigned char* source);

#endif

