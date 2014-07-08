
#include "./prFeatureData_temp.h"
#include "./prFeatureData_temp.private.h"


PR_CASEDATATEMP*
PR_NewCaseDataTemp(unsigned char numClass, unsigned int numFeature)
{
	PR_CASEDATATEMP* data = new PR_CASEDATATEMP;
	
	data->idCase = 0;
	data->numCandidate = 0;
	data->numCandidateInClass = new unsigned int [numClass+1];
	memset(data->numCandidateInClass,0x00,sizeof(unsigned int)*(numClass+1));

	data->rootTable = NULL;
	data->specific = NULL;

	return data;
}


PR_ARRAYCASEDATATEMP*
PR_NewArrayCaseDataTemp(unsigned int numCase, unsigned char numClass, unsigned int numFeature)
{
	PR_ARRAYCASEDATATEMP* table = new PR_ARRAYCASEDATATEMP;

	table->numCandidate = 0;
	table->numCandidateInClass = new unsigned int [numClass+1];
	memset(table->numCandidateInClass,0x00,sizeof(unsigned int)*(numClass+1));
	table->numCase = numCase;
	table->numClass = numClass;

	table->tableFeature = PR_NewFeatTagTable(numFeature);
	
	table->temp = new PR_DATATEMPORAL;
	table->temp->candidateID = tmpfile();
	table->temp->classLabel = tmpfile();
	table->temp->cost = tmpfile();

//	table->temp->feature = new FILE* [numFeature];
//	for(unsigned int k=0; k<numFeature; k++)	table->temp->feature[k] = tmpfile();
	table->temp->feature = tmpfile();

	table->temp->likelihood = new FILE* [numClass];
	for(unsigned char j=0; j<numClass; j++)		table->temp->likelihood[j] = tmpfile();

	table->arrayCase = new PR_CASEDATATEMP* [numCase];
	for(unsigned int i=0; i<numCase; i++)
	{
	//	printf("%d/%d..",i,numCase);
		table->arrayCase[i] = PR_NewCaseDataTemp(numClass, numFeature);
		table->arrayCase[i]->rootTable = (void*)table;
	//	printf("\r");
	}

	return table;
}

PR_ARRAYCASEDATATEMP*
PR_NewArrayCaseDataTempWithoutArrayCase(unsigned char numClass, unsigned int numFeature)
{
	PR_ARRAYCASEDATATEMP* table = new PR_ARRAYCASEDATATEMP;
	printf("(a");

	table->temp = new PR_DATATEMPORAL;
	table->temp->candidateID = tmpfile();
	table->temp->classLabel = tmpfile();
	table->temp->cost = tmpfile();
	printf("e");

	table->temp->likelihood = new FILE* [numClass];
	for(unsigned char j=0; j<numClass; j++)		table->temp->likelihood[j] = tmpfile();
	printf("g");
	
	table->numCase = 0;
	table->numClass = numClass;
	printf("c");
	
	table->numCandidate = 0;
	table->numCandidateInClass = new unsigned int [numClass+1];
	memset(table->numCandidateInClass,0x00,sizeof(unsigned int)*(numClass+1));
	printf("b");

	table->tableFeature = PR_NewFeatTagTable(numFeature);
	printf("d");

//	table->temp->feature = new FILE* [numFeature];
//	for(unsigned int k=0; k<numFeature; k++)		table->temp->feature[k] = tmpfile();
	table->temp->feature = tmpfile();
	printf("f");

	table->arrayCase = NULL;
	printf(")");

	return table;
}

void PR_DeleteCaseDataTemp(PR_CASEDATATEMP* data)
{
	if(data->specific!=NULL)	delete data->specific;
	delete [] data->numCandidateInClass;

	delete data;
}

