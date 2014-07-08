
#pragma warning(disable: 4996)

#include "prCaseDataIO.h"
#include "prCaseDataIO.private.h"

#include "../LibPR.h"


int PR_WriteBinaryCaseDataForFILE(FILE* fp, PR_CASEDATA* caseData, char booOutLikelihood)
{
	PR_CASELIST* rootCaseList = (PR_CASELIST*)caseData->rootCaseList;
	if(rootCaseList==NULL)	return PR_AVEND;

	// caseID
	fwrite(&caseData->idCase, sizeof(unsigned int), 1, fp);

	// numClass
	unsigned int numClass = (unsigned int)rootCaseList->numClass;
	fwrite(&numClass, sizeof(unsigned int), 1, fp);

	// numCandOfClass
	fwrite(caseData->numCandOfClass, sizeof(unsigned int), numClass, fp);

	// numCand = sum(numCandOfClass[i];0<=i<numClass)
	fwrite(&caseData->numCand, sizeof(unsigned int), 1, fp);

	// numFeat
	fwrite(&rootCaseList->featTagTable->numFeat, sizeof(unsigned int), 1, fp);

	// featTagTable
	unsigned int nfeat;
	for(nfeat=0; nfeat<rootCaseList->featTagTable->numFeat; nfeat++)
	{
		fwrite(&rootCaseList->featTagTable->tags[nfeat]->group, sizeof(unsigned short), 1, fp);
		fwrite(&rootCaseList->featTagTable->tags[nfeat]->element, sizeof(unsigned short), 1, fp);
	}

	// flagOutLikeloods -> numOutLikelihoods
	unsigned int numOutLikelihoodClass = (unsigned int)numClass;
	if(!booOutLikelihood)	numOutLikelihoodClass = 0;
	fwrite(&numOutLikelihoodClass, sizeof(unsigned int), 1, fp);

	// sizeSubVolume (in preparation)
	unsigned int sizeSubVolume=0;
	fwrite(&sizeSubVolume, sizeof(unsigned int), 1, fp);

	// classLabel
	unsigned int ncand, dmyClass;
	PR_CANDDATA* currentCand = caseData->childNodeFirstCand;
	for(ncand=0; ncand<caseData->numCand; ncand++)
	{
		if(currentCand==NULL)	return PR_AVEND;
		
		dmyClass = (unsigned int)currentCand->classLabel;
		if(dmyClass>=numClass)
		{
			fprintf(stderr,"error: class%d >= numClass%d..\n",dmyClass,numClass);
			return PR_AVEND;
		}
		
		fwrite(&dmyClass, sizeof(unsigned int), 1, fp);
		currentCand = currentCand->nodeNextCand;
	}

	// featValues
	for(nfeat=0; nfeat<rootCaseList->featTagTable->numFeat; nfeat++)
	{
		currentCand = caseData->childNodeFirstCand;
		for(ncand=0; ncand<caseData->numCand; ncand++)
		{
			fwrite(&currentCand->featValues[nfeat],sizeof(float),1,fp);
			currentCand = currentCand->nodeNextCand;
		}
	}

	// likelihoods
	unsigned int nclass;
	for(nclass=0; nclass<numOutLikelihoodClass; nclass++)
	{
		currentCand = caseData->childNodeFirstCand;
		for(ncand=0; ncand<caseData->numCand; ncand++)
		{
			fwrite(&currentCand->likelihood[nclass],sizeof(double),1,fp);
			currentCand = currentCand->nodeNextCand;
		}
	}

	return PR_EOT;
}

int PR_SaveBinaryCaseData(PR_CASEDATA* caseData, char* fileName, char booOutLikelihood)
{
	if(caseData==NULL)	return PR_AVEND;

	FILE* fp;
	if((fp=fopen(fileName,"wb"))==NULL)	return PR_AVEND;

//	fprintf(stderr,"save binary file \"%s\"...",fileName);

	int res = PR_WriteBinaryCaseDataForFILE(fp,caseData,booOutLikelihood);

	fclose(fp);
	if(res==PR_AVEND)	remove(fileName);
//	fprintf(stderr,"complete!\n");

	return res;
}

