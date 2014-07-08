////////////////////////////////////////////////////////////////////////////////////////
//
//		LibPR : library for classification
//
//		prKernels.h : for function1
//
//		main coder: M.Nemoto
//
//		update history
//		2010.01.13	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


#include "./prCandDataSet.h"
#include "./prFeatTag.h"
#include "./prMatrixTools.h"


#ifndef PR_KERNEL_METHOD_TOOLS
#define PR_KERNEL_METHOD_TOOLS


#define PR_KERNEL_LINEAR		1
#define PR_KERNEL_POLYNOMIAL	2
#define PR_KERNEL_GAUSS			3


typedef struct _Sample_Set_For_Kernel_Method
{
	float** value;
	unsigned char numClass;
	unsigned int numSample;
	unsigned int* numSampleInClass;
	PR_FEATTAGTABLE* tableFeat;
}
PR_SAMPLESETFORKERNELMETHOD;


PR_SAMPLESETFORKERNELMETHOD* PR_NewSampleSetForKernelMethod(unsigned char nClass, unsigned int nSample, unsigned int nFeature);
void PR_DeleteSampleSetForKernelMethodWithIndividualFeatureData(PR_SAMPLESETFORKERNELMETHOD* set);
void PR_DeleteSampleSetForKernelMethodSheringFeatureDataWithCandData(PR_SAMPLESETFORKERNELMETHOD* set);

int PR_LinkFeatureDataOfSampleSetForKernelMethodAndFeatureDataOfCandData(PR_SAMPLESETFORKERNELMETHOD* set, PR_CASELIST* list);
int PR_CopyFeatureDataOfCandDataToSampleSetForKernelMethod(PR_SAMPLESETFORKERNELMETHOD* set, PR_CASELIST* list);

PR_MATHMATRIX* PR_HilbertSpaceObjection(PR_SAMPLESETFORKERNELMETHOD* sample, PR_CANDDATA* data, int kernel, double con, double multi, double sigma);
PR_MATHMATRIX* PR_HilbertSpaceObjectionModeFloatArray(PR_SAMPLESETFORKERNELMETHOD* sample, float* ary, int kernel, double con, double multi, double sigma);
PR_MATHMATRIX* PR_GramMatrix(PR_SAMPLESETFORKERNELMETHOD* sample, int kernel, double con, double multi, double sigma);


#endif