void PR_DeleteArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* ary)
{
	for(unsigned int i=0; i<ary->numCase; i++)		PR_DeleteCaseDataTemp(ary->arrayCase[i]);
	if(ary->numCase)	delete [] ary->arrayCase;

	fclose(ary->temp->candidateID);
	fclose(ary->temp->classLabel);
	fclose(ary->temp->cost);

//	for(unsigned int i=0; i<ary->tableFeature->numFeat; i++)	fclose(ary->temp->feature[i]);
//	delete [] ary->temp->feature;
	fclose(ary->temp->feature);

	for(unsigned char c=0; c<ary->numClass; c++)	fclose(ary->temp->likelihood[c]);
	delete [] ary->temp->likelihood;

	delete ary->temp; 

	delete [] ary->numCandidateInClass;
	PR_DeleteFeatTagTable(ary->tableFeature);

	delete ary;
}

PR_ARRAYCASEDATATEMP* PR_NewReadArrayCaseDataTemp(PR_TABLEINPUTFILE* inputFiles)
{
	unsigned char numClass=0;
	unsigned int numFeat=0;
	for(unsigned int i=0; i<inputFiles->num; i++)
	{
		PR_NUMCLASSANDFEATOFCASE* nums = PR_ReadNumClassAndNumFeatFromCaseFile(inputFiles->info[0]->name);
		if(numClass<nums->cls)		numClass = nums->cls;
		if(numFeat<nums->feat)		numFeat = nums->feat;
		delete nums;
	}
	printf("num: cls%d, feat%d\n",numClass,numFeat);

	PR_ARRAYCASEDATATEMP* data = PR_NewArrayCaseDataTemp(inputFiles->num,numClass,numFeat);
	PR_CASELIST* caseList = PR_NewCaseList(numClass,numFeat);
	PR_CANDDATA* candData;
	
	for(unsigned int i=0; i<inputFiles->num; i++)
	{
		printf("%d/%d; ",i+1,inputFiles->num);

		PR_CASEDATA* caseData = PR_ReadBinaryCaseData(caseList,inputFiles->info[i]->name);
	//	printf("id%04d(%d),",inputFiles->info[i]->idCase,caseData);
		data->arrayCase[i]->idCase = inputFiles->info[i]->idCase;
		
		if(i==0)
		for(unsigned int f=0; f<numFeat; f++)
		{
			data->tableFeature->tags[f]->group = caseList->featTagTable->tags[f]->group;
			data->tableFeature->tags[f]->element = caseList->featTagTable->tags[f]->element;
		//	printf("%d[%04x.%04x],",f,data->tableFeature->tags[f]->group,data->tableFeature->tags[f]->element);
		}
	//	printf("fTable,");


		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
	//		printf("cnt%d ",data->numCandidate);
			
			fwrite(&candData->idCand,sizeof(unsigned int),1,data->temp->candidateID);
			fwrite(&candData->classLabel,sizeof(unsigned char),1,data->temp->classLabel);
	//		printf("a");

	//		for(unsigned int f=0; f<numFeat; f++)
	//			fwrite(&candData->featValues[f],sizeof(float),1,data->temp->feature[f]);
	//		printf("b");

			double dmycost=1.0;
			fwrite(&dmycost,sizeof(double),1,data->temp->cost);

			data->numCandidate++;
			data->arrayCase[i]->numCandidate++;
	//		printf("c");

			if(candData->classLabel<numClass)
			{
				data->numCandidateInClass[candData->classLabel]++;
				data->arrayCase[i]->numCandidateInClass[candData->classLabel]++;
			}
			else
			{
				data->numCandidateInClass[numClass]++;
				data->arrayCase[i]->numCandidateInClass[numClass]++;
			}
	//		printf("d\r");
			
			candData = candData->nodeNextCand;
		}

		for(unsigned int iFeat=0; iFeat<numFeat; iFeat++)
		{
			candData = caseData->childNodeFirstCand;
			while(candData!=NULL)
			{
				fwrite(&candData->featValues[iFeat],sizeof(float),1,data->temp->feature);
				candData = candData->nodeNextCand;
			}
		}

	//	printf("temporary,");

	//	printf("\nnum:%d =",data->arrayCase[i]->numCandidate);
	//	for(unsigned char a=0; a<=data->numClass; a++)
	//		if(data->arrayCase[i]->numCandidateInClass[a])
	//			printf(" [%d]%d +",a,data->arrayCase[i]->numCandidateInClass[a]);
	//	printf("\b");

		PR_DeleteCaseData(caseData);
	//	printf("ok\n");
	}

	PR_DeleteCaseList(caseList);

	if(data->numCandidateInClass[numClass])
		fprintf(stderr,"num candidate label is over %d: %d\n",numClass,data->numCandidateInClass[numClass]);

