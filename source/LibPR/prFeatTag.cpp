
#pragma warning(disable: 4996)

#include "./prFeatTag.h"
#include "./prFeatTag.private.h"

#include "../LibPR.h"


PR_FEATTAG* PR_NewFeatTag()
{
	PR_FEATTAG* featTag = new PR_FEATTAG;
	if(featTag==NULL)	return NULL;

	featTag->group = 0xffff;
	featTag->element = 0xffff;
	return featTag;
}

PR_FEATTAG* PR_NewFeatTagPlus(unsigned short group, unsigned short element)
{
	PR_FEATTAG* featTag = new PR_FEATTAG;
	if(featTag==NULL)	return NULL;
	featTag->group = group;
	featTag->element = element;
	return featTag;
}

void PR_DeleteFeatTag(PR_FEATTAG* featTag)
{
	if(featTag!=NULL)	delete featTag;
}

PR_FEATTAGTABLE* PR_NewFeatTagTable(unsigned int numFeat)
{
	PR_FEATTAGTABLE* tagTable = new PR_FEATTAGTABLE;
	if(tagTable==NULL)	return NULL;
	
	tagTable->numFeat = numFeat;
	tagTable->tags = new PR_FEATTAG* [numFeat];
	if(tagTable->tags==NULL)
	{
		delete tagTable;
		return NULL;
	}

	unsigned int ii, jj;

	for(ii=0; ii<numFeat; ii++)
	{
		tagTable->tags[ii] = PR_NewFeatTag();
		
		if(tagTable->tags[ii]==NULL)
		{
			for(jj=0; jj<ii; jj++)	PR_DeleteFeatTag(tagTable->tags[jj]);
			delete tagTable;
			return NULL;
		}
	}

	return tagTable;
}

void PR_DeleteFeatTagTable(PR_FEATTAGTABLE* tagTable)
{
	int dbg=0;

	if(tagTable==NULL)	return;

	unsigned int ii;
	for(ii=0; ii<tagTable->numFeat; ii++)
	{
		if(dbg)	printf("f%03d/%03d-> ",ii,tagTable->numFeat);
		PR_DeleteFeatTag(tagTable->tags[ii]);
		if(dbg)	printf("ok%d  \r",ii);
	}

	delete [] tagTable->tags;
	delete tagTable;
	if(dbg)	printf("\n");

	return;
}

int PR_CompareFeatTag(PR_FEATTAG* tagData1, PR_FEATTAG* tagData2)
{
	if(tagData1->group==tagData2->group
		&& tagData1->element==tagData2->element)	return PR_EOT;
	else											return PR_AVEND;
}

int PR_CompareFeatTagTable(PR_FEATTAGTABLE* table1, PR_FEATTAGTABLE* table2)
{
	if(table1->numFeat!=table2->numFeat)	return PR_AVEND;

	for(unsigned int ii=0; ii<table1->numFeat; ii++)
	{
		if(PR_CompareFeatTag(table1->tags[ii], table2->tags[ii])==PR_AVEND)	return PR_AVEND;
	}

	return PR_EOT;
}

int PR_CheckIfFeatTagIsMemberOfFeatTagTable(PR_FEATTAG* tagData, PR_FEATTAGTABLE* featTagTable, unsigned int* indexInFeatureTag)
{
	if(tagData==NULL)
	{
		printf(" tagData==NULL ");
		return PR_AVEND;
	}
	if(featTagTable==NULL)
	{
		printf(" featTagTable==NULL ");
		return PR_AVEND;
	}

	unsigned int ii;
	for(ii=0; ii<featTagTable->numFeat; ii++)
	{
		if(featTagTable->tags[ii]==NULL)
		{
			printf(" featTagTable->tags[%d]==NULL ", ii);
			return PR_AVEND;
		}
		if(PR_CompareFeatTag(tagData, featTagTable->tags[ii])==PR_EOT)	break;
	}
	*indexInFeatureTag = ii;

	if(ii<featTagTable->numFeat)	return PR_EOT;
	else							return PR_EOT;
}

unsigned int PR_ExtractFeatNumberInFeatTagTable(PR_FEATTAGTABLE* tagTable, PR_FEATTAG* featTag)
{
	if(tagTable==NULL)
	{
		printf(" tagTable==NULL ");
		return tagTable->numFeat;
	}
	else if(featTag==NULL)
	{
		printf(" featTag==NULL ");
		return tagTable->numFeat;
	}

	unsigned int ii;
	for(ii=0; ii<tagTable->numFeat; ii++)
	{
		if(tagTable->tags[ii]==NULL)
		{
			printf(" tagTable->tags[%d]==NULL ",ii);
			return PR_AVEND;
		}
		if(PR_CompareFeatTag(featTag, tagTable->tags[ii])==PR_EOT)	break;
	}

	return ii;
}


int PR_CopyFeatTag(PR_FEATTAG* dest, PR_FEATTAG* source)
{
	if(dest==NULL || source==NULL)	return PR_AVEND;

	dest->group = source->group;
	dest->element = source->element;

	return PR_EOT;
}

int PR_CopyFeatTagTable(PR_FEATTAGTABLE* dest, PR_FEATTAGTABLE* source)
{
	if(dest->numFeat!=source->numFeat)	return PR_AVEND;

	unsigned int ii;
	for(ii=0; ii<dest->numFeat; ii++)
	{
		if(dest->tags[ii]==NULL)
		{
			printf(" dest->tag[%d]=NULL ",ii);
			return PR_AVEND;
		}
		else if(source->tags[ii]==NULL)
		{
			printf(" source->tag[%d]=NULL ",ii);
			return PR_AVEND;
		}

		dest->tags[ii]->group = source->tags[ii]->group;
		dest->tags[ii]->element = source->tags[ii]->element;
	}

	return PR_EOT;
}

PR_FEATTAGTABLE* PR_ReadTextFeatTagTable(char* fileName)
{
	if(strcmp(fileName,"NULL")==0)		return NULL;

	FILE* fp;
	if((fp=fopen(fileName,"r"))==NULL)
	{
		fprintf(stderr,"cannot open file \"%s\"\n",fileName);
		return NULL;
	}

	unsigned int numFeat;
	fscanf(fp,"%d",&numFeat);

	PR_FEATTAGTABLE* tagTable = PR_NewFeatTagTable(numFeat);
	
	for(unsigned int ii=0; ii<numFeat; ii++)
	{
		if((fscanf(fp,"%04x",&tagTable->tags[ii]->group))==EOF)
		{
			PR_DeleteFeatTagTable(tagTable);
			fclose(fp);
			return NULL;
		}
		if((fscanf(fp,"%04x",&tagTable->tags[ii]->element))==EOF)
		{
			PR_DeleteFeatTagTable(tagTable);
			fclose(fp);
			return NULL;
		}
	}

	fclose(fp);
	return tagTable;
}

int PR_SaveTextFeatTagTable(PR_FEATTAGTABLE* tagTable, char* fileName)
{
	if(tagTable==NULL)
	{
		fprintf(stderr,"cannot save FeatTagTable because of memory false\n");
		return PR_AVEND;
	}

	FILE* fp;
	if((fp=fopen(fileName,"w"))==NULL)
	{
		fprintf(stderr,"cannot save file \"%s\"\n",fileName);
		return PR_AVEND;
	}

	fprintf(fp,"%d\n",tagTable->numFeat);
	for(unsigned int ii=0; ii<tagTable->numFeat; ii++)
		fprintf(fp,"%04x %04x\n",tagTable->tags[ii]->group,tagTable->tags[ii]->element);

	fclose(fp);

	return PR_EOT;
}

