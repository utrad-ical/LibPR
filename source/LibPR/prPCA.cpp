////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prPCA.cpp : for function
//
//			main coder: M. Nemoto
//
//			update history
//
//			2010.05.10	start coding
//			
//
//
////////////////////////////////////////////////////////////////////////////////////////


#include "prPCA.h"
#include "prPCA.private.h"


PR_PRINCIPALCOMPONENT* PR_NewPrincipalComponent(unsigned int numFeature)
{
	PR_PRINCIPALCOMPONENT* data = new PR_PRINCIPALCOMPONENT;
	data->eigenValues = new float [numFeature];
	memset(data->eigenValues,0x00,sizeof(float)*numFeature);
	data->loading = PR_NewMathMatrix(numFeature,numFeature);
	data->mean = PR_NewMathMatrix(numFeature,1);
	return data;
}

void PR_DeletePrincipalComponent(PR_PRINCIPALCOMPONENT* data)
{
	delete [] data->eigenValues;
	PR_DeleteMathMatrix(data->loading);
	PR_DeleteMathMatrix(data->mean);
	delete data;
}

PR_DATASETPRINCIPALCOMPONENT* PR_NewDatasetPrincipalComponent(int numClass, unsigned int numFeature)
{
	PR_DATASETPRINCIPALCOMPONENT* dataset = new PR_DATASETPRINCIPALCOMPONENT;
	dataset->data = new PR_PRINCIPALCOMPONENT* [numClass];
	for(int i=0; i<numClass; i++)	dataset->data[i] = PR_NewPrincipalComponent(numFeature);
	dataset->numClass = numClass;
//	printf("make(class%d,feat%d)\n",dataset->numClass,numFeature);
	return dataset;
}

void PR_DeleteDatasetPrincipalComponent(PR_DATASETPRINCIPALCOMPONENT* dataset)
{
//	printf("num=%d,",dataset->numClass);
	for(int i=0; i<dataset->numClass; i++)	PR_DeletePrincipalComponent(dataset->data[i]);
//	printf("del1,");
	delete [] dataset->data;
//	printf("del2\n");
	delete dataset;
}


PR_DATASETPRINCIPALCOMPONENT* PR_ProcessingPCA(PR_CASELIST* caseList)
{
	unsigned char numClass = caseList->numClass;
	unsigned int numFeature = caseList->featTagTable->numFeat;
	printf("nCls%d, nFeat%d\n",numClass,numFeature);

	PR_DATASETPRINCIPALCOMPONENT* dataset = new PR_DATASETPRINCIPALCOMPONENT;
	dataset->numClass = numClass;
	dataset->data = new PR_PRINCIPALCOMPONENT* [numClass];

	for(unsigned char cls=0; cls<numClass; cls++)
	{
		printf("cls%d(/%d); ",cls,numClass);
		dataset->data[cls] = new PR_PRINCIPALCOMPONENT;
		dataset->data[cls]->eigenValues = new float [numFeature];
		dataset->data[cls]->mean = PR_NewMathMatrix(numFeature,1);
		
		printf("gauss,");
		PR_GAUSSPARAM* gauss = PR_NewGaussParam(caseList->featTagTable);
		PR_DeleteMathMatrix(gauss->mean); 
		gauss->mean = dataset->data[cls]->mean;
		PR_CalcGaussParam(caseList,cls,1,gauss);
		
		printf("eigen,");
		dataset->data[cls]->loading = PR_CalcEigenVector(gauss->covar,dataset->data[cls]->eigenValues);

		printf("del,");
		gauss->mean = NULL;
		PR_DeleteGaussParam(gauss);

		printf("ok\n");
	}

	return dataset;
}


