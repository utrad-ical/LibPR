
#pragma warning(disable: 4996)

#include "prHistogram.h"
#include "prHistogram.private.h"

#include "../LibPR.h"


PR_PHISTODATA* 
PR_NewPHistoData(unsigned int numDiv)
{
	PR_PHISTODATA* data = new PR_PHISTODATA;

	data->numData = 0;
	data->pOver = data->pUnder = 0.0;
	data->prob = new double [numDiv];
	memset(data->prob,0x00,sizeof(double)*numDiv);
	
	return data;
}

void
PR_DeletePHistoData(PR_PHISTODATA* data)
{
	delete [] data->prob;
	delete data;
}

PR_HISTOTABLE* 
PR_NewHistoTable(unsigned char numClass, unsigned int numDiv)
{
	PR_HISTOTABLE* table = new PR_HISTOTABLE;

	table->feat = PR_NewFeatTag();
	table->numClass = numClass;
	table->numDiv = numDiv;
	table->pHisto = new PR_PHISTODATA* [numClass];
	table->range = new double [numDiv+1];

	for(unsigned char ii=0; ii<numClass; ii++)
	{
		table->pHisto[ii] = PR_NewPHistoData(numDiv);
		table->pHisto[ii]->root = table;
	}

	return table;
}

void 
PR_DeleteHistoTable(PR_HISTOTABLE* table)
{
	PR_DeleteFeatTag(table->feat);
	delete [] table->range;
	for(unsigned char ii=0; ii<table->numClass; ii++)
		PR_DeletePHistoData(table->pHisto[ii]);
	delete [] table->pHisto;

	delete table;
}

int PR_SaveHistoTable(PR_HISTOTABLE* table, char* filename)
{
	FILE* fp = fopen(filename,"w");
	fprintf(fp,"feat,[%04x.%04x]\n",table->feat->group,table->feat->element);

	fprintf(fp,"range,,");
	for(unsigned char cls=0; cls<table->numClass; cls++)
		fprintf(fp,"cls[%d],",cls);
	fprintf(fp,"\n");

	fprintf(fp,"under,,");
	for(unsigned char cls=0; cls<table->numClass; cls++)
		fprintf(fp,"%e,",table->pHisto[cls]->pUnder);
	fprintf(fp,"\n");

	for(unsigned int div=0; div<table->numDiv; div++)
	{
		fprintf(fp,"%e,%e,",table->range[div],table->range[div+1]);
		for(unsigned char cls=0; cls<table->numClass; cls++)
			fprintf(fp,"%e,",table->pHisto[cls]->prob[div]);
		fprintf(fp,"\n");
	}

	fprintf(fp,"over,,");
	for(unsigned char cls=0; cls<table->numClass; cls++)
		fprintf(fp,"%e,",table->pHisto[cls]->pUnder);
	fprintf(fp,"\n");

	fclose(fp);

	return PR_EOT;
}


PR_HISTOTABLE*
PR_CalculateFeatHistograms(PR_CASELIST* caseList, unsigned int target, unsigned int numDiv, double outRat)
{
//	printf("in,");

	PR_CountNumCandsForCaseList(caseList);

	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;


	// decide min & max values of holizontal axis for histogram 

	double minValue = (double)caseList->childNodeFirstCase->childNodeFirstCand->featValues[target];
	double maxValue = (double)caseList->childNodeFirstCase->childNodeFirstCand->featValues[target];
	double minDmy=minValue, maxDmy=maxValue;

	double* avg = new double [caseList->numClass];
	double* dev = new double [caseList->numClass];

	unsigned char cls, maxCls=0;
	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			cls = candData->classLabel;
			avg[cls]+=(double)candData->featValues[target];
			if(minDmy>(double)candData->featValues[target])		minDmy=(double)candData->featValues[target];
			if(maxDmy<(double)candData->featValues[target])		maxDmy=(double)candData->featValues[target];
			if(maxCls<cls)		maxCls=cls;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}

	for(cls=0; cls<=maxCls; cls++)
		avg[cls]/=(double)caseList->totalNumCandOfClass[cls];

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			cls = candData->classLabel;
			dev[cls]+=((double)candData->featValues[target]-avg[cls])*((double)candData->featValues[target]-avg[cls]);
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}

	for(cls=0; cls<=maxCls; cls++)
	{
		dev[cls]=sqrt(dev[cls]/(double)caseList->totalNumCandOfClass[cls]);

		if(minValue>avg[cls]-dev[cls]*outRat)	minValue=avg[cls]-dev[cls]*outRat;
		if(maxValue<avg[cls]+dev[cls]*outRat)	maxValue=avg[cls]+dev[cls]*outRat;
	}

//	if(minValue<minDmy)		minValue=minDmy;
//	if(maxValue>maxDmy)		maxValue=maxDmy;

//	printf("rangeHisto[%e,%e],",minValue,maxValue);


