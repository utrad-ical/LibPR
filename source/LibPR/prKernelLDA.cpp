

#include "./prKernelLDA.h"

#include "../LibPR.h"
#include "./prCandDataSet.h"
#include "./prFeatTag.h"
#include "./prKernelMethodTools.h"
#include "./prMatrixTools.h"

#define DEBUG_KERNEL_FDA
#pragma warning(disable: 4996)

double* 
PR_KernelLinearDiscriminantAnalysisTraining(
	PR_SAMPLESETFORKERNELMETHOD* sampleSet, int idxKernel, double con, double multi, double sigma)
{
	unsigned char c;
	unsigned int i, j, t;
	printf("0");

#ifdef DEBUG_KERNEL_FDA
	FILE* fpValue = fopen("./00_value.txt","w");
	fprintf(fpValue,"cls feat...\n");
	for(j=0; j<sampleSet->numSample; j++)
	{
		if(j<sampleSet->numSampleInClass[0])	fprintf(fpValue,"0 ");
		else									fprintf(fpValue,"1 ");
		for(i=0; i<sampleSet->tableFeat->numFeat; i++)
			fprintf(fpValue,"%e ",sampleSet->value[j][i]);
		fprintf(fpValue,"\n");
	}
	fclose(fpValue);
#endif


	{
		double* mean = new double [sampleSet->tableFeat->numFeat];
		double* dev = new double [sampleSet->tableFeat->numFeat];
		memset(mean,0x00,sizeof(double)*sampleSet->tableFeat->numFeat);
		memset(dev,0x00,sizeof(double)*sampleSet->tableFeat->numFeat);

		for(j=0; j<sampleSet->numSample; j++)
		for(i=0; i<sampleSet->tableFeat->numFeat; i++)
			mean[i] += (double)sampleSet->value[j][i]/(double)sampleSet->numSample;

		for(j=0; j<sampleSet->numSample; j++)
		for(i=0; i<sampleSet->tableFeat->numFeat; i++)
			dev[i] += ((double)sampleSet->value[j][i]-mean[i])*((double)sampleSet->value[j][i]-mean[i]);

		for(i=0; i<sampleSet->tableFeat->numFeat; i++)
			dev[i] = sqrt(dev[i]);

		for(j=0; j<sampleSet->numSample; j++)
		for(i=0; i<sampleSet->tableFeat->numFeat; i++)
			sampleSet->value[j][i] = (float)(((double)sampleSet->value[j][i]-mean[i])/dev[i]);

		delete [] mean;
		delete [] dev;
	}


#ifdef DEBUG_KERNEL_FDA
	FILE* fpValueN = fopen("./01_valueNor.txt","w");
	fprintf(fpValue,"cls feat...\n");
	for(j=0; j<sampleSet->numSample; j++)
	{
		if(j<sampleSet->numSampleInClass[0])	fprintf(fpValueN,"0 ");
		else									fprintf(fpValueN,"1 ");
		for(i=0; i<sampleSet->tableFeat->numFeat; i++)
			fprintf(fpValueN,"%e ",sampleSet->value[j][i]);
		fprintf(fpValueN,"\n");
	}
	fclose(fpValueN);
#endif




	// within-matrix: mW
	PR_MATHMATRIX* mK = PR_GramMatrix(sampleSet,idxKernel,con,multi,sigma);
	printf("1");

#ifdef DEBUG_KERNEL_FDA
	FILE* fpGram = fopen("./02_gram.txt","w");
	for(j=0; j<sampleSet->numSample; j++)
	{
		for(i=0; i<sampleSet->numSample; i++)
			fprintf(fpGram,"%e ",mK->dataMatrix[j][i]);
		fprintf(fpGram,"\n");
	}
	fclose(fpGram);
#endif
	
	PR_MATHMATRIX* mW = PR_NewMathMatrix(sampleSet->numSample,sampleSet->numSample);
	printf("2\n");
	for(j=0; j<sampleSet->numSample; j++)
	{
		for(i=j; i<sampleSet->numSample; i++)
		{
			printf("[%d][%d]",j,i);
			for(t=0; t<sampleSet->numSample; t++)
				mW->dataMatrix[j][i] += mK->dataMatrix[j][t]*mK->dataMatrix[i][t];
			mW->dataMatrix[i][j] = mW->dataMatrix[j][i];
			printf(" = %e \r",mW->dataMatrix[j][i]);
		}
	}
	printf("\n");

	PR_DeleteMathMatrix(mK);
	printf("a");

#ifdef DEBUG_KERNEL_FDA
	FILE* fpGram2 = fopen("./03_gram2.txt","w");
	for(j=0; j<sampleSet->numSample; j++)
	{
		for(i=0; i<sampleSet->numSample; i++)
			fprintf(fpGram2,"%e ",mW->dataMatrix[j][i]);
		fprintf(fpGram2,"\n");
	}
	fclose(fpGram2);
#endif


	// sum of Hilbelt space objected vector (within class) : mL[class(0~1)] 
	PR_MATHMATRIX** mL = new PR_MATHMATRIX* [3];

	unsigned int sum=0;
	for(c=0; c<2; c++)
	{
		printf("[%d],num%d,",c,sampleSet->numSampleInClass[c]);

		mL[c] = PR_NewMathMatrix(sampleSet->numSample,1);
		printf("new,");

		for(j=sum; j<sum+sampleSet->numSampleInClass[c]; j++)
		{
			PR_MATHMATRIX* dmyL = PR_HilbertSpaceObjectionModeFloatArray(sampleSet,sampleSet->value[j],idxKernel,con,multi,sigma);		
			for(i=0; i<sampleSet->numSample; i++)
			{
				mL[c]->dataMatrix[0][i]+=dmyL->dataMatrix[0][i];
	//			printf("%.3e,",mL[c]->dataMatrix[0][i]);
			}
			PR_DeleteMathMatrix(dmyL);
		}
		printf("make,");

		for(j=0; j<sampleSet->numSample; j++)
		for(i=j; i<sampleSet->numSample; i++)
		{
			double dmy = (mL[c]->dataMatrix[0][j]*mL[c]->dataMatrix[0][i])/(double)sampleSet->numSampleInClass[c];
			mW->dataMatrix[j][i] -= dmy;
			mW->dataMatrix[i][j] -= dmy;
	//		printf("[%d][%d]%.3e,",j,i,mW->dataMatrix[j][i]);
		}

		sum += sampleSet->numSampleInClass[c];
		printf("sum%d,",sum);
	}
	printf("b");

#ifdef DEBUG_KERNEL_FDA
	FILE* fpWithin = fopen("./04_within.txt","w");
	for(j=0; j<sampleSet->numSample; j++)
	{
		for(i=0; i<sampleSet->numSample; i++)
			fprintf(fpWithin,"%e ",mW->dataMatrix[j][i]);
		fprintf(fpWithin,"\n");
	}
	fclose(fpWithin);
#endif



	// inverse of mW
	PR_MATHMATRIX* mInvW = PR_InverseMatrix(mW);
	PR_DeleteMathMatrix(mW);
	printf("c");

#ifdef DEBUG_KERNEL_FDA
	FILE* fpWithinInv = fopen("./05_withinInv.txt","w");
	for(j=0; j<sampleSet->numSample; j++)
	{
		for(i=0; i<sampleSet->numSample; i++)
			fprintf(fpWithinInv,"%e ",mInvW->dataMatrix[j][i]);
		fprintf(fpWithinInv,"\n");
	}
	fclose(fpWithinInv);
#endif


	// mL of whole dataset: mL[2]
	mL[2] = PR_NewMathMatrix(sampleSet->numSample,1);
	for(i=0; i<sampleSet->numSample; i++)
		mL[2]->dataMatrix[0][i] = (mL[0]->dataMatrix[0][i]+mL[1]->dataMatrix[0][i])/(double)sampleSet->numSample;

#ifdef DEBUG_KERNEL_FDA
	FILE* fpHilbelt = fopen("./06_hilbelt.txt","w");
	for(c=0; c<3; c++)
	{
		for(i=0; i<sampleSet->numSample; i++)
		{
			if(c<2)	fprintf(fpHilbelt,"%e ",mL[c]->dataMatrix[0][i]);
			else	fprintf(fpHilbelt,"%e ",mL[c]->dataMatrix[0][i]*(double)sampleSet->numSample);
		}
		fprintf(fpHilbelt,"\n");
	}
	fclose(fpHilbelt);
#endif


	for(c=0; c<2; c++)
	for(i=0; i<sampleSet->numSample; i++)
	{
		mL[c]->dataMatrix[0][i] /= (double)sampleSet->numSampleInClass[c];
		mL[c]->dataMatrix[0][i] -= mL[2]->dataMatrix[0][i];
	}

	PR_DeleteMathMatrix(mL[2]);
	printf("d");


#ifdef DEBUG_KERNEL_FDA
	FILE* fpBetween = fopen("./07_between.txt","w");

	PR_MATHMATRIX* mLt = PR_NewMathMatrix(1,sampleSet->numSample);
	for(c=0; c<2; c++)
	{
		fprintf(fpBetween,"cls-%d\n",c);

		for(i=0; i<sampleSet->numSample; i++)
			mLt->dataMatrix[i][0] = mL[c]->dataMatrix[0][i];
		PR_MATHMATRIX* mBd = PR_MultiplyMathMatrix(mLt,mL[c]);

		for(j=0; j<sampleSet->numSample; j++)
		{
			for(i=0; i<sampleSet->numSample; i++)
				fprintf(fpBetween,"%e ",mBd->dataMatrix[j][i]);
			fprintf(fpBetween,"\n");
		}
		fprintf(fpBetween,"\n");
		PR_DeleteMathMatrix(mBd);
	}
	PR_DeleteMathMatrix(mLt);

	fclose(fpBetween);
#endif


	// evaluation matrix: mJ=mW^-1¥mB
	PR_MATHMATRIX* mJ = PR_NewMathMatrix(sampleSet->numSample,sampleSet->numSample);
	PR_MATHMATRIX* mDmyColB = PR_NewMathMatrix(sampleSet->numSample,1);

	for(i=0; i<sampleSet->numSample; i++)
	{
		PR_InitializeMathMatrix(mDmyColB,0.0);

		for(c=0; c<2; c++)
		for(t=0; t<sampleSet->numSample; t++)
			mDmyColB->dataMatrix[0][i] += mL[c]->dataMatrix[0][t]*mL[c]->dataMatrix[0][i]*(double)sampleSet->numSampleInClass[c];

		for(j=0; j<sampleSet->numSample; j++)
		for(t=0; t<sampleSet->numSample; t++)
			mJ->dataMatrix[j][i] += mInvW->dataMatrix[j][t]*mDmyColB->dataMatrix[0][t];
	}
	printf("e");

#ifdef DEBUG_KERNEL_FDA
	FILE* fpJ = fopen("./08_evMatrix.txt","w");
	for(j=0; j<sampleSet->numSample; j++)
	{
		for(i=0; i<sampleSet->numSample; i++)
			fprintf(fpJ,"%e ",mJ->dataMatrix[j][i]);
		fprintf(fpJ,"\n");
	}
	fclose(fpJ);
#endif


	// delete(1)
	PR_DeleteMathMatrix(mInvW);
	PR_DeleteMathMatrix(mDmyColB);

	for(c=0; c<2; c++)	PR_DeleteMathMatrix(mL[c]);
	delete [] mL;
	printf("f");


	// get eigen vector of mJ
	float* eValue = new float [sampleSet->numSample];
	PR_MATHMATRIX* eVector = PR_CalcEigenVector(mJ,eValue);
	PR_DeleteMathMatrix(mJ);
	delete [] eValue;

	double* outVector = new double [sampleSet->numSample];
	memcpy(outVector,eVector->dataMatrix[0],sizeof(double)*sampleSet->numSample);
	PR_DeleteMathMatrix(eVector);
	printf("ok!\n");

#ifdef DEBUG_KERNEL_FDA
	FILE* fpVect = fopen("./09_evMatrix.txt","w");
	for(j=0; j<sampleSet->numSample; j++)
	{
		fprintf(fpVect,"%e ",outVector[j]);	
	}
	fprintf(fpVect,"\n");
	fclose(fpVect);
#endif

	return outVector;
}

