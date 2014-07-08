////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prMahalanobisDist.h : for function
//
//			main coder: M. Nemoto
//
//			update history
//
//			2009.05.26	start coding
//			
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_MAHALANOBISDIST_H
#define PR_MAHALANOBISDIST_H


#include "./prCandDataSet.h"
#include "./prFeatTag.h"
#include "./prMatrixTools.h"


typedef struct _PR_GAUSS_PARAM
{
	PR_MATHMATRIX* mean;
	PR_MATHMATRIX* covar;
	PR_FEATTAGTABLE* fTable;
}
PR_GAUSSPARAM;

typedef struct _PR_MAHALANOBIS_PARAM
{
	unsigned char numClass;
	PR_GAUSSPARAM** gauss;
	PR_FEATTAGTABLE* featTable;
}
PR_MAHALANOBISPARAM;


//PR_GAUSSPARAM* PR_NewGaussParam(unsigned int numFeat);
PR_GAUSSPARAM* PR_NewGaussParam(PR_FEATTAGTABLE* table);
void PR_DeleteGaussParam(PR_GAUSSPARAM* gParam);

int PR_CalcGaussParam(PR_CASELIST* caseList, unsigned char targetClass, char mode, PR_GAUSSPARAM* gParam);
int PR_CalcMean(PR_CASELIST* caseList, unsigned char targetClass, char mode, PR_GAUSSPARAM* gParam);
int PR_CalcCoVariance(PR_CASELIST* caseList, unsigned char targetClass, char mode, PR_GAUSSPARAM* gParam);


PR_MAHALANOBISPARAM* PR_NewMahalanobisParam(unsigned char numClass, PR_FEATTAGTABLE* featTable);
void PR_DeleteMahalanobisParam(PR_MAHALANOBISPARAM* mParam);


PR_MAHALANOBISPARAM* PR_NewCalcMahalanobisParam(PR_CASELIST* caseList);
int PR_CalcMahalanobisParam(PR_CASELIST* caseList, PR_MAHALANOBISPARAM* mParam);


int PR_CalcMahalanobisDist(PR_CASELIST* caseList, unsigned char targetClass, PR_GAUSSPARAM* gParam);
int PR_CalcMahalanobisDistAllClass(PR_CASELIST* caseList, PR_MAHALANOBISPARAM* mParam);

int PR_CalcMahalanobisDistForOneCase(PR_CASEDATA* caseData, unsigned char targetClass, PR_GAUSSPARAM* mParam);
int PR_CalcMahalanobisDistForOneCaseAllClass(PR_CASEDATA* caseData, PR_MAHALANOBISPARAM* mParam);

int PR_SaveBinaryMahalanobisParam(PR_MAHALANOBISPARAM* param, char* filename);
PR_MAHALANOBISPARAM* PR_ReadBinaryMahalanobisParam(char* filename);

int PR_SaveTextMahalanobisParam(PR_MAHALANOBISPARAM* param, char* filename);
PR_MAHALANOBISPARAM* PR_ReadTextMahalanobisParam(char* filename);


int
PR_FprintMahalanobisParam(PR_MAHALANOBISPARAM* param, FILE* fp);


int 
PR_CalcMahalanobisDistRatioForCand(
	PR_CANDDATA* candData, unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn);
int
PR_CalcMahalanobisDistRatioForCase(
	PR_CASEDATA* caseData, unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn);
int
PR_CalcMahalanobisDistRatioForList(
	PR_CASELIST* caseList, unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn);


PR_MAHALANOBISPARAM* PR_GetMahalanobisParamOfSpecificFeatutes(PR_MAHALANOBISPARAM* source, PR_FEATTAGTABLE* destTable);

#endif // PR_MAHALANOBISDIST_H