PR_DATASETPRINCIPALCOMPONENT* PR_ProcessingLimitedDimensionPCA(PR_CASELIST* caseList, unsigned int num)
{
	unsigned char numClass = caseList->numClass;
	unsigned int numFeature = caseList->featTagTable->numFeat;
	printf("nCls%d, nFeat%d\n",numClass,numFeature);

	PR_DATASETPRINCIPALCOMPONENT* dataset = new PR_DATASETPRINCIPALCOMPONENT;
	dataset->numClass = numClass;
	dataset->data = new PR_PRINCIPALCOMPONENT* [numClass];

	for(unsigned char cls=0; cls<numClass; cls++)
	{
		printf("cls%d(/%d); ",cls,numClass);
		dataset->data[cls] = new PR_PRINCIPALCOMPONENT;
		dataset->data[cls]->mean = PR_NewMathMatrix(numFeature,1);

		printf("gauss,");
		PR_GAUSSPARAM* gauss = PR_NewGaussParam(caseList->featTagTable);
		PR_DeleteMathMatrix(gauss->mean); 
		gauss->mean = dataset->data[cls]->mean;
		PR_CalcGaussParam(caseList,cls,1,gauss);

		printf("eigen(n%d/%d),",num,numFeature);
		PR_EIGENCOMPONENT* ecomp = PR_CalcEigenVectorLimitedDimension(gauss->covar,num);
		dataset->data[cls]->eigenValues = ecomp->eValue;
		dataset->data[cls]->loading = ecomp->eMatrix;

		printf("del,");
		gauss->mean = NULL;
		PR_DeleteGaussParam(gauss);
		delete ecomp;

		printf("ok\n");
	}

	printf("nF=%d\n",caseList->featTagTable->numFeat);

	return dataset;
}





float* PR_GetContributionRatioPCA(PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char targetClass)
{
	unsigned int numFeature = dataPCA->data[0]->mean->width;
	float* contRatio = new float [numFeature];
	double sumEigenValue = 0.0;

	for(unsigned int i=0; i<numFeature; i++)
		sumEigenValue += fabs((double)dataPCA->data[targetClass]->eigenValues[i]);

	for(unsigned int i=0; i<numFeature; i++)
		contRatio[i] = (float)(fabs((double)dataPCA->data[targetClass]->eigenValues[i])/sumEigenValue);

	return contRatio;
}


unsigned int 
PR_GetDimensionForCumulativeContributionRatioThresholdPCA(
	PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char targetClass, double threshold)
{
	unsigned int i, num=dataPCA->data[targetClass]->mean->width;
	double sum=0.0,	cumulation=0.0;

	for(i=0; i<num; i++)
		sum += fabs((double)dataPCA->data[targetClass]->eigenValues[i]);

	for(i=0; i<num; i++)
	{
		cumulation += fabs((double)dataPCA->data[targetClass]->eigenValues[i])/sum;
		if(cumulation>threshold)	break;
	}

	return i;
}


float PR_CalcGivenPrincipalComponentScorePCA(PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, 
										  unsigned char targetClass, unsigned int targetPrincipalComponent)
{
	unsigned int numFeature = dataPCA->data[0]->mean->width;

	double sum=0.0;
	for(unsigned int i=0; i<numFeature; i++)
	{
		double sub = (double)candData->featValues[i]-dataPCA->data[targetClass]->mean->dataMatrix[0][i];
		sum += sub*dataPCA->data[targetClass]->loading->dataMatrix[i][targetPrincipalComponent];
	}

	return (float)sum;
}

float* PR_GetArrayPrincipalScores(PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char cTarget)
{
	if(dataPCA==NULL)									return NULL;
	else if(dataPCA->data[cTarget]->loading->width==0)	return NULL;			

	unsigned int numEigen = dataPCA->data[cTarget]->loading->width;
	float* scores = new float [numEigen];

	for(unsigned int i=0; i<numEigen; i++)
		scores[i] = PR_CalcGivenPrincipalComponentScorePCA(candData,dataPCA,cTarget,i);

	return scores;
}


double PR_GetSquaredNormOfEigenResidue(PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char cTarget)
{
	PR_CASELIST* cList = (PR_CASELIST*)((PR_CASEDATA*)candData->rootCaseData)->rootCaseList;

	unsigned int feat, eigen;
	unsigned int numFeat = cList->featTagTable->numFeat;
	unsigned int numEigen;

	if(dataPCA!=NULL)	numEigen = dataPCA->data[cTarget]->loading->width;
	else				numEigen = 0;

	float* score = PR_GetArrayPrincipalScores(candData,dataPCA,cTarget);

	double norm=0.0, normOrg=0.0, normProj=0.0;

	for(feat=0; feat<numFeat; feat++)
	{
		double sub = (double)candData->featValues[feat]-dataPCA->data[cTarget]->mean->dataMatrix[0][feat];
		normOrg += sub*sub;
	}

	for(eigen=0; eigen<numEigen; eigen++)
	{
		normProj += (double)score[eigen]*(double)score[eigen];
	}

	norm =  normOrg-normProj;

	/* 
	for(feat=0; feat<numFeat; feat++)
	{
	//	printf("#%d: ",feat);
		double dmyFromMean = (double)candData->featValues[feat]-dataPCA->data[cTarget]->mean->dataMatrix[0][feat];

		double dmyProjected = 0.0;
		for(eigen=0; eigen<numEigen; eigen++)
			dmyProjected += (double)score[eigen]*dataPCA->data[cTarget]->loading->dataMatrix[feat][eigen];
	//	printf("proj=%.3e, ",dmyProjected);

		double subDmy = dmyFromMean-dmyProjected;
	//	printf("sub=%.3e, ",subDmy);
		norm += subDmy*subDmy;
	//	printf("addSum[%d]=%.3e",feat,subDmy);
	}
	*/
	
	if(score!=NULL)	delete [] score;

	return norm;
	
}