/*	// count class num
	unsigned char maxCls=0;
	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			if(candData->classLabel)
			{
				if(minFeatValueTP>candData->featValues[target])		minFeatValueTP = candData->featValues[target];
				if(maxFeatValueTP<candData->featValues[target])		maxFeatValueTP = candData->featValues[target];
			}

			if(maxCls<candData->classLabel)		maxCls=candData->classLabel;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
	printf("tp(%e~%e),",minFeatValueTP,maxFeatValueTP);
	printf("cls%d,",maxCls);
*/

	// count cand (all / each class)
	unsigned char numCls = maxCls+1;
/*
	unsigned int numCandAll=0;
	unsigned int* numCandInCls = new unsigned int [numCls];
	memset(numCandInCls,0x00,sizeof(unsigned int)*numCls);

	for(cls=0; cls<=maxCls; cls++)
		numCandInCls[cls]=caseList->totalNumCandOfClass[cls];
/*
	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			numCandAll++;
			numCandInCls[candData->classLabel]++;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
	printf("cnt,");
*/

/*
	// copy target feat
	unsigned int cnt=0;
	float* dmyAry = new float [numCandAll];
	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			dmyAry[cnt++] = candData->featValues[target];
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
	printf("y");


	// sort
	PR_QuickSortAscending(dmyAry, numCandAll);
	printf("x");


	// cutting-out num 
	unsigned int numOut = (unsigned int)((double)numCandAll*outRat+0.5);
*/

	// make histogram
	PR_HISTOTABLE* histoTable = PR_NewHistoTable(numCls, numDiv);
//	printf("table,");

	PR_CopyFeatTag(histoTable->feat, caseList->featTagTable->tags[target]);

/*
	histoTable->range[0] = (double)dmyAry[numOut/2];
	histoTable->range[numDiv] = (double)dmyAry[numCandAll-1-numOut/2];
*/
	double unit = (maxValue-minValue)/(double)numDiv;
	histoTable->range[0] = minValue;
	for(unsigned int ii=1; ii<numDiv; ii++)	histoTable->range[ii] = minValue+unit*(double)ii;
	histoTable->range[numDiv] = maxValue;
//	printf("range,");

/*
	histoTable->range[0] = (double)(minFeatValueTP-marginHistoRange);
	histoTable->range[numDiv] = (double)(maxFeatValueTP+marginHistoRange);

	double widElem = (histoTable->range[numDiv]-histoTable->range[0])/(double)numDiv;
	for(unsigned int ii=1; ii<numDiv; ii++)
		histoTable->range[ii] = histoTable->range[ii-1]+widElem;
*/
//	double* unit = new double [numCls];
//	for(unsigned char ii=0; ii<numCls; ii++)
//	{
//		histoTable->pHisto[ii]->numData = numCandInCls[ii];
//		unit[ii] = 1.0/(double)numCandInCls[ii];
//	}
//	printf("unit,");

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			unsigned char cls = candData->classLabel;
			double value = (double)candData->featValues[target];

			if(value<minValue)
				histoTable->pHisto[cls]->pUnder += 1.0;
			else if(value>=maxValue)
				histoTable->pHisto[cls]->pOver += 1.0;
			else
			{
				unsigned int idx = (unsigned int)((value-minValue)/unit);
				if(idx<numDiv)	histoTable->pHisto[cls]->prob[idx] += 1.0;
				else			histoTable->pHisto[cls]->pOver += 1.0;
			}
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
//	printf("count,");


	for(cls=0; cls<=maxCls; cls++)
	{
		histoTable->pHisto[cls]->numData = caseList->totalNumCandOfClass[cls];

		histoTable->pHisto[cls]->pUnder /= (double)histoTable->pHisto[cls]->numData;
		histoTable->pHisto[cls]->pOver /= (double)histoTable->pHisto[cls]->numData;
		for(unsigned int ii=0; ii<numDiv; ii++)
			histoTable->pHisto[cls]->prob[ii] /= (double)histoTable->pHisto[cls]->numData;
	}
//	printf("calcProb\n");


//	delete [] numCandInCls;
//	delete [] dmyAry;
//	delete [] unit;

	return histoTable;
}


