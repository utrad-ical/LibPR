

#ifndef PR_UBOOST_TEMPORAL_H
#define PR_UBOOST_TEMPORAL_H


#include "./prFeatureData_temp.h"
#include "./prListEnsemble.h"


int PR_TrainEnsembleUBoost_Temp(PR_ARRAYCASEDATATEMP* tableMeta, PR_ENSEMBLE* ensemble,
								int channelBoost, double eta, char bDataWeightLogOutput);

int PR_InitializeCostBoostTwoClassTemporal(PR_ARRAYCASEDATATEMP* tableMeta, int mode);

int PR_CalcEnsembleOutputOfCaseDataTemp(PR_CASEDATATEMP* dataTemp, PR_ENSEMBLE* ensemble);
int PR_CalcEnsembleOutputOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* aryData, PR_ENSEMBLE* ensemble);


#endif