float*
PR_GetArrayOfEigenResidueNormSquare(
	PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char cTarget,
	int numTrial, int* trialDimensionNum)
{
	PR_CASELIST* cList = (PR_CASELIST*)((PR_CASEDATA*)candData->rootCaseData)->rootCaseList;

	unsigned int feat, eigen;
	unsigned int numFeat = cList->featTagTable->numFeat;
	unsigned int numEigen;
	if(dataPCA!=NULL)	numEigen = dataPCA->data[cTarget]->loading->width;
	else				numEigen = 0;

	int cntTrial=0;
	float* arySqNorm = new float [numTrial];
	double norm=0.0, normOrg=0.0, normProj=0.0;

	for(feat=0; feat<numFeat; feat++)
	{
		double sub = (double)candData->featValues[feat]-dataPCA->data[cTarget]->mean->dataMatrix[0][feat];
		normOrg += sub*sub;
	}

	if(trialDimensionNum[0]==0)
	{
		arySqNorm[0] = (float)normOrg;
		cntTrial++;
	}

	if(trialDimensionNum[numTrial-1]>0)
	{
		float* score = PR_GetArrayPrincipalScores(candData,dataPCA,cTarget);

		for(eigen=0; eigen<=(unsigned int)trialDimensionNum[numTrial-1]; eigen++)
		{
			normProj += (double)score[eigen]*(double)score[eigen];

			if(eigen+1==trialDimensionNum[cntTrial])
			{
				arySqNorm[cntTrial] = (float)(normOrg-normProj);
				cntTrial++;
			}

			if(cntTrial>=numTrial)	break;
		}

		if(score!=NULL)		delete [] score;
	}

	/*
	float* score = PR_GetArrayPrincipalScores(candData,dataPCA,cTarget);
	double* residue = new double [numFeat];
	double ssd = 0.0;
	
	for(feat=0; feat<numFeat; feat++)
		residue[feat] = (double)candData->featValues[feat]-dataPCA->data[cTarget]->mean->dataMatrix[0][feat];

	if(trialDimensionNum[0]==0)
	{
		ssd = 0.0;
		for(feat=0; feat<numFeat; feat++)	ssd += residue[feat]*residue[feat];
		arySqNorm[0] = (float)ssd;
		cntTrial++;
	}

	for(eigen=0; eigen<=(unsigned int)trialDimensionNum[numTrial-1]; eigen++)
	{
		for(feat=0; feat<numFeat; feat++)	residue[feat] -= (double)score[eigen]*dataPCA->data[cTarget]->loading->dataMatrix[feat][eigen];
		ssd = 0.0;
		if(eigen+1==trialDimensionNum[cntTrial])
		{
			for(feat=0; feat<numFeat; feat++)	ssd += residue[feat]*residue[feat];
			arySqNorm[cntTrial] = (float)ssd;
			cntTrial++;
		}
		if(cntTrial>=numTrial)	break;
	}

	if(score!=NULL)		delete [] score;
	delete [] residue;
	*/

	return arySqNorm;
}


int PR_SaveBinaryDatasetPrincipalComponent(PR_DATASETPRINCIPALCOMPONENT* dataset, char* filename)
{
	printf("save %s..",filename);

	unsigned char numClass = dataset->numClass;
	unsigned int numFeature = dataset->data[0]->mean->width;
	unsigned int numEigen = dataset->data[0]->loading->width;

	FILE* fp = fopen(filename,"wb");

	// version
	char version[16];
	sprintf(version,SAVE_PCA_VERSION);
	fwrite(version,sizeof(char),9,fp);

	// numClass
	fwrite(&numClass, sizeof(unsigned char), 1, fp);

	// numFeat
	fwrite(&numFeature, sizeof(unsigned int), 1, fp);

	// numEigen
	fwrite(&numEigen, sizeof(unsigned int), 1, fp);

	for(unsigned char cls=0; cls<dataset->numClass; cls++)
	{
		// Mean
		fwrite(dataset->data[cls]->mean->dataMatrix[0], sizeof(double), numFeature, fp);

		// Loading-Matrix
		for(unsigned int i=0; i<numFeature; i++)
			fwrite(dataset->data[cls]->loading->dataMatrix[i], sizeof(double), numEigen, fp);

		// Eigen Values
		fwrite(dataset->data[cls]->eigenValues, sizeof(float), numEigen, fp);
	}

	fclose(fp);
	printf(" ok!\n");

	return 0;
}

