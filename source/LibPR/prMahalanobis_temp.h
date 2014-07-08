


#ifndef PR_MAHALANOBIS_TEMP
#define PR_MAHALANOBIS_TEMP

#include "./prFeatureData_temp.h"
#include "./prMahalanobis.h"


PR_MAHALANOBISPARAM* PR_NewCalclateMahalanobisParameter_Temp(PR_ARRAYCASEDATATEMP* aryCase);

int
PR_CalcMahalanobisDistOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* aryCase, PR_MAHALANOBISPARAM* param);
int
PR_CalcMahalanobisDistOfCaseDataTemp(PR_CASEDATATEMP* caseData, PR_MAHALANOBISPARAM* param);
int
PR_CalcOneClassMahalanobisDistOfCaseDataTemp(PR_CASEDATATEMP* caseData, PR_MAHALANOBISPARAM* param, unsigned char targetClass);

int 
PR_CalcLikelihoodRatioOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* data, unsigned char idxNumerator, unsigned char idxDenominator, unsigned char idxResultIn);


#endif

