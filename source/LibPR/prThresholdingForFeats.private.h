
#ifndef PR_THRESHOLDING_PRIVATE
#define PR_THRESHOLDING_PRIVATE

#include "../LibPR.h"
#include "./prCandDataSet.h"
#include "./prErrorMessage.h"
#include "./prSortTools.h"

typedef struct _PR_SUBCANDGROUP
{
	unsigned int idCase;
	unsigned int idCand;
	unsigned int numCand;
	PR_CANDDATA** candPt;
	struct _PR_SUBCANDGROUP* nodePrev;
	struct _PR_SUBCANDGROUP* nodeNext;
	void* rootList;
}
PR_SUBCANDGROUP;


typedef struct _PR_SUBCANDGROUPLIST
{
	unsigned int numNode;
	PR_SUBCANDGROUP* firstNode;
	PR_SUBCANDGROUP* lastNode;
}
PR_SUBCANDGROUPLIST;

#endif

