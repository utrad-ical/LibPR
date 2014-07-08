////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prMahalanobis.cpp : for function
//
//			main coder: M. Nemoto
//
//			update history
//
//			2009.05.26	start coding
//			
//
//
////////////////////////////////////////////////////////////////////////////////////////


#pragma warning(disable: 4996)

#include "prMahalanobis.h"

#include "../LibPR.h"


PR_GAUSSPARAM* 
//PR_NewGaussParam(unsigned int numFeat)
PR_NewGaussParam(PR_FEATTAGTABLE* table)
{
	if(table->numFeat==0)	return NULL;

	PR_GAUSSPARAM* gParam = new PR_GAUSSPARAM;
	gParam->mean = PR_NewMathMatrix(table->numFeat, 1);
	gParam->covar = PR_NewMathMatrix(table->numFeat, table->numFeat);
	gParam->fTable = table;
	return gParam;
}


void 
PR_DeleteGaussParam(PR_GAUSSPARAM* gParam)
{
	if(gParam==NULL)	return;

	PR_DeleteMathMatrix(gParam->mean);
	PR_DeleteMathMatrix(gParam->covar);
	delete gParam;
	return;
}


int PR_CalcGaussParam(PR_CASELIST* caseList, unsigned char targetClass, char mode, PR_GAUSSPARAM* gParam)
{
	printf("1,");
	
	if(caseList==NULL || gParam==NULL)	return PR_AVEND;

	unsigned int numFeat = caseList->featTagTable->numFeat;
	if(gParam->mean->width!=numFeat || gParam->covar->height!=numFeat || gParam->covar->width!=numFeat)	return PR_AVEND;

	// check use features' index
	unsigned int ii, jj;
	unsigned int* idxDmy = new unsigned int;
	unsigned int* idxFeat = new unsigned int [numFeat];

	for(ii=0; ii<numFeat; ii++)
	{
		int a = PR_CheckIfFeatTagIsMemberOfFeatTagTable(gParam->fTable->tags[ii], caseList->featTagTable, idxDmy);
		if(a==PR_AVEND)
		{
			delete idxDmy;
			delete [] idxFeat;
			return a;
		}
		idxFeat[ii] = *idxDmy;
	}
	delete idxDmy;
	printf("2,");

	PR_CASEDATA* currentCase;
	PR_CANDDATA* currentCand;
	
	double nCandF64 = (double)(caseList->totalNumCandOfClass[targetClass]);
	if(nCandF64==0.0)	return PR_AVEND;

	// calculate mean vector
	currentCase = caseList->childNodeFirstCase;
	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;
		while(currentCand!=NULL)
		{
			if((mode && currentCand->classLabel>=targetClass) || (!mode && currentCand->classLabel==targetClass))
			{
//				numCand++;
				for(ii=0; ii<numFeat; ii++)
					gParam->mean->dataMatrix[0][ii] += (double)currentCand->featValues[idxFeat[ii]]/nCandF64;
			}
			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}
//	if(numCand==0)	return PR_AVEND;

//	double ndmy = (double)numCand;
//	for(ii=0; ii<gParam->fTable->numFeat; ii++)		gParam->mean->dataMatrix[0][ii] /= ndmy;
	printf("3,");

	// calculate co-variance
	nCandF64 -= 1;
	double* subary = new double [numFeat];
	currentCase = caseList->childNodeFirstCase;
	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;
		while(currentCand!=NULL)
		{
			if((mode && currentCand->classLabel>=targetClass) || (!mode && currentCand->classLabel==targetClass))
			{
				for(jj=0; jj<numFeat; jj++)
					subary[jj] = (double)(currentCand->featValues[idxFeat[jj]]-gParam->mean->dataMatrix[0][jj]);

				for(jj=0; jj<numFeat; jj++)
				for(ii=jj; ii<numFeat; ii++)
				{
					gParam->covar->dataMatrix[jj][ii] += subary[jj]*subary[ii]/nCandF64;
				}
			}
			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}

//	ndmy -= 1.0;
	for(jj=0; jj<numFeat; jj++)
	for(ii=jj; ii<numFeat; ii++)
	{
//		gParam->covar->dataMatrix[jj][ii] /= ndmy;
		gParam->covar->dataMatrix[ii][jj] = gParam->covar->dataMatrix[jj][ii];
	}
	printf("4,");

	delete [] idxFeat;
	delete [] subary;
	return PR_EOT;
}


