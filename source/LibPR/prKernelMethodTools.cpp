

#include "./prKernelMethodTools.h"
#include "./prKernelMethodTools.private.h"


#include <stdlib.h>
#include <math.h>

#include "../LibPR.h"
#include "./prCandDataSet.h"
#include "./prFeatTag.h"
#include "./prMatrixTools.h"



PR_SAMPLESETFORKERNELMETHOD* 
PR_NewSampleSetForKernelMethod(
	unsigned char nClass, unsigned int nSample, unsigned int nFeature)
{
	PR_SAMPLESETFORKERNELMETHOD* set = new PR_SAMPLESETFORKERNELMETHOD;
	
	set->numClass = nClass;
	
	set->numSample = nSample;
	set->numSampleInClass = new unsigned int [nClass];
	memset(set->numSampleInClass,0x00,sizeof(unsigned int)*nClass);
	
	set->tableFeat = PR_NewFeatTagTable(nFeature);
	
	if(nSample)
	{
		set->value = new float* [nSample];
		set->value[0] = new float [nSample*nFeature];
		for(unsigned int i=1; i<nSample; i++)	set->value[i] = set->value[0]+i*nFeature;
		memset(set->value[0],0x00,sizeof(float)*nSample*nFeature);
	}
	else
		set->value = NULL;

	return set;
}

void
PR_DeleteSampleSetForKernelMethodWithIndividualFeatureData(PR_SAMPLESETFORKERNELMETHOD* set)
{
	delete [] set->numSampleInClass;
	PR_DeleteFeatTagTable(set->tableFeat);
	delete [] set->value[0];
	delete [] set->value;
	delete set;
}

void 
PR_DeleteSampleSetForKernelMethodSheringFeatureDataWithCandData(PR_SAMPLESETFORKERNELMETHOD* set)
{
	delete [] set->numSampleInClass;
	PR_DeleteFeatTagTable(set->tableFeat);
	if(set->value!=NULL)	delete [] set->value;
	delete set;
}

int
PR_LinkFeatureDataOfSampleSetForKernelMethodAndFeatureDataOfCandData(PR_SAMPLESETFORKERNELMETHOD* set, PR_CASELIST* list)
{
	if(set->tableFeat->numFeat != list->featTagTable->numFeat)
	{
		fprintf(stderr,"wrong number of feature!");
		return PR_AVEND;
	}
	else if(set->value!=NULL || set->numSample)
	{
		fprintf(stderr,"set->value has set memory..");
		return PR_AVEND;
	}

	PR_CopyFeatTagTable(set->tableFeat,list->featTagTable);

	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;

	set->numSample = 0;
	caseData = list->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			if(!candData->booOutlier && candData->classLabel<set->numClass)	set->numSample++;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}

	set->value = new float* [set->numSample];
	memset(set->numSampleInClass,0x00,sizeof(unsigned int)*set->numClass);
	
	unsigned int cnt=0;
	for(unsigned char c=0; c<set->numClass; c++)
	{
		caseData = list->childNodeFirstCase;
		while(caseData!=NULL)
		{
			candData = caseData->childNodeFirstCand;
			while(candData!=NULL)
			{
				if(!candData->booOutlier && candData->classLabel==c)
				{
					set->numSampleInClass[c]++;
					set->value[cnt] = candData->featValues;
					cnt++;
				}
				candData = candData->nodeNextCand;
			}
			caseData = caseData->nodeNextCase;
		}

		printf("[%d]%d,",c,set->numSampleInClass[c]);
	}
	printf("=>cnt%d\n",cnt);

	return PR_EOT;
}

int
PR_CopyFeatureDataOfCandDataToSampleSetForKernelMethod(PR_SAMPLESETFORKERNELMETHOD* set, PR_CASELIST* list)
{
	if(set->tableFeat->numFeat != list->featTagTable->numFeat)
	{
		fprintf(stderr,"wrong number of feature!");
		return PR_AVEND;
	}

	PR_CopyFeatTagTable(set->tableFeat,list->featTagTable);

	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;

	unsigned int cnt=0;
	caseData = list->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			if(!candData->booOutlier && candData->classLabel<set->numClass)	cnt++;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}

	if(set->numSample!=cnt)
	{
		set->numSample = cnt;
		delete [] set->value[0];
		delete [] set->value;

		set->value = new float* [cnt];
		set->value[0] = new float [cnt*set->tableFeat->numFeat];
		for(unsigned int i=1; i<cnt; i++)
			set->value[i] = set->value[0]+set->tableFeat->numFeat*i;
	}

//	set->value = new float* [set->numSample];
	memset(set->numSampleInClass,0x00,sizeof(unsigned int)*set->numClass);
	
	cnt=0;
	for(unsigned char c=0; c<set->numClass; c++)
	{
		caseData = list->childNodeFirstCase;
		while(caseData!=NULL)
		{
			candData = caseData->childNodeFirstCand;
			while(candData!=NULL)
			{
				if(!candData->booOutlier && candData->classLabel==c)
				{
					set->numSampleInClass[c]++;
					memcpy(set->value[cnt],candData->featValues,sizeof(float)*set->tableFeat->numFeat);
					cnt++;
				}
				candData = candData->nodeNextCand;
			}
			caseData = caseData->nodeNextCase;
		}
	}

	return PR_EOT;
}



