////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : Pattern Recognition library
//
//			prAnalysisDataset.cpp : for function1
//
//			main coder: M. Nemoto
//
//			update history
//
//			2007.08.27	start coding
//			
//
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4996)

#include "prCandDataSet.h"
#include "prCandDataSet.private.h"


PR_CANDDATA* PR_NewCandData(unsigned char numClass, unsigned int numFeat)
{
	if(numClass==0 || numFeat==0)
	{
		fprintf(stderr,"NumError(@PR_NewCandData) Class=%d,Feat=%d\n",numClass,numFeat);
		return NULL;
	}

	PR_CANDDATA* candData = new PR_CANDDATA;
	if(candData==NULL)	return NULL;

	candData->idCand=0;
	candData->booOutlier=0;
	candData->classLabel=0;
	candData->featValues = new float [numFeat];
	memset(candData->featValues,0x00,sizeof(float)*numFeat);
	candData->likelihood = new double [numClass];
	memset(candData->likelihood,0x00,sizeof(double)*numClass);

	candData->optionParam = NULL;

	candData->nodePrevCand = NULL;
	candData->nodeNextCand = NULL;
	candData->rootCaseData = NULL;

	return candData;
}

PR_CASEDATA* PR_NewCaseData(unsigned char numClass)
{
	if(numClass==0)
	{
		fprintf(stderr,"NumError(@PR_NewCaseData) Class=%d\n",numClass);
		return NULL;
	}

	PR_CASEDATA* caseData = new PR_CASEDATA;
	if(caseData==NULL)
	{
		fprintf(stderr,"InsufficientMemory(@PR_NewCaseData)\n",numClass);
		return NULL;
	}

	caseData->idCase = 0;
	caseData->numCand = 0;
	caseData->numCandOfClass = new unsigned int [numClass+1];
	memset(caseData->numCandOfClass,0x00,sizeof(unsigned int)*(numClass+1));

	caseData->childNodeFirstCand = NULL;
	caseData->childNodeLastCand = NULL;
	caseData->nodePrevCase = NULL;
	caseData->nodeNextCase = NULL;
	caseData->rootCaseList = NULL;

	return caseData;
}

PR_CASELIST* PR_NewCaseList(unsigned char numClass, unsigned int numFeat)
{
	if(numClass==0 || numFeat==0)
	{
		fprintf(stderr,"NumError(@PR_NewCaseList) Class=%d,Feat=%d\n",numClass,numFeat);
		return NULL;
	}

	PR_CASELIST* caseList = new PR_CASELIST;
	if(caseList==NULL)	return NULL;

	caseList->numCase = 0;
	caseList->numClass = numClass;
	caseList->totalNumCand = 0;
	
	caseList->totalNumCandOfClass = new unsigned long [numClass+1];
	memset(caseList->totalNumCandOfClass,0x00,sizeof(unsigned long)*(numClass+1));

	caseList->featTagTable = PR_NewFeatTagTable(numFeat);

	unsigned int ii;
	for(ii=0; ii<numFeat; ii++)
	{
		caseList->featTagTable->tags[ii]->group = (unsigned short)(ii>>16);
		caseList->featTagTable->tags[ii]->element = (unsigned short)(ii&0xFFFF);
	}

	caseList->childNodeFirstCase = NULL;
	caseList->childNodeLastCase = NULL;

	return caseList;
}


void PR_DeleteCandData(PR_CANDDATA* candData)
{
	int dbg=0;

	if(candData==NULL)	return;

	if(candData->optionParam!=NULL)
	{
		fprintf(stderr,"ExistPrivateParameters(@PR_DeleteCandData)\n");
		getchar();
		exit(1);
		return;
	}
	if(dbg)	printf("d1,");

	if(candData->rootCaseData!=NULL)
	{
		PR_CASEDATA* rootCase = (PR_CASEDATA*)candData->rootCaseData;

		if(rootCase->childNodeFirstCand == candData)	rootCase->childNodeFirstCand = candData->nodeNextCand;
		if(rootCase->childNodeLastCand == candData)		rootCase->childNodeLastCand  = candData->nodePrevCand;

		rootCase->numCand--;

		if(rootCase->rootCaseList!=NULL)
		{
			PR_CASELIST* rootList = (PR_CASELIST*)rootCase->rootCaseList;
			rootList->totalNumCand--;

			unsigned char cls = candData->classLabel;
			if(cls>=rootList->numClass)		cls = rootList->numClass-1;

			rootCase->numCandOfClass[cls]--;
			rootList->totalNumCandOfClass[cls]--;
		}
	}
	if(dbg)	printf("d2,");

	PR_CANDDATA* prevCand = candData->nodePrevCand;
	PR_CANDDATA* nextCand = candData->nodeNextCand;
	if(dbg)	printf("d3,");

	if(prevCand!=NULL)	prevCand->nodeNextCand = nextCand;
	if(nextCand!=NULL)	nextCand->nodePrevCand = prevCand;
	if(dbg)	printf("d4,");

	candData->optionParam = NULL;
	delete [] candData->featValues;
	delete [] candData->likelihood;
	delete candData;
	if(dbg)	printf("d5,");

	return;
}