//	printf("\nnum:%d =",data->numCandidate);
//	for(unsigned char a=0; a<=data->numClass; a++)
//		if(data->numCandidateInClass[a])
//			printf(" [%d]%d +",a,data->numCandidateInClass[a]);
//	printf("\b\n");

	return data;
}


int PR_CountNumCandidateInClassOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* data)
{
//	printf("- recount -\n");

	unsigned char numClass=data->numClass;
	memset(data->numCandidateInClass,0x00,sizeof(unsigned int)*(numClass+1));
	data->numCandidate = 0;

	for(unsigned int i=0; i<data->numCase; i++)
	{
//		printf("#%d=id%d; ",i,data->arrayCase[i]->idCase);
//		printf("num%d=",data->arrayCase[i]->numCandidate);

		memset(data->arrayCase[i]->numCandidateInClass,0x00,sizeof(unsigned int)*(numClass+1));
		unsigned char* aryLabel = PR_ExtractOneCaseArrayOfClassLabel(data->arrayCase[i]);

		for(unsigned int j=0; j<data->arrayCase[i]->numCandidate; j++)
		{
			unsigned char label = aryLabel[j];
			if(label>=numClass)		label = numClass;
			data->numCandidateInClass[label]++;
			data->arrayCase[i]->numCandidateInClass[label]++;
		}

//		for(unsigned char c=0; c<data->numClass; c++)
//			printf("[%d]%d(addsum%d)+",c,data->arrayCase[i]->numCandidateInClass[c],data->numCandidateInClass[c]);
//		printf("\n");
		
		data->numCandidate += data->arrayCase[i]->numCandidate;
		delete [] aryLabel;
	}

//	printf("total: num%d=",data->numCandidate);
//	for(unsigned char c=0; c<data->numClass; c++)
//		printf("[%d]%d+",c,data->numCandidateInClass[c]);
//	printf("\n");

	return PR_EOT;
}

float* PR_ExtractOneCaseArrayOfFullFeatures(PR_CASEDATATEMP* data)
{
	PR_ARRAYCASEDATATEMP* dataRoot = (PR_ARRAYCASEDATATEMP*)data->rootTable;

	unsigned int iCase, nCandPrev=0;
	for(iCase=0; iCase<dataRoot->numCase; iCase++)
	{
		if(data==dataRoot->arrayCase[iCase])	break;
		nCandPrev += dataRoot->arrayCase[iCase]->numCandidate;
	}
	if(iCase==dataRoot->numCase)
	{
		printf("unfound specified CaseDataTemp(id%04d) .. \n",data->idCase);
		return NULL;
	}

	unsigned int nFeat=dataRoot->tableFeature->numFeat;
	unsigned int nCandCurr=data->numCandidate;
	float* ary = new float [nCandCurr*nFeat];

	fseek(dataRoot->temp->feature,sizeof(float)*nCandPrev*nFeat,SEEK_SET);
	unsigned int sz = (unsigned int)fread(ary,sizeof(float),nCandCurr*nFeat,dataRoot->temp->feature);

	if(sz!=nCandCurr*nFeat)
	{
		printf("Error: extract one-case fullFeatArray (candID%d)\n",data->idCase);
		delete [] ary;
		return NULL;
	}

	return ary;
}