int PR_CalcMean(PR_CASELIST* caseList, unsigned char targetClass, char mode, PR_GAUSSPARAM* gParam)
{
	if(caseList==NULL || gParam==NULL)	return PR_AVEND;

	unsigned int numFeat = caseList->featTagTable->numFeat;
	if(gParam->mean->width!=numFeat || gParam->covar->height!=numFeat || gParam->covar->width!=numFeat)	return PR_AVEND;

	// check use features' index
	unsigned int ii;
	unsigned int* idxDmy = new unsigned int;
	unsigned int* idxFeat = new unsigned int [numFeat];

	for(ii=0; ii<numFeat; ii++)
	{
		int a = PR_CheckIfFeatTagIsMemberOfFeatTagTable(gParam->fTable->tags[ii], caseList->featTagTable, idxDmy);
		if(a==PR_AVEND)
		{
			delete idxDmy;
			delete [] idxFeat;
			return a;
		}
		idxFeat[ii] = *idxDmy;
	}
	delete idxDmy;

	// calculate mean vector
//	unsigned int numCand=0;
	double nCandF64 = (double)(caseList->totalNumCandOfClass[targetClass]);
	PR_CASEDATA* currentCase;
	PR_CANDDATA* currentCand;
	currentCase = caseList->childNodeFirstCase;
	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;
		while(currentCand!=NULL)
		{
			if((mode && currentCand->classLabel>=targetClass) || (!mode && currentCand->classLabel==targetClass))
			{
//				numCand++;
				for(ii=0; ii<numFeat; ii++)
					gParam->mean->dataMatrix[0][ii] += (double)currentCand->featValues[idxFeat[ii]]/nCandF64;
			}
			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}
//	if(numCand==0)	return PR_AVEND;

//	double ndmy = (double)numCand;
//	for(ii=0; ii<gParam->fTable->numFeat; ii++)		gParam->mean->dataMatrix[0][ii] /= ndmy;

	delete [] idxFeat;
	return PR_EOT;
}