void PR_DeleteCaseData(PR_CASEDATA* caseData)
{
	int dbg=0;

	if(caseData==NULL)	return;
	if(dbg)	printf("\nca0,");

	if(caseData->rootCaseList!=NULL)
	{
		PR_CASELIST* rootCaseList = (PR_CASELIST*)caseData->rootCaseList;

		if(caseData == rootCaseList->childNodeFirstCase)
			rootCaseList->childNodeFirstCase = caseData->nodeNextCase;
		if(caseData == rootCaseList->childNodeLastCase)
			rootCaseList->childNodeLastCase = caseData->nodePrevCase;

		rootCaseList->numCase--;
	}
	if(dbg)	printf("ca1\n");

	unsigned int cntCand=caseData->numCand;
	PR_CANDDATA* currentCand = caseData->childNodeLastCand;
	while(currentCand!=NULL)
	{
		if(dbg)	printf("num(remain-cand)=%d ",caseData->numCand);

		PR_DeleteCandData(currentCand);
		currentCand = caseData->childNodeLastCand;

		if(dbg)	printf("  \r");
	}
	if(dbg)	printf("\nca2\n");

	PR_CASEDATA* prevCase = caseData->nodePrevCase;
	PR_CASEDATA* nextCase = caseData->nodeNextCase;

	if(prevCase!=NULL)	prevCase->nodeNextCase = nextCase;
	if(nextCase!=NULL)	nextCase->nodePrevCase = prevCase;
	if(dbg)	printf("ca3,");

	delete [] caseData->numCandOfClass;
	delete caseData;
	if(dbg)	printf("ca4\n");

	return;
}

void PR_DeleteCaseList(PR_CASELIST* caseList)
{
	int dbg=0;

	if(caseList==NULL)	return;

	PR_CountNumCandsForCaseList(caseList);
	if(dbg)	printf("\nL0,");

	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	if(dbg)	printf("L1,");
	
	PR_DeleteFeatTagTable(caseList->featTagTable);
	if(dbg)	printf("L3,");

	while(currentCase!=NULL)
	{
		PR_DeleteCaseData(currentCase);
		currentCase = caseList->childNodeFirstCase;
	}
	if(dbg)	printf("L2\n");

	delete [] caseList->totalNumCandOfClass;
	delete caseList;
	if(dbg)	printf("L4\n");

	return;
}

int PR_DeleteAllCaseDataInCaseList(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return PR_AVEND;

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		PR_DeleteCaseData(caseData);
		caseData = caseList->childNodeFirstCase;
	}

	return PR_EOT;
}

int PR_CheckIfCandDataIsMemberOfCaseData(PR_CASEDATA* caseData, PR_CANDDATA* candData)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"InputCaseData=NULL(@PR_CheckIfCandDataIsMemberOfCaseData)\n");
		return PR_AVEND;
	}
	else if(candData==NULL)
	{
		fprintf(stderr,"InputCandData=NULL(@PR_CheckIfCandDataIsMemberOfCaseData)\n");
		return PR_AVEND;
	}

	int boolean = PR_AVEND;
	PR_CANDDATA* currentCand = caseData->childNodeFirstCand;
	PR_CANDDATA* previousCand = currentCand->nodePrevCand;

	while(currentCand!=NULL)
	{
		if(currentCand==candData)
		{
			boolean = PR_EOT;
			break;
		}	
		previousCand = currentCand;
		currentCand = previousCand->nodeNextCand;
	}

	return boolean;
}

