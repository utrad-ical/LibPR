

#pragma warning(disable: 4996)

#include "./prThresholdingForFeats.h"
#include "./prThresholdingForFeats.private.h"

#include "../LibPR.h"
#include "./prCandDataSet.h"
#include "./prErrorMessage.h"
#include "./prSortTools.h"


PR_THRESHOLD_FORFEAT* PR_NewThreshiodForFeat()
{
	PR_THRESHOLD_FORFEAT* thdForFeat = new PR_THRESHOLD_FORFEAT;

	thdForFeat->featTag = PR_NewFeatTag();
	thdForFeat->mode = 0;
	thdForFeat->threshold = 0.0f;
	thdForFeat->ratRemoveFP = 0.0;
	thdForFeat->ratMisdeleteTP = 0.0;

	return thdForFeat;
}

void PR_DeleteThreshiodForFeat(PR_THRESHOLD_FORFEAT* thdForFeat)
{
	if(thdForFeat==NULL)	return;

	PR_DeleteFeatTag(thdForFeat->featTag);
	delete thdForFeat;
}


PR_SUBCANDGROUP* PR_NewSubCandGroup(
unsigned int idCase,
unsigned int idCand,
unsigned int numCand
){
	PR_SUBCANDGROUP* subGroup = new PR_SUBCANDGROUP;
	if(subGroup==NULL)
		return (PR_SUBCANDGROUP*)PR_ErrorMessageNULL("error PR_NewSubGroupSameIdCand");

	subGroup->idCase = idCase;
	subGroup->idCand = idCand;
	subGroup->numCand = numCand;
	subGroup->candPt = new PR_CANDDATA* [numCand];
//	subGroup->featValue = new float [numCand];
//	memset(subGroup->featValue,0x00,sizeof(float)*numCand);
	subGroup->nodePrev = NULL;
	subGroup->nodeNext = NULL;

	return subGroup;
}

void PR_DeleteSubCandGroup(PR_SUBCANDGROUP* subGroup)
{
	if(subGroup==NULL)	return;

	PR_SUBCANDGROUPLIST* rootList = (PR_SUBCANDGROUPLIST*)(subGroup->rootList);
	PR_SUBCANDGROUP* dmyPrev = subGroup->nodePrev;
	PR_SUBCANDGROUP* dmyNext = subGroup->nodeNext;

	if(dmyPrev!=NULL)	dmyPrev->nodeNext = dmyNext;
	if(dmyNext!=NULL)	dmyNext->nodePrev = dmyPrev;

	if(rootList!=NULL && rootList->firstNode==subGroup)		rootList->firstNode = dmyNext;
	if(rootList!=NULL && rootList->lastNode==subGroup)		rootList->lastNode = dmyPrev;

	delete [] subGroup->candPt;
	delete subGroup;
}

PR_SUBCANDGROUPLIST* PR_NewSubCandGroupList()
{
	PR_SUBCANDGROUPLIST* groupList = new PR_SUBCANDGROUPLIST;
	groupList->numNode = 0;
	groupList->firstNode = NULL;
	groupList->lastNode = NULL;
	return groupList;
}

void PR_DeleteSubCandGroupList(PR_SUBCANDGROUPLIST* groupList)
{
	if(groupList==NULL)	return;

	while(groupList->firstNode!=NULL)
		PR_DeleteSubCandGroup(groupList->lastNode);

	delete groupList;
}

int PR_AddSubCandGroupToListLast(PR_SUBCANDGROUPLIST* groupList, PR_SUBCANDGROUP* group)
{
	if(groupList==NULL || group==NULL)
		PR_ErrorMessageInt("error PR_NewSubCandGroupAddToList");

	PR_SUBCANDGROUP* dmyLast = groupList->lastNode;
	if(dmyLast==NULL)
	{
		groupList->firstNode = group;
		groupList->lastNode = group;
	}
	else
	{
		group->nodePrev = dmyLast;
		dmyLast->nodeNext = group;
		groupList->lastNode = group;
	}

	group->rootList = groupList;
	groupList->numNode++;

	return PR_EOT;
}