float* PR_ExtractFullArrayOfOneFeature(PR_ARRAYCASEDATATEMP* tableMeta, unsigned int idxFeature)
{
	unsigned int nCandPrev=0, nCandCurr, nFeat=tableMeta->tableFeature->numFeat;
	float* ary = new float [tableMeta->numCandidate];

	for(unsigned int i=0; i<tableMeta->numCase; i++)
	{
		nCandCurr=tableMeta->arrayCase[i]->numCandidate;

		fseek(tableMeta->temp->feature,sizeof(float)*(nCandPrev*nFeat+nCandCurr*idxFeature),SEEK_SET);
		unsigned int sz = (unsigned int)fread(ary+nCandPrev,sizeof(float),nCandCurr,tableMeta->temp->feature);

		if(sz!=nCandCurr)
		{
			printf("Error: extract full featArray (feat%d, cand%d)\n",idxFeature,i);
			delete [] ary;
			return NULL;
		}

		nCandPrev+=nCandCurr;
	}

	return ary;
}

float* PR_ExtractOneCaseArrayOfOneFeature(PR_CASEDATATEMP* data, unsigned int idxFeature)
{
	PR_ARRAYCASEDATATEMP* dataRoot = (PR_ARRAYCASEDATATEMP*)data->rootTable;

	unsigned int iCase, nCandPrev=0;
	for(iCase=0; iCase<dataRoot->numCase; iCase++)
	{
		if(data==dataRoot->arrayCase[iCase])	break;
		nCandPrev += dataRoot->arrayCase[iCase]->numCandidate;
	}
	if(iCase==dataRoot->numCase)
	{
		printf("unfound specified CaseDataTemp(id%04d) .. \n",data->idCase);
		return NULL;
	}

	unsigned int nFeat=dataRoot->tableFeature->numFeat;
	unsigned int nCandCurr=data->numCandidate;
	float* ary = new float [nCandCurr];

	fseek(dataRoot->temp->feature,sizeof(float)*(nCandPrev*nFeat+nCandCurr*idxFeature),SEEK_SET);
	unsigned int sz = (unsigned int)fread(ary,sizeof(float),nCandCurr,dataRoot->temp->feature);

	if(sz!=nCandCurr)
	{
		printf("Error: extract one-case featArray (feat%d, candID%d)\n",idxFeature,data->idCase);
		delete [] ary;
		return NULL;
	}

	return ary;
}

unsigned char* PR_ExtractFullArrayOfClassLabel(PR_ARRAYCASEDATATEMP* tableMeta)
{
	unsigned int readSz, num=tableMeta->numCandidate;
	unsigned char* ary = new unsigned char [num];

	rewind(tableMeta->temp->classLabel);
	readSz = (unsigned int)fread(ary,sizeof(unsigned char),num,tableMeta->temp->classLabel);

//	if(readSz!=sizeof(unsigned char)*num)
	if(readSz!=num)
	{
		printf("error: extracting full array label (sz%d)\n",readSz);
		delete [] ary;
		return NULL;
	}

	return ary;
}

unsigned char* PR_ExtractOneCaseArrayOfClassLabel(PR_CASEDATATEMP* data)
{
	PR_ARRAYCASEDATATEMP* root = (PR_ARRAYCASEDATATEMP*)data->rootTable;
	unsigned int cidx, sz, num=data->numCandidate, addsum=0;
	
	for(cidx=0; cidx<root->numCase; cidx++)
	{
		if(data==root->arrayCase[cidx])	break;
		addsum += root->arrayCase[cidx]->numCandidate;
	}

	if(cidx==root->numCase)
	{
		printf("unfound specified CaseDataTemp(id%04d) .. \n",data->idCase);
		return NULL;
	}
	
	unsigned char* ary = new unsigned char [num];

	FILE* fp = root->temp->classLabel;
	fseek(fp,sizeof(unsigned char)*addsum,SEEK_SET);
	sz = (unsigned int)fread(ary,sizeof(unsigned char),num,fp);

//	if(sz!=sizeof(unsigned char)*num)
	if(sz!=num)
	{
		printf("error: extracting one-case array classLabel\n");
		delete [] ary;
		return NULL;
	}

	return ary;
}

double* PR_ExtractFullArrayOfCost(PR_ARRAYCASEDATATEMP* tableMeta)
{
	unsigned int readSz, num=tableMeta->numCandidate;
	double* ary = new double [num];

	rewind(tableMeta->temp->cost);
	readSz = (unsigned int)fread(ary,sizeof(double),num,tableMeta->temp->cost);

//	if(readSz!=sizeof(double)*num)
	if(readSz!=num)
	{
		printf("error: extracting full array cost\n");
		delete [] ary;
		return NULL;
	}

	return ary;
}

