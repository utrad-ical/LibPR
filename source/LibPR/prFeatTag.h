
#ifndef PR_FEATURE_TAGS_H
#define PR_FEATURE_TAGS_H

#include <cstdio>

typedef struct _PR_FEATTAG
{
	unsigned short group;
	unsigned short element;
}
PR_FEATTAG;

typedef struct _PR_FEATTAGTABLE
{
	unsigned int numFeat;
	PR_FEATTAG** tags;
}
PR_FEATTAGTABLE;


// new
PR_FEATTAG* PR_NewFeatTag();
PR_FEATTAG* PR_NewFeatTagPlus(unsigned short group, unsigned short element);
PR_FEATTAGTABLE* PR_NewFeatTagTable(unsigned int numFeat);

// delete
void PR_DeleteFeatTag(PR_FEATTAG* featTag);
void PR_DeleteFeatTagTable(PR_FEATTAGTABLE* tagTable);

// compare
int PR_CompareFeatTag(PR_FEATTAG* tagData1, PR_FEATTAG* tagData2);
int PR_CompareFeatTagTable(PR_FEATTAGTABLE* table1, PR_FEATTAGTABLE* table2);

// check
int PR_CheckIfFeatTagIsMemberOfFeatTagTable(PR_FEATTAG* tagData, PR_FEATTAGTABLE* featTagTable, unsigned int* indexInFeatureTag);
unsigned int PR_ExtractFeatNumberInFeatTagTable(PR_FEATTAGTABLE* tagTable, PR_FEATTAG* featTag);

// copy
int PR_CopyFeatTagTable(PR_FEATTAGTABLE* dest, PR_FEATTAGTABLE* source);
int PR_CopyFeatTag(PR_FEATTAG* dest, PR_FEATTAG* source);

// file
PR_FEATTAGTABLE* PR_ReadTextFeatTagTable(char* fileName);
int PR_SaveTextFeatTagTable(PR_FEATTAGTABLE* tagTable, char* fileName);


#endif