int PR_CheckIfCaseDataIsMemberOfCaseList(PR_CASELIST* caseList, PR_CASEDATA* caseData)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"caseData=NULL(@PR_CheckIfCaseDataIsMemberOfCaseList)\n");
		return PR_AVEND;
	}
	else if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_CheckIfCaseDataIsMemberOfCaseList)\n");
		return PR_AVEND;
	}

	int boolean = PR_AVEND;
	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	PR_CASEDATA* previousCase = currentCase->nodePrevCase;

	while(currentCase!=NULL)
	{
		if(currentCase==caseData)
		{
			boolean = PR_EOT;
			break;
		}	
		previousCase = currentCase;
		currentCase = previousCase->nodeNextCase;
	}

	return boolean;
}

int PR_InsertCandDataToHeadOfCaseData(PR_CASEDATA* caseData, PR_CANDDATA* insertCand, char booKnownClassLabel)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"caseData=NULL(@PR_InsertCandDataToHeadOfCaseData)\n");
		return PR_AVEND;
	}
	else if(insertCand==NULL)
	{
		fprintf(stderr,"insertCandData=NULL(@PR_InsertCandDataToHeadOfCaseData)\n");
		return PR_AVEND;
	}

	if(caseData->childNodeFirstCand==NULL)
		caseData->childNodeFirstCand = caseData->childNodeLastCand = insertCand;
	else
	{
		insertCand->nodeNextCand = caseData->childNodeFirstCand;
		caseData->childNodeFirstCand->nodePrevCand = insertCand;
		caseData->childNodeFirstCand = insertCand;
	}
	caseData->numCand++;

	insertCand->rootCaseData = caseData;

	if(caseData->rootCaseList!=NULL)
	{
		PR_CASELIST* rootCase = (PR_CASELIST*)caseData->rootCaseList;
		rootCase->totalNumCand++;

		if(booKnownClassLabel)
		{
			unsigned char cls = insertCand->classLabel;
			if(cls>rootCase->numClass)	cls = rootCase->numClass;
			rootCase->totalNumCandOfClass[cls]++;
		}
	}

	return PR_EOT;
}

int PR_InsertCandDataToTailOfCaseData(PR_CASEDATA* caseData, PR_CANDDATA* insertCand, char booKnownClassLabel)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"InputCaseData=NULL(@PR_InsertCandDataToHeadOfCaseData)\n");
		return PR_AVEND;
	}
	else if(insertCand==NULL)
	{
		fprintf(stderr,"InputAddCandData=NULL(@PR_InsertCandDataToHeadOfCaseData)\n");
		return PR_AVEND;
	}

	if(caseData->childNodeLastCand==NULL)
		caseData->childNodeFirstCand = caseData->childNodeLastCand = insertCand;
	else
	{
		insertCand->nodePrevCand = caseData->childNodeLastCand;
		caseData->childNodeLastCand->nodeNextCand = insertCand;
		caseData->childNodeLastCand = insertCand;
	}
	caseData->numCand++;

	insertCand->rootCaseData = caseData;

	if(caseData->rootCaseList!=NULL)
	{
		PR_CASELIST* rootCase = (PR_CASELIST*)caseData->rootCaseList;
		rootCase->totalNumCand++;

		if(booKnownClassLabel)
		{
			unsigned char cls = insertCand->classLabel;
			if(cls>rootCase->numClass)	cls = rootCase->numClass;
			rootCase->totalNumCandOfClass[cls]++;
		}
	}

	return PR_EOT;
}

