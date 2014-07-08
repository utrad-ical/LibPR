
#pragma warning(disable: 4996)

#include "../LibPR.h"

#include "./prCandDataSet.h"
#include "./prCaseDataIO.h"
//#include "./prDiscreteAdaBoost.h"
#include "./prErrorMessage.h"
#include "./prErrorOfClassifier.h"
#include "./prFeatureData_temp.h"
#include "./prFeatTag.h"
#include "./prTableInputFile.h"
#include "./prListEnsemble.h"
#include "./prMatrixTools.h"

#include <stdio.h>
#include <string.h>


typedef struct _PR_ARRAY_CANDIDATE_FOR_TRAINING_BY_TEMP
{
	unsigned char* classLabel;
	double* cost;
	double* outEnsemble;
	char* outWeakClassifier;

	double paramEta;
	unsigned int num;
}
PR_ARRAYCANDIDATEFORTRAININGBYTEMP;



int 
PR_TrainEnsembleUBoost_Temp(PR_ARRAYCASEDATATEMP* tableMeta,
							PR_ENSEMBLE* ensemble, int channelBoost, double eta,
							char bDataWeightLogOutput);



int PR_NormalizeValueInDataAry(double* ary, unsigned int num);
int PR_IncludeCostInDataWeightForBoost_Temp(PR_ARRAYCASEDATATEMP* tableMeta, double* aryDataWeight);
char* PR_CalculateOutputOfWeakClassifier_Temp(PR_ARRAYCASEDATATEMP* tableMeta, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier);

int 
PR_EvaluateWeightedMinErrorOfFeatureThresholding_Temp(PR_ARRAYCASEDATATEMP* tableMeta, unsigned int idxUseFeature, 
													  unsigned int* aryRank, unsigned char* classLabel, double* dataWeight,
													  PR_BOOSTWEAKCLASSIFIERPARAMS* param,
													  char modeEvaluation);

int 
PR_GetWeightOfWeakClassifier_Temp(PR_ARRAYCASEDATATEMP* tableMeta, 
								  double* aryOutEnsemble, char* aryOutWeak, unsigned char* classLabel,
								  PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, int channelBoost, double eta);

double PR_OutputSumOfLossMadaBoost_Temp(double alpha, void* option);
double PR_OutputSumOfLossLogitBoost_Temp(double alpha, void* option);
double PR_OutputSumOfLossEtaBoost_Temp(double alpha, void* option);
double PR_OutputSumOfLossMostBRobustEtaBoost_Temp(double alpha, void* option);

int 
UpdateWeightOfCandidateByObtainedEnsembleOutput_Temp(double* dataWeight, double* outEnsemble, unsigned char* classLabel, 
													 unsigned int numCandidate, 
													 int channelBoost, double eta);



///-- in prUBoost.private.h --///

#define CGOLD 0.3819660
#define ZEPS 1.0e-10
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
double PR_BrentMethodGetArgMinForFunction(double ax, double bx, double cx, double (*function)(double, void*), void* param, 
										  int numIteration, double *xmin);

double PR_OutputSumOfLossAdaBoost(double alpha, void* option);
double PR_OutputSumOfLossMadaBoost(double alpha, void* option);
double PR_OutputSumOfLossLogitBoost(double alpha, void* option);
double PR_OutputSumOfLossEtaBoost(double alpha, void* option);
double PR_OutputSumOfLossMostBRobustEtaBoost(double alpha, void* option);

