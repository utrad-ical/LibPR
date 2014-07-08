
#ifndef PR_HISTOGRAM
#define PR_HISTOGRAM

// start cording 09/06/04


#include "./prCandDataSet.h"
#include "./prFeatTag.h"
#include "./prSortTools.h"
#include "./prFeatureData_temp.h"

typedef struct _PR_PHISTODATA
{
	unsigned int numData;
	double* prob;
	double pUnder, pOver;
	void* root;				// (PR_HISTOTABLE*)
}
PR_PHISTODATA;

typedef struct _PR_HISTOTABLE
{
	PR_FEATTAG* feat;
	unsigned char numClass;
	unsigned int numDiv;
	double* range;
	PR_PHISTODATA** pHisto;
}
PR_HISTOTABLE;



PR_HISTOTABLE* PR_NewHistoTable(unsigned char numClass, unsigned int numDiv);
void PR_DeleteHistoTable(PR_HISTOTABLE* table);

PR_HISTOTABLE* PR_CalculateFeatHistograms(PR_CASELIST* caseList, unsigned int target, unsigned int numDiv, double outRat);
PR_HISTOTABLE* PR_CalculateFeatHistograms02(PR_CASELIST* caseList, unsigned int target, unsigned int numDiv, 
											double minValue, double maxValue);

PR_HISTOTABLE* PR_CalculateFeatHistograms02_temp(PR_ARRAYCASEDATATEMP* dataTemp, unsigned int target, unsigned int numDiv, 
												 double minValue, double maxValue);

int PR_SaveHistoTable(PR_HISTOTABLE* table, char* filename);


#endif