int PR_InsertCandDataToCaseData(PR_CASEDATA* caseData, PR_CANDDATA* insertCand, PR_CANDDATA* insertNextTo, char booKnownClassLabel)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"InputCaseData=NULL(@PR_InsertCandDataToCaseData)\n");
		return PR_AVEND;
	}
	else if(insertCand==NULL)
	{
		fprintf(stderr,"insertCand=NULL(@PR_InsertCandDataToCaseData)\n");
		return PR_AVEND;
	}
	else if(insertNextTo==NULL)
	{
		fprintf(stderr,"addNextTo=NULL(@PR_InsertCandDataToCaseData)\n");
		return PR_AVEND;
	}
	else if(PR_CheckIfCandDataIsMemberOfCaseData(caseData, insertNextTo)==PR_AVEND)
	{
		fprintf(stderr,"NotFindAddNextTo(@PR_NewCandDataAddToCaseData)\n");
		return PR_AVEND;
	}

	insertCand->nodePrevCand = insertNextTo;
	insertCand->nodeNextCand = insertNextTo->nodeNextCand;

	insertNextTo->nodeNextCand->nodePrevCand = insertCand;
	insertNextTo->nodeNextCand = insertCand;

	caseData->numCand++;

	insertCand->rootCaseData = caseData;

	if(caseData->rootCaseList!=NULL)
	{
		PR_CASELIST* rootCase = (PR_CASELIST*)caseData->rootCaseList;
		rootCase->totalNumCand++;

	//	if(booKnownClassLabel && insertCand->classLabel<rootCase->numClass)
	//		rootCase->totalNumCandOfClass[insertCand->classLabel]++;
		if(booKnownClassLabel)
		{
			unsigned char cls = insertCand->classLabel;
			if(cls>rootCase->numClass)	cls = rootCase->numClass;
			rootCase->totalNumCandOfClass[cls]++;
		}
	}

	return PR_EOT;
}


PR_CANDDATA* PR_NewCandDataAddToHeadOfCaseData(PR_CASEDATA* caseData)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"caseData=NULL(@PR_NewCandDataAddToHeadOfCaseData)\n");
		return NULL;
	}
	else if(caseData->rootCaseList==NULL)
	{
		fprintf(stderr,"caseData->rootCaseList=NULL(@PR_NewCandDataAddToHeadOfCaseData)\n");
		return NULL;
	}

	PR_CASELIST* rootCaseList = (PR_CASELIST*)caseData->rootCaseList;
	unsigned char numClass = rootCaseList->numClass;
	unsigned int numFeat = rootCaseList->featTagTable->numFeat;

	PR_CANDDATA* addCand = PR_NewCandData(numClass,numFeat);
	if(addCand==NULL)
	{
		fprintf(stderr,"addCand=NULL(@PR_NewCandDataAddToHeadOfCaseData)\n");
		return NULL;
	}
	PR_InsertCandDataToHeadOfCaseData(caseData, addCand, 0);

	return addCand;
}

PR_CANDDATA* PR_NewCandDataAddToTailOfCaseData(PR_CASEDATA* caseData)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"InputCaseData=NULL(@PR_NewCandDataAddToTailOfCaseData)\n");
		return NULL;
	}
	else if(caseData->rootCaseList==NULL)
	{
		fprintf(stderr,"caseData->rootCaseList=NULL(@PR_NewCandDataAddToTailOfCaseData)\n");
		return NULL;
	}

	PR_CASELIST* rootCaseList = (PR_CASELIST*)caseData->rootCaseList;
	unsigned char numClass = rootCaseList->numClass;
	unsigned int numFeat = rootCaseList->featTagTable->numFeat;

	PR_CANDDATA* addCand = PR_NewCandData(numClass,numFeat);
	if(addCand==NULL)
	{
		fprintf(stderr,"addCand=NULL(@PR_NewCandDataAddToTailOfCaseData)\n");
		return NULL;
	}
	PR_InsertCandDataToTailOfCaseData(caseData, addCand, 0);

	return addCand;
}

PR_CANDDATA* PR_NewCandDataAddToCaseData(PR_CASEDATA* caseData, PR_CANDDATA* addNextTo)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"InputCaseData=NULL(@PR_NewCandDataAddToCaseData)\n");
		return NULL;
	}
	else if(caseData->rootCaseList==NULL)
	{
		fprintf(stderr,"caseData->rootCaseList=NULL(@PR_NewCandDataAddToCaseData)\n");
		return NULL;
	}
	else if(addNextTo==NULL)
	{
		fprintf(stderr,"addNextTo=NULL(@PR_NewCandDataAddToCaseData)\n");
		return NULL;
	}
	else if(PR_CheckIfCandDataIsMemberOfCaseData(caseData, addNextTo)==PR_AVEND)
	{
		fprintf(stderr,"NotFindAddNextTo(@PR_NewCandDataAddToCaseData)\n");
		return NULL;
	}

	PR_CASELIST* rootCaseList = (PR_CASELIST*)caseData->rootCaseList;
	unsigned char numClass = rootCaseList->numClass;
	unsigned int numFeat = rootCaseList->featTagTable->numFeat;

	PR_CANDDATA* addCand = PR_NewCandData(numClass,numFeat);
	if(addCand==NULL)
	{
		fprintf(stderr,"addCand=NULL(@PR_NewCandDataAddToCaseData)\n");
		return NULL;
	}

	PR_InsertCandDataToCaseData(caseData, addCand, addNextTo, 0);

	return addCand;
}