PR_CASEDATA* PR_ReadBinaryCaseDataInFILE(FILE* fp, PR_CASELIST* caseList)
{
	unsigned int dmyCaseID;
	fread(&dmyCaseID, sizeof(unsigned int), 1, fp);

	unsigned int dmyNumClass;
	fread(&dmyNumClass, sizeof(unsigned int), 1, fp);

	unsigned int* dmyNumCandOfClass = new unsigned int [dmyNumClass];
	fread(dmyNumCandOfClass, sizeof(unsigned int), dmyNumClass, fp);

	unsigned int dmyNumCandTotal;
	fread(&dmyNumCandTotal, sizeof(unsigned int), 1, fp);

	unsigned int dmyNumFeat;
	fread(&dmyNumFeat, sizeof(unsigned int), 1, fp);

	PR_FEATTAGTABLE* dmyFeatTable = PR_NewFeatTagTable(dmyNumFeat);
	for(unsigned int ii=0; ii<dmyNumFeat; ii++)
	{
		fread(&(dmyFeatTable->tags[ii]->group), sizeof(unsigned short), 1, fp);
		fread(&(dmyFeatTable->tags[ii]->element), sizeof(unsigned short), 1, fp);
	}

	// data-check
	if(caseList==NULL)	// 1st case
	{
	//	printf("caseList_address=%d->",caseList);
		caseList = PR_NewCaseList((unsigned char)dmyNumClass,dmyNumFeat);
		PR_CopyFeatTagTable(caseList->featTagTable,dmyFeatTable);
	//	printf("%d\n",caseList);
	}
	else	// 2nd and other lower lank cases
	{
		int booEra=0;

		if(dmyNumFeat!=caseList->featTagTable->numFeat)		booEra=2;

		if(PR_CompareFeatTagTable(caseList->featTagTable,dmyFeatTable)==PR_AVEND)	booEra=3;
		
		if(booEra)
		{
			printf("error(%d),",booEra);

			delete [] dmyNumCandOfClass;
			PR_DeleteFeatTagTable(dmyFeatTable);
			fclose(fp);
			return NULL;
		}
	}

	// add case data node to case list
	PR_CASEDATA* caseData = PR_NewCaseDataAddToTailOfCaseList(caseList);
	caseData->idCase = dmyCaseID;

	// flagOutLikeloods = numClassOfSavedLikelihood
	unsigned int numClsOfSavedLhod;
	fread(&numClsOfSavedLhod, sizeof(unsigned int), 1, fp);

	// sizeSubVolume (in preparation)
	unsigned int sizeSubVolume;
	fread(&sizeSubVolume, sizeof(unsigned int), 1, fp);

	// classLabel
	PR_CANDDATA* currentCand;
	unsigned int dmyCls;
	int boo=0;
	
	for(unsigned int ii=0; ii<dmyNumCandTotal; ii++)
	{
		currentCand = PR_NewCandDataAddToTailOfCaseData(caseData);

		if(currentCand==NULL)
		{
			PR_DeleteCaseData(caseData);
			fclose(fp);
			return NULL;
		}
	
		fread(&dmyCls, sizeof(unsigned int), 1, fp);
		if(dmyCls>=dmyNumClass)
		{			
			if(!boo)
				printf("find mislabel(%d>=nClass%d), change to zero\n",dmyCls,caseList->numClass);
			dmyCls=0;
			boo=1;
		}
		currentCand->classLabel = (unsigned char)dmyCls;
	}

	// featValues
	float* dmySpaceFloat = new float [caseData->numCand];

	for(unsigned int jj=0; jj<dmyNumFeat; jj++)
	{
		fread(dmySpaceFloat, sizeof(float), caseData->numCand, fp);
		currentCand = caseData->childNodeFirstCand;

		for(unsigned int ii=0; ii<caseData->numCand; ii++)
		{
			currentCand->featValues[jj] = dmySpaceFloat[ii];
			currentCand = currentCand->nodeNextCand;
		}
	}

	// likelihoods
	for(unsigned int kk=0; kk<numClsOfSavedLhod; kk++)
	{
		currentCand = caseData->childNodeFirstCand;

		for(unsigned int ii=0; ii<caseData->numCand; ii++)
		{
			fread(&currentCand->likelihood[kk],sizeof(double),1,fp);
			currentCand = currentCand->nodeNextCand;
		}
	}

	// memory free
	delete [] dmyNumCandOfClass;
	PR_DeleteFeatTagTable(dmyFeatTable);
	delete [] dmySpaceFloat;

	// chech if fp is EOF or not 
	if(fgetc(fp)!=EOF)
	{
		PR_DeleteCaseData(caseData);
		fclose(fp);
		fprintf(stderr,"false...\n");

		return NULL;
	}
	
	PR_CountNumCandsForCaseList(caseList);

	return caseData;
}