double
PR_CalculateValueOfKernelLinearDiscriminantAnalysisForCandData(
	PR_CANDDATA* data, PR_SAMPLESETFORKERNELMETHOD* sample, double* bpVector, int kernel, double con, double multi, double sigma)
{
	PR_MATHMATRIX* aryProject = PR_HilbertSpaceObjection(sample,data,kernel,con,multi,sigma);
	if(aryProject==NULL)	return 0.0;

	double output=0.0;
	for(unsigned int i=0; i<aryProject->width; i++)
		output += aryProject->dataMatrix[0][i]*bpVector[i];

	PR_DeleteMathMatrix(aryProject);

	return output;
}

int
PR_CalculateValueOfKernelLinearDiscriminantAnalysisForCaseList(
	PR_CASELIST* list, PR_SAMPLESETFORKERNELMETHOD* sample, double* bpVector, int kernel, double con, double multi, double sigma)
{
	PR_CASEDATA* caseData = list->childNodeFirstCase;
	PR_CANDDATA* candData;

	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData)
		{
			if(!candData->booOutlier)
			{
				double out = PR_CalculateValueOfKernelLinearDiscriminantAnalysisForCandData(candData,sample,bpVector,kernel,con,multi,sigma);
				candData->likelihood[1] = out;
			}
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}

	return PR_EOT;
}