double* PR_ExtractOneCaseArrayOfCost(PR_CASEDATATEMP* data)
{
	PR_ARRAYCASEDATATEMP* root = (PR_ARRAYCASEDATATEMP*)data->rootTable;
	unsigned int cidx, sz, num=data->numCandidate, addsum=0;
	
	for(cidx=0; cidx<root->numCase; cidx++)
	{
		if(data==root->arrayCase[cidx])	break;
		addsum += root->arrayCase[cidx]->numCandidate;
	}

	if(cidx==root->numCase)
	{
		printf("unfound specified CaseDataTemp(id%04d) .. \n",data->idCase);
		return NULL;
	}
	
	double* ary = new double [num];

	fseek(root->temp->cost,sizeof(double)*addsum,SEEK_SET);
	sz = (unsigned int)fread(ary,sizeof(double),num,root->temp->cost);

//	if(sz!=sizeof(double)*num)
	if(sz!=num)
	{
		printf("error: extracting one-case array cost\n");
		delete [] ary;
		return NULL;
	}

	return ary;
}

double* PR_ExtractFullArrayOfLikelihood(PR_ARRAYCASEDATATEMP* tableMeta, unsigned char classLabel)
{
	unsigned int readSz, num=tableMeta->numCandidate;
	double* ary = new double [num];

	rewind(tableMeta->temp->likelihood[classLabel]);
	readSz = (unsigned int)fread(ary,sizeof(double),num, tableMeta->temp->likelihood[classLabel]);

//	if(readSz!=sizeof(double)*num)
	if(readSz!=num)
	{
		printf("error: extracting full array likelihood(L%d,sz%d/%d)\n",classLabel,readSz,num);
		delete [] ary;
		return NULL;
	}

	return ary;
}

double* PR_ExtractOneCaseArrayOfLikelihood(PR_CASEDATATEMP* data, unsigned char classLabel)
{
	PR_ARRAYCASEDATATEMP* root = (PR_ARRAYCASEDATATEMP*)data->rootTable;
	unsigned int cidx, sz, num=data->numCandidate, addsum=0;
	
	for(cidx=0; cidx<root->numCase; cidx++)
	{
		if(data==root->arrayCase[cidx])	break;
		addsum += root->arrayCase[cidx]->numCandidate;
	}

	if(cidx==root->numCase)
	{
		printf("unfound specified CaseDataTemp(id%04d) .. \n",data->idCase);
		return NULL;
	}
	
	double* ary = new double [num];

	FILE* fp = root->temp->likelihood[classLabel];
	rewind(fp);
	fseek(fp,sizeof(double)*addsum,SEEK_SET);
	sz = (unsigned int)fread(ary,sizeof(double),num,fp);

//	if(sz!=sizeof(double)*num)
	if(sz!=num)
	{
		printf("error: extracting one-case array likelihood(%d)\n",classLabel);
		delete [] ary;
		return NULL;
	}

	return ary;
}

unsigned int* PR_ExtractFullArrayOfCandidateID(PR_ARRAYCASEDATATEMP* tableMeta)
{
	unsigned int readSz, num=tableMeta->numCandidate;
	unsigned int* ary = new unsigned int [num];

	rewind(tableMeta->temp->candidateID);
	readSz = (unsigned int)fread(ary,sizeof(unsigned int),num,tableMeta->temp->candidateID);

	if(readSz!=num)
	{
		printf("error: extracting full array candidateID\n");
		delete [] ary;
		return NULL;
	}

	return ary;
}

