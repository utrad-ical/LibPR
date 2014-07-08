////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prBoostEnsemblePrimitive.h : for Primitive function for Boosting (= ensenble learning)
//			main coder: M. Nemoto
//
//			update history
//			2011.06.15	start coding
//
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_BOOST_ENSEMBLE_PRIMITIVE__H
#define PR_BOOST_ENSEMBLE_PRIMITIVE__H


#include "./prCandDataSet.h"
#include "./prListEnsemble.h"


PR_ENSEMBLE* PR_NewBoostEnsemble(unsigned int numNode);
int PR_DeleteBoostEnsemble(PR_ENSEMBLE* ensemble);


int PR_SaveTextBoostEnsemble(PR_ENSEMBLE* ensemble, char* filename);
int PR_SaveBinaryBoostEnsemble(PR_ENSEMBLE* ensemble, char* filename);

PR_ENSEMBLE* PR_ReadTextBoostEnsemble(char* filename);
PR_ENSEMBLE* PR_ReadBinaryBoostEnsemble(char* filename);


typedef struct _PR_BOOST_WEAK_CLASSIFIER_PARAM
{
	PR_FEATTAG*		featureTag;
	char			mode;			// -1:low-pass, +1:high-pass
	unsigned int	nodeIndex;
	double			nodeWeight;
	float			threshold;		
	double			trainError;
}
PR_BOOSTWEAKCLASSIFIERPARAMS;

PR_BOOSTWEAKCLASSIFIERPARAMS* PR_NewBoostWeakClassifierParams();
void PR_DeleteBoostWeakClassifierParams(PR_BOOSTWEAKCLASSIFIERPARAMS* nodeParam);
int PR_CopyBoostWeakClassifierParams(PR_BOOSTWEAKCLASSIFIERPARAMS* dest, PR_BOOSTWEAKCLASSIFIERPARAMS* src);

typedef struct _PR_BOOSTOPTION
{
	double dataWeight;
	double cost;
//	double onProcCost;
}
PR_BOOSTOPTION;

int PR_NewAddBoostOptionToAllCands(PR_CASELIST* caseList);
void PR_DeleteBoostOptionInAllCands(PR_CASELIST* caseList);
void PR_DeleteBoostOptionOfCand(PR_CANDDATA* candData);


#define PR_DWEIGHTINITIALIZE_EVEN	1
#define PR_DWEIGHTINITIALIZE_CLASS	2

int PR_InitializeCostBoost(PR_CASELIST* caseList, int mode);
//int PR_ReguralizeCostBoost(PR_CASELIST* caseList);


int PR_CalcOutputBoostEnsembleForCases(PR_ENSEMBLE* ensemble, PR_CASELIST* caseList);
int PR_CalcOutputBoostEnsembleForCase(PR_ENSEMBLE* ensemble, PR_CASEDATA* caseData);
int PR_CalcOutputBoostEnsembleForCand(PR_ENSEMBLE* ensemble, PR_CANDDATA* candData);

int PR_NormalizeOutputBoostEnsembleForCand(PR_ENSEMBLE* ensemble, PR_CANDDATA* canddata);
int PR_NormalizeOutputBoostEnsembleForCase(PR_ENSEMBLE* ensemble, PR_CASEDATA* casedata);
int PR_NormalizeOutputBoostEnsembleForCases(PR_ENSEMBLE* ensemble, PR_CASELIST* caselist);


#endif

