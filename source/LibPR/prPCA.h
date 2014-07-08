
#ifndef PR_PRINCIPAL_COMPONENT_ANALYSIS_H
#define PR_PRINCIPAL_COMPONENT_ANALYSIS_H


#include "prCandDataSet.h"
#include "prMatrixTools.h"


typedef struct _PR_PRINCIPAL_COMPONENT
{
	PR_MATHMATRIX* mean;		// width:numFeat
	PR_MATHMATRIX* loading;		// width:numEigen
	float* eigenValues;
}
PR_PRINCIPALCOMPONENT;

typedef struct _PR_DATASET_PRINCIPAL_COMPONENT
{
	int numClass;
	PR_PRINCIPALCOMPONENT** data;
}
PR_DATASETPRINCIPALCOMPONENT;


PR_DATASETPRINCIPALCOMPONENT* PR_NewDatasetPrincipalComponent(int numClass, unsigned int numFeature);
void PR_DeleteDatasetPrincipalComponent(PR_DATASETPRINCIPALCOMPONENT* dataset);

PR_DATASETPRINCIPALCOMPONENT* PR_ProcessingPCA(PR_CASELIST* caseList);
PR_DATASETPRINCIPALCOMPONENT* PR_ProcessingLimitedDimensionPCA(PR_CASELIST* caseList, unsigned int num);

float* PR_GetContributionRatioPCA(PR_DATASETPRINCIPALCOMPONENT* dataset, unsigned char targetClass);

unsigned int PR_GetDimensionForCumulativeContributionRatioThresholdPCA(
	PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char targetClass, double threshold);

float PR_CalcGivenPrincipalComponentScorePCA(PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, 
										  unsigned char targetClass, unsigned int targetPrincipalComponent);

float* PR_GetArrayPrincipalScores(PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char cTarget);

double PR_GetSquaredNormOfEigenResidue(PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char cTarget);

float* PR_GetArrayOfEigenResidueNormSquare(
	PR_CANDDATA* candData, PR_DATASETPRINCIPALCOMPONENT* dataPCA, unsigned char cTarget,
	int numTrial, int* trialDimensionNum);



#define SAVE_PCA_VERSION	"ver100607"

int PR_SaveBinaryDatasetPrincipalComponent(PR_DATASETPRINCIPALCOMPONENT* dataset, char* filename);
PR_DATASETPRINCIPALCOMPONENT* PR_ReadBinaryDatasetPrincipalComponent(char* filename);


#endif