unsigned int* PR_ExtractOneCaseArrayOfCandidateID(PR_CASEDATATEMP* data)
{
	PR_ARRAYCASEDATATEMP* root = (PR_ARRAYCASEDATATEMP*)data->rootTable;
	unsigned int cidx, sz, num=data->numCandidate, addsum=0;
	
	for(cidx=0; cidx<root->numCase; cidx++)
	{
		if(data==root->arrayCase[cidx])		break;
		addsum += root->arrayCase[cidx]->numCandidate;
	}

	if(cidx==root->numCase)
	{
		printf("unfound specified CaseDataTemp(id%04d) .. \n",data->idCase);
		return NULL;
	}
	
	unsigned int* ary = new unsigned int [num];

	FILE* fp = root->temp->candidateID;
	rewind(fp);
	fseek(fp,sizeof(unsigned int)*addsum,SEEK_SET);
	sz = (unsigned int)fread(ary,sizeof(unsigned int),num,fp);

	if(sz!=num)
	{
		printf("error: extracting one-case array candidateID\n");
		delete [] ary;
		return NULL;
	}

	return ary;
}


#define OUT_FEATTAG(a)	a->group,a->element
int PR_RestructCaseDataByNewFeatTableTemp(PR_ARRAYCASEDATATEMP* data, PR_FEATTAGTABLE* tableNewFeature)
{
	unsigned int i2;
	unsigned int fnumOld=data->tableFeature->numFeat, fnumNew=tableNewFeature->numFeat;
	
	for(i2=0; i2<tableNewFeature->numFeat; i2++)
	if(PR_ExtractFeatNumberInFeatTagTable(data->tableFeature,tableNewFeature->tags[i2])==data->tableFeature->numFeat)
	{
		fprintf(stderr,"error: unexist-feat[%04x.%04x]\n",OUT_FEATTAG(tableNewFeature->tags[i2]));
		getchar();
		exit(1);
	}
	
	unsigned int* newNumbers = new unsigned int [fnumNew];
	
//	printf("\n");
//	printf("remainFeats; ");
	for(unsigned int i=0; i<fnumNew; i++)
	{
		newNumbers[i] = PR_ExtractFeatNumberInFeatTagTable(data->tableFeature,tableNewFeature->tags[i]);
//		printf("[%d]%d,",i,newNumbers[i]);
	}
//	printf("\n");

	FILE* oldTempFeat = data->temp->feature;
//	printf("temporal; %s and ",oldTempFeat->_tmpfname);
	rewind(oldTempFeat);

	data->temp->feature = tmpfile();
//	printf("%s\n",data->temp->feature->_tmpfname);
	rewind(data->temp->feature);

	for(unsigned int iCase=0; iCase<data->numCase; iCase++)
	{
		unsigned int nCandInCase = data->arrayCase[iCase]->numCandidate;

		float* dmyArray = new float [nCandInCase*fnumOld];
		fread(dmyArray,sizeof(float),nCandInCase*fnumOld,oldTempFeat);

		for(unsigned int iFeat=0; iFeat<fnumNew; iFeat++)
		{
			unsigned int idxFeat=newNumbers[iFeat];
			fwrite(&dmyArray[idxFeat*nCandInCase],sizeof(float),nCandInCase,data->temp->feature);
		}

		delete [] dmyArray;
	}

	delete [] newNumbers;
	fclose(oldTempFeat);

	PR_DeleteFeatTagTable(data->tableFeature);
	data->tableFeature = PR_NewFeatTagTable(fnumNew);
	PR_CopyFeatTagTable(data->tableFeature,tableNewFeature);
	
	return PR_EOT;
}


int PR_ReplaceClassLabelInArrayClassDataTemp(PR_ARRAYCASEDATATEMP* data, unsigned char* source)
{
//	unsigned int num=0, writeSz, numInCase;
//	FILE* fp;

	fclose(data->temp->classLabel);
	data->temp->classLabel = tmpfile();
	
	unsigned int num = data->numCandidate;
	unsigned int size = (unsigned int)fwrite(source,sizeof(unsigned char),num,data->temp->classLabel);

	if(size!=num)
	{
		printf("error: replace label(sz%d,n%d)\n",size,num);
		return PR_AVEND;
	}

	/*
	for(unsigned int i=0; i<dataTemp->numCase; i++)
	{
		numInCase = dataTemp->arrayCase[i]->numCandidate;
		fp = dataTemp->arrayCase[i]->tempClass;
		fclose(fp);
		fp = tmpfile();
		writeSz = (unsigned int)fwrite(source+num,sizeof(unsigned char),numInCase,fp);

		if(writeSz!=sizeof(unsigned char)*numInCase)
		{
			printf("class numCand error! (id%d)\n",dataTemp->arrayCase[i]->idCase);
			return PR_AVEND;
		}

		num += dataTemp->arrayCase[i]->numCandidate;
		rewind(fp);
	}
	*/
	
	return PR_EOT;
}