PR_SUBCANDGROUP* PR_SearchSubCandGroupInList(
PR_SUBCANDGROUPLIST* groupList,
unsigned int idCase,
unsigned int idCand
){
	if(groupList==NULL)	
		return (PR_SUBCANDGROUP*)PR_ErrorMessageNULL("unknown SubCandGroupList");

	PR_SUBCANDGROUP* current = groupList->firstNode;
	PR_SUBCANDGROUP* dmy;

	while(current!=NULL)
	{
		if(current->idCase==idCase && current->idCand==idCand)	return current;
		dmy = current;
		current = dmy->nodeNext;
	}

	return NULL;
}


PR_THRESHOLD_FORFEAT* PR_DetectOutlierCandsByThresholdingForFeats(
PR_CASELIST* caseList,
double ensuredRatioTP,
double ensuredRatioAreaOfTP
){
	fprintf(stderr,"\nPR_DetectOutlierCandsByThresholdingForFeats\n");

	unsigned int ii, jj, kk;
	PR_CASEDATA *currentCase, *dmyCase;
	PR_CANDDATA *currentCand, *dmyCand;

	PR_SUBCANDGROUPLIST* candGroupList = PR_NewSubCandGroupList();

	unsigned int* countOfCaseID;
	currentCase = caseList->childNodeFirstCase;

	while(currentCase!=NULL)
	{
		unsigned int numObj = currentCase->numCand+1;
		countOfCaseID = new unsigned int [numObj];
		memset(countOfCaseID,0x00,sizeof(unsigned int)*numObj);

		currentCand = currentCase->childNodeFirstCand;
		while(currentCand!=NULL)
		{
	//		fprintf(stderr,"case%d, cand%d\r",currentCase->idCase,currentCand->idCand);
			if(currentCand->classLabel!=0)
			{
				if(currentCand->idCand > currentCase->numCand)
				{
					delete [] countOfCaseID;
					return (PR_THRESHOLD_FORFEAT*)PR_ErrorMessageNULL("error candID");
				}
				countOfCaseID[currentCand->idCand]++;
			}
			dmyCand = currentCand;
			currentCand = dmyCand->nodeNextCand;
		}

		for(jj=0; jj<numObj; jj++)
		if(countOfCaseID[jj]>1)
		{
	//		fprintf(stderr,"case%d, TpObject#.%d\n",currentCase->idCase,jj);
			PR_SUBCANDGROUP* dmyGroup = PR_NewSubCandGroup(currentCase->idCase, jj, countOfCaseID[jj]);
			PR_AddSubCandGroupToListLast(candGroupList, dmyGroup);
			
			ii=0;
			currentCand = currentCase->childNodeFirstCand;
			while(currentCand!=NULL)
			{
				if(currentCand->idCand==jj)		dmyGroup->candPt[ii++]=currentCand;
				dmyCand = currentCand;
				currentCand = dmyCand->nodeNextCand;
			}
		}

		dmyCase = currentCase;
		currentCase = dmyCase->nodeNextCase;
		delete [] countOfCaseID;
	}
	//fprintf(stderr,"aaa\n\n");
	

	PR_THRESHOLD_FORFEAT* thresholdData = PR_NewThreshiodForFeat();

	PR_SUBCANDGROUP *candGroup, *dmyGroup;
	unsigned int numTp;
	float* featTpAsc = new float [caseList->totalNumCand - caseList->totalNumCandOfClass[0]*9/10];
	float* featTpDes = new float [caseList->totalNumCand - caseList->totalNumCandOfClass[0]*9/10];

	for(kk=0; kk<caseList->featTagTable->numFeat; kk++)
	{
	//	if(kk==7)
	//	fprintf(stderr,"\n%02d feat[%04x.%04x]\n",
	//					kk,caseList->featTagTable->tags[kk]->group,caseList->featTagTable->tags[kk]->element);

		// set feature array of Tp (#{element of an idCand}=1)
		numTp=0;
		currentCase = caseList->childNodeFirstCase;
		while(currentCase!=NULL)
		{
			currentCand = currentCase->childNodeFirstCand;
			while(currentCand!=NULL)
			{
				if(currentCand->classLabel!=0)
				{
	//				fprintf(stderr,"case%d, cand#.%d, #tp%d\r",currentCase->idCase,currentCand->idCand,numTp);

					unsigned int idCase = currentCase->idCase;
					unsigned int idCand = currentCand->idCand;
					if((PR_SearchSubCandGroupInList(candGroupList, idCase, idCand))==NULL)
					{
						featTpAsc[numTp] = currentCand->featValues[kk];
						featTpDes[numTp] = currentCand->featValues[kk];
						numTp++;
					}
				}
				dmyCand = currentCand;
				currentCand = dmyCand->nodeNextCand;
			}
			dmyCase = currentCase;
			currentCase = dmyCase->nodeNextCase;
		}


		// set feature array of Tp (#{element of an idCand}>=2)
		candGroup = candGroupList->firstNode;
		while(candGroup!=NULL)
		{
			float* dmyFeat = new float [candGroup->numCand];
			unsigned int numNonOutlier=0, numOutlier;

			for(jj=0; jj<candGroup->numCand; jj++)
			if(candGroup->candPt[jj]->booOutlier==0)
			{
				dmyFeat[numNonOutlier] = candGroup->candPt[jj]->featValues[kk];
				numNonOutlier++;
			}
			numOutlier = candGroup->numCand-numNonOutlier;

			PR_QuickSortAscending(dmyFeat, numNonOutlier);
			unsigned int numMaxFNPiece = (unsigned int)((double)candGroup->numCand*(1.0-ensuredRatioAreaOfTP));

			if(numMaxFNPiece>numOutlier)	numMaxFNPiece -= numOutlier;
			else							numMaxFNPiece = 0;

			featTpAsc[numTp] = dmyFeat[numMaxFNPiece];
			featTpDes[numTp] = dmyFeat[numNonOutlier-1-numMaxFNPiece];
		//	featTpAsc[numTp] = dmyFeat[numNonOutlier-1-numMaxFNPiece];
		//	featTpDes[numTp] = dmyFeat[numMaxFNPiece];

		/*	if(kk==7)
			{
				printf("#%02d [%d,%d]; sumNumCand=%d, nonOutlier=%d, numMaxFNp=%d, ",
						numTp, candGroup->idCase,candGroup->idCand,
						candGroup->numCand, numNonOutlier, numMaxFNPiece);
				fprintf(stderr,"min%e, max%e\n",featTpAsc[numTp],featTpDes[numTp]);
			//	for(jj=0; jj<numNonOutlier; jj++)
			//	{
			//		fprintf(stderr,"%e, ",dmyFeat[jj]);
			//	//	if(jj>10)	break;
			//	}
			//	getchar();
			}*/
			
			/*
			PR_QuickSortAscending(dmyFeat, candGroup->numCand);
			unsigned int numMaxFNPiece = (unsigned int)((double)candGroup->numCand*(1.0-ensuredRatioAreaOfTP));
			featTpAsc[numTp] = dmyFeat[numMaxFNPiece];
			featTpDes[numTp] = dmyFeat[candGroup->numCand-1-numMaxFNPiece];
			fprintf(stderr,"#%d ascend:%e, descend:%e\n",numTp,featTpAsc[numTp],featTpDes[numTp]);
			*/
		
			numTp++;
			delete [] dmyFeat;
			dmyGroup = candGroup;
			candGroup = dmyGroup->nodeNext;
		}


		// obtain threshold values (high/low-pass) 
		PR_QuickSortAscending(featTpAsc, numTp);
	//	fprintf(stderr,"qsort_ascend...");
		PR_QuickSortDescending(featTpDes, numTp);
	//	fprintf(stderr,"qsort_descend...\r");

		unsigned int numMaxFN = (unsigned int)((double)numTp*(1.0-ensuredRatioTP));
		float thdHighPass, thdLowPass;
		if(numMaxFN>0)
		{
			ii=1;
			while(ii<numMaxFN && featTpAsc[numMaxFN]==featTpAsc[numMaxFN-ii])	ii++;
			thdHighPass = (featTpAsc[numMaxFN-1]+featTpAsc[numMaxFN])/2.0f;
			thdLowPass = (featTpDes[numMaxFN-1]+featTpDes[numMaxFN])/2.0f;
		}
		else
		{
			ii=1;
			while(ii<numTp-1 && featTpAsc[0]==featTpAsc[ii])	ii++;
			thdHighPass = featTpAsc[0]-fabs(featTpAsc[0]-featTpAsc[ii])/2.0f;
			thdLowPass = featTpDes[0]+fabs(featTpDes[0]-featTpDes[ii])/2.0f;
		}
	//	fprintf(stderr,"thdHighPass:%e, thdLowPass:%e\n",thdHighPass,thdLowPass);


		// count #{Tn candidates}
		unsigned long cntTnLowPass=0, cntTnHighPass=0;

		currentCase = caseList->childNodeFirstCase;
		while(currentCase!=NULL)
		{
			currentCand = currentCase->childNodeFirstCand;
			while(currentCand!=NULL)
			{
				if(currentCand->classLabel==0 && currentCand->booOutlier==0)
				{
					if(thdLowPass<currentCand->featValues[kk])		cntTnLowPass++;
					if(thdHighPass>currentCand->featValues[kk])		cntTnHighPass++;
				}
				dmyCand = currentCand;
				currentCand = dmyCand->nodeNextCand;
			}
			dmyCase = currentCase;
			currentCase = dmyCase->nodeNextCase;
		}
	//	fprintf(stderr,"cntTnHighPass:%d, cntTnLowPass:%d\n",cntTnHighPass,cntTnLowPass);


		// 
		double removeFprLowPass = (double)cntTnLowPass/(double)caseList->totalNumCandOfClass[0];
		double removeFprHighPass = (double)cntTnHighPass/(double)caseList->totalNumCandOfClass[0];
		char booMod=0;
		
		if(thresholdData->ratRemoveFP<removeFprLowPass)
		{
			PR_CopyFeatTag(thresholdData->featTag, caseList->featTagTable->tags[kk]);
			thresholdData->mode = -1;
			thresholdData->ratMisdeleteTP = (double)numMaxFN/(double)numTp;
			thresholdData->ratRemoveFP = removeFprLowPass;
			thresholdData->threshold = thdLowPass;
			booMod=1;
		}
		if(thresholdData->ratRemoveFP<removeFprHighPass)
		{
			PR_CopyFeatTag(thresholdData->featTag, caseList->featTagTable->tags[kk]);
			thresholdData->mode = +1;
			thresholdData->ratMisdeleteTP = (double)numMaxFN/(double)numTp;
			thresholdData->ratRemoveFP = removeFprHighPass;
			thresholdData->threshold = thdHighPass;
			booMod=1;
		}

		if(booMod)
		{
			fprintf(stderr,"[%04x %04x] (%+d); FNR=%e, TNR=%e, thd=%e\n",
							thresholdData->featTag->group, thresholdData->featTag->element,
							thresholdData->mode, thresholdData->ratMisdeleteTP, thresholdData->ratRemoveFP,
							thresholdData->threshold);
		//	getchar();
		}
	}


	// detect outliers and change booleans of outliers
	char mode = thresholdData->mode;
	float thd = thresholdData->threshold;
	unsigned int feat;
	PR_CheckIfFeatTagIsMemberOfFeatTagTable(thresholdData->featTag, caseList->featTagTable, &feat);

	unsigned int cntOutFP=0, cntOutTP=0;

	currentCase = caseList->childNodeFirstCase;
	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;
		while(currentCand!=NULL)
		{
			if(currentCand->booOutlier==0)
			if((mode==+1 && currentCand->featValues[feat]<thd) || (mode==-1 && currentCand->featValues[feat]>thd))
			{
				currentCand->booOutlier=1;
				if(currentCand->classLabel)	cntOutTP++;
				else						cntOutFP++;
			}
			dmyCand = currentCand;
			currentCand = dmyCand->nodeNextCand;
		}
		dmyCase = currentCase;
		currentCase = dmyCase->nodeNextCase;
	}

	fprintf(stderr,"\n[%04x %04x] (%+d); FNR=%e, TNR=%e, thd=%e\n",
					thresholdData->featTag->group, thresholdData->featTag->element,
					thresholdData->mode, thresholdData->ratMisdeleteTP, thresholdData->ratRemoveFP,
					thresholdData->threshold);
	fprintf(stderr,"#{outlier} TP=%d, FP=%d\n\n",cntOutTP,cntOutFP);

	PR_DeleteSubCandGroupList(candGroupList);
	delete [] featTpAsc;
	delete [] featTpDes;

	return thresholdData;
}

