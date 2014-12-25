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


#ifndef PR_UBOOST_MT_2_H
#define PR_UBOOST_MT_2_H


#include "./prCandDataSet.h"
#include "./prListEnsemble.h"


PR_ENSEMBLE* PR_SimpleAdaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_SimpleMadaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_SimpleLogitBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_SimpleEtaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);
PR_ENSEMBLE* PR_SimpleRobustEtaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);

PR_ENSEMBLE* PR_CostSensitiveAdaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveMadaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveLogitBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveEtaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveRobustEtaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);

int PR_UBoostEssentialFuncMT_LowMemory(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int channelBoost, double eta, int nThreads);

#endif

