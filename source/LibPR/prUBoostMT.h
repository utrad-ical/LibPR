////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prUBoostMT.h : Cost-Sensitive U-Boosting 
//			main coder: M. Nemoto
//
//			regferences
//				[1] ***, et al. "Cost-sensitive boosting for classification of imbalanced data"
//					Patern Recognition, vol.40, pp.3358-3378, 2007
//				[2] Y Sun, et al. "Cost-sensitive boosting for classification of imbalanced data"
//					Patern Recognition, vol.40, pp.3358-3378, 2007
//
//			update history
//			2011.06.23	start coding
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_UBOOST_MULTI_THREAD_H
#define PR_UBOOST_MULTI_THREAD_H


#include "./prCandDataSet.h"
#include "./prListEnsemble.h"


PR_ENSEMBLE* PR_SimpleAdaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_SimpleMadaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_SimpleLogitBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_SimpleEtaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);
PR_ENSEMBLE* PR_SimpleRobustEtaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);

PR_ENSEMBLE* PR_CostSensitiveAdaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveMadaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveLogitBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveEtaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);
PR_ENSEMBLE* PR_CostSensitiveRobustEtaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads);


PR_ENSEMBLE* PR_UBoostMT(PR_CASELIST* caselist, int chBoost, unsigned int numClassifier, int chAddCost, double eta, int numThreads);
int PR_UBoostEssentialFuncMT(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int channelBoost, double eta, int nThreads);


#endif

