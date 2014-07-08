


#include "./prMahalanobis_temp.h"
#include "./prMahalanobis_temp.private.h"


PR_MAHALANOBISPARAM* 
PR_NewCalclateMahalanobisParameter_Temp(PR_ARRAYCASEDATATEMP* aryCase)
{
	if(aryCase==NULL)	return NULL;

	printf("new,");
	PR_MAHALANOBISPARAM* param = PR_NewMahalanobisParam(aryCase->numClass, aryCase->tableFeature);
	printf("calc,\n");
	PR_CalculateMahalanobisParam_Temp(aryCase, param);
	return param;
}

/**/
int 
PR_CalcMahalanobisDistOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* aryCase, PR_MAHALANOBISPARAM* param)
{
	if(aryCase==NULL || param==NULL)	return PR_AVEND;

	for(unsigned int i=0; i<aryCase->numCase; i++)
	{
		PR_CalcMahalanobisDistOfCaseDataTemp(aryCase->arrayCase[i],param);
	}

	return PR_EOT;
}
/**/

int 
PR_CalcMahalanobisDistOfCaseDataTemp(PR_CASEDATATEMP* caseData, PR_MAHALANOBISPARAM* param)
{
	for(unsigned char cc=0; cc<param->numClass; cc++)
		PR_CalcOneClassMahalanobisDistOfCaseDataTemp(caseData,param,cc);

	return PR_EOT;
}

int 
PR_CalcOneClassMahalanobisDistOfCaseDataTemp(
	PR_CASEDATATEMP* caseData, PR_MAHALANOBISPARAM* param, unsigned char targetClass)
{
	if(caseData==NULL || param==NULL)
	{
		printf("error1");
		return PR_AVEND;
	}
	if(param->numClass<=targetClass)
	{
		printf("error2");
		return PR_AVEND;
	}


	unsigned int i, j;
	PR_ARRAYCASEDATATEMP* root = (PR_ARRAYCASEDATATEMP*)caseData->rootTable;
	PR_GAUSSPARAM* gParam = param->gauss[targetClass];


	unsigned int crank,start=0;
	for(crank=0; crank<root->numCase; crank++)
	{
		if(root->arrayCase[crank]==caseData)	break;
		start += root->arrayCase[crank]->numCandidate;
	}
//	printf("st%d(/full%d) ",start,root->numCandidate);
	

	unsigned int nFeat = gParam->fTable->numFeat;
	unsigned int* fLinkage = new unsigned int [nFeat];
	for(i=0; i<nFeat; i++)
	{
//		printf("#%d(%04x.%04x)=",i,gParam->fTable->tags[i]->group,gParam->fTable->tags[i]->element);
		fLinkage[i] = PR_ExtractFeatNumberInFeatTagTable(root->tableFeature,gParam->fTable->tags[i]);
//		printf("%d/%d",fLinkage[i],root->tableFeature->numFeat);
//		fseek(root->temp->feature[fLinkage[i]],sizeof(float)*start,SEEK_SET);
//		printf(",");
	}
//	printf("\nlink,seek,");


	PR_MATHMATRIX* sub1 = PR_NewMathMatrix(gParam->fTable->numFeat,1);
	PR_MATHMATRIX* sub2 = PR_NewMathMatrix(1,gParam->fTable->numFeat);
	PR_MATHMATRIX* invCoVar = PR_InverseMatrix(gParam->covar);	
	double* output = new double [caseData->numCandidate];
//	printf("set,");

	unsigned int nCand=caseData->numCandidate;
	unsigned int nFeatInMeta=root->tableFeature->numFeat;

	float* aryFullFeat = PR_ExtractOneCaseArrayOfFullFeatures(caseData);

	for(j=0; j<caseData->numCandidate; j++)
	{
		for(i=0; i<nFeat; i++)
		{
			float value = aryFullFeat[i*nCand+j];
	//		fread(&value,sizeof(float),1,root->temp->feature[fLinkage[i]]);
			sub1->dataMatrix[0][i] = gParam->mean->dataMatrix[0][i]-(double)value;
			sub2->dataMatrix[i][0] = sub1->dataMatrix[0][i];
		}

		PR_MATHMATRIX* dmy1 = PR_MultiplyMathMatrix(sub1, invCoVar);
		PR_MATHMATRIX* dmy2 = PR_MultiplyMathMatrix(dmy1, sub2);
	//	currentCand->likelihood[targetClass] = dmy2->dataMatrix[0][0];
	//	fwrite(&dmy2->dataMatrix[0][0],sizeof(double),1,caseData->tempLikelihood[targetClass]);
		output[j] = dmy2->dataMatrix[0][0];

		PR_DeleteMathMatrix(dmy1);
		PR_DeleteMathMatrix(dmy2);
	}
//	printf("calc,");

	delete [] fLinkage;
	PR_DeleteMathMatrix(sub1);
	PR_DeleteMathMatrix(sub2);
	PR_DeleteMathMatrix(invCoVar);
	delete [] aryFullFeat;
//	printf("del,");


	double* dmylhod = PR_ExtractFullArrayOfLikelihood(root,targetClass);
	if(dmylhod==NULL)	dmylhod = new double [root->numCandidate];
	
	fclose(root->temp->likelihood[targetClass]);
	root->temp->likelihood[targetClass] = tmpfile();

	memcpy(dmylhod+start,output,sizeof(double)*caseData->numCandidate);
	size_t sz = fwrite(dmylhod,sizeof(double),root->numCandidate,root->temp->likelihood[targetClass]);
//	printf("wSz%d,",sz);

	delete [] output;
	delete [] dmylhod;


	return PR_EOT;
}

