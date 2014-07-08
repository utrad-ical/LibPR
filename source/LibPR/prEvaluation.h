////////////////////////////////////////////////////////////////////////////////////////
//
//
//		Series: prEvaluation
//
//			main coder: M. Nemoto
//
//			update history
//
//			2009.11.19	start coding
//			2007.xx.xx	bug fixed for YYY
//
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef PR_EVALUATION_H
#define PR_EVALUATION_H

typedef struct _PR_ROC_DATA
{
	unsigned long num,numTP,numFP;
	float* value;
	float* axisFPR;
	float* axisTPR;
	char** notes;
	double areaUnderRoc;
}
PR_ROCDATA;

PR_ROCDATA* PR_NewRocData(unsigned long num);
void PR_DeleteRocData(PR_ROCDATA* data);
PR_ROCDATA* PR_CalculateTwoClassROC(unsigned long num, float* data, unsigned char* label, double* weight, char** memo);
int PR_WriteRocTxt(PR_ROCDATA* data, char* filename, unsigned long maxNumLine);

PR_ROCDATA* PR_CalculateSimpleTwoClassROC(unsigned long num, float* data, unsigned char* label);
PR_ROCDATA* PR_CalculateSimpleTwoClassRocByDistance(unsigned long num, float* data, unsigned char* label);


typedef struct _PR_CLASSIFICATION_PERFORMANCE
{
	float threshold;
	float tpr;
	float fpr;
	float error;
}
PR_CLASSIFICATIONPERFORMANCE;

PR_CLASSIFICATIONPERFORMANCE* PR_NewClassificationPerformance();
void PR_DeleteClassificationPerformance(PR_CLASSIFICATIONPERFORMANCE* data);

PR_CLASSIFICATIONPERFORMANCE* PR_GetThresholdWithMinErrorFromRocData(PR_ROCDATA* rocData);
PR_CLASSIFICATIONPERFORMANCE* PR_GetClassificationPerformanceOfOneThresholdFromRocData(PR_ROCDATA* rocData, float threshold);
PR_CLASSIFICATIONPERFORMANCE* PR_GetClassificationPerformanceOfOneTPR(PR_ROCDATA* rocData, float tpr);
PR_CLASSIFICATIONPERFORMANCE* PR_GetClassificationPerformanceOfMaxTPR(PR_ROCDATA* rocData);


#endif

