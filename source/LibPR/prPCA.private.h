
#pragma warning(disable: 4996)
#include "prMahalanobis.h"


PR_PRINCIPALCOMPONENT* PR_NewPrincipalComponent(unsigned int numFeature);
void PR_DeletePrincipalComponent(PR_PRINCIPALCOMPONENT* data);

int PR_SaveBinaryDatasetPrincipalComponentOld(PR_DATASETPRINCIPALCOMPONENT* dataset, char* filename);
PR_DATASETPRINCIPALCOMPONENT* PR_ReadBinaryDatasetPrincipalComponentOld(char* filename);