int PR_CalcCoVariance(PR_CASELIST* caseList, unsigned char targetClass, char mode, PR_GAUSSPARAM* gParam)
{
	if(caseList==NULL || gParam==NULL)	return PR_AVEND;

	
	unsigned int numFeat = caseList->featTagTable->numFeat;
	if(gParam->mean->width!=numFeat || gParam->covar->height!=numFeat || gParam->covar->width!=numFeat)	return PR_AVEND;

	// check use features' index
	unsigned int ii, jj;

	// calculate co-variance
	double* subary = new double [numFeat];
	double nCandF64 = (double)(caseList->totalNumCandOfClass[targetClass])-1.0;
	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	while(currentCase!=NULL)
	{
		PR_CANDDATA* currentCand = currentCase->childNodeFirstCand;
		while(currentCand!=NULL)
		{
			if((mode && currentCand->classLabel>=targetClass) || (!mode && currentCand->classLabel==targetClass))
			{
				for(jj=0; jj<numFeat; jj++)
					subary[jj] = ((double)currentCand->featValues[jj]-gParam->mean->dataMatrix[0][jj]);

				for(jj=0; jj<numFeat; jj++)
				for(ii=jj; ii<numFeat; ii++)
				{
					gParam->covar->dataMatrix[jj][ii] += subary[jj]*subary[ii]/nCandF64;
				}
			}
			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}

	
	for(jj=0; jj<numFeat; jj++)
	for(ii=jj; ii<numFeat; ii++)
	{
//		gParam->covar->dataMatrix[jj][ii] /= ndmy;
		gParam->covar->dataMatrix[ii][jj] = gParam->covar->dataMatrix[jj][ii];
	}

//	delete [] idxFeat;
	delete [] subary;
	return PR_EOT;
}




PR_MAHALANOBISPARAM* 
PR_NewMahalanobisParam(unsigned char numClass, PR_FEATTAGTABLE* featTable)
{
	if(numClass==0 || featTable==NULL)	return NULL;

	PR_MAHALANOBISPARAM* mParam = new PR_MAHALANOBISPARAM;

	mParam->featTable = PR_NewFeatTagTable(featTable->numFeat);
	PR_CopyFeatTagTable(mParam->featTable, featTable);

	mParam->gauss = new PR_GAUSSPARAM* [numClass];
	for(unsigned char ii=0; ii<numClass; ii++)
		mParam->gauss[ii] = PR_NewGaussParam(mParam->featTable);
	
	mParam->numClass = numClass;

	return mParam;
}


void 
PR_DeleteMahalanobisParam(PR_MAHALANOBISPARAM* mParam)
{
	if(mParam==NULL)	return;

	PR_DeleteFeatTagTable(mParam->featTable);

	for(unsigned char ii=0; ii<mParam->numClass; ii++)
		PR_DeleteGaussParam(mParam->gauss[ii]);
	delete [] mParam->gauss;

	delete mParam;

	return;
}


int 
PR_CalcMahalanobisParam(PR_CASELIST* caseList, PR_MAHALANOBISPARAM* mParam)
{
	if(caseList==NULL || mParam==NULL)	return PR_AVEND;

	for(unsigned char cc=0; cc<mParam->numClass; cc++)
	{
	//	printf("cls%d;\n",cc);
		char mode=0;
		if(cc==mParam->numClass-1)	mode=1;
		int r = PR_CalcGaussParam(caseList, cc, mode, mParam->gauss[cc]);
	//	printf("return%d\n",r);
	}

	return PR_EOT;
}

PR_MAHALANOBISPARAM* 
PR_NewCalcMahalanobisParam(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return NULL;

	PR_MAHALANOBISPARAM* mparam;
	mparam = PR_NewMahalanobisParam(caseList->numClass, caseList->featTagTable);

	PR_CalcMahalanobisParam(caseList, mparam);

	return mparam;
}

int
PR_CalcMahalanobisDistForOneCase(PR_CASEDATA* caseData, unsigned char targetClass, PR_GAUSSPARAM* gParam)
{
	PR_CASELIST* caseList = (PR_CASELIST*)(caseData->rootCaseList);

	// check use features' index
	unsigned int ii, jj;
	unsigned int* idxDmy = new unsigned int;
	unsigned int* idxFeat = new unsigned int [gParam->fTable->numFeat];
	for(ii=0; ii<gParam->fTable->numFeat; ii++)
	{
		int a=PR_CheckIfFeatTagIsMemberOfFeatTagTable(gParam->fTable->tags[ii], caseList->featTagTable, idxDmy);
		if(a==PR_AVEND)
		{
//			printf(" era:feat%d ",ii);
			delete idxDmy;
			delete [] idxFeat;
			return a;
		}
		idxFeat[ii] = *idxDmy;
//		printf("pG%d->%d,",ii,idxFeat[ii]);
	}
	delete idxDmy;
//	printf("1,");


	// calculate mahalanobis distance
	PR_MATHMATRIX* sub1 = PR_NewMathMatrix(gParam->fTable->numFeat,1);
	PR_MATHMATRIX* sub2 = PR_NewMathMatrix(1,gParam->fTable->numFeat);
	PR_MATHMATRIX* invCoVar = PR_InverseMatrix(gParam->covar);		// inverse matrix of co-var
//	printf("2,");

	PR_CANDDATA* candData = caseData->childNodeFirstCand;
	while(candData!=NULL)
	{
		for(ii=0; ii<gParam->fTable->numFeat; ii++)
		{
	//		printf("%d:",ii);
			jj = idxFeat[ii];
	//		printf("[%d]%.2e:",jj,candData->featValues[jj]);
			sub1->dataMatrix[0][ii] = gParam->mean->dataMatrix[0][ii]-(double)candData->featValues[jj];
			sub2->dataMatrix[ii][0] = gParam->mean->dataMatrix[0][ii]-(double)candData->featValues[jj];
	//		printf("-%.2e=%.2e,",gParam->mean->dataMatrix[0][ii],sub1->dataMatrix[0][ii]);
		}
	//	printf("a");
		PR_MATHMATRIX* dmy1 = PR_MultiplyMathMatrix(sub1, invCoVar);
		PR_MATHMATRIX* dmy2 = PR_MultiplyMathMatrix(dmy1, sub2);
		candData->likelihood[targetClass] = dmy2->dataMatrix[0][0];
	//	printf("i");
		PR_DeleteMathMatrix(dmy1);
		PR_DeleteMathMatrix(dmy2);
	//	printf("u");

		candData = candData->nodeNextCand;
	//	printf("e,");
	}

	// memory free
	delete [] idxFeat;
	PR_DeleteMathMatrix(sub1);
	PR_DeleteMathMatrix(sub2);
	PR_DeleteMathMatrix(invCoVar);
//	printf("3,");

	return PR_EOT;
}

int 
PR_CalcMahalanobisDistForOneCaseAllClass(PR_CASEDATA* caseData, PR_MAHALANOBISPARAM* mParam)
{
	if(caseData==NULL || mParam==NULL)	return PR_AVEND;

	for(unsigned char cc=0; cc<mParam->numClass; cc++)
	{
//		printf("c%d,",cc);
		mParam->gauss[cc]->fTable = mParam->featTable;
		int aa = PR_CalcMahalanobisDistForOneCase(caseData,cc,mParam->gauss[cc]);
//		printf("ok,");
		if(aa==PR_AVEND)	return PR_AVEND;
	}

	return PR_EOT;
}

int 
PR_CalcMahalanobisDist(PR_CASELIST* caseList, unsigned char targetClass, PR_GAUSSPARAM* gParam)
{
	if(caseList==NULL || gParam==NULL)	return PR_AVEND;

	if(caseList->numClass < targetClass)	return PR_AVEND;

//	PR_FEATTAGTABLE* featTable = gParam->fTable;

	// check use features' index
	unsigned int ii, jj;
	unsigned int* idxDmy = new unsigned int;
	unsigned int* idxFeat = new unsigned int [gParam->fTable->numFeat];

	
	for(ii=0; ii<gParam->fTable->numFeat; ii++)
	{
		int a;
		a=PR_CheckIfFeatTagIsMemberOfFeatTagTable(gParam->fTable->tags[ii], caseList->featTagTable, idxDmy);
		
		if(a==PR_AVEND)
		{
			printf(" era:feat%d ",ii);
			delete idxDmy;
			delete [] idxFeat;
			return a;
		}
		
		idxFeat[ii] = *idxDmy;
	//	printf("pG%d->%d,",ii,idxFeat[ii]);
	}
	

	delete idxDmy;


	// calculate mahalanobis distance
	PR_CASEDATA* currentCase;
	PR_CANDDATA* currentCand;
	currentCase = caseList->childNodeFirstCase;

//	printf("matrix\n");
//	for(jj=0; jj<gParam->fTable->numFeat; jj++)
//	{
//		for(ii=0; ii<gParam->fTable->numFeat; ii++)
//			printf("%e ",gParam->covar->dataMatrix[jj][ii]);
//		printf("\n");
//	}
//	printf("\n");

	PR_MATHMATRIX* sub1 = PR_NewMathMatrix(gParam->fTable->numFeat,1);
	PR_MATHMATRIX* sub2 = PR_NewMathMatrix(1,gParam->fTable->numFeat);
	PR_MATHMATRIX* invCoVar = PR_InverseMatrix(gParam->covar);		// inverse matrix of co-var

//	printf("inv\n");
//	for(jj=0; jj<gParam->fTable->numFeat; jj++)
//	{
//		for(ii=0; ii<gParam->fTable->numFeat; ii++)
//			printf("%e ",invCoVar->dataMatrix[jj][ii]);
//		printf("\n");
//	}
//	printf("\n");

	/**/
	while(currentCase!=NULL)
	{
	//	printf("id%d,",currentCase->idCase);
		currentCand = currentCase->childNodeFirstCand;

		while(currentCand!=NULL)
		{
			for(ii=0; ii<gParam->fTable->numFeat; ii++)
			{
				jj = idxFeat[ii];
				sub1->dataMatrix[0][ii] = gParam->mean->dataMatrix[0][ii]-(double)currentCand->featValues[jj];
				sub2->dataMatrix[ii][0] = gParam->mean->dataMatrix[0][ii]-(double)currentCand->featValues[jj];
		//		printf("%e ",sub1->dataMatrix[0][ii]);
			}
		//	printf("\n");

			PR_MATHMATRIX* dmy1 = PR_MultiplyMathMatrix(sub1, invCoVar);
			PR_MATHMATRIX* dmy2 = PR_MultiplyMathMatrix(dmy1, sub2);

			currentCand->likelihood[targetClass] = dmy2->dataMatrix[0][0];
		//	printf("d");

			PR_DeleteMathMatrix(dmy1);
			PR_DeleteMathMatrix(dmy2);
		//	printf("e");
			
			currentCand = currentCand->nodeNextCand;
		//	printf("f\n");
		}

		currentCase = currentCase->nodeNextCase;
	}
	/**/

//	printf("c");

	// memory free
	delete [] idxFeat;

	PR_DeleteMathMatrix(sub1);
	PR_DeleteMathMatrix(sub2);
	PR_DeleteMathMatrix(invCoVar);

//	printf("d");

	return PR_EOT;
}


int PR_CalcMahalanobisDistAllClass(PR_CASELIST* caseList, PR_MAHALANOBISPARAM* mParam)
{
	if(caseList==NULL || mParam==NULL)	return PR_AVEND;

	unsigned char cc;
	for(cc=0; cc<mParam->numClass; cc++)
	{
		mParam->gauss[cc]->fTable = mParam->featTable;
	//	printf("c%d;",cc);
		int aa = PR_CalcMahalanobisDist(caseList, cc, mParam->gauss[cc]);
	//	printf("%d\n",aa);
		if(aa==PR_AVEND)	return PR_AVEND;
	}

	return PR_EOT;
}


int
PR_FprintMahalanobisParam(PR_MAHALANOBISPARAM* param, FILE* fp)
{
	unsigned char cc, numClass = param->numClass;
	unsigned int ii, jj, numFeat = param->featTable->numFeat;


	fprintf(fp,"#{class},%d,,#{feat},%d\n",numClass,numFeat);

	for(ii=0; ii<numFeat; ii++)
	{
		cc = (unsigned char)(ii%(unsigned int)(param->numClass+1));
		
		if(cc==param->numClass)
		{
			fprintf(fp,"[%04x %04x],",param->featTable->tags[ii]->group,
									  param->featTable->tags[ii]->element);
		}
		else
		{
			fprintf(fp,"[%04x %04x],",param->gauss[cc]->fTable->tags[ii]->group,
									  param->gauss[cc]->fTable->tags[ii]->element);
		}
	}
	fprintf(fp,"\n");

	for(cc=0; cc<numClass; cc++)
	{
		fprintf(fp,"class-%d\n",cc);

		fprintf(fp,"mean\n");
		for(ii=0; ii<numFeat; ii++)
			fprintf(fp,"%le,",param->gauss[cc]->mean->dataMatrix[0][ii]);
		fprintf(fp,"\n");

		fprintf(fp,"co-var\n");
		for(jj=0; jj<numFeat; jj++)
		{
			for(ii=0; ii<numFeat; ii++)
				fprintf(fp,"%le,",param->gauss[cc]->covar->dataMatrix[jj][ii]);
			fprintf(fp,"\n");
		}
	}
	fprintf(fp,"\n");

	return PR_EOT;
}


int 
PR_SaveBinaryMahalanobisParam(PR_MAHALANOBISPARAM* param, char* filename)
{
	unsigned char ccnt, numClass;
	unsigned int fcnt, numFeat;

	FILE* fp;

	if((fp=fopen(filename,"wb"))==NULL)		return PR_AVEND;
	if(param==NULL)		return PR_AVEND;


	numClass = param->numClass;
	numFeat = param->featTable->numFeat;


	char identify[20];
	strcpy(identify,"PR_MahalanobisParam");
	fwrite(identify, sizeof(char), 19, fp);

	// num of class
	fwrite(&numClass, sizeof(unsigned char), 1, fp);

	// num of feat
	fwrite(&numFeat, sizeof(unsigned int), 1, fp);

	// feature tags
	for(fcnt=0; fcnt<numFeat; fcnt++)
	{
		fwrite(&param->featTable->tags[fcnt]->group, sizeof(unsigned short), 1, fp);
		fwrite(&param->featTable->tags[fcnt]->element, sizeof(unsigned short), 1, fp);
	}

	for(ccnt=0; ccnt<numClass; ccnt++)
	{
		// mean
		fwrite(param->gauss[ccnt]->mean->dataMatrix[0], sizeof(double), numFeat, fp);

		// co-var (only lower-triangular)
		for(fcnt=0; fcnt<numFeat; fcnt++)
			fwrite(param->gauss[ccnt]->covar->dataMatrix[fcnt], sizeof(double), fcnt+1, fp);
	}


	fclose(fp);

	return PR_EOT;
}


PR_MAHALANOBISPARAM* PR_ReadBinaryMahalanobisParam(char* filename)
{

	FILE* fp=fopen(filename,"rb");
	if(fp==NULL)
	{
		fprintf(stderr,"%s ",filename);
		return (PR_MAHALANOBISPARAM*)PR_ErrorMessageNULL("cannot be open [Mahalanobis].");
	}

	char identify[20];
	fread(identify,sizeof(char),19,fp);
	identify[19]='\0';
	printf("%s\n",identify);

	if(strcmp(identify,"PR_MahalanobisParam")!=0)
	{
		fclose(fp);
		fprintf(stderr,"%s ",filename);
		return (PR_MAHALANOBISPARAM*)PR_ErrorMessageNULL("is not MahalanobisParam.");
	}

	unsigned char ccnt, numClass;
	unsigned int fcnt, numFeat;

	// num of class
	fread(&numClass, sizeof(unsigned char), 1, fp);
	printf("cls%d,",numClass);

	// num of feat
	fread(&numFeat, sizeof(unsigned int), 1, fp);
	printf("feat%d",numFeat);

	// feature tags
	PR_FEATTAGTABLE* table = PR_NewFeatTagTable(numFeat);
	for(fcnt=0; fcnt<numFeat; fcnt++)
	{
		fread(&table->tags[fcnt]->group, sizeof(unsigned short), 1, fp);
		fread(&table->tags[fcnt]->element, sizeof(unsigned short), 1, fp);
//		printf("[%04x %04x],",table->tags[fcnt]->group,table->tags[fcnt]->element);
	}
	printf("\n");

	PR_MAHALANOBISPARAM* param = PR_NewMahalanobisParam(numClass, table);
//	PR_CopyFeatTagTable(param->featTable,table);


	for(ccnt=0; ccnt<numClass; ccnt++)
	{
		param->gauss[ccnt]->fTable = param->featTable;
		
		// mean
		fread(param->gauss[ccnt]->mean->dataMatrix[0], sizeof(double), numFeat, fp);

		// co-var (only lower-triangular)
		for(fcnt=0; fcnt<numFeat; fcnt++)
			fread(param->gauss[ccnt]->covar->dataMatrix[fcnt], sizeof(double), fcnt+1, fp);
	}

	for(ccnt=0; ccnt<numClass; ccnt++)
	for(unsigned int jj=0; jj<numFeat; jj++)
	for(unsigned int ii=0; ii<=jj; ii++)
	{
		param->gauss[ccnt]->covar->dataMatrix[ii][jj] = param->gauss[ccnt]->covar->dataMatrix[jj][ii];
	}

	PR_DeleteFeatTagTable(table);
	fclose(fp);

	return param;
}


int PR_SaveTextMahalanobisParam(PR_MAHALANOBISPARAM* param, char* filename)
{
	unsigned char ccnt, numClass;
	unsigned int fcnt, numFeat;

	FILE* fp;

	if((fp=fopen(filename,"w"))==NULL)		return PR_AVEND;
	if(param==NULL)		return PR_AVEND;


	numClass = param->numClass;
	numFeat = param->featTable->numFeat;


	fprintf(fp,"PR_MahalanobisParam\n");
//	char identify[20];
//	strcpy(identify,"PR_MahalanobisParam");
//	fwrite(identify, sizeof(char), 19, fp);

	// num of class
	fprintf(fp,"%d\n",numClass);
//	fwrite(&numClass, sizeof(unsigned char), 1, fp);

	// num of feat
	fprintf(fp,"%d\n",numFeat);
//	fwrite(&numFeat, sizeof(unsigned int), 1, fp);

	// feature tags
	for(fcnt=0; fcnt<numFeat; fcnt++)
	{
		fprintf(fp,"%04x %04x,",param->featTable->tags[fcnt]->group,param->featTable->tags[fcnt]->element);
	//	fwrite(&param->featTable->tags[fcnt]->group, sizeof(unsigned short), 1, fp);
	//	fwrite(&param->featTable->tags[fcnt]->element, sizeof(unsigned short), 1, fp);
	}
	fprintf(fp,"\n");

	for(ccnt=0; ccnt<numClass; ccnt++)
	{
		// mean
		fprintf(fp,"mean[%02d]\n",ccnt);
		for(fcnt=0; fcnt<numFeat; fcnt++)
			fprintf(fp,"%le,",param->gauss[ccnt]->mean->dataMatrix[0][fcnt]);
		fprintf(fp,"\n");
	//	fwrite(param->gauss[ccnt]->mean->dataMatrix[0], sizeof(double), numFeat, fp);

		// co-var (only lower-triangular)
		fprintf(fp,"covar[%02d]\n",ccnt);
		for(fcnt=0; fcnt<numFeat; fcnt++)
		{
			for(unsigned int fcnt2=0; fcnt2<numFeat; fcnt2++)
				fprintf(fp,"%le,",param->gauss[ccnt]->covar->dataMatrix[fcnt][fcnt2]);
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n");
	//	for(fcnt=0; fcnt<numFeat; fcnt++)
	//		fwrite(param->gauss[ccnt]->covar->dataMatrix[fcnt], sizeof(double), fcnt+1, fp);
	}


	fclose(fp);

	return PR_EOT;
}

/*
PR_MAHALANOBISPARAM* PR_ReadTextMahalanobisParam(char* filename)
{
	PR_MAHALANOBISPARAM* param;
	return param;
}
*/


int 
PR_CalcMahalanobisDistRatioForCand(
	PR_CANDDATA* candData, unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn)
{
	PR_CASEDATA* caseData = (PR_CASEDATA*)candData->rootCaseData;
	PR_CASELIST* caseList = (PR_CASELIST*)caseData->rootCaseList;
	unsigned int numClass = caseList->numClass;

	if(numClass<idxNumerator || numClass<idxDenominator || numClass<idxResultIn)
	{
		fprintf(stderr,"error! numClass%d <-> rat=cls[%d]/cls[%d]->lhod[%d]\n",numClass,idxNumerator,idxDenominator,idxResultIn);
		return PR_AVEND;
	}

	if(candData->likelihood[idxDenominator]<=0.0)
	{
		fprintf(stderr,"error! lhod[%d]=%f<0.0\n",idxResultIn,candData->likelihood[idxResultIn]);
		return PR_AVEND;
	}

//	fprintf(stderr,"%.3e/%.3e=%.3e, ",candData->likelihood[idxNumerator],candData->likelihood[idxDenominator],
//										candData->likelihood[idxNumerator]/candData->likelihood[idxDenominator]);
	candData->likelihood[idxResultIn] = candData->likelihood[idxNumerator]/candData->likelihood[idxDenominator];
	return PR_EOT;
}

int
PR_CalcMahalanobisDistRatioForCase(
	PR_CASEDATA* caseData, unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn)
{
	PR_CANDDATA* candData=caseData->childNodeFirstCand;
	while(candData!=NULL)
	{
		int ret=PR_CalcMahalanobisDistRatioForCand(candData,idxNumerator,idxDenominator,idxResultIn);
		if(ret==PR_AVEND)	return PR_AVEND;
		candData=candData->nodeNextCand;
	}
	return PR_EOT;
}

int
PR_CalcMahalanobisDistRatioForList(
	PR_CASELIST* caseList, unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn)
{
	PR_CASEDATA* caseData=caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		int ret=PR_CalcMahalanobisDistRatioForCase(caseData,idxNumerator,idxDenominator,idxResultIn);
		if(ret==PR_AVEND)	return PR_AVEND;
		caseData=caseData->nodeNextCase;
	}
	return PR_EOT;
}


PR_MAHALANOBISPARAM* PR_GetMahalanobisParamOfSpecificFeatutes(PR_MAHALANOBISPARAM* source, PR_FEATTAGTABLE* destTable)
{
	PR_MAHALANOBISPARAM* param = PR_NewMahalanobisParam(source->numClass,destTable);
	printf("a");

	unsigned int* linkNum = new unsigned int [destTable->numFeat];

	for(unsigned int j=0; j<source->featTable->numFeat; j++)
	{
		for(unsigned int i=0; i<destTable->numFeat; i++)
		if(PR_CompareFeatTag(source->featTable->tags[j],destTable->tags[i]) == PR_EOT)
		{
			linkNum[i]=j;
			break;
		}
	}
	printf("b");

	for(unsigned char cls=0; cls<source->numClass; cls++)
	{
		for(unsigned int j=0; j<destTable->numFeat; j++)
		{
			param->gauss[cls]->mean->dataMatrix[0][j] = source->gauss[cls]->mean->dataMatrix[0][linkNum[j]];

			for(unsigned int i=0; i<destTable->numFeat; i++)
			{
				param->gauss[cls]->covar->dataMatrix[j][i] = source->gauss[cls]->covar->dataMatrix[linkNum[j]][linkNum[i]];
			}
		}
	}
	printf("c");

	delete [] linkNum;
	printf("d\n");

	return param;
}

