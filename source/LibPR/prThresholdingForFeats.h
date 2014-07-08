
#ifndef PR_THRESHOLDING
#define PR_THRESHOLDING

#include "../LibPR.h"
#include "./prCandDataSet.h"
#include "./prErrorMessage.h"
#include "./prSortTools.h"

typedef struct _PR_THRESHOLD_FORFEAT
{
	PR_FEATTAG* featTag;
	char mode;					// low-pass=-1, high-pass=+1
	float threshold;
	double ratRemoveFP;
	double ratMisdeleteTP;
}
PR_THRESHOLD_FORFEAT;


PR_THRESHOLD_FORFEAT* PR_NewThreshiodForFeat();
void PR_DeleteThreshiodForFeat(PR_THRESHOLD_FORFEAT* thdForFeat);

PR_THRESHOLD_FORFEAT* PR_DetectOutlierCandsByThresholdingForFeats(
PR_CASELIST* caseList,
double ensuredRatioTP,
double ensuredRatioAreaOfTP
);

#endif

