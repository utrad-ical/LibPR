////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			xxx-2.cxx : for function1
//
//			main coder: Y. Masutani
//
//			update history
//
//			2007.05.03	start coding
//			2007.05.05	bug fixed for SSS
//
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma warning (disable:4996)

//#include "VOL.h"
//#include "../LibPR.h"

#include "./prStatistics.h"
#include "./prStatistics.private.h"



PR_MEANVECTOR* PR_NewMeanVector(int numClass, unsigned int numFeat)
{
	PR_MEANVECTOR* vect = new PR_MEANVECTOR;
	vect->numClass = numClass;
	vect->meanVector = new PR_MATHMATRIX* [numClass];
	for(int ii=0; ii<numClass; ii++)	vect->meanVector[ii] = PR_NewMathMatrix(numFeat,1);
	return vect;
}

void PR_DeleteMeanVector(PR_MEANVECTOR* vect)
{
	for(int ii=0; ii<vect->numClass; ii++)	PR_DeleteMathMatrix(vect->meanVector[ii]);
	delete [] vect->meanVector;
	delete vect;
	return;
}

PR_COVARIANCE* PR_NewCoVariance(int numClass, unsigned int numFeat)
{
	PR_COVARIANCE* vect = new PR_COVARIANCE;
	vect->numClass = numClass;
	vect->coVariance = new PR_MATHMATRIX* [numClass];
	for(int ii=0; ii<numClass; ii++)	vect->coVariance[ii] = PR_NewMathMatrix(numFeat,numFeat);
	return vect;
}

void PR_DeleteCoVariance(PR_COVARIANCE* vect)
{
	for(int ii=0; ii<vect->numClass; ii++)	PR_DeleteMathMatrix(vect->coVariance[ii]);
	delete [] vect->coVariance;
	delete vect;
	return;
}

PR_MEANVECTOR* PR_CalcMeanVector(PR_CASELIST* caseList, int numClass)
{
	int cc, numCalcClass = numClass;
	if(numClass==0)		numCalcClass=(int)caseList->numClass;

	unsigned int ff, numFeat = caseList->featTagTable->numFeat;


	PR_MEANVECTOR* vect = PR_NewMeanVector(numCalcClass, numFeat);

//	unsigned int* countCand = new unsigned int [numCalcClass];
//	memset(countCand,0x00,sizeof(unsigned int)*numCalcClass);
	double* nCand = new double [numClass];
	for(int i=0; i<numClass; i++)	nCand[i] = (double)caseList->totalNumCandOfClass[i];
		
	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	PR_CANDDATA* currentCand;

	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;

		while(currentCand!=NULL)
		{
			cc=currentCand->classLabel;
//			if(cc>=numCalcClass)	cc=numCalcClass-1;
//			countCand[cc]++;

			for(ff=0; ff<numFeat; ff++)
				vect->meanVector[cc]->dataMatrix[0][ff] += (double)currentCand->featValues[ff]/nCand[cc];

			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}

//	for(cc=0; cc<numCalcClass; cc++)
//	for(ff=0; ff<numFeat; ff++)
//		vect->meanVector[cc]->dataMatrix[0][ff] /= (double)countCand[cc];

//	delete [] countCand;
	delete [] nCand;

	return vect;
}

PR_COVARIANCE* PR_CalcCoVariance(PR_CASELIST* caseList, PR_MEANVECTOR* mean)
{
	int cc, numCalcClass = mean->numClass;
	unsigned int ff1, ff2, numFeat = caseList->featTagTable->numFeat;

	PR_COVARIANCE* coVar = PR_NewCoVariance(numCalcClass, numFeat);

//	unsigned int* countCand = new unsigned int [numCalcClass];
//	memset(countCand,0x00,sizeof(unsigned int)*numCalcClass);
	double* nCand = new double [mean->numClass];
	for(int i=0; i<mean->numClass; i++)	nCand[i] = (double)caseList->totalNumCandOfClass[i]-1.0;

	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	PR_CANDDATA* currentCand;

	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;

		while(currentCand!=NULL)
		{
			cc=currentCand->classLabel;
//			if(cc>=numCalcClass)	cc=numCalcClass-1;
//			countCand[cc]++;

			for(ff2=0; ff2<numFeat; ff2++)
			{
				double subF2 = (double)currentCand->featValues[ff2]-mean->meanVector[cc]->dataMatrix[0][ff2];

				for(ff1=ff2; ff1<numFeat; ff1++)
				{
					double subF1 = (double)currentCand->featValues[ff1]-mean->meanVector[cc]->dataMatrix[0][ff1];
					coVar->coVariance[cc]->dataMatrix[ff2][ff1] += subF1*subF2/nCand[cc];
				}
			}
			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}

	for(cc=0; cc<numCalcClass; cc++)
	for(ff2=0; ff2<numFeat; ff2++)
	for(ff1=ff2; ff1<numFeat; ff1++)
	{
//		coVar->coVariance[cc]->dataMatrix[ff2][ff1] /= (double)countCand[cc]-1.0;
		if(ff1!=ff2)	coVar->coVariance[cc]->dataMatrix[ff1][ff2] = coVar->coVariance[cc]->dataMatrix[ff2][ff1];
	}

//	delete [] countCand;
	delete [] nCand;

	return coVar;
}

