
#include "./prErrorOfClassifier.h"
#include "./prErrorOfClassifier.private.h"

#include "../LibPR.h"


PR_ERROROFCLASSIFIER_2C* PR_NewErrorOfClassifier2C()
{
	PR_ERROROFCLASSIFIER_2C* eraData = new PR_ERROROFCLASSIFIER_2C;
	if(eraData==NULL)	return NULL;

	eraData->numFN = eraData->numFP = eraData->numTN = eraData->numTP = 0;
	eraData->accuracy = 0.0;
	eraData->sensitivity = 0.0;
	eraData->specificity = 0.0;

	return eraData;
}

void PR_DeleteErrorOfClassifier2C(PR_ERROROFCLASSIFIER_2C* eraData)
{
	if(eraData!=NULL)	delete eraData;
}

PR_ERROROFCLASSIFIER_2C* PR_CalculateErrorOfClassifier2CForTrainCandArray(PR_TRAINCANDARRAY* cands, double threshold)
{
	if(cands==NULL)		return NULL;

	unsigned int ncand;
	PR_CASELIST* root = (PR_CASELIST*)((PR_CASEDATA*)(cands->data[0]->rootCaseData))->rootCaseList;

	PR_ERROROFCLASSIFIER_2C* eraData = PR_NewErrorOfClassifier2C();

	for(ncand=0; ncand<cands->numData; ncand++)
	{
		if(cands->data[ncand]->likelihood[1]>=threshold)
		{
			if(cands->data[ncand]->classLabel==0)	eraData->numFP++;
			else									eraData->numTP++;
		}
		else
		{
			if(cands->data[ncand]->classLabel==0)	eraData->numTN++;
			else									eraData->numFN++;
		}
	}

	eraData->accuracy = (double)(eraData->numTN+eraData->numTP) / (double)(eraData->numFN+eraData->numFP+eraData->numTN+eraData->numTP);
	eraData->sensitivity = (double)(eraData->numTP) / (double)(eraData->numFN+eraData->numTP);
	eraData->specificity = (double)(eraData->numTN) / (double)(eraData->numFP+eraData->numTN);

	return eraData;
}

