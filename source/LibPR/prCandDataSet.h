////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : Pattern Recognition library
//
//			prAnalysisDataset.cpp : for function1
//
//			main coder: M. Nemoto
//
//			update history
//
//			2007.08.27	start coding
//			
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_SAMPLE_DAT_H
#define PR_SAMPLE_DAT_H

#include <cstdio>
#include "./prFeatTag.h"


//- Each Candidate -//
typedef struct	_PR_CANDDATA
{
	unsigned int			idCand;
	char					booOutlier;			// !0- > outlier 
	unsigned char			classLabel;			// Class Label (1=MassÅC0=NormalÅC99=Unknown)
	float*					featValues;			// Feature Vectors
	double*					likelihood;			// likelihoods
	void*					optionParam;		// private param for classifier

	struct _PR_CANDDATA*	nodePrevCand;
	struct _PR_CANDDATA*	nodeNextCand;
	void*					rootCaseData;
}
PR_CANDDATA;


//- Each Case -//
typedef struct _PR_CASEDATA
{
	unsigned int			idCase;				// idCase
	unsigned int			numCand;			// #{Candidates}
	unsigned int*			numCandOfClass;		// #{Candidates in Each Class}

	PR_CANDDATA*			childNodeFirstCand;
	PR_CANDDATA*			childNodeLastCand;
	struct _PR_CASEDATA*	nodePrevCase;
	struct _PR_CASEDATA*	nodeNextCase;
	void*					rootCaseList;
}
PR_CASEDATA;

//- Case List -//
typedef struct _PR_CASELIST
{
	unsigned int		numCase;				// #{Case}
	unsigned char		numClass;				// #{Classes}
	unsigned long		totalNumCand;			// #{Total Read Candidate}
	unsigned long*		totalNumCandOfClass;	// #{Total Candidate in Eacn Class} ( Total[**]=#{Candidates in Class-**} )
	PR_FEATTAGTABLE*	featTagTable;

	PR_CASEDATA*		childNodeFirstCase;
	PR_CASEDATA*		childNodeLastCase;
}
PR_CASELIST;

// Set Memory
PR_CANDDATA* PR_NewCandData(unsigned char numClass, unsigned int numFeat);
PR_CASEDATA* PR_NewCaseData(unsigned char numClass);
PR_CASELIST* PR_NewCaseList(unsigned char numClass, unsigned int numFeat);

// Delete Memory
void PR_DeleteCandData(PR_CANDDATA* candData);
void PR_DeleteCaseData(PR_CASEDATA* caseData);
void PR_DeleteCaseList(PR_CASELIST* caseList);
int PR_DeleteAllCaseDataInCaseList(PR_CASELIST* caseList);

// Check in List
int PR_CheckIfCandDataIsMemberOfCaseData(PR_CASEDATA* caseData, PR_CANDDATA* candData);
int PR_CheckIfCaseDataIsMemberOfCaseList(PR_CASELIST* caseList, PR_CASEDATA* caseData);

// New + Add Member to List
PR_CANDDATA* PR_NewCandDataAddToHeadOfCaseData(PR_CASEDATA* caseData);
PR_CANDDATA* PR_NewCandDataAddToTailOfCaseData(PR_CASEDATA* caseData);
PR_CANDDATA* PR_NewCandDataAddToCaseData(PR_CASEDATA* caseData, PR_CANDDATA* addNextTo);

// Add Member to List
int PR_InsertCandDataToHeadOfCaseData(PR_CASEDATA* caseData, PR_CANDDATA* insertCand, char booKnownClassLabel);
int PR_InsertCandDataToTailOfCaseData(PR_CASEDATA* caseData, PR_CANDDATA* insertCand, char booKnownClassLabel);
int PR_InsertCandDataToCaseData(PR_CASEDATA* caseData, PR_CANDDATA* insertCand, PR_CANDDATA* insertNextTo, char booKnownClassLabel);

// New + Add Member to List
PR_CASEDATA* PR_NewCaseDataAddToHeadOfCaseList(PR_CASELIST* caseList);
PR_CASEDATA* PR_NewCaseDataAddToTailOfCaseList(PR_CASELIST* caseList);
PR_CASEDATA* PR_NewCaseDataAddToCaseList(PR_CASELIST* caseList, PR_CASEDATA* addNextTo);

// Add Member to List
int PR_InsertCaseDataToHeadOfCaseList(PR_CASELIST* caseList, PR_CASEDATA* insertCase);
int PR_InsertCaseDataToTailOfCaseList(PR_CASELIST* caseList, PR_CASEDATA* insertCase);
int PR_InsertCaseDataToCaseList(PR_CASELIST* caseList, PR_CASEDATA* insertCase, PR_CASEDATA* incertNextTo);

// Count Member in List
int PR_CountNumCandsForCaseData(PR_CASEDATA* caseData, unsigned char numClass);
int PR_CountNumCandsForCaseList(PR_CASELIST* caseList);

// Remove Member from List
int PR_PullOutCandDataFromCaseData(PR_CASEDATA* caseData, PR_CANDDATA* candData);
int PR_PullOutCaseDataFromCaseList(PR_CASELIST* caseList, PR_CASEDATA* caseData);
int PR_PullOutAllCaseDataInCaseList(PR_CASELIST* caseList);

// Reset Feature Data Array in List
int PR_RemakeFeatValueArrayOfCaseList(PR_CASELIST* caseList, PR_FEATTAGTABLE* newTable);

PR_CASELIST* PR_DuplicateCaseList(PR_CASELIST* source);



// specific
typedef struct _PR_TRAINCANDARRAY
{
	PR_CANDDATA** data;
	unsigned char numClass;
	unsigned int numData;
	unsigned int* numDataOfClass;
}
PR_TRAINCANDARRAY;

PR_TRAINCANDARRAY* PR_NewTrainCandArray(unsigned char numClass, unsigned int numCand);
void PR_DeleteTrainCandArray(PR_TRAINCANDARRAY* cands);


#endif