int PR_SaveBinaryDatasetPrincipalComponentOld(PR_DATASETPRINCIPALCOMPONENT* dataset, char* filename)
{
	printf("save %s..",filename);

	unsigned char numClass = dataset->numClass;
	unsigned int numFeature = dataset->data[0]->mean->width;

	FILE* fp = fopen(filename,"wb");

	// numClass
	fwrite(&numClass, sizeof(unsigned char), 1, fp);

	// numFeat
	fwrite(&numFeature, sizeof(unsigned int), 1, fp);

	for(unsigned char cls=0; cls<dataset->numClass; cls++)
	{
		// Mean
		fwrite(dataset->data[cls]->mean->dataMatrix[0], sizeof(double), numFeature, fp);

		// Loading-Matrix
		for(unsigned int i=0; i<numFeature; i++)
			fwrite(dataset->data[cls]->loading->dataMatrix[i], sizeof(double), numFeature, fp);

		// Eigen Values
		fwrite(dataset->data[cls]->eigenValues, sizeof(float), numFeature, fp);
	}

	fclose(fp);
	printf(" ok!\n");

	return 0;
}

PR_DATASETPRINCIPALCOMPONENT* PR_ReadBinaryDatasetPrincipalComponentOld(char* filename)
{
	unsigned char numClass;
	unsigned int numFeature;

	printf("read_old: %s..",filename);
	FILE* fp = fopen(filename,"rb");

	// numClass
	fread(&numClass, sizeof(unsigned char), 1, fp);

	// numFeat
	fread(&numFeature, sizeof(unsigned int), 1, fp);

	// Memory Set
	PR_DATASETPRINCIPALCOMPONENT* dataset = PR_NewDatasetPrincipalComponent((int)numClass, numFeature);

	for(unsigned char cls=0; cls<dataset->numClass; cls++)
	{
		// Mean
		fread(dataset->data[cls]->mean->dataMatrix[0], sizeof(double), numFeature, fp);

		// Loading-Matrix
		for(unsigned int i=0; i<numFeature; i++)
			fread(dataset->data[cls]->loading->dataMatrix[i], sizeof(double), numFeature, fp);

		// Eigen Values
		fread(dataset->data[cls]->eigenValues, sizeof(float), numFeature, fp);
	}

	fclose(fp);
	printf("ok!\n");

	return dataset;
}

PR_DATASETPRINCIPALCOMPONENT* PR_ReadBinaryDatasetPrincipalComponent(char* filename)
{
	unsigned char numClass;
	unsigned int numFeature, numEigen;

	printf("read: %s..",filename);
	FILE* fp = fopen(filename,"rb");

	if(fp==NULL)
	{
		printf("cannot open\n");
		return NULL;
	}


	// version
	char version[16];
	fread(version,sizeof(char),9,fp);

	if(strncmp(version,SAVE_PCA_VERSION,9)!=0)
	{
		printf("%s, ",version);
		fclose(fp);
		return PR_ReadBinaryDatasetPrincipalComponentOld(filename);
	}
	

	// numClass
	fread(&numClass, sizeof(unsigned char), 1, fp);

	// numFeat
	fread(&numFeature, sizeof(unsigned int), 1, fp);

	// numEigen
	fread(&numEigen, sizeof(unsigned int), 1, fp);


	// Memory Set
	PR_DATASETPRINCIPALCOMPONENT* dataset = PR_NewDatasetPrincipalComponent((int)numClass, numFeature);

	for(unsigned char cls=0; cls<dataset->numClass; cls++)
	{
		// Mean
		fread(dataset->data[cls]->mean->dataMatrix[0], sizeof(double), numFeature, fp);

		// Loading-Matrix
		for(unsigned int i=0; i<numFeature; i++)
			fread(dataset->data[cls]->loading->dataMatrix[i], sizeof(double), numEigen, fp);

		// Eigen Values
		fread(dataset->data[cls]->eigenValues, sizeof(float), numEigen, fp);
	}

	fclose(fp);
	printf("ok!\n");

	return dataset;
}