/*
int 
PR_CalcMahalanobisDistOfCaseDataTemp(PR_CASEDATATEMP* caseData, PR_MAHALANOBISPARAM* param)
{
	if(caseData==NULL || param==NULL)	return PR_AVEND;

	for(unsigned char cc=0; cc<param->numClass; cc++)
		PR_CalculateMahalanobisDistanceOneClassForCase_Temp(caseData,param,cc);

	return PR_EOT;
} 
*/

int 
PR_CalcLikelihoodRatioOfArrayCaseDataTemp(
	PR_ARRAYCASEDATATEMP* data,  unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn)
{
	unsigned int i,j;
	FILE* tmpRat = tmpfile();
	
	for(j=0; j<data->numCase; j++)
	{
		double* lhod1 = PR_ExtractOneCaseArrayOfLikelihood(data->arrayCase[j],idxNumerator);
		double* lhod2 = PR_ExtractOneCaseArrayOfLikelihood(data->arrayCase[j],idxDenominator);
		
		for(i=0; i<data->arrayCase[j]->numCandidate; i++)
		{
			double dmy = lhod1[i]/lhod2[i];
			fwrite(&dmy,sizeof(double),1,tmpRat);
		}

		delete [] lhod1;
		delete [] lhod2;
	}

	fclose(data->temp->likelihood[idxResultIn]);
	data->temp->likelihood[idxResultIn] = tmpfile();
	rewind(tmpRat);

	for(j=0; j<data->numCase; j++)
	{
		double* rat = new double [data->arrayCase[j]->numCandidate];
		fread(rat,sizeof(double),data->arrayCase[j]->numCandidate,tmpRat);
		fwrite(rat,sizeof(double),data->arrayCase[j]->numCandidate,data->temp->likelihood[idxResultIn]);
		delete [] rat;
	}

	/*
	unsigned int num=data->numCandidate;
	
	double* lhod1 = new double [num];
	double* lhod2 = new double [num];

	rewind(data->temp->likelihood[idxNumerator]);
	rewind(data->temp->likelihood[idxDenominator]);
	fread(lhod1,sizeof(double),num,data->temp->likelihood[idxNumerator]);
	fread(lhod2,sizeof(double),num,data->temp->likelihood[idxDenominator]);
	
	fclose(data->temp->likelihood[idxResultIn]);
	data->temp->likelihood[idxResultIn] = tmpfile();

	for(i=0; i<num; i++)
	{
		double dmy = lhod1[i]/lhod2[i];
		fwrite(&dmy,sizeof(double),1,data->temp->likelihood[idxResultIn]);
	}

	delete [] lhod1;
	delete [] lhod2;
	*/

	return PR_EOT;
}


//PR_CalcOneClassMahalanobisDistOfCaseDataTemp(
//	PR_CASEDATATEMP* caseData, PR_MAHALANOBISPARAM* param, unsigned char targetClass)

int 
PR_CalculateMahalanobisDistanceOneClassForArrayCase_Temp(
	PR_ARRAYCASEDATATEMP* data, PR_MAHALANOBISPARAM* param, unsigned char targetClass)
{
	if(data==NULL || param==NULL)	return PR_AVEND;
	if(param->numClass>=targetClass)	return PR_AVEND;

	for(unsigned int iCase=0; iCase<data->numCase; iCase++)
		PR_CalcOneClassMahalanobisDistOfCaseDataTemp(data->arrayCase[iCase],param,targetClass);

/*
	unsigned int i, j;
	PR_GAUSSPARAM* gParam = param->gauss[targetClass];


	unsigned int nFeat = gParam->fTable->numFeat;
	unsigned int* fLinkage = new unsigned int [nFeat];
	for(i=0; i<nFeat; i++)
	{
		fLinkage[i] = PR_ExtractFeatNumberInFeatTagTable(data->tableFeature,gParam->fTable->tags[i]);
		rewind(data->temp->feature[fLinkage[i]]);
	}


	PR_MATHMATRIX* sub1 = PR_NewMathMatrix(gParam->fTable->numFeat,1);
	PR_MATHMATRIX* sub2 = PR_NewMathMatrix(1,gParam->fTable->numFeat);
	PR_MATHMATRIX* invCoVar = PR_InverseMatrix(gParam->covar);

	fclose(data->temp->likelihood[targetClass]);
	data->temp->likelihood[targetClass] = tmpfile();

	for(j=0; j<data->numCandidate; j++)
	{
		for(i=0; i<nFeat; i++)
		{
			float value;
			fread(&value,sizeof(float),1,data->temp->feature[fLinkage[i]]);
			sub1->dataMatrix[0][i] = gParam->mean->dataMatrix[0][i]-(double)value;
			sub2->dataMatrix[i][0] = sub1->dataMatrix[0][i];
		}

		PR_MATHMATRIX* dmy1 = PR_MultiplyMathMatrix(sub1, invCoVar);
		PR_MATHMATRIX* dmy2 = PR_MultiplyMathMatrix(dmy1, sub2);

		fwrite(&dmy2->dataMatrix[0][0],sizeof(double),1,data->temp->likelihood[targetClass]);

		PR_DeleteMathMatrix(dmy1);
		PR_DeleteMathMatrix(dmy2);
	}


	delete [] fLinkage;
	PR_DeleteMathMatrix(sub1);
	PR_DeleteMathMatrix(sub2);
	PR_DeleteMathMatrix(invCoVar);
*/

	return PR_EOT;
}