PR_CASEDATA* PR_ReadBinaryCaseData(PR_CASELIST* caseList, char* fileName)
{
	FILE* fp;
	if((fp=fopen(fileName,"rb"))==NULL)
	{
		printf("error! read \"%s\"\n",fileName);
		return NULL;
	}
//	fprintf(stderr,"read \"%s\"... ",fileName);

	PR_CASEDATA* casedata = PR_ReadBinaryCaseDataInFILE(fp,caseList);

	fclose(fp);
//	fprintf(stderr,"complete\n");

	return casedata;
}


int PR_CheckAndModifyFileNamesExtensionToLibPR(char* name, int mode)
{
	if(mode!=PR_FILE__EXTENSION_TEXT && mode!=PR_FILE__EXTENSION_BINARY)
		return PR_ErrorMessageInt("mode error (@PR_CheckAndModifyFileNamesExtensionToLibPR)");

	char extension[8];
	switch(mode)
	{
	case PR_FILE__EXTENSION_TEXT:
		sprintf(extension,".txt");
		break;
	case PR_FILE__EXTENSION_BINARY:
		sprintf(extension,".lpr");
		break;
	}

	char* position = strstr(name, extension);
	if(position==NULL)	strcat(name,extension);

	return PR_EOT;
}



PR_CASEDATA* PR_InsertReadBinaryCaseDataToCaseList(PR_CASELIST* caseList, char* fileName)
{
	if(caseList==NULL || fileName==NULL)		return NULL;

	unsigned int ii=0,jj=0,kk=0;
	unsigned char numClass = caseList->numClass;

	FILE* fp;
	if((fp=fopen(fileName,"rb"))==NULL)		return NULL;

	PR_CASEDATA* caseData = PR_NewCaseDataAddToTailOfCaseList(caseList);

	fprintf(stderr,"read bin \"%s\"... ",fileName);

	// caseID
	fread(&caseData->idCase, sizeof(unsigned int), 1, fp);
//	printf("id,");

	// numClass
	unsigned int dmyNumClass;
	fread(&dmyNumClass, sizeof(unsigned int), 1, fp);
	if(dmyNumClass>(unsigned int)numClass)
	{
		PR_DeleteCaseData(caseData);
		return NULL;
	}
//	printf("cls,");

	// numCandOfClass
	fread(caseData->numCandOfClass, sizeof(unsigned int), dmyNumClass, fp);

	// numCand = sum(numCandOfClass[i];0<=i<numClass)
	unsigned int numCandFromBin;
	fread(&numCandFromBin, sizeof(unsigned int), 1, fp);
//	printf("cand,");

	// numFeat
	unsigned int dmyNumFeat;
	fread(&dmyNumFeat, sizeof(unsigned int), 1, fp);
	if(dmyNumFeat<caseList->featTagTable->numFeat)
	{
		PR_DeleteCaseData(caseData);
		return NULL;
	}
//	printf("feat,");

	// featTagTable
	int checkRes;
	PR_FEATTAGTABLE* dmyTags = PR_NewFeatTagTable(1);
	unsigned int* indexOfSpecifiedFeat = new unsigned int [dmyNumFeat];

	char* bReadFeatIsInFeatTagTable = new char [caseList->featTagTable->numFeat];
	memset(bReadFeatIsInFeatTagTable,0x00,sizeof(char)*caseList->featTagTable->numFeat);

	for(ii=0; ii<dmyNumFeat; ii++)
	{
		fread(&(*dmyTags->tags)->group, sizeof(unsigned short), 1, fp);
		fread(&(*dmyTags->tags)->element, sizeof(unsigned short), 1, fp);
		checkRes = PR_CheckIfFeatTagIsMemberOfFeatTagTable(*dmyTags->tags, caseList->featTagTable, indexOfSpecifiedFeat+ii);
		if(checkRes==PR_AVEND)
		{
			PR_DeleteFeatTagTable(dmyTags);
			delete [] indexOfSpecifiedFeat;
			fclose(fp);
			PR_DeleteCaseData(caseData);
			return NULL;
		}
		else if(checkRes==PR_EOT)
			bReadFeatIsInFeatTagTable[indexOfSpecifiedFeat[ii]]=1;
	//	printf("\nfeat%d->%d",ii,indexOfSpecifiedFeat[ii]);
	}
//	printf("ftable,");

	for(jj=0; jj<caseList->featTagTable->numFeat; jj++)
	if(!bReadFeatIsInFeatTagTable[jj])
	{
		PR_DeleteFeatTagTable(dmyTags);
		delete [] indexOfSpecifiedFeat;
		delete [] bReadFeatIsInFeatTagTable;
		fclose(fp);
		PR_DeleteCaseData(caseData);
		return NULL;
	}
//	printf("2,");

	PR_DeleteFeatTagTable(dmyTags);
	delete [] bReadFeatIsInFeatTagTable;
//	printf("3,");


	// flagOutLikeloods -> numOutLikelihoods
	unsigned int numOutLikelihoodClass;
	fread(&numOutLikelihoodClass, sizeof(unsigned int), 1, fp);
//	printf("lhod,");

	// sizeSubVolume (in preparation)
	unsigned int sizeSubVolume;
	fread(&sizeSubVolume, sizeof(unsigned int), 1, fp);
//	printf("szSv\n");

	// classLabel
	unsigned int dmyClass;
	PR_CANDDATA* currentCand;
	for(ii=0; ii<numCandFromBin; ii++)
	{
		currentCand = PR_NewCandDataAddToTailOfCaseData(caseData);
		if(currentCand==NULL)
		{
			fclose(fp);
			PR_DeleteCaseData(caseData);
			printf("false");
			getchar();
			return NULL;
		}
		fread(&dmyClass, sizeof(unsigned int), 1, fp);
		currentCand->classLabel = (unsigned char)dmyClass;
		caseList->totalNumCandOfClass[dmyClass]++;
	}
//	printf("label,");

	// featValues
	float* dmySpaceFloat = new float [caseData->numCand+10];
	for(jj=0; jj<dmyNumFeat; jj++)
	{
//		printf("f%d",jj);
		currentCand = caseData->childNodeFirstCand;
		fread(dmySpaceFloat, sizeof(float), caseData->numCand, fp);
		unsigned int dmyIndex = indexOfSpecifiedFeat[jj];
//		printf("(%d)",dmyIndex);

		if(dmyIndex<caseList->featTagTable->numFeat)
		for(ii=0; ii<caseData->numCand; ii++)
		{
			currentCand->featValues[dmyIndex] = dmySpaceFloat[ii];
		//	fread(&currentCand->featValues[dmyIndex],sizeof(float),1,fp);
			PR_CANDDATA* dmyCand = currentCand;
			currentCand = dmyCand->nodeNextCand;
		}
//		printf(",");
	}
	delete [] dmySpaceFloat;
//	printf("fvalue\n");

	// likelihoods
	for(kk=0; kk<numOutLikelihoodClass; kk++)
	{
		currentCand = caseData->childNodeFirstCand;
		for(ii=0; ii<caseData->numCand; ii++)
		{
			fread(&currentCand->likelihood[kk],sizeof(double),1,fp);
			currentCand = currentCand->nodeNextCand;
		}
	}
//	printf("lhod,");

	delete [] indexOfSpecifiedFeat;
	
	// chech if fp is EOF or not 
	if(fgetc(fp)!=EOF)
	{
		fprintf(stderr,"false...\n");
		fclose(fp);
		PR_DeleteCaseData(caseData);
		return NULL;
	}

	fprintf(stderr,"complete!\n");
	fclose(fp);
	return caseData;
}