int PR_InsertCaseDataToHeadOfCaseList(PR_CASELIST* caseList, PR_CASEDATA* insertCase)
{
	if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_InsertCaseDataToHeadOfCaseList)\n");
		return PR_AVEND;
	}
	else if(insertCase==NULL)
	{
		fprintf(stderr,"insertCase=NULL(@PR_InsertCaseDataToHeadOfCaseList)\n");
		return PR_AVEND;
	}

	insertCase->nodeNextCase = insertCase->nodePrevCase = NULL;

	if(caseList->childNodeFirstCase==NULL)
		caseList->childNodeFirstCase = caseList->childNodeLastCase = insertCase;
	else
	{
		insertCase->nodeNextCase = caseList->childNodeFirstCase;
		caseList->childNodeFirstCase->nodePrevCase = insertCase;
		caseList->childNodeFirstCase = insertCase;
	}
	caseList->numCase++;
	insertCase->rootCaseList = caseList;

	return PR_CountNumCandsForCaseList(caseList);
}

int PR_InsertCaseDataToTailOfCaseList(PR_CASELIST* caseList, PR_CASEDATA* insertCase)
{
	if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_InsertCaseDataToTailOfCaseList)\n");
		return PR_AVEND;
	}
	else if(insertCase==NULL)
	{
		fprintf(stderr,"insertCase=NULL(@PR_InsertCaseDataToTailOfCaseList)\n");
		return PR_AVEND;
	}

	if(PR_CheckIfCaseDataIsMemberOfCaseList(caseList, insertCase)!=PR_AVEND)
	{
		fprintf(stderr,"insertCase is already included\n");
		return PR_EOT;
	}

	insertCase->nodeNextCase = insertCase->nodePrevCase = NULL;

	if(caseList->childNodeFirstCase==NULL)
		caseList->childNodeFirstCase = caseList->childNodeLastCase = insertCase;
	else
	{
		insertCase->nodePrevCase = caseList->childNodeLastCase;
		caseList->childNodeLastCase->nodeNextCase = insertCase;
		caseList->childNodeLastCase = insertCase;
	}
	caseList->numCase++;
	insertCase->rootCaseList = caseList;

	return PR_CountNumCandsForCaseList(caseList);
}

int PR_InsertCaseDataToCaseList(PR_CASELIST* caseList, PR_CASEDATA* insertCase, PR_CASEDATA* incertNextTo)
{
	if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_InsertCaseDataToCaseList)\n");
		return PR_AVEND;
	}
	else if(insertCase==NULL)
	{
		fprintf(stderr,"insertCase=NULL(@PR_InsertCaseDataToCaseList)\n");
		return PR_AVEND;
	}
	else if(incertNextTo==NULL)
	{
		fprintf(stderr,"incertNextTo=NULL(@PR_InsertCaseDataToCaseList)\n");
		return PR_AVEND;
	}
	else if(PR_CheckIfCaseDataIsMemberOfCaseList(caseList, incertNextTo)==PR_AVEND)
	{
		fprintf(stderr,"NotFindAddNextTo(@PR_InsertCaseDataToCaseList)\n");
		return PR_AVEND;
	}

	insertCase->nodePrevCase = incertNextTo;
	insertCase->nodeNextCase = incertNextTo->nodeNextCase;

	if(incertNextTo->nodeNextCase!=NULL)
		incertNextTo->nodeNextCase->nodePrevCase = insertCase;
	incertNextTo->nodeNextCase = insertCase;
	
	if(caseList->childNodeLastCase == incertNextTo)
		caseList->childNodeLastCase = insertCase;

	caseList->numCase++;
	insertCase->rootCaseList = caseList;

	return PR_CountNumCandsForCaseList(caseList);
}

