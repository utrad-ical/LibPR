////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prUBoostTemp.h : for Boosting (= ensenble learning) by Loss-U-Function 
//			main coder: M. Nemoto
//
//			update history
//			2011.06.21	start coding
//			2011.06.21	adaBoost, madaBoost, logitBoost, etaBoost, robustEtaBoost (binary)
//
//
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef PR_UBOOST_TEMPORAL_H
#define PR_UBOOST_TEMPORAL_H


#include "./prCandDataSet.h"
#include "./prListEnsemble.h"


int PR_TrainAdaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
int PR_TrainMadaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
int PR_TrainLogitBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble);
int PR_TrainEtaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double eta);
int PR_TrainRobustEtaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double eta);


#endif