unsigned char PR_ReadNumClassFromBinaryCaseData(char* fileName)
{
	unsigned int caseID, numClass;

	FILE* fp;
	if((fp=fopen(fileName,"rb"))==NULL)		return NULL;
//	fprintf(stderr,"read binary file \"%s\"...",fileName);

	// caseID
	fread(&caseID, sizeof(unsigned int), 1, fp);

	// numClass
	fread(&numClass, sizeof(unsigned int), 1, fp);

//	fprintf(stderr,"#{class}=%d\n",numClass);

	fclose(fp);
	return (unsigned char)numClass;
}



PR_INPUTCASEFILENAMETABLE* PR_NewInputCaseFileNameTable(unsigned int numCase, unsigned int numClass)
{
	PR_INPUTCASEFILENAMETABLE* table = new PR_INPUTCASEFILENAMETABLE;
	table->numCase = numCase;
	table->numClass = numClass;
	table->strFileName = new char* [numCase];
	for(unsigned int i=0; i<numCase; i++)	table->strFileName[i] = new char [256];
	return table;
}

void PR_DeleteInputCaseFileNameTable(PR_INPUTCASEFILENAMETABLE* table)
{
	for(unsigned int i=0; i<table->numCase; i++)	delete [] table->strFileName[i];
	delete [] table->strFileName;
	delete [] table;
}