PR_CASEDATA* PR_NewCaseDataAddToHeadOfCaseList(PR_CASELIST* caseList)
{
	if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_NewCaseDataAddToHeadOfCaseList)\n");
		return NULL;
	}

	PR_CASEDATA* addCase = PR_NewCaseData(caseList->numClass);
	if(addCase==NULL)
	{
		fprintf(stderr,"addCase=NULL(@PR_NewCaseDataAddToHeadOfCaseList)\n");
		return NULL;
	}
	PR_InsertCaseDataToHeadOfCaseList(caseList, addCase);

	return addCase;
}

PR_CASEDATA* PR_NewCaseDataAddToTailOfCaseList(PR_CASELIST* caseList)
{
	if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_NewCaseDataAddToTailOfCaseList)\n");
		return NULL;
	}

	PR_CASEDATA* addCase = PR_NewCaseData(caseList->numClass);
	if(addCase==NULL)
	{
		fprintf(stderr,"addCase=NULL(@PR_NewCaseDataAddToTailOfCaseList)\n");
		return NULL;
	}

	PR_InsertCaseDataToTailOfCaseList(caseList, addCase);

	return addCase;
}

PR_CASEDATA* PR_NewCaseDataAddToCaseList(PR_CASELIST* caseList, PR_CASEDATA* addNextTo)
{
	if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_NewCaseDataAddToCaseList)\n");
		return NULL;
	}
	else if(addNextTo==NULL)
	{
		fprintf(stderr,"addNextTo=NULL(@PR_NewCaseDataAddToCaseList)\n");
		return NULL;
	}
	else if(PR_CheckIfCaseDataIsMemberOfCaseList(caseList, addNextTo)==PR_AVEND)
	{
		fprintf(stderr,"NotFindAddNextTo(@PR_NewCaseDataAddToCaseList)\n");
		return NULL;
	}

	PR_CASEDATA* addCase = PR_NewCaseData(caseList->numClass);
	if(addCase==NULL)
	{
		fprintf(stderr,"addCase=NULL(@PR_NewCaseDataAddToCaseList)\n");
		return NULL;
	}
	PR_InsertCaseDataToCaseList(caseList, addCase, addNextTo);

	return addCase;
}

int PR_CountNumCandsForCaseData(PR_CASEDATA* caseData, unsigned char numClass)
{
	if(caseData==NULL)
	{
		fprintf(stderr,"caseData=NULL(@PR_CountNumCandsForCaseData)\n");
		return PR_AVEND;
	}

	if(caseData->childNodeFirstCand==NULL && caseData->childNodeLastCand==NULL)	return PR_EOT;

	int ret=PR_EOT;
	caseData->numCand = 0;
	memset(caseData->numCandOfClass, 0x00, sizeof(unsigned int)*numClass);
	PR_CANDDATA* currentCand = caseData->childNodeFirstCand;
	PR_CANDDATA* beforeCand = currentCand->nodePrevCand;

	if(beforeCand!=NULL)
	{
		ret = PR_EOT;
		fprintf(stderr,"caseData->childNodeFirstCand is not first\n");

		while(beforeCand!=NULL)
		{
			currentCand = beforeCand;
			beforeCand = currentCand->nodePrevCand;
		}

		caseData->childNodeFirstCand = currentCand;
	}

	while(currentCand!=NULL)
	{
		unsigned char cls = currentCand->classLabel;
		if(cls>numClass)	cls=numClass;
		
		caseData->numCand++;
		caseData->numCandOfClass[cls]++;

		beforeCand = currentCand;
		currentCand = beforeCand->nodeNextCand;
	}

	if(beforeCand!=caseData->childNodeLastCand)
	{
		ret = PR_EOT;
		fprintf(stderr,"caseData->childNodeLastCand is not tail\n");
		caseData->childNodeLastCand = beforeCand;
	}

	return ret;
}

