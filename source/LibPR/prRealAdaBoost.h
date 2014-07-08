
#ifndef PR_REAL_ADABOOST_H
#define PR_REAL_ADABOOST_H


#include "./prCandDataSet.h"
#include "./prFeatTag.h"
#include "./prListEnsemble.h"


typedef struct _PR_GAUSSFUNCNODE
{
	double* avg;
	PR_MATHMATRIX* covar;

	struct _PR_GAUSSFUNCNODE* prevGaussNode;
	struct _PR_GAUSSFUNCNODE* nextGaussNode;
	void* rootMixtureModel;
}
PR_GAUSSFUNCNODE;

typedef struct _PR_GAUSSMIXTUREMODEL
{
	int	numGaussNode;

	PR_GAUSSFUNCNODE* firstGaussNode;
	PR_GAUSSFUNCNODE* lastGaussNode;
}
PR_GAUSSMIXTUREMODEL;




typedef struct _PR_DENSITYFUNCGAUSS
{
	unsigned char numClass;
	PR_GAUSSMIXTUREMODEL** gaussModels;
}
PR_DENSITYFUNCGAUSS;

PR_DENSITYFUNCGAUSS* PR_NewDensityFuncGauss(unsigned char numClass);


typedef struct _PR_REALADABOOST_NODEPARAM
{
//	PR_FEATTAG*		featureTag;
//	char			mode;			// -1:low-pass, +1:high-pass

	void* function;			// density function
	char indexFunction;		// index of function
	PR_FEATTAGTABLE* feat;	// tag of used features

	unsigned int nodeIndex;
	double nodeWeight;
	float threshold;		
	double trainError;
}
PR_REALADABOOST_NODEPARAM;

PR_REALADABOOST_NODEPARAM* PR_NewRealAdaBoostNodeParam();
PR_REALADABOOST_NODEPARAM* PR_NewRealAdaBoostNodeParamOfEnsembleNode(PR_ENSEMBLENODE* node);

void PR_DeleteDiscreteAdaBoostNodeParam(PR_BOOSTWEAKCLASSIFIERPARAMS* nodeParam);
int PR_DeleteEnsembleNodeWithDiscreteAdaBoostNodeParam(PR_ENSEMBLENODE* nodeEnsemble);

int PR_SaveTextDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName);
int PR_ReadTextDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName);

int PR_TrainDiscreteAdaBoostEnsemble(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);

int PR_NewBoostWeakClassifierParamsAddToEnsembleList(PR_ENSEMBLE* ensemble);


int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCand(PR_ENSEMBLE* ensemble, PR_CANDDATA* candData);
int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCase(PR_ENSEMBLE* ensemble, PR_CASEDATA* caseData);
int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForList(PR_ENSEMBLE* ensemble, PR_CASELIST* caseList);


#endif