PR_NUMCLASSANDFEATOFCASE* PR_ReadNumClassAndNumFeatFromCaseFile(char* filename)
{
//	printf("file\"%s\"\n",filename);

	FILE* fp;
	if((fp=fopen(filename,"rb"))==NULL)		return NULL;

	unsigned int caseID;
	fread(&caseID, sizeof(unsigned int), 1, fp);
//	printf("caseID%d, ",caseID);

	unsigned int numClass;
	fread(&numClass, sizeof(unsigned int), 1, fp);
//	printf("numClass%d, ",numClass);

	unsigned int* numCandInCls = new unsigned int [numClass];
	fread(numCandInCls, sizeof(unsigned int), numClass, fp);
	delete [] numCandInCls;

	unsigned int numCandTotal;
	fread(&numCandTotal, sizeof(unsigned int), 1, fp);
//	printf("numCandTotal%d, ",numCandTotal);

	unsigned int numFeat;
	fread(&numFeat, sizeof(unsigned int), 1, fp);
//	printf("numFeat%d\n",numFeat);

	fclose(fp);

	PR_NUMCLASSANDFEATOFCASE* nums = new PR_NUMCLASSANDFEATOFCASE;
	nums->cls = (unsigned char)numClass;
	nums->feat = numFeat;

	return nums;
}

PR_CASELIST* PR_NewReadBinaryCaseList(PR_INPUTCASEFILENAMETABLE* table)
{
	unsigned char maxNumCls=0;
	unsigned int maxNumFeat=0;

	for(unsigned int i=0; i<table->numCase; i++)
	{
		PR_NUMCLASSANDFEATOFCASE* nums = PR_ReadNumClassAndNumFeatFromCaseFile(table->strFileName[i]);
		if(nums!=NULL)
		{
			if(maxNumCls<nums->cls)		maxNumCls=nums->cls;
			if(maxNumFeat<nums->feat)	maxNumFeat=nums->feat;
			delete nums;
		}
	}

	if(maxNumCls>(unsigned char)table->numClass)	maxNumCls=(unsigned char)table->numClass;

	PR_CASELIST* caseList = PR_NewCaseList(maxNumCls,maxNumFeat);

	for(unsigned int i=0; i<table->numCase; i++)
	{
		if(PR_ReadBinaryCaseData(caseList,table->strFileName[i])==NULL)
		{
			fprintf(stderr,"error: not success reading \"%s\"..",table->strFileName[i]);
			return NULL;
		}
	}

	return caseList;
}


PR_FEATTAGTABLE* PR_ReadFeatTableFromCaseDataFile(char* fileName)
{
//	unsigned int ii=0,jj=0,kk=0;

	FILE* fp;
	if((fp=fopen(fileName,"rb"))==NULL)
	{
		printf("error! read \"%s\"\n",fileName);
		return NULL;
	}

	// caseID
	unsigned int dmyCaseID;
	fread(&dmyCaseID, sizeof(unsigned int), 1, fp);

	// numClass
	unsigned int dmyNumClass;
	fread(&dmyNumClass, sizeof(unsigned int), 1, fp);

	// numCandOfClass
	unsigned int* dmyNumCandOfClass = new unsigned int [dmyNumClass];
	fread(dmyNumCandOfClass, sizeof(unsigned int), dmyNumClass, fp);

	// numCandTotal
	unsigned int dmyNumCandTotal;
	fread(&dmyNumCandTotal, sizeof(unsigned int), 1, fp);

	// numFeat
	unsigned int dmyNumFeat;
	fread(&dmyNumFeat, sizeof(unsigned int), 1, fp);

	// featTagTable
	PR_FEATTAGTABLE* dmyFeatTable = PR_NewFeatTagTable(dmyNumFeat);
	for(unsigned int ii=0; ii<dmyNumFeat; ii++)
	{
		fread(&(dmyFeatTable->tags[ii]->group), sizeof(unsigned short), 1, fp);
		fread(&(dmyFeatTable->tags[ii]->element), sizeof(unsigned short), 1, fp);
	}

	fclose(fp);
	delete [] dmyNumCandOfClass;

	return dmyFeatTable;
}



PR_CASELIST* PR_NewReadBinaryCases(char** pathes, int num)
{
	PR_CASELIST* caseList = NULL;
	PR_CASEDATA* caseData;

	int countNULL=0;

	for(int i=0; i<num; i++)
	{
		// read candidate dataset of i-th case
		caseData = PR_ReadBinaryCaseData(caseList,pathes[i]);

		// get address of PR_CASELIST newly allocated
		if(caseList==NULL)	caseList = (PR_CASELIST*)caseData->rootCaseList;

		if(caseData!=NULL)	printf("read %d/%d\r",i-countNULL,num-countNULL);
		else				countNULL++;
	}

	return caseList;
}


