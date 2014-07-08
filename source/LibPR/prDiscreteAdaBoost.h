
#ifndef PR_DISCRETE_ADABOOST_H
#define PR_DISCRETE_ADABOOST_H


#include "./prCandDataSet.h"
#include "./prCaseDataIO.h"
#include "./prErrorMessage.h"
#include "./prErrorOfClassifier.h"
#include "./prFeatTag.h"
#include "./prListEnsemble.h"


#define PR_DWEIGHTINITIALIZE_EVEN	1
#define PR_DWEIGHTINITIALIZE_CLASS	2


typedef struct _PR_BOOSTOPTION
{
	double dataWeight;
	double initialCost;
	double onProcCost;
}
PR_BOOSTOPTION;


PR_BOOSTOPTION* PR_NewBoostOptionAddToCandData(PR_CANDDATA* candData);
int PR_NewBoostOptionAddToCandDataOfCaseList(PR_CASELIST* caseList);

void PR_DeleteBoostOptionOfCandData(PR_CANDDATA* candData);
void PR_DeleteBoostOptionOfCaseData(PR_CASEDATA* caseData);
void PR_DeleteBoostOptionOfCaseList(PR_CASELIST* caseList);

int PR_InitializeDataWeightTwoClass(PR_CASELIST* caseList, int mode);
int PR_SetInitialCost(PR_CASELIST* caseList, int mode);


typedef struct _PR_DISCRETEADABOOST_NODEPARAM
{
	PR_FEATTAG*		featureTag;
	char			mode;			// -1:low-pass, +1:high-pass
	unsigned int	nodeIndex;
	double			nodeWeight;
	float			threshold;		
	double			trainError;
}
PR_DISCRETEADABOOST_NODEPARAM;

PR_DISCRETEADABOOST_NODEPARAM* PR_NewDiscreteAdaBoostNodeParam();
PR_DISCRETEADABOOST_NODEPARAM* PR_NewDiscreteAdaBoostNodeParamOfEnsembleNode(PR_ENSEMBLENODE* node);

int
PR_CopyWeakClassifierOfDiscreteAdaBoostEnsemble(PR_ENSEMBLE* dest, PR_ENSEMBLE* src);


PR_ENSEMBLE* PR_NewDiscreteAdaBoostEnsemble(unsigned int numNode);
int PR_DeleteDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble);


int PR_TrainDiscreteAdaBoostEnsemble(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
int PR_TrainMadaBoostEnsembleDiscrete2C(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
int PR_TrainLogitBoostEnsembleDiscrete2C(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);


int PR_SaveTxtFileDiscrateAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName);
int PR_SaveBinFileDiscrateAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName);

PR_ENSEMBLE* PR_ReadTxtFileDiscreteAdaBoostEnsemble(char* fileName);
PR_ENSEMBLE* PR_ReadBinFileDiscreteAdaBoostEnsemble(char* fileName);

int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCand(PR_ENSEMBLE* ensemble, PR_CANDDATA* candData);
int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCase(PR_ENSEMBLE* ensemble, PR_CASEDATA* caseData);
int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForList(PR_ENSEMBLE* ensemble, PR_CASELIST* caseList);

int PR_CalcLhodMarginDiscAdaBoostFromCase(PR_CASEDATA* caseData, PR_ENSEMBLE* ensemble);
int PR_CalcLhodMarginDiscAdaBoostFromList(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);

int PR_StandardizeEnsembleOutputOfDiscereteAdaBoostForCase(PR_CASEDATA* caseData, PR_ENSEMBLE* ensemble, double min, double max);
int PR_StandardizeEnsembleOutputOfDiscereteAdaBoostForList(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double min, double max);

void PR_DeleteDiscreteAdaBoostNodeParam(PR_DISCRETEADABOOST_NODEPARAM* nodeParam);
int PR_DeleteEnsembleNodeWithDiscreteAdaBoostNodeParam(PR_ENSEMBLENODE* nodeEnsemble);

int PR_NewDiscreteAdaBoostNodeParamAddToEnsembleList(PR_ENSEMBLE* ensemble);

int PR_SaveTextDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName);
int PR_ReadTextDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName);

int PR_SaveBinaryDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName);
int PR_ReadBinaryDiscreteAdaBoostEnsemble(char* fileName, PR_ENSEMBLE* ensemble);


int PR_UpdataDataWeightUBoost(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, unsigned int numModel, char modeBoost);
int PR_IncludeCostInDataWeight(PR_CASELIST* caseList);
double PR_NormalizeDataWeight(PR_CASELIST* caseList);


typedef struct _PR_LOSSFUNCBOOST
{
	unsigned int num, numFP, numTP;
	double* lossFunc;
	double* lossFuncFP;
	double* lossFuncTP;
}
PR_LOSSFUNCBOOST;

PR_LOSSFUNCBOOST* PR_NewLossFuncBoost(unsigned int num);
void PR_DeleteLossFuncBoost(PR_LOSSFUNCBOOST* loss);
PR_LOSSFUNCBOOST* PR_CalculateLossOfBoostEnsemble(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);




int 
PR_TrainDiscreteAdaBoostEnsembleLogOutput(
	PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, char* logfileDataWeight, char modeBoost);


unsigned int PR_GetNumCandIDFromCaseList(PR_CASELIST* caseList);
unsigned int* PR_CountCandInIDGroupFromCaseList(PR_CASELIST* caseList, unsigned int numGroup);


int PR_DisplayInConsoleWindowEnsembleDetail(PR_ENSEMBLE* ensemble);

/*
int PR_TrainAdaBoostEnsembleByLossU(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
int PR_TrainMadaBoostEnsembleByLossU(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
int PR_TrainLogitBoostEnsembleByLossU(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
*/

#endif