int 
PR_CalculateMahalanobisParam_Temp(PR_ARRAYCASEDATATEMP* data, PR_MAHALANOBISPARAM* param)
{
	if(data==NULL || param==NULL)
	{
		printf("error1: PR_CalculateMahalanobisParam_Temp\n");
		return PR_AVEND;
	}

	unsigned char c;
	unsigned int i, j, j1, j2, k;
	printf("set\n");


	///-- linkage between "data->tableFeature" and "param->featTable"
	unsigned int* fLinkage = new unsigned int [param->featTable->numFeat];
	for(i=0; i<param->featTable->numFeat; i++)
	{
		fLinkage[i] = PR_ExtractFeatNumberInFeatTagTable(data->tableFeature,param->featTable->tags[i]);
		if(fLinkage[i]>=data->tableFeature->numFeat)
		{
			printf("error2: PR_CalculateMahalanobisParam_Temp\n");
			return PR_AVEND;
		}
		printf("#%d=%d, ",i,fLinkage[i]);
	}
	printf("link\n");
	

	///-- average
	for(k=0; k<data->numCase; k++)
	{
		unsigned char* arylabel = PR_ExtractOneCaseArrayOfClassLabel(data->arrayCase[k]);

		for(j=0; j<param->featTable->numFeat; j++)
		{
			unsigned int idx = fLinkage[j];
			float* aryfeat = PR_ExtractOneCaseArrayOfOneFeature(data->arrayCase[k],idx);

			for(i=0; i<data->arrayCase[k]->numCandidate; i++)
			{
				unsigned char ldmy = arylabel[i];
				double ndmy = (double)data->numCandidateInClass[ldmy];
				param->gauss[ldmy]->mean->dataMatrix[0][j] += (double)aryfeat[i]/ndmy;
			}

			delete [] aryfeat;
		}

		delete [] arylabel;
	}
	printf("avg1,");

//	for(c=0; c<param->numClass; c++)
//		for(j=0; j<param->featTable->numFeat; j++)
//			param->gauss[c]->mean->dataMatrix[0][j] /= (double)data->numCandidateInClass[c];
//	printf("2,");


	///-- co-variance
	for(k=0; k<data->numCase; k++)
	{
		unsigned char* arylabel = PR_ExtractOneCaseArrayOfClassLabel(data->arrayCase[k]);

		for(j1=0; j1<param->featTable->numFeat; j1++)
		{
			unsigned int idx1 = fLinkage[j1];
			float* aryfeat1 = PR_ExtractOneCaseArrayOfOneFeature(data->arrayCase[k],idx1);

			for(j2=j1; j2<param->featTable->numFeat; j2++)
			{
				unsigned int idx2 = fLinkage[j2];
				float* aryfeat2;
				if(j2==j1)	aryfeat2 = aryfeat1;
				else		aryfeat2 = PR_ExtractOneCaseArrayOfOneFeature(data->arrayCase[k],idx2);

				for(i=0; i<data->arrayCase[k]->numCandidate; i++)
				{
					unsigned char ldmy = arylabel[i];
					double ndmy = (double)data->numCandidateInClass[ldmy]-1.0;
				//	unsigned char label = arylabel[i];
					double sub1 = (double)aryfeat1[i]-param->gauss[ldmy]->mean->dataMatrix[0][j1];
					double sub2 = (double)aryfeat2[i]-param->gauss[ldmy]->mean->dataMatrix[0][j2];
					param->gauss[ldmy]->covar->dataMatrix[j1][j2] += sub1*sub2/ndmy;
				}

				if(j2!=j1)	delete [] aryfeat2;
			}

			delete [] aryfeat1;
		}

		delete [] arylabel;
	}
	printf("covar1,");

	for(c=0; c<param->numClass; c++)
	{
		for(j1=0; j1<param->featTable->numFeat; j1++)
		for(j2=j1; j2<param->featTable->numFeat; j2++)
		{
//			param->gauss[c]->covar->dataMatrix[j1][j2] /= (double)data->numCandidateInClass[c]-1.0;
			param->gauss[c]->covar->dataMatrix[j2][j1] = param->gauss[c]->covar->dataMatrix[j1][j2];
		}
	}
	printf("2,");


	delete [] fLinkage;


	printf("ok!\n");
	return PR_EOT;
}