int PR_BandPassForOneFeatureTemp(PR_ARRAYCASEDATATEMP* data, unsigned int idFeat, float min, float max)
{

	fprintf(stderr,"band-pass: feat#%d=[%04x.%04x]\n",idFeat,data->tableFeature->tags[idFeat]->group,data->tableFeature->tags[idFeat]->element);

	PR_CountNumCandidateInClassOfArrayCaseDataTemp(data);
	fprintf(stderr,"n=%d(",data->numCandidate);
	for(unsigned int k=0; k<data->numCase; k++)
		fprintf(stderr,"%d,",data->numCandidateInClass[k]);
	fprintf(stderr,"\b)\n");


	unsigned int numNew = 0;
	unsigned int* numNewInCase = new unsigned int [data->numCase];
	memset(numNewInCase,0x00,sizeof(unsigned int)*data->numCase);
	FILE* tempBoo = tmpfile();


	unsigned int* cntRemain = new unsigned int [data->numClass+1];

	for(unsigned int k=0; k<data->numCase; k++)
	{
	//	printf("#%d=case%d;",k,data->arrayCase[k]->idCase);

		float* aryfeat = PR_ExtractOneCaseArrayOfOneFeature(data->arrayCase[k],idFeat);
		char* boo = new char [data->arrayCase[k]->numCandidate];
		memset(boo,0x00,sizeof(char)*data->arrayCase[k]->numCandidate);

		unsigned char* label = PR_ExtractOneCaseArrayOfClassLabel(data->arrayCase[k]);
		memset(cntRemain,0x00,sizeof(unsigned int)*(data->numClass+1));

		for(unsigned int i=0; i<data->arrayCase[k]->numCandidate; i++)
		if(aryfeat[i]>=min && aryfeat[i]<=max)
		{
			boo[i]=1;
			numNew++;
			numNewInCase[k]++;
			cntRemain[label[i]]++;
		}

	//	printf("num%d/%d (addsum%d)\n",numNewInCase[k],data->arrayCase[k]->numCandidate,numNew);
		fwrite(boo,sizeof(char),data->arrayCase[k]->numCandidate,tempBoo);
		delete [] aryfeat;
		delete [] boo;
		delete [] label;
	}

	delete [] cntRemain;


	
	size_t sz;

	PR_DATATEMPORAL* oldTemp = data->temp;
	data->temp = new PR_DATATEMPORAL;
//	data->temp->feature = new FILE* [data->tableFeature->numFeat];
	data->temp->likelihood = new FILE* [data->numClass];
//	printf("&old=%d, &new=%d\n",oldTemp,data->temp);

	rewind(tempBoo);
	rewind(oldTemp->candidateID);
	rewind(oldTemp->classLabel);
	rewind(oldTemp->cost);

	data->temp->candidateID = tmpfile();
	data->temp->classLabel = tmpfile();
	data->temp->cost = tmpfile();
	
	unsigned int cnt=0,cntId=0,cntLabel=0,cntCost=0;

	for(unsigned int k=0; k<data->numCase; k++)
	{
		unsigned int cnum=data->arrayCase[k]->numCandidate;

		char* boo = new char [cnum];
		sz = fread(boo,sizeof(char),cnum,tempBoo);

		unsigned int* id = new unsigned int [cnum];
		sz = fread(id,sizeof(unsigned int),cnum,oldTemp->candidateID);

		unsigned char* cls = new unsigned char [cnum];
		sz = fread(cls,sizeof(unsigned char),cnum,oldTemp->classLabel);

		double* cost = new double [cnum];
		sz = fread(cost,sizeof(double),cnum,oldTemp->cost);

		for(unsigned int i=0; i<cnum; i++)
		if(boo[i])
		{
			fwrite(id+i,sizeof(unsigned int),1,data->temp->candidateID);
			fwrite(cls+i,sizeof(unsigned char),1,data->temp->classLabel);
			fwrite(cost+i,sizeof(double),1,data->temp->cost);
		}

		delete [] boo;
		delete [] id;
		delete [] cls;
		delete [] cost;
	}

	fclose(oldTemp->candidateID);
	fclose(oldTemp->classLabel);
	fclose(oldTemp->cost);
//	printf("id&label&cost,");


	
//	for(unsigned int f=0; f<data->tableFeature->numFeat; f++)
//	{
//		rewind(oldTemp->feature[f]);
//		data->temp->feature[f] = tmpfile();
//	}

	rewind(tempBoo);
	rewind(oldTemp->feature);

	for(unsigned int k=0; k<data->numCase; k++)
	{
		unsigned int nCand=data->arrayCase[k]->numCandidate;

		char* boo = new char [nCand];
		sz = fread(boo,sizeof(char),nCand,tempBoo);

	//	for(unsigned int f=0; f<data->tableFeature->numFeat; f++)
	//	{
	//		float* feat = new float [cnum];
	//		sz = fread(feat,sizeof(float),cnum,oldTemp->feature[f]);
	//		
	//		for(unsigned int i=0; i<cnum; i++)
	//			if(boo[i])	fwrite(feat+i,sizeof(float),1,data->temp->feature[f]);
	//
	//		delete [] feat;
	//	}
		
		unsigned int nData=nCand*data->tableFeature->numFeat;
		float* aryFeat = new float [nData];
		sz = fread(aryFeat,sizeof(float),nData,oldTemp->feature);

		unsigned int iFeat,iCand;
		for(iFeat=0; iFeat<data->tableFeature->numFeat; iFeat++)
		for(iCand=0; iCand<nCand; iCand++)
		{
			if(boo[iCand])	fwrite(&aryFeat[iFeat*nCand+iCand],sizeof(float),1,data->temp->feature);
		}

		delete [] aryFeat;
		delete [] boo;
	}
	
//	for(unsigned int f=0; f<data->tableFeature->numFeat; f++)
//		fclose(oldTemp->feature[f]);
//	printf("feat,");
	fclose(oldTemp->feature);


	rewind(tempBoo);
	for(unsigned char c=0; c<data->numClass; c++)
	{
		rewind(oldTemp->likelihood[c]);
		data->temp->likelihood[c] = tmpfile();
	}

	for(unsigned int k=0; k<data->numCase; k++)
	{
		unsigned int cnum=data->arrayCase[k]->numCandidate;

		char* boo = new char [cnum];
		sz = fread(boo,sizeof(char),cnum,tempBoo);

		for(unsigned char c=0; c<data->numClass; c++)
		{
			double* lhod = new double [cnum];
			sz = fread(lhod,sizeof(double),cnum,oldTemp->likelihood[c]);

			for(unsigned int i=0; i<cnum; i++)
				if(boo[i])	fwrite(lhod+i,sizeof(double),1,data->temp->likelihood[c]);

			delete [] lhod;
		}

		delete [] boo;
	}

	for(unsigned char c=0; c<data->numClass; c++)
		fclose(oldTemp->likelihood[c]);
//	printf("lhod,");


	fclose(tempBoo);
	delete [] oldTemp->feature;
	delete [] oldTemp->likelihood;
	delete oldTemp;

	
	for(unsigned int k=0; k<data->numCase; k++)
		data->arrayCase[k]->numCandidate = numNewInCase[k];
	data->numCandidate = numNew;

	delete [] numNewInCase;


	PR_CountNumCandidateInClassOfArrayCaseDataTemp(data);
	fprintf(stderr," -> %d(",data->numCandidate);
	for(unsigned int k=0; k<data->numCase; k++)
		fprintf(stderr,"%d,",data->numCandidateInClass[k]);
	fprintf(stderr,"\b)\n");
//	printf("ok!\n");


	return PR_EOT;
}

