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


#ifndef PR_BOOST_ENSEMBLE_PRIMITIVE_MT__H
#define PR_BOOST_ENSEMBLE_PRIMITIVE_MT__H


#include "./prCandDataSet.h"
#include "./prListEnsemble.h"


int PR_CalcOutputBoostEnsembleForCaseMT(PR_ENSEMBLE* ensemble, PR_CASEDATA* casedata, int nThread);


#endif