PR_MATHMATRIX*
PR_HilbertSpaceObjection(
	PR_SAMPLESETFORKERNELMETHOD* sample, PR_CANDDATA* data, int kernel, double con, double multi, double sigma)
{
	unsigned int i, j, numFeat=sample->tableFeat->numFeat;

	PR_CASELIST* root = (PR_CASELIST*)((PR_CASEDATA*)data->rootCaseData)->rootCaseList;
	int boo = PR_CompareFeatTagTable(sample->tableFeat,root->featTagTable);
	float* valueOfData = data->featValues;

	if(boo==PR_AVEND)
	{
		valueOfData = new float [numFeat];
		for(j=0; j<numFeat; j++)
		{
			i = PR_ExtractFeatNumberInFeatTagTable(root->featTagTable,sample->tableFeat->tags[j]);
			if(i>=root->featTagTable->numFeat)
			{
				fprintf(stderr, "cannot find %dth=[%04x.%04x]..",
								j,sample->tableFeat->tags[j]->group,sample->tableFeat->tags[j]->element);
			}
			valueOfData[j] = data->featValues[i];
		}
	}

	PR_MATHMATRIX* mout = PR_NewMathMatrix(sample->numSample,1);
	double sqSigma = sigma*sigma;

	for(i=0; i<sample->numSample; i++)
	{
		if(kernel==PR_KERNEL_LINEAR)
			mout->dataMatrix[0][i] = PR_LinearKernel(numFeat,sample->value[i],valueOfData);
		else if(kernel==PR_KERNEL_POLYNOMIAL)
			mout->dataMatrix[0][i] = PR_PolynomialKernel(numFeat,sample->value[i],valueOfData,con,multi);
		else if(kernel==PR_KERNEL_GAUSS)
			mout->dataMatrix[0][i] = PR_GaussKernel(numFeat,sample->value[i],valueOfData,sqSigma);
	}

	if(boo==PR_AVEND)	delete [] valueOfData;

	return mout;
}



PR_MATHMATRIX*
PR_HilbertSpaceObjectionModeFloatArray(
	PR_SAMPLESETFORKERNELMETHOD* sample, float* ary, int kernel, double con, double multi, double sigma)
{
	unsigned int i, numFeat=sample->tableFeat->numFeat;

	PR_MATHMATRIX* mout = PR_NewMathMatrix(sample->numSample,1);
	double sqSigma = sigma*sigma;

	for(i=0; i<sample->numSample; i++)
	{
		if(kernel==PR_KERNEL_LINEAR)
			mout->dataMatrix[0][i] = PR_LinearKernel(numFeat,sample->value[i],ary);
		else if(kernel==PR_KERNEL_POLYNOMIAL)
			mout->dataMatrix[0][i] = PR_PolynomialKernel(numFeat,sample->value[i],ary,con,multi);
		else if(kernel==PR_KERNEL_GAUSS)
			mout->dataMatrix[0][i] = PR_GaussKernel(numFeat,sample->value[i],ary,sqSigma);
	}

	return mout;
}


PR_MATHMATRIX*
PR_GramMatrix(
	PR_SAMPLESETFORKERNELMETHOD* sample, int kernel, double con, double multi, double sigma)
{
	unsigned int i, j, numFeat=sample->tableFeat->numFeat;
	PR_MATHMATRIX* mout = PR_NewMathMatrix(sample->numSample,sample->numSample);

	double sqSigma = sigma*sigma;

	for(j=0; j<sample->numSample; j++)
	for(i=j; i<sample->numSample; i++)
	{
		if(kernel==PR_KERNEL_LINEAR)
			mout->dataMatrix[j][i] = PR_LinearKernel(numFeat,sample->value[j],sample->value[i]);
		else if(kernel==PR_KERNEL_POLYNOMIAL)
			mout->dataMatrix[j][i] = PR_PolynomialKernel(numFeat,sample->value[j],sample->value[i],con,multi);
		else if(kernel==PR_KERNEL_GAUSS)
			mout->dataMatrix[j][i] = PR_GaussKernel(numFeat,sample->value[j],sample->value[i],sqSigma);

		if(j!=i)	mout->dataMatrix[i][j]=mout->dataMatrix[j][i];
	}

	return mout;
}




double PR_LinearKernel(unsigned int num, float* ary1, float* ary2)
//double PR_LinearKernel(PR_CANDDATA* data1, PR_CANDDATA* data2)
{
	double out=0.0;
	for(unsigned int i=0; i<num; i++)
		out += (double)ary1[i]*(double)ary2[i];

//	unsigned int i, j;
//	PR_CASELIST* cList1 = (PR_CASELIST*)((PR_CASEDATA*)data1->rootCaseData)->rootCaseList;
//	PR_CASELIST* cList2 = (PR_CASELIST*)((PR_CASEDATA*)data2->rootCaseData)->rootCaseList;
//	for(i=0; i<cList1->featTagTable->numFeat; i++)
//	{
//		j = PR_ExtractFeatNumberInFeatTagTable(cList2->featTagTable,cList1->featTagTable->tags[i]);
//		if(j>=cList2->featTagTable->numFeat)
//		{
//			printf("not-found %d[%04x.%04x]",
//					i,cList1->featTagTable->tags[i]->group,cList1->featTagTable->tags[i]->element);
//			exit(1);
//		}
//		out += (double)data1->featValues[i]*(double)data2->featValues[j];
//	}

	return out;
}


double PR_PolynomialKernel(unsigned int num, float* ary1, float* ary2, double con, double multi)
{
	double out = PR_LinearKernel(num,ary1,ary2);
	return pow(out+con,multi);
}


double PR_GaussKernel(unsigned int num, float* ary1, float* ary2, double sqSigma)
{
	double sub, sqEra=0.0;

	for(unsigned int i=0; i<num; i++)
	{
		sub = (double)ary1[i]-(double)ary2[i];
		sqEra += sub*sub;
	}

	return exp(-sqEra/sqSigma);
}


