


#ifndef PR_ERROR_OF_CLASSIFIER_H

#include "./prCandDataSet.h"

typedef struct _PR_ERROROFCLASSIFIER_2C
{
	unsigned int numTP, numTN, numFN, numFP;
	double accuracy;
	double sensitivity;
	double specificity;
}
PR_ERROROFCLASSIFIER_2C;


PR_ERROROFCLASSIFIER_2C* PR_NewErrorOfClassifier2C();

void PR_DeleteErrorOfClassifier2C(PR_ERROROFCLASSIFIER_2C* eraData);

PR_ERROROFCLASSIFIER_2C* PR_CalculateErrorOfClassifier2CForTrainCandArray(PR_TRAINCANDARRAY* cands, float threshold);
//PR_ERROROFCLASSIFIER_2C* PR_CalculateErrorOfClassifier2CForTrainCandArray(PR_TRAINCANDARRAY* cands);
//PR_ERROROFCLASSIFIER_2C* PR_CalculateErrorOfClassifier2CForCaseList(PR_TRAINCANDARRAY* cands);


#define PR_ERROR_OF_CLASSIFIER_H
#endif