int PR_CountNumCandsForCaseList(PR_CASELIST* caseList)
{
	if(caseList==NULL)
	{
		fprintf(stderr,"caseList=NULL(@PR_CountNumCandsForCaseList)\n");
		return PR_AVEND;
	}

	if(caseList->childNodeFirstCase==NULL && caseList->childNodeLastCase==NULL)		return PR_EOT;

	int res = PR_EOT;
	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	PR_CASEDATA* beforeCase = currentCase->nodePrevCase;

	if(beforeCase!=NULL)
	{
		res = PR_EOT;
		fprintf(stderr,"caseList->childNodeFirstCase is not first\n");
		while(beforeCase!=NULL)
		{
			currentCase = beforeCase;
			beforeCase = currentCase->nodePrevCase;
		}
		caseList->childNodeFirstCase = currentCase;
	}

	caseList->totalNumCand = 0;
	memset(caseList->totalNumCandOfClass,0x00,sizeof(unsigned long)*(caseList->numClass+1));

	while(res!=PR_AVEND && currentCase!=NULL)
	{
		int dmyRes = PR_CountNumCandsForCaseData(currentCase, caseList->numClass);
		if(res>dmyRes)	res = dmyRes;

		caseList->totalNumCand += currentCase->numCand;
		for(unsigned int cls=0; cls<=caseList->numClass; cls++)
			caseList->totalNumCandOfClass[cls] += (unsigned long)currentCase->numCandOfClass[cls];
		
		beforeCase = currentCase;
		currentCase = beforeCase->nodeNextCase;
	}

	if(res!=PR_AVEND && beforeCase!=caseList->childNodeLastCase)
	{
		res = PR_EOT;
		fprintf(stderr,"caseList->childNodeLastCase is not tail\n");
		caseList->childNodeLastCase = beforeCase;
	}

	return res;
}


int PR_PullOutCandDataFromCaseData(PR_CASEDATA* caseData, PR_CANDDATA* candData)
{
	if(caseData==NULL || candData==NULL)	return PR_AVEND;

	if((PR_CheckIfCandDataIsMemberOfCaseData(caseData, candData))==PR_AVEND)	return PR_AVEND;

	PR_CANDDATA* prevCand = candData->nodePrevCand;
	PR_CANDDATA* nextCand = candData->nodeNextCand;

	if(prevCand!=NULL)	prevCand->nodeNextCand = nextCand;
	if(nextCand!=NULL)	nextCand->nodePrevCand = prevCand;

	if(caseData->childNodeFirstCand==candData)	caseData->childNodeFirstCand = nextCand;
	if(caseData->childNodeLastCand==candData)	caseData->childNodeLastCand = prevCand;

	caseData->numCand--;
	unsigned char cls=candData->classLabel;
	
	if(caseData->rootCaseList!=NULL)
	{
		PR_CASELIST* rootList = (PR_CASELIST*)caseData->rootCaseList;
		if(cls>rootList->numClass)	cls = rootList->numClass;
		caseData->numCandOfClass[cls]--;
		rootList->totalNumCand--;
		rootList->totalNumCandOfClass[cls]--;
	}

	candData->nodeNextCand = candData->nodePrevCand = NULL;
	candData->rootCaseData = NULL;

	return PR_EOT;
}

int PR_PullOutCaseDataFromCaseList(PR_CASELIST* caseList, PR_CASEDATA* caseData)
{
	if(caseList==NULL || caseData==NULL)	return PR_AVEND;

	if((PR_CheckIfCaseDataIsMemberOfCaseList(caseList, caseData))==PR_AVEND)	return PR_AVEND;

	PR_CASEDATA* prevCase = caseData->nodePrevCase;
	PR_CASEDATA* nextCase = caseData->nodeNextCase;

	if(prevCase!=NULL)	prevCase->nodeNextCase = nextCase;
	if(nextCase!=NULL)	nextCase->nodePrevCase = prevCase;

	if(caseList->childNodeFirstCase==caseData)	caseList->childNodeFirstCase = nextCase;
	if(caseList->childNodeLastCase==caseData)	caseList->childNodeLastCase = prevCase;

	caseList->numCase--;
	caseList->totalNumCand -= (unsigned long)caseData->numCand;
	for(unsigned char ii=0; ii<=caseList->numClass; ii++)
		caseList->totalNumCandOfClass[ii] -= (unsigned long)caseData->numCandOfClass[ii];

	caseData->nodeNextCase = caseData->nodePrevCase = NULL;
	caseData->rootCaseList = NULL;

	return PR_EOT;
}

