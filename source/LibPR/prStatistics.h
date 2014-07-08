////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			xxx-2.h : for function2
//
//			main coder: Y. Masutani
//
//			update history
//
//			2007.05.02	start coding
//			2007.05.03	bug fixed for yyy
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_STATISTICS_H
#define PR_STATISTICS_H


#include "./prCandDataSet.h"
#include "./prMatrixTools.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */


typedef struct _PR_MEANVECTOR
{
	int numClass;
	PR_MATHMATRIX** meanVector;		// #(class) X matrix[1][#(feat)] 
}
PR_MEANVECTOR;

PR_MEANVECTOR* PR_NewMeanVector(int numClass, unsigned int numFeat);
void PR_DeleteMeanVector(PR_MEANVECTOR* vect);
PR_MEANVECTOR* PR_CalcMeanVector(PR_CASELIST* caseList, int numClass);


typedef struct _PR_COVARIANCE
{
	int numClass;
	PR_MATHMATRIX** coVariance;		// #(class) X matrix[#(feat)][#(feat)] 
}
PR_COVARIANCE;

PR_COVARIANCE* PR_NewCoVariance(int numClass, unsigned int numFeat);
void PR_DeleteCoVariance(PR_COVARIANCE* vect);
PR_COVARIANCE* PR_CalcCoVariance(PR_CASELIST* caseList, PR_MEANVECTOR* mean);



#endif

