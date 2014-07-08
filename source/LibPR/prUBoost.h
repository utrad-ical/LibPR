////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prUBoost.h : for Boosting (= ensenble learning) by Loss-U-Function 
//
//			main coder: M. Nemoto
//
//			references:
//			1.	N Murata, et al.: Information geometry of U-boost and bregman divergence. 
//				Neural Computation, vol.16, pp.1432-1481, 2004.
//			2.	Y Sun, et al.: Cost-sensitive boosting for classification of imbalanced data.
//				Patern Recognition, vol.40, pp.3358-3378, 2007.
//
//			update history:
//			2009.12.07	start coding
//			2009.12.08	adaBoost, madaBoost, logitBoost, etaBoost, robustEtaBoost (binary)
//			2009.12.09	increment Cost-sensitive Boosting (AdaC2)
//			2011.06.22  function integration with prDiscreteAdaBoost group
//
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_UBOOST_H
#define PR_UBOOST_H


#include "./prCandDataSet.h"
#include "./prListEnsemble.h"


PR_ENSEMBLE* PR_SimpleAdaBoost(PR_CASELIST* caseList, unsigned int numClassifier);
PR_ENSEMBLE* PR_SimpleMadaBoost(PR_CASELIST* caseList, unsigned int numClassifier);
PR_ENSEMBLE* PR_SimpleLogitBoost(PR_CASELIST* caseList, unsigned int numClassifier);
PR_ENSEMBLE* PR_SimpleEtaBoost(PR_CASELIST* caseList, unsigned int numClassifier, double eta);
PR_ENSEMBLE* PR_SimpleRobustEtaBoost(PR_CASELIST* caseList, unsigned int numClassifier, double eta);

PR_ENSEMBLE* PR_CostSensitiveAdaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier);
PR_ENSEMBLE* PR_CostSensitiveMadaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier);
PR_ENSEMBLE* PR_CostSensitiveLogitBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier);
PR_ENSEMBLE* PR_CostSensitiveEtaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier, double eta);
PR_ENSEMBLE* PR_CostSensitiveRobustEtaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier, double eta);

PR_ENSEMBLE* PR_UBoost(PR_CASELIST* caseList, int chBoost, unsigned int numClassifier, int chAddCost, double eta);

int PR_UBoostEssentialFunc(PR_CASELIST* caseList,PR_ENSEMBLE* ensemble,int channelBoost,double eta);

//int PR_GetWeightOfWeakClassifier(PR_CANDDATAARRAY* aryCand, int channelBoost, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, double eta)


#endif

