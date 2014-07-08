////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			xxx-2.h : for function2
//
//			main coder: Y. Masutani
//
//			update history
//
//			2007.05.02	start coding
//			2007.05.03	bug fixed for yyy
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef MATRIX_CALC_TOOLS_H
#define MATRIX_CALC_TOOLS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct _PR_MATHMATRIX
{
	unsigned int	width;			// X-Size
	unsigned int	height;			// Y-Size
	double			**dataMatrix;	// Matrix Main
}
PR_MATHMATRIX;


PR_MATHMATRIX* PR_NewMathMatrix(unsigned int width, unsigned int height);

void PR_DeleteMathMatrix(PR_MATHMATRIX* matrix);

int PR_CopyMathMatrix(PR_MATHMATRIX* dest, PR_MATHMATRIX* ref);

PR_MATHMATRIX* PR_DuplicateMathMatrix(PR_MATHMATRIX* ref);

int PR_InitializeMathMatrix(PR_MATHMATRIX* matrix, double initialize);

PR_MATHMATRIX* PR_AddMathMatrix(PR_MATHMATRIX* matrixA, PR_MATHMATRIX* matrixB);

PR_MATHMATRIX* PR_SubtractMathMatrix(PR_MATHMATRIX* matrixFrom, PR_MATHMATRIX* matrixSub);

PR_MATHMATRIX* PR_MultiplyMathMatrix(PR_MATHMATRIX* matrixA, PR_MATHMATRIX* matrixB);
PR_MATHMATRIX* PR_MultiplyMatrix(PR_MATHMATRIX* MatrixL, PR_MATHMATRIX* MatrixR);


PR_MATHMATRIX* PR_InverseMatrix(PR_MATHMATRIX*);

PR_MATHMATRIX* PR_CalcEigenVector(PR_MATHMATRIX*, float*);
PR_MATHMATRIX* PR_CalcEigenVectorJacobi(PR_MATHMATRIX *input, float *eValue);

double PR_CalcDeterminantDouble(PR_MATHMATRIX*);

int PR_PrintMathMatrix(PR_MATHMATRIX* data);


typedef struct _PR_EIGEN_COMPONENT
{
	PR_MATHMATRIX* eMatrix;
	float* eValue;
}
PR_EIGENCOMPONENT;

PR_EIGENCOMPONENT* PR_CalcEigenVectorLimitedDimension(PR_MATHMATRIX *matrix, unsigned int num);

/*
#define QSORT_FOR_FLOAT_MODE_ASCENDING 1
#define QSORT_FOR_FLOAT_MODE_DESCENDING 2
void QSortForFloat(float* Dat, unsigned int num, char mode);


unsigned int* QuickSortPlus(float *a, unsigned int n);
unsigned int* QuickSortPlus_Ascending(float *a, unsigned int n);
unsigned int* QuickSortPlus_Descending(float *a, unsigned int n);


PR_MATHMATRIX* PR_MultiplyMatrix(PR_MATHMATRIX*, PR_MATHMATRIX*);



PR_MATHMATRIX *PR_CalcEigenVector(PR_MATHMATRIX*, float*);

PR_MATHMATRIX* PR_CalcAverageVevtor(PR_MATHMATRIX*);

PR_MATHMATRIX* PR_CalcCoVarianceMatrix(PR_MATHMATRIX*, PR_MATHMATRIX*);

PR_MATHMATRIX* CalcMahalanobisDistance(PR_MATHMATRIX*, PR_MATHMATRIX*, PR_MATHMATRIX*);

double CalcDeterminant(PR_MATHMATRIX*);

*/


typedef struct _LeastSquarePolynomialParams
{
	int num_degree;			// degree_of_a_polynomial
	double* Coefficient;	// coefficients_of_a_polynomial
	double bias_x;			// average_of_input_x
}
LeastSquarePolynomialParams;

LeastSquarePolynomialParams* NewLeastSquarePolynomialParams(
int num
);

void DeleteLeastSquarePolynomialParams(
LeastSquarePolynomialParams* input
);

LeastSquarePolynomialParams* CalcLeastSquarePolynomialParams(
unsigned int num_sample,
double* Xi,
double* Yi,
unsigned int num_degree
);



int
PR_LUDecompose(PR_MATHMATRIX* matrix);


#endif

