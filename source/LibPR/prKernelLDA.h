////////////////////////////////////////////////////////////////////////////////////////
//
//		LibPR : library for classification
//
//		prKernelLDA.h : for function1
//
//		main coder: M.Nemoto
//
//		update history
//		2010.01.13	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef PR_KERNEL_FISHER_LDA
#define PR_PERNEL_FISHER_LDA


#include "./prCandDataSet.h"
#include "./prKernelMethodTools.h"
#include "./prMatrixTools.h"


double* 
PR_KernelLinearDiscriminantAnalysisTraining(
	PR_SAMPLESETFORKERNELMETHOD* sampleSet, int idxKernel, double con, double multi, double sigma);

double
PR_CalculateValueOfKernelLinearDiscriminantAnalysisForCandData(
	PR_CANDDATA* data, PR_SAMPLESETFORKERNELMETHOD* sample, double* bpVector, int kernel, double con, double multi, double sigma);

int
PR_CalculateValueOfKernelLinearDiscriminantAnalysisForCaseList(
	PR_CASELIST* list, PR_SAMPLESETFORKERNELMETHOD* sample, double* bpVector, int kernel, double con, double multi, double sigma);


#endif

