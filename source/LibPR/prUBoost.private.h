
#pragma warning(disable: 4996)

#include "../LibPR.h"

#include "./prCandDataSet.h"
#include "./prCaseDataIO.h"
//#include "./prDiscreteAdaBoost.h"
#include "./prErrorMessage.h"
#include "./prErrorOfClassifier.h"
#include "./prFeatTag.h"
#include "./prTableInputFile.h"
#include "./prListEnsemble.h"
#include "./prMatrixTools.h"

#include "GetLapTime.h"

#include <stdio.h>
#include <string.h>


typedef struct _PR_CANDIDATE_DATA_ARRAY
{
//	unsigned long num;
//	PR_CANDDATA** cands;
//	char* outputWeakClassifier;
//	double eta;
	unsigned long num;
	PR_CANDDATA** cands;
}
PR_CANDDATAARRAY;



//int PR_UBoostEssentialFunc(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int channelBoost, double eta, char bDataWeightLogOutput);


PR_CANDDATAARRAY* PR_NewCandDataArray(PR_CASELIST* caseList, double eta);
void PR_DeleteCandDataArray(PR_CANDDATAARRAY* aryCand);


unsigned long PR_MeasureDataSizeOfCandidates(PR_CASELIST* caseList);

unsigned long* PR_GetAscendSortResultOfFeature(PR_CANDDATAARRAY* args, PR_FEATTAG* tag);
unsigned int* PR_GetAscendSortResultOfFeatureOld(PR_CANDDATAARRAY* args, PR_FEATTAG* tag);

int PR_IncludeCostInDataWeightForBoost(PR_CANDDATAARRAY* ary);
double PR_NormalizeDataWeightForBoost(PR_CANDDATAARRAY* ary);
int PR_CostSensitiveNormalizeDataWeightBoost(PR_CANDDATAARRAY* ary);

//int PR_EvaluateWeightedMinErrorOfFeatureThresholding(PR_CANDDATAARRAY* args, unsigned int indexFeature,
//													 PR_BOOSTWEAKCLASSIFIERPARAMS* param, unsigned long* rankAscendSort,
//													 char modeEvaluation);
int PR_ThresholdingOptimizationByWeightedDataBoost(
		PR_CANDDATAARRAY* dataAry, unsigned int iFeature, unsigned long* ascendSorted, char mode, 
		PR_BOOSTWEAKCLASSIFIERPARAMS* output);
int PR_ThresholdingOptimizationByWeightedDataBoostOld(
		PR_CANDDATAARRAY* dataAry, unsigned int iFeature, unsigned int* ascendSorted, char mode, 
		PR_BOOSTWEAKCLASSIFIERPARAMS* output);

int PR_GetWeightOfWeakClassifier(PR_CANDDATAARRAY* aryCand, int channelBoost, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier,double Zt);
int PR_ReflectWeakClassifierOutputOnLikelihood(PR_CANDDATAARRAY* args, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier);

int PR_UpdateDataWeight(PR_CANDDATAARRAY* args, int channelBoost, double eta);

double PR_OutputSumOfLossAdaBoost(double alpha, void* option);
double PR_OutputSumOfLossMadaBoost(double alpha, void* option);
double PR_OutputSumOfLossLogitBoost(double alpha, void* option);
double PR_OutputSumOfLossEtaBoost(double alpha, void* option);
double PR_OutputSumOfLossMostBRobustEtaBoost(double alpha, void* option);


char* PR_CalculateOutputOfWeakClassifier(PR_CANDDATAARRAY* aryCand, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier);
//int PR_CalculateOutputOfWeakClassifier(PR_CANDDATAARRAY* aryCand, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier);

double PR_SumDataWeight(PR_CANDDATAARRAY* args);

double basis_CalcLossSumBoost(double wCurrent, unsigned int iTop, unsigned int iEnd, PR_CANDDATA** cands, char* outWeakModel, double eta, int boostType);


typedef struct _PR_CALCULATING_LOSS_BOOST
{
	PR_CANDDATAARRAY* dataAry;
	char* outWeakModel;
	double eta;
	int nThreads;
}
PR_CALCLOSSBOOST;


#define CGOLD 0.3819660
#define ZEPS 1.0e-10
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
double PR_BrentMethodGetArgMinForFunction(double ax, double bx, double cx, double (*function)(double, void*), void* param, 
										  int numIteration, double *xmin);


// group of "prDiscreteAdaBoost"
//int PR_ReguralizeDataWeightForBoost(PR_CASELIST* caseList);