int PR_RemakeFeatValueArrayOfCaseList(PR_CASELIST* caseList, PR_FEATTAGTABLE* newTable)
{
	unsigned int ii, jj;

	// ckeck input data
	if(caseList==NULL || newTable==NULL)	return PR_AVEND;
	if(caseList->featTagTable->numFeat < newTable->numFeat)		return PR_AVEND;
	if(PR_CompareFeatTagTable(caseList->featTagTable, newTable)==PR_EOT)	return PR_EOT;
	
	// find relation between old-table and new-table
	unsigned int fnumOld = caseList->featTagTable->numFeat;
	unsigned int fnumNew = newTable->numFeat;
	unsigned int* idxFeat = new unsigned int [fnumNew];
	printf("old%d->new%d\n",fnumOld,fnumNew);

	for(jj=0; jj<fnumNew; jj++)
	{
		idxFeat[jj]=fnumOld;

		for(ii=0; ii<fnumOld; ii++)
		if(PR_CompareFeatTag(newTable->tags[jj], caseList->featTagTable->tags[ii])==PR_EOT)
		{
			idxFeat[jj]=ii;
			break;
		}

		if(idxFeat[jj]==fnumOld)
		{
			fprintf(stderr,"unfound feat [%04x %04x]...\n",
							newTable->tags[jj]->group,newTable->tags[jj]->element);
			delete [] idxFeat;
			return PR_AVEND;
		}
	}


	// rewrite "candData->featValues"
	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;
	
	float* dmyFeatAry = new float [fnumOld];

	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		printf("caseID%d..",caseData->idCase);

		while(candData!=NULL)
		{
			memcpy(dmyFeatAry, candData->featValues, sizeof(float)*fnumOld);

			delete [] candData->featValues;
			candData->featValues = new float [fnumNew];

			for(jj=0; jj<fnumNew; jj++)
				candData->featValues[jj] = dmyFeatAry[idxFeat[jj]];
 
			candData = candData->nodeNextCand;
		}
	
		printf("complete\r");
		caseData = caseData->nodeNextCase;
	}

	// rewrite "caseList->featTagTable"
	PR_DeleteFeatTagTable(caseList->featTagTable);
	caseList->featTagTable = PR_NewFeatTagTable(fnumNew);
	PR_CopyFeatTagTable(caseList->featTagTable, newTable);

	// memory free
	delete [] idxFeat;
	delete [] dmyFeatAry;

	return PR_EOT;
}

int PR_PullOutAllCaseDataInCaseList(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return PR_AVEND;

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		PR_PullOutCaseDataFromCaseList(caseList, caseData);
		caseData = caseList->childNodeFirstCase;
	}

	return PR_EOT;
}


PR_CASELIST* PR_DuplicateCaseList(PR_CASELIST* source)
{
	PR_CASELIST* dest = PR_NewCaseList(source->numClass,source->featTagTable->numFeat);
	PR_CopyFeatTagTable(dest->featTagTable,source->featTagTable);

	PR_CASEDATA* sCase = source->childNodeFirstCase;
	while(sCase!=NULL)
	{
		PR_CASEDATA* dCase = PR_NewCaseDataAddToTailOfCaseList(dest);
		dCase->idCase = sCase->idCase;
		PR_CANDDATA* sCand = sCase->childNodeFirstCand;

		while(sCand!=NULL)
		{
			PR_CANDDATA* dCand = PR_NewCandDataAddToTailOfCaseData(dCase);
			dCand->booOutlier = sCand->booOutlier;
			dCand->classLabel = sCand->classLabel;
			memcpy(dCand->featValues,sCand->featValues,sizeof(float)*dest->featTagTable->numFeat);
			dCand->idCand = sCand->idCand;
			sCand = sCand->nodeNextCand;
		}

		sCase->nodeNextCase;
	}

	return dest;
}



PR_TRAINCANDARRAY* PR_NewTrainCandArray(unsigned char numClass, unsigned int numCand)
{
	PR_TRAINCANDARRAY* cands = new PR_TRAINCANDARRAY;
	if(cands==NULL)		return (PR_TRAINCANDARRAY*)PR_ErrorMessageNULL("ahoaho");

	cands->data = new PR_CANDDATA* [numCand];
	for(unsigned int i=0; i<numCand; i++)	cands->data[i]=NULL;

	cands->numClass = numClass;
	cands->numData = numCand;
	
	cands->numDataOfClass = new unsigned int [numClass];
	memset(cands->numDataOfClass, 0x00, sizeof(unsigned int)*numClass);
	
	return cands;
}

void PR_DeleteTrainCandArray(PR_TRAINCANDARRAY* cands)
{
	if(cands==NULL)	return;

	delete [] cands->numDataOfClass;
	delete [] cands->data;
	delete cands;
}