PR_HISTOTABLE*
PR_CalculateFeatHistograms02(
	PR_CASELIST* caseList, unsigned int target, unsigned int numDiv, double minValue, double maxValue)
{
//	printf("in,");

	PR_CountNumCandsForCaseList(caseList);

	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;


	// decide min & max values of holizontal axis for histogram 
//	double minValue = (double)caseList->childNodeFirstCase->childNodeFirstCand->featValues[target];
//	double maxValue = (double)caseList->childNodeFirstCase->childNodeFirstCand->featValues[target];
//	double minDmy=minValue, maxDmy=maxValue;

//	double* avg = new double [caseList->numClass];
//	double* dev = new double [caseList->numClass];

	unsigned char cls, maxCls=0;
	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			cls = candData->classLabel;
//			avg[cls]+=(double)candData->featValues[target];
//			if(minDmy>(double)candData->featValues[target])		minDmy=(double)candData->featValues[target];
//			if(maxDmy<(double)candData->featValues[target])		maxDmy=(double)candData->featValues[target];
			if(maxCls<cls)		maxCls=cls;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
/*
	for(cls=0; cls<=maxCls; cls++)
		avg[cls]/=(double)caseList->totalNumCandOfClass[cls];

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			cls = candData->classLabel;
			dev[cls]+=((double)candData->featValues[target]-avg[cls])*((double)candData->featValues[target]-avg[cls]);
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}

	for(cls=0; cls<=maxCls; cls++)
	{
		dev[cls]=sqrt(dev[cls]/(double)caseList->totalNumCandOfClass[cls]);

		if(minValue>avg[cls]-dev[cls]*outRat)	minValue=avg[cls]-dev[cls]*outRat;
		if(maxValue<avg[cls]+dev[cls]*outRat)	maxValue=avg[cls]+dev[cls]*outRat;
	}

	if(minValue<minDmy)		minValue=minDmy;
	if(maxValue>maxDmy)		maxValue=maxDmy;
*/

	// count cand (all / each class)
	unsigned char numCls = maxCls+1;


	// make histogram
	PR_HISTOTABLE* histoTable = PR_NewHistoTable(numCls, numDiv);
//	printf("table,");

	PR_CopyFeatTag(histoTable->feat, caseList->featTagTable->tags[target]);

	double unit = (maxValue-minValue)/(double)numDiv;
	histoTable->range[0] = minValue;
	for(unsigned int ii=1; ii<numDiv; ii++)	histoTable->range[ii] = minValue+unit*(double)ii;
	histoTable->range[numDiv] = maxValue;
//	printf("range,");

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			unsigned char cls = candData->classLabel;
			double value = (double)candData->featValues[target];

			if(value<minValue)
				histoTable->pHisto[cls]->pUnder += 1.0;
			else if(value>=maxValue)
				histoTable->pHisto[cls]->pOver += 1.0;
			else
			{
				unsigned int idx = (unsigned int)((value-minValue)/unit);
				if(idx<numDiv)	histoTable->pHisto[cls]->prob[idx] += 1.0;
				else			histoTable->pHisto[cls]->pOver += 1.0;
			}
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
//	printf("count,");


	for(cls=0; cls<=maxCls; cls++)
	{
		histoTable->pHisto[cls]->numData = caseList->totalNumCandOfClass[cls];

		histoTable->pHisto[cls]->pUnder /= (double)histoTable->pHisto[cls]->numData;
		histoTable->pHisto[cls]->pOver /= (double)histoTable->pHisto[cls]->numData;

		for(unsigned int ii=0; ii<numDiv; ii++)
			histoTable->pHisto[cls]->prob[ii] /= (double)histoTable->pHisto[cls]->numData;
	}
//	printf("calcProb\n");


//	delete [] avg;
//	delete [] dev;

	return histoTable;
}


PR_HISTOTABLE*
PR_CalculateFeatHistograms02_temp(
	PR_ARRAYCASEDATATEMP* dataTemp, unsigned int target, unsigned int numDiv, double minValue, double maxValue)
{
	unsigned int i;
	unsigned char cls, maxCls=0, numCls;

	unsigned char* aryLabel = PR_ExtractFullArrayOfClassLabel(dataTemp);
	float* aryFeat = PR_ExtractFullArrayOfOneFeature(dataTemp,target);
	
	for(i=0; i<dataTemp->numCandidate; i++)
	{
		if(maxCls<aryLabel[i])	maxCls=aryLabel[i];
	}
	numCls = maxCls+1;


	// make histogram
	PR_HISTOTABLE* histoTable = PR_NewHistoTable(numCls, numDiv);
	PR_CopyFeatTag(histoTable->feat, dataTemp->tableFeature->tags[target]);

	double unit = (maxValue-minValue)/(double)numDiv;
	histoTable->range[0] = minValue;
	for(unsigned int ii=1; ii<numDiv; ii++)	histoTable->range[ii] = minValue+unit*(double)ii;
	histoTable->range[numDiv] = maxValue;
//	printf("range,");


	for(i=0; i<dataTemp->numCandidate; i++)
	{
		cls=aryLabel[i];
		double value = (double)aryFeat[i];

		if(value<minValue)
			histoTable->pHisto[cls]->pUnder += 1.0;
		else if(value>=maxValue)
			histoTable->pHisto[cls]->pOver += 1.0;
		else
		{
			unsigned int idx = (unsigned int)((value-minValue)/unit);
			if(idx<numDiv)	histoTable->pHisto[cls]->prob[idx] += 1.0;
			else			histoTable->pHisto[cls]->pOver += 1.0;
		}
	}


	for(cls=0; cls<=maxCls; cls++)
	{
		histoTable->pHisto[cls]->numData = dataTemp->numCandidateInClass[cls];//caseList->totalNumCandOfClass[cls];

		histoTable->pHisto[cls]->pUnder /= (double)histoTable->pHisto[cls]->numData;
		histoTable->pHisto[cls]->pOver /= (double)histoTable->pHisto[cls]->numData;
		for(i=0; i<numDiv; i++)
			histoTable->pHisto[cls]->prob[i] /= (double)histoTable->pHisto[cls]->numData;
	}


	delete [] aryLabel;
	delete [] aryFeat;


	return histoTable;
}

