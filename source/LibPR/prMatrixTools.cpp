////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			xxx-2.cxx : for function1
//
//			main coder: Y. Masutani
//
//			update history
//
//			2007.05.03	start coding
//			2007.05.05	bug fixed for SSS
//
//
////////////////////////////////////////////////////////////////////////////////////////


#include "prMatrixTools.h"
#include "prMatrixTools.private.h"


PR_MATHMATRIX* PR_NewMathMatrix(unsigned int width, unsigned int height)
{
	PR_MATHMATRIX *matrix = new PR_MATHMATRIX;

	if(matrix==NULL)	return NULL;

	matrix->dataMatrix = new double* [height];
	if(matrix->dataMatrix==NULL)
	{
		delete matrix;
		return NULL;
	}

	matrix->dataMatrix[0] = new double [height*width];
	if(matrix->dataMatrix[0]==NULL)
	{
		delete [] matrix->dataMatrix;
		delete matrix;
		return NULL;
	}

	for(unsigned int ii=1; ii<height; ii++)
		matrix->dataMatrix[ii] = &matrix->dataMatrix[0][width*ii];
	memset(matrix->dataMatrix[0],0x00,sizeof(double)*height*width);

	matrix->width = width;
	matrix->height = height;

	return matrix;
}

void PR_DeleteMathMatrix(PR_MATHMATRIX* matrix)
{
	if(matrix==NULL)	return;

	delete [] matrix->dataMatrix[0];
	delete [] matrix->dataMatrix;
	delete matrix;
}

int PR_CopyMathMatrix(PR_MATHMATRIX* dest, PR_MATHMATRIX* ref)
{
	unsigned int i, j;

	if(dest==NULL || ref==NULL)		return PR_AVEND;
	if(ref->height!=dest->height || ref->width!=dest->width)	return PR_AVEND;

	for(j=0; j<ref->height; j++)
	for(i=0; i<ref->width; i++)
		dest->dataMatrix[j][i] = ref->dataMatrix[j][i];

	return PR_EOT;
}

PR_MATHMATRIX* PR_DuplicateMathMatrix(PR_MATHMATRIX* ref)
{
	if(ref==NULL)	return NULL;

	PR_MATHMATRIX* dest = PR_NewMathMatrix(ref->width, ref->height);
	if(dest==NULL)		return NULL;

	PR_CopyMathMatrix(dest, ref);

	return dest;
}

int PR_InitializeMathMatrix(PR_MATHMATRIX* matrix, double initialize)
{
	if(matrix==NULL)	return PR_AVEND;

	for(unsigned int j=0; j<matrix->height; j++)
	for(unsigned int i=0; i<matrix->width; i++)
		matrix->dataMatrix[j][i] = initialize;

	return PR_EOT;
}


PR_MATHMATRIX* PR_AddMathMatrix(PR_MATHMATRIX* matrixA, PR_MATHMATRIX* matrixB)
{
	if(matrixA==NULL || matrixB==NULL)	return NULL;

	if(matrixA->height!=matrixB->height || matrixA->width!=matrixB->width)
		return NULL;

	PR_MATHMATRIX* added = PR_DuplicateMathMatrix(matrixA);
	if(added==NULL)	return NULL;

	for(unsigned int jj=0; jj<matrixA->height; jj++)
	for(unsigned int ii=0; ii<matrixA->width; ii++)
		added->dataMatrix[jj][ii] += matrixB->dataMatrix[jj][ii];

	return added;
}

PR_MATHMATRIX* PR_SubtractMathMatrix(PR_MATHMATRIX* matrixFrom, PR_MATHMATRIX* matrixSub)
{
	if(matrixFrom==NULL || matrixSub==NULL)	return NULL;

	if(matrixFrom->height!=matrixSub->height || matrixFrom->width!=matrixSub->width)
		return NULL;

	PR_MATHMATRIX* subtracted = PR_DuplicateMathMatrix(matrixFrom);
	if(subtracted==NULL)	return NULL;

	for(unsigned int jj=0; jj<matrixFrom->height; jj++)
	for(unsigned int ii=0; ii<matrixFrom->width; ii++)
		subtracted->dataMatrix[jj][ii] -= matrixSub->dataMatrix[jj][ii];

	return subtracted;
}

PR_MATHMATRIX* PR_MultiplyMathMatrix(PR_MATHMATRIX* matrixLeft, PR_MATHMATRIX* matrixRight)
{
	if(matrixLeft==NULL || matrixRight==NULL)		return NULL;
	if(matrixLeft->width != matrixRight->height)	return NULL;

	PR_MATHMATRIX* multiplyed = PR_NewMathMatrix(matrixRight->width, matrixLeft->height);
	if(multiplyed==NULL)	return NULL;

	for(unsigned int jj=0; jj<multiplyed->height; jj++)
	for(unsigned int ii=0; ii<multiplyed->width; ii++)
	{
		for(unsigned int cnt=0; cnt<matrixLeft->width; cnt++)
		{
			if(matrixLeft->dataMatrix[jj][cnt]!=0.0 && matrixRight->dataMatrix[cnt][ii]!=0.0)
			{
				multiplyed->dataMatrix[jj][ii] += matrixLeft->dataMatrix[jj][cnt]*matrixRight->dataMatrix[cnt][ii];
			//	printf()
			}
		}
	}

	return multiplyed;
}



int CopyToRecipeMatrix(
PR_MATHMATRIX* Refe,	// Reference
PR_MATHMATRIX* Dest		// Destination
){
	unsigned int i, j;

//	printf("\n[%d,%d]->[%d,%d]\n",Refe->height,Refe->width,Dest->height,Dest->width);
	if(Refe->height+1!=Dest->height || Refe->width+1!=Dest->width)	return 1;

//	printf("reference;\n");
//	for(j=1; j<=Refe->height; j++)
//	{
//		for(i=1; i<=Refe->width; i++)	printf("%.3e ",Refe->dataMatrix[j-1][i-1]);
//		printf("\n");
//	}
//	printf("\n");

	for(j=1; j<=Refe->height; j++)
	for(i=1; i<=Refe->width; i++)
		Dest->dataMatrix[j][i]=Refe->dataMatrix[j-1][i-1];

//	printf("destination;\n");
//	for(j=1; j<=Refe->height; j++){
//		for(i=1; i<=Refe->width; i++){
//			printf("%.3e ",Dest->dataMatrix[j][i]);
//		}printf("\n");
//	}
//	printf("\n");

	return 0;
}



// Multiplying
PR_MATHMATRIX* PR_MultiplyMatrix(
PR_MATHMATRIX* MatrixL,
PR_MATHMATRIX* MatrixR
){

	PR_MATHMATRIX *Result;

	if( MatrixL->width!=MatrixR->height )
	{
		fprintf(stderr, "Error at \"PR_MultiplyMatrix\"...\n");
		fprintf(stderr, "[%d,%d]x[%d,%d]\n",MatrixL->height,MatrixL->width,MatrixR->height,MatrixR->width);
		exit(0);
	}

//	fprintf(stderr, "L[%d,%d] x R[%d,%d]\n",MatrixL->height,MatrixL->width,MatrixR->height,MatrixR->width);

	Result = PR_NewMathMatrix(MatrixR->width, MatrixL->height);
//	fprintf(stderr,"wid%d x hei%d\n",MatrixR->width,MatrixL->height);

	int booPrint=0;
	if(MatrixR->width==1)	booPrint=1;

	unsigned int Cnt, ii, jj;

	for(jj=0; jj<Result->height; jj++)
	for(ii=0; ii<Result->width; ii++)
	{
//		if(booPrint && jj<3)	fprintf(stderr,"[%d,%d]\n",jj,ii);
		Result->dataMatrix[jj][ii]=0.0;

		for(Cnt=0; Cnt<MatrixL->width; Cnt++)
		{
			Result->dataMatrix[jj][ii] += MatrixL->dataMatrix[jj][Cnt] * MatrixR->dataMatrix[Cnt][ii];
			
//			if(booPrint && jj<3)	
//				fprintf(stderr,"%.3e*%.3e=%.3e(sum%.3e) -> ",MatrixL->dataMatrix[jj][Cnt],MatrixR->dataMatrix[Cnt][ii],
//															 MatrixL->dataMatrix[jj][Cnt]*MatrixR->dataMatrix[Cnt][ii],
//															 Result->dataMatrix[jj][ii]);
		}
//		if(booPrint && jj<3)	getchar();
	}

	return Result;
}


//- Calculate Inverse Matrix -//
PR_MATHMATRIX* PR_InverseMatrix(
PR_MATHMATRIX* Ref
){
	unsigned int ii, jj;

	if( Ref->width!=Ref->height )
	{
		fprintf(stderr, "Error at \"PR_InverseMatrix\"...\n");
		exit(0);
	}

	PR_MATHMATRIX *Inv = PR_NewMathMatrix(Ref->width, Ref->height);
//	printf("REF[%dx%d],INV[%dx%d],",Ref->width,Ref->height,Inv->width,Inv->height);

	switch(Ref->width)
	{

	case 1:
		{
			Inv->dataMatrix[0][0] = 1/Ref->dataMatrix[0][0];
		}
		break;

	case 2:
		{
			double a11 = Ref->dataMatrix[0][0];
			double a12 = Ref->dataMatrix[0][1];
			double a21 = Ref->dataMatrix[1][0];
			double a22 = Ref->dataMatrix[1][1];

			double detA = a11 * a22 - a12 * a21;

			if( detA != 0 )
			{
				Inv->dataMatrix[0][0] = (float)(  a22 / detA );
				Inv->dataMatrix[0][1] = (float)( -a12 / detA );
				Inv->dataMatrix[1][0] = (float)( -a21 / detA );
				Inv->dataMatrix[1][1] = (float)(  a11 / detA );
			}
			else
			{
				fprintf(stderr, "Error : Determinant of the matrix is zero.\n");
				Inv->dataMatrix[0][0]=-1.0f;
				return Inv;
			//	return NULL;
			}
		}
		break;

		
	case 3:
		{
			double a11 = Ref->dataMatrix[0][0],  a12 = Ref->dataMatrix[0][1],  a13 = Ref->dataMatrix[0][2];
			double a21 = Ref->dataMatrix[1][0],  a22 = Ref->dataMatrix[1][1],  a23 = Ref->dataMatrix[1][2];
			double a31 = Ref->dataMatrix[2][0],  a32 = Ref->dataMatrix[2][1],  a33 = Ref->dataMatrix[2][2];

			double _22x33_23x32_ = a22 * a33 - a23 * a32;
			double _21x32_22x31_ = a21 * a32 - a22 * a31;
			double _23x31_21x33_ = a23 * a31 - a21 * a33;

			double detA = a11 * _22x33_23x32_ + a13 * _21x32_22x31_ + a12 * _23x31_21x33_;
			
			if( detA != 0 )
			{
				double A11 = _22x33_23x32_;
				double A12 = a13 * a32 - a12 * a33;
				double A13 = a12 * a23 - a13 * a22;
				double A21 = _23x31_21x33_;
				double A22 = a11 * a33 - a13 * a31;
				double A23 = a13 * a21 - a11 * a23;
				double A31 = _21x32_22x31_;
				double A32 = a12 * a31 - a11 * a32;
				double A33 = a11 * a22 - a12 * a21;

				double _1_detA = 1.0 / detA;

				Inv->dataMatrix[0][0] = (float)( A11 * _1_detA );
				Inv->dataMatrix[0][1] = (float)( A12 * _1_detA );
				Inv->dataMatrix[0][2] = (float)( A13 * _1_detA );
				Inv->dataMatrix[1][0] = (float)( A21 * _1_detA );
				Inv->dataMatrix[1][1] = (float)( A22 * _1_detA );
				Inv->dataMatrix[1][2] = (float)( A23 * _1_detA );
				Inv->dataMatrix[2][0] = (float)( A31 * _1_detA );
				Inv->dataMatrix[2][1] = (float)( A32 * _1_detA );
				Inv->dataMatrix[2][2] = (float)( A33 * _1_detA );
			}
			else
			{
				fprintf(stderr, "Error : Determinant of the matrix is zero.\n");
				Inv->dataMatrix[0][0]=-1.0f;
				return Inv;
			//	return NULL;
			}
		}
		break;

	case 4:
		{
			double a11 = Ref->dataMatrix[0][0],  a12 = Ref->dataMatrix[0][1],  a13 = Ref->dataMatrix[0][2],  a14 = Ref->dataMatrix[0][3];
			double a21 = Ref->dataMatrix[1][0],  a22 = Ref->dataMatrix[1][1],  a23 = Ref->dataMatrix[1][2],  a24 = Ref->dataMatrix[1][3];
			double a31 = Ref->dataMatrix[2][0],  a32 = Ref->dataMatrix[2][1],  a33 = Ref->dataMatrix[2][2],  a34 = Ref->dataMatrix[2][3];
			double a41 = Ref->dataMatrix[3][0],  a42 = Ref->dataMatrix[3][1],  a43 = Ref->dataMatrix[3][2],  a44 = Ref->dataMatrix[3][3];

			double _33x44_34x43_ = a33 * a44 - a34 * a43;
			double _34x42_32x44_ = a34 * a42 - a32 * a44;
			double _32x43_33x42_ = a32 * a43 - a33 * a42;
			double _31x44_34x41_ = a31 * a44 - a34 * a41;
			double _33x41_31x43_ = a33 * a41 - a31 * a43;
			double _31x42_32x41_ = a31 * a42 - a32 * a41;


			double detA = a11 * ( a22 * _33x44_34x43_ + a23 * _34x42_32x44_ + a24 * _32x43_33x42_ )
						- a12 * ( a21 * _33x44_34x43_ - a23 * _31x44_34x41_ - a24 * _33x41_31x43_ )
						- a13 * ( a21 * _34x42_32x44_ + a22 * _31x44_34x41_ - a24 * _31x42_32x41_ )
						- a14 * ( a21 * _32x43_33x42_ + a22 * _33x41_31x43_ + a23 * _31x42_32x41_ );

			if( detA != 0 )
			{
				double _23x44_24x43_ = a23 * a44 - a24 * a43;
				double _24x42_22x44_ = a24 * a42 - a22 * a44;
				double _22x43_23x42_ = a22 * a43 - a23 * a42;
				double _24x33_23x34_ = a24 * a33 - a23 * a34;
				double _22x34_24x32_ = a22 * a34 - a24 * a32;
				double _23x32_22x33_ = a23 * a32 - a22 * a33;
				double _21x44_24x41_ = a21 * a44 - a24 * a41;
				double _23x41_21x43_ = a23 * a41 - a21 * a43;
				double _24x31_21x34_ = a24 * a31 - a21 * a34;
				double _21x33_23x31_ = a21 * a33 - a23 * a31;
				double _21x42_22x41_ = a21 * a42 - a22 * a41;
				double _22x31_21x32_ = a22 * a31 - a21 * a32;

				// calculation of each elements
				double A11 =  a22 * _33x44_34x43_ + a23 * _34x42_32x44_ + a24 * _32x43_33x42_;
				double A12 = -a12 * _33x44_34x43_ - a13 * _34x42_32x44_ - a14 * _32x43_33x42_;
				double A13 =  a12 * _23x44_24x43_ + a13 * _24x42_22x44_ + a14 * _22x43_23x42_;
				double A14 =  a12 * _24x33_23x34_ + a13 * _22x34_24x32_ + a14 * _23x32_22x33_;
				double A21 = -a21 * _33x44_34x43_ + a23 * _31x44_34x41_ + a24 * _33x41_31x43_;
				double A22 =  a11 * _33x44_34x43_ - a13 * _31x44_34x41_ - a14 * _33x41_31x43_;
				double A23 = -a11 * _23x44_24x43_ + a13 * _21x44_24x41_ + a14 * _23x41_21x43_;
				double A24 = -a11 * _24x33_23x34_ + a13 * _24x31_21x34_ + a14 * _21x33_23x31_;
				double A31 = -a21 * _34x42_32x44_ - a22 * _31x44_34x41_ + a24 * _31x42_32x41_;
				double A32 =  a11 * _34x42_32x44_ + a12 * _31x44_34x41_ - a14 * _31x42_32x41_;
				double A33 = -a11 * _24x42_22x44_ - a12 * _21x44_24x41_ + a14 * _21x42_22x41_;
				double A34 = -a11 * _22x34_24x32_ - a12 * _24x31_21x34_ + a14 * _22x31_21x32_;
				double A41 = -a21 * _32x43_33x42_ - a22 * _33x41_31x43_ - a23 * _31x42_32x41_;
				double A42 =  a11 * _32x43_33x42_ + a12 * _33x41_31x43_ + a13 * _31x42_32x41_;
				double A43 = -a11 * _22x43_23x42_ - a12 * _23x41_21x43_ - a13 * _21x42_22x41_;
				double A44 = -a11 * _23x32_22x33_ - a12 * _21x33_23x31_ - a13 * _22x31_21x32_;

				double _1_detA = 1.0 / detA;
				
				Inv->dataMatrix[0][0] = (float)( A11 * _1_detA );
				Inv->dataMatrix[0][1] = (float)( A12 * _1_detA );
				Inv->dataMatrix[0][2] = (float)( A13 * _1_detA );
				Inv->dataMatrix[0][3] = (float)( A14 * _1_detA );
				Inv->dataMatrix[1][0] = (float)( A21 * _1_detA );
				Inv->dataMatrix[1][1] = (float)( A22 * _1_detA );
				Inv->dataMatrix[1][2] = (float)( A23 * _1_detA );
				Inv->dataMatrix[1][3] = (float)( A24 * _1_detA );
				Inv->dataMatrix[2][0] = (float)( A31 * _1_detA );
				Inv->dataMatrix[2][1] = (float)( A32 * _1_detA );
				Inv->dataMatrix[2][2] = (float)( A33 * _1_detA );
				Inv->dataMatrix[2][3] = (float)( A34 * _1_detA );
				Inv->dataMatrix[3][0] = (float)( A41 * _1_detA );
				Inv->dataMatrix[3][1] = (float)( A42 * _1_detA );
				Inv->dataMatrix[3][2] = (float)( A43 * _1_detA );
				Inv->dataMatrix[3][3] = (float)( A44 * _1_detA );
			}
			else
			{
				fprintf(stderr, "Error : Determinant of the matrix is zero.\n");
				Inv->dataMatrix[0][0]=-1.0f;
				return Inv;
			//	return NULL;
			}
		}
		break;
		

	default:
	/**/
		PR_MATHMATRIX *MatrixLU = PR_NewMathMatrix(Ref->width+1, Ref->height+1);
	//	printf("a. REF[%dx%d], INV[%dx%d]\n",Ref->width,Ref->height,Inv->width,Inv->height);	

		for(jj=0; jj<Ref->height; jj++)
		for(ii=0; ii<Ref->width; ii++)
		{
			MatrixLU->dataMatrix[jj+1][ii+1] = Ref->dataMatrix[jj][ii]; 
		}
	//	printf("b. REF[%dx%d], INV[%dx%d]\n",Ref->width,Ref->height,Inv->width,Inv->height);	
	
		double  SwapNum;	
		int*   SwapIdx = new int [Ref->width+10];
		double* Column = new double [Ref->width+10];
	//	printf("c");
		
		double** matrix_dmy = new double* [Ref->height+1];
		for(jj=0; jj<=Ref->height; jj++)
		{
			matrix_dmy[jj] = new double [Ref->width+1];
			for(ii=0; ii<=Ref->width; ii++)		matrix_dmy[jj][ii]=(double)MatrixLU->dataMatrix[jj][ii];
		}
	//	printf("d");
	
	//	ludcmp(MatrixLU->dataMatrix, Ref->width, SwapIdx, &SwapNum);
		f64_ludcmp(matrix_dmy, (int)Ref->width, SwapIdx, &SwapNum);
	//	printf("e. REF[%dx%d], INV[%dx%d]\n",Ref->width,Ref->height,Inv->width,Inv->height);	
	
		for(jj=1; jj<=Ref->height; jj++)
		{
			for(ii=0; ii<=Ref->width; ii++)	Column[ii] = 0.0;
		//	memset(Column, 0, sizeof(double)*(Ref->width+1));
			Column[jj] = 1.0;
		//	lubksb(MatrixLU->dataMatrix, (int)Ref->width, SwapIdx, Column);
			f64_lubksb(matrix_dmy, (int)Ref->width, SwapIdx, Column);
	
			for(ii=1; ii<=Ref->width; ii++)  Inv->dataMatrix[jj-1][ii-1] = (float)Column[ii];
		}
	//	printf("f. REF[%dx%d], INV[%dx%d]\n",Ref->width,Ref->height,Inv->width,Inv->height);
	

		PR_DeleteMathMatrix(MatrixLU);
	//	printf("g");
		
		delete [] SwapIdx;
		delete [] Column;
	//	printf("i");

		for(jj=0; jj<=Ref->height; jj++)	delete [] matrix_dmy[jj];
		delete [] matrix_dmy;
	//	printf("h");
	/**/
/**/
		char bEra=0;
		double SumEra=0.0;

	//	printf("REF[%dx%d], INV[%dx%d]\n",Ref->width,Ref->height,Inv->width,Inv->height);
		PR_MATHMATRIX *Verify = PR_MultiplyMathMatrix(Ref, Inv);
	//	printf("j");

		for(jj=0; jj<Verify->height; jj++)
		for(ii=0; ii<Verify->width; ii++)
		{
			if(jj==ii)	SumEra += fabs(1.0-Verify->dataMatrix[jj][ii]);
			else		SumEra += fabs(Verify->dataMatrix[jj][ii]);	
		}
	//	printf("k");
	
		// printf("SumEra=%f\n",SumEra);
		if(SumEra>1.0f)
		{
			fprintf(stderr, "Large_sumEraLU(%.3f)",SumEra);
			
			if(SumEra>10.0f)
			{
				fprintf(stderr, "->GausJordan");
			//	getchar();
				/**/
				Inv->dataMatrix[0][0]=-1.0f;
				
				PR_MATHMATRIX *aaa = PR_NewMathMatrix(Ref->width+1, Ref->height+1);
				PR_MATHMATRIX *bbb = PR_NewMathMatrix(Ref->width, Ref->height+1);
			
				for(jj=0; jj<Ref->height; jj++)
				for(ii=0; ii<Ref->width; ii++)
					aaa->dataMatrix[jj+1][ii+1] = Ref->dataMatrix[jj][ii]; 

				f64_gaussj(aaa->dataMatrix, (int)Ref->height, bbb->dataMatrix, (int)Ref->width-1);

				for(jj=0; jj<Ref->height; jj++)
				for(ii=0; ii<Ref->width; ii++)
					Inv->dataMatrix[jj][ii] = aaa->dataMatrix[jj+1][ii+1];

				PR_DeleteMathMatrix(aaa);
				PR_DeleteMathMatrix(bbb);
	/**/
			}
		}
		
		PR_DeleteMathMatrix(Verify);
/**/
	//	printf("l\n");
	break;
	} // end of switch
	
	return Inv;
}

double** NewDoubleMatrix(
unsigned int xe,
unsigned int ye
){
	double** output = new double* [ye];
	double* output_1d = new double [ye*xe];

	for(unsigned int j=0; j<ye; j++)	output[j] = &output_1d[j*xe];

	memset(output_1d,0x00,sizeof(double)*ye*xe);

	return output;
}

double** CopyToDoubleMatrixForNRDouble(
PR_MATHMATRIX *input
){
	double** output = NewDoubleMatrix(input->width+1, input->height+1);

	for(unsigned int j=0; j<input->height; j++)
	for(unsigned int i=0; i<input->width; i++)
		output[j+1][i+1] = (double)input->dataMatrix[j][i];

	return output;
}

int CopyNRDoubleMatrixToMathMatrix(double** src, PR_MATHMATRIX *dest)
{
	for(unsigned int j=0; j<dest->height; j++)
	for(unsigned int i=0; i<dest->width; i++)
		dest->dataMatrix[j][i] = src[j+1][i+1];
	return 0;
}

void DeleteDoubleMatrix(
double** input
){
	delete [] *input;
	delete [] input;
}


PR_MATHMATRIX *PR_CalcEigenVectorJacobi(PR_MATHMATRIX *input, float *eValue)
{
	double** copyMatrix = CopyToDoubleMatrixForNRDouble(input);
	unsigned int num = input->width;
	double* d = new double [num+1];
	double** dmyLinkMatrix = NewDoubleMatrix(input->width+1, input->height+1);
	int* nrot = new int [num+1];

	f64_jacobi(copyMatrix,num,d,dmyLinkMatrix,nrot);

	DeleteDoubleMatrix(copyMatrix);
	delete nrot;

	PR_MATHMATRIX *linking = PR_NewMathMatrix(num,num);
	CopyNRDoubleMatrixToMathMatrix(dmyLinkMatrix,linking);

	for(unsigned int i=0; i<num; i++)
		eValue[i] = (float)d[i+1];
	
	delete [] d;
	DeleteDoubleMatrix(dmyLinkMatrix);

	return linking;
}

PR_MATHMATRIX *PR_CalcEigenVector(PR_MATHMATRIX *Input,float *EigenValue)
{
	unsigned int i,j;
	unsigned int n=Input->height;

	double** dmy_eigen = CopyToDoubleMatrixForNRDouble(Input);
	
	double* triple_diag = new double [n+1];
	double* sub_diago = new double [n+1];

	f64_tred2(dmy_eigen, n, triple_diag, sub_diago);
	f64_tqli(triple_diag, sub_diago, n, dmy_eigen);

	for(i=0; i<n; i++)
		EigenValue[i]=(float)triple_diag[i+1];

	/*
	for(i=0; i<n; i++)
		printf("%d;%.3e,%.3e\n",i,EigenValue[i],triple_diag[i+1]);

	for(j=1; j<=n; j++){
		for(i=1; i<=n; i++){
			printf("%.4e ",dmy_eigen[j][i]);
		}printf("\n");
	}
	*/

	unsigned long* Order = PR_QuickSortDescendingWithRank(EigenValue, n);

	/*
	for(i=0; i<n; i++)
		printf("%d(%d);%.3e\n",i,Order[i],EigenValue[i]);
	*/

	delete [] triple_diag;
	delete [] sub_diago;

	
	PR_MATHMATRIX *EigenVector = PR_NewMathMatrix(n,n);

	for(j=0; j<n; j++)
	for(i=0; i<n; i++)
		EigenVector->dataMatrix[j][i]=(float)dmy_eigen[j+1][Order[i]+1];

//	for(j=0; j<n; j++)
//	{
//		int j2=Order[j];
//		for(i=0; i<n; i++)
//			EigenVector->dataMatrix[j][i]=(float)dmy_eigen[j2+1][i+1];
//	}

	DeleteDoubleMatrix(dmy_eigen);
	delete [] Order;

	return EigenVector;
}

double PR_InnerProduct(double* ary1, double* ary2, unsigned int vlength)
{
	double ip=0;
	for(unsigned int i=0; i<vlength; i++)		ip += ary1[i]*ary2[i];
	return ip;
}


double* PR_GetEigenVectorNR(double** matrix, unsigned int msize, double lambda)
{
	for(unsigned int i=1; i<=msize; i++)	matrix[i][i] -= lambda;
	int* indx = new int [msize+1];
	double d;
	f64_ludcmp(matrix,(int)msize,indx,&d);

	unsigned int cnt=0;
	double mu,v2,v2s;
	double* evector = new double [msize+1];
	double* dmy_vect_v = new double [msize+1];
	memset(evector,0x00,sizeof(double)*(msize+1));
	evector[0] = evector[1] = 1.0;
	while(1)
	{
		cnt++;
		memcpy(dmy_vect_v,evector,sizeof(double)*(msize+1));
		f64_lubksb(matrix,(int)msize,indx,dmy_vect_v);
		mu = PR_InnerProduct(evector+1,dmy_vect_v+1,msize);
		v2 = PR_InnerProduct(dmy_vect_v+1,dmy_vect_v+1,msize);
		v2s = sqrt(v2);
		for(unsigned int i=1; i<=msize; i++)	evector[i] = dmy_vect_v[i]/v2s;

		if(cnt>=20)	break;
		else if(fabs(1.0-mu*mu/v2)<1.0e-13)	break;
	//	else if(cnt>=5 && fabs(1.0-mu*mu/v2)<1.0e-13)	break;
	}
//	while((cnt>=10 || fabs(1.0-mu*mu/v2)>1.0e-20) && cnt<100);

	delete [] indx;
	delete [] dmy_vect_v;

	printf("nITL=%d(era%.3e),",cnt,fabs(1.0-mu*mu/v2));
	return evector;
}

PR_EIGENCOMPONENT* PR_CalcEigenVectorLimitedDimension(PR_MATHMATRIX *matrix, unsigned int num)
{
	double** dmy = CopyToDoubleMatrixForNRDouble(matrix);

	// 1.balancing
	for(unsigned int j=1; j<matrix->height; j++)
	for(unsigned int i=0; i<j; i++)
	if(dmy[j+1][i+1]!=matrix->dataMatrix[i+1][j+1])
	{
		f64_balanc(dmy,matrix->height);
		goto END_BALANCING;
	}
END_BALANCING:;

	// 2.modify to hessenberg
	f64_elmhes(dmy,matrix->height);

	// 3.process hessenberg to obtain eigen values (=wr) by QR-method
	double* wr = new double [matrix->height+1];
	double* wi = new double [matrix->height+1];
	f64_hqr(dmy,matrix->height,wr,wi);
	DeleteDoubleMatrix(dmy);
	delete [] wi;
	
	// 4.copy eigen values
	PR_EIGENCOMPONENT* eigen = new PR_EIGENCOMPONENT;
	eigen->eValue = new float [matrix->height];
	for(unsigned int j=0; j<matrix->height; j++)	eigen->eValue[j] = (float)wr[j+1];
	delete [] wr;
	PR_QuickSortDescending(eigen->eValue,matrix->height);

	// 5.get 1st~[num]th eigen vectors
	eigen->eMatrix = PR_NewMathMatrix(num,matrix->height);
	for(unsigned int i=0; i<num; i++)
	{
		dmy = CopyToDoubleMatrixForNRDouble(matrix);
		double* evector = PR_GetEigenVectorNR(dmy,matrix->height,(double)eigen->eValue[i]);
		for(unsigned int j=0; j<matrix->height; j++)	eigen->eMatrix->dataMatrix[j][i] = evector[j+1];
		delete [] evector;
		DeleteDoubleMatrix(dmy);
	}

	return eigen;
}



PR_MATHMATRIX *PR_CalcAverageVevtor(
PR_MATHMATRIX	*Data
){

	PR_MATHMATRIX *AvgVector = PR_NewMathMatrix(Data->width, 1);
	
	unsigned int ii, jj;

	for(ii=0; ii<Data->width; ii++)
	{
		double tmp = 0;
		for(jj=0; jj<Data->height; jj++)	tmp += Data->dataMatrix[jj][ii];
	
		AvgVector->dataMatrix[0][ii] = (float)(tmp/Data->height);
	}

	return AvgVector;
}


//- Calculate Co-Variance Matrix -//
PR_MATHMATRIX* PR_CalcCoVarianceMatrix(
PR_MATHMATRIX* Data,
PR_MATHMATRIX* Avg
){
	
	if( Data->width!=Avg->width )
	{
		fprintf(stderr, "Error at \"PR_CalcCoVarianceMatrix\"...\n");
		exit(0);
	}

	PR_MATHMATRIX *CoVar = PR_NewMathMatrix(Data->width, Data->width);
	
	unsigned int ii, jj;
	unsigned int ih, iw;
	double *DistToAvg = new double[Data->width];

	double** MTmp = new double*[Data->width];
	for(unsigned int jj=0; jj<Data->width; jj++)
	{ 
		MTmp[jj] = new double[Data->width];
		for(unsigned int ii=0; ii<Data->width; ii++)
			MTmp[jj][ii] = 0.0;
	}

	for(jj=0; jj<Data->height; jj++){

		for(ii=0; ii<Data->width; ii++){
			DistToAvg[ii] = Data->dataMatrix[jj][ii]-Avg->dataMatrix[0][ii];
		}

		for(ih=0; ih<Data->width; ih++)
		for(iw=ih; iw<Data->width; iw++)
		{
			MTmp[ih][iw] += (DistToAvg[ih]*DistToAvg[iw]);
		}
	}

	for(ih=0; ih<Data->width; ih++)
	for(iw=0; iw<ih; iw++)
	{
		MTmp[ih][iw] = MTmp[iw][ih];
	}

	for(jj=0; jj<Data->width; jj++)
	for(ii=0; ii<Data->width; ii++)
	{
		CoVar->dataMatrix[jj][ii] = (float)(MTmp[jj][ii]/Data->height);
	}

	delete [] DistToAvg;

	for(unsigned int jj=0; jj<Data->width; jj++){  delete[] MTmp[jj]; }
	delete[] MTmp;

	return CoVar;
}

//- Calculate Mahalanobis Distance -//
PR_MATHMATRIX* CalcMahalanobisDistance(
PR_MATHMATRIX* Data,
PR_MATHMATRIX* Avg,
PR_MATHMATRIX* CoVar
){

	if( Data->width!=Avg->width || Data->width!=CoVar->width || Data->width!=CoVar->height )
	{
		fprintf(stderr, "Error at \"PR_CalcMahalanobisDistance\"...\n");
		exit(0);
	}

//	PR_MATHMATRIX* CopyVar = PR_NewMathMatrix(Data->width, Data->height); 

	unsigned short ii, jj;
	unsigned short i, j;
	int Thd=1;

	double AbsMax=0.0;

//	printf("CoVar1;\n");
	for(jj=0; jj<CoVar->height; jj++)
	{
		for(ii=0; ii<CoVar->width; ii++)
		{
//			printf("%.5f ",CoVar->dataMatrix[jj][ii]);
		}
//		printf("\n");
	}


	PR_MATHMATRIX* Dist = PR_NewMathMatrix(Data->height, 1);
	
	PR_MATHMATRIX* InvCoVar = PR_InverseMatrix(CoVar);
	PR_MATHMATRIX* DistToAvg_t = PR_NewMathMatrix(Data->width, 1);
	PR_MATHMATRIX* DistToAvg = PR_NewMathMatrix(1,Data->width);
	PR_MATHMATRIX* Interium;
	PR_MATHMATRIX* Bottom;


	//for(ii=0; ii<Data->width; ii++){ fprintf(stderr, "%f, ", Avg->dataMatrix[0][ii]); }
	//fprintf(stderr, "\n");


//	printf("Inv_CoVar;\n");
	for(jj=0; jj<InvCoVar->height; jj++)
	{
		for(ii=0; ii<InvCoVar->width; ii++)
		{
			if(AbsMax<fabs(InvCoVar->dataMatrix[jj][ii]))		AbsMax=fabs(InvCoVar->dataMatrix[jj][ii]);
//			printf("%.5f ",InvCoVar->dataMatrix[jj][ii]);
		}
//		printf("\n");
	}
	

	for(jj=0; jj<Data->height; jj++)
	{
//		printf("reg[%d/%d] feat-avg={",jj,Data->height);

		for(ii=0; ii<Data->width; ii++)
		{
			DistToAvg_t->dataMatrix[0][ii] = Data->dataMatrix[jj][ii]-Avg->dataMatrix[0][ii];
			DistToAvg->dataMatrix[ii][0] = Data->dataMatrix[jj][ii]-Avg->dataMatrix[0][ii];
//			printf("%.5f ",DistToAvg_t->dataMatrix[0][ii]);
		}
//		printf("}\n");

		Interium = PR_MultiplyMatrix(DistToAvg_t, InvCoVar);

//		printf("Interium;\n");
		for(j=0; j<Interium->height; j++)
		{
			for(i=0; i<Interium->width; i++)
			{
//				printf("%.5f ",Interium->dataMatrix[j][i]);
			}
//			printf("\n");
		}

		Bottom = PR_MultiplyMatrix(Interium, DistToAvg);

//		printf("A. Dist=%.5f",Bottom->dataMatrix[0][0]);

		Dist->dataMatrix[0][jj] = Bottom->dataMatrix[0][0];
		//fprintf(stderr, "%f\n", Dist->dataMatrix[0][jj]);

		PR_DeleteMathMatrix(Interium);
		PR_DeleteMathMatrix(Bottom);

//		printf("\n");
	}

	PR_DeleteMathMatrix(InvCoVar);
	PR_DeleteMathMatrix(DistToAvg_t);
	PR_DeleteMathMatrix(DistToAvg);

//	printf("\n");

	return Dist;
}


double CalcDeterminant(
PR_MATHMATRIX* Data
){
	if(Data->height != Data->width)
	{
		printf("> Cannot calculate determinant (H%dxW%d)\n",Data->height,Data->width);
		exit(1);
	}

	if(Data->height==1)
		return Data->dataMatrix[0][0];
	else if(Data->height==2)
	{
		double aa, bb;
		aa = Data->dataMatrix[0][0]*Data->dataMatrix[1][1];
		bb = Data->dataMatrix[0][1]*Data->dataMatrix[1][0];
		return (float)(aa-bb);
	}

	unsigned int Col, Row;
	double accum_plus, accum_minus, determinant=0.0;

	for(unsigned int Start=0; Start<Data->height; Start++)
	{
		accum_plus=1.0, accum_minus=1.0;
		for(unsigned int Step=0; Step<Data->height; Step++)
		{
			Row = Start+Step;
			if(Row>=Data->height)	Row -= Data->height;
			Col = Step;
			accum_plus *= (double)Data->dataMatrix[Row][Col];

			Col = Data->height-Step-1;
			accum_minus *= (double)Data->dataMatrix[Row][Col];
		}
		determinant += accum_plus-accum_minus;
	}

	/*
	float** input_nm = new float* [Data->height+1];
	input_nm[0] = new float [(Data->height+1)*(Data->height+1)];
	for(unsigned int ii=1; ii<=Data->height; ii++)
		input_nm[ii] = &input_nm[0][ii*(Data->height+1)];

	for(unsigned int jj=0; jj<Data->height; jj++)
		for(unsigned int ii=0; ii<Data->height; ii++)
			input_nm[jj+1][ii+1] = Data->dataMatrix[jj][ii];

	int *index = new int [Data->height+1];
	float dd;

	ludcmp(input_nm,Data->height,index,dd);

	double determinant=(double)dd;
	for(unsigned int jj=1; jj<=Data->height; jj++)
		determinant *= input_nm[jj][jj];
	
	delete [] *input_nm;
	delete [] input_nm;
	delete [] index;

	return (float)determinant;
	*/

	return (float)determinant;
}

double PR_CalcDeterminantDouble(
PR_MATHMATRIX* Data
){
	if(Data->height != Data->width)
	{
		printf("> Cannot calculate determinant (H%dxW%d)\n",Data->height,Data->width);
		exit(1);
	}

	if(Data->height==1)
		return (double)Data->dataMatrix[0][0];
	else if(Data->height==2)
	{
		double aa, bb;
		aa = Data->dataMatrix[0][0]*Data->dataMatrix[1][1];
		bb = Data->dataMatrix[0][1]*Data->dataMatrix[1][0];
		return aa-bb;
	}

	unsigned int Col, Row;
	double accum_plus, accum_minus, determinant=0.0;

//	printf("%dx%d matrix\n",Data->width,Data->height);

	for(unsigned int Start=0; Start<Data->height; Start++)
	{
		accum_plus=1.0, accum_minus=1.0;
		for(unsigned int Step=0; Step<Data->height; Step++)
		{
			Row = Start+Step;
			if(Row>=Data->height)	Row -= Data->height;
			Col = Step;
			accum_plus *= (double)Data->dataMatrix[Row][Col];
//			printf("(+)%.3f[%2d][%2d], ",Data->dataMatrix[Row][Col],Row,Col);

			Col = Data->height-Step-1;
			accum_minus *= (double)Data->dataMatrix[Row][Col];
//			printf("(-)%.3f[%2d][%2d]\n",Data->dataMatrix[Row][Col],Row,Col);
		}
		determinant += accum_plus-accum_minus;	
//		printf("(+)%.3f & (-)%.3f => %.3f; %.3f\n",
//				accum_plus,accum_minus,accum_plus-accum_minus,determinant);
	}

	/*
	float** input_nm = new float* [Data->height+1];
	input_nm[0] = new float [(Data->height+1)*(Data->height+1)];
	for(unsigned int ii=1; ii<=Data->height; ii++)
		input_nm[ii] = &input_nm[0][ii*(Data->height+1)];

	for(unsigned int jj=0; jj<Data->height; jj++)
		for(unsigned int ii=0; ii<Data->height; ii++)
			input_nm[jj+1][ii+1] = Data->dataMatrix[jj][ii];

	int *index = new int [Data->height+1];
	float dd;

	ludcmp(input_nm,Data->height,index,dd);

	double determinant=(double)dd;
	for(unsigned int jj=1; jj<=Data->height; jj++)
		determinant *= input_nm[jj][jj];
	
	delete [] *input_nm;
	delete [] input_nm;
	delete [] index;

	return (float)determinant;
	*/

	return determinant;
}


LeastSquarePolynomialParams* NewLeastSquarePolynomialParams(
int num
){
	LeastSquarePolynomialParams* output = new LeastSquarePolynomialParams;

	output->num_degree = num;
	output->Coefficient = new double [num+1];
	memset(output->Coefficient,0x00,sizeof(double)*(num+1));
	output->bias_x = 0.0;

	return output;
}

void DeleteLeastSquarePolynomialParams(
LeastSquarePolynomialParams* input
){
	delete [] input->Coefficient;
	delete input;
}

LeastSquarePolynomialParams* CalcLeastSquarePolynomialParams(
unsigned int num_sample,
double* Xi,
double* Yi,
unsigned int num_degree
){
	unsigned int ii,jj,kk;


	//- output parameters -//
	LeastSquarePolynomialParams* out = NewLeastSquarePolynomialParams(num_degree);
//	printf("out->bias_x=%e",out->bias_x);

	for(kk=0; kk<num_sample; kk++)
	{
		out->bias_x += Xi[kk]/(double)num_sample;
//		printf("->%e[%d]",out->bias_x,kk);
	}
//	printf("\nfinish!");
//	getchar();


	//- matrix for calculation -//
	double** Matrix_XPOW = new double* [num_degree+1];
	Matrix_XPOW[0] = new double [(num_degree+1)*(num_degree+1)];
	for(jj=1; jj<=num_degree; jj++)	Matrix_XPOW[jj] = &Matrix_XPOW[0][jj*(num_degree+1)];
	memset(Matrix_XPOW[0],0x00,sizeof(double)*(num_degree+1)*(num_degree+1));

	double* Matrix_XY = new double [num_degree+1];
	memset(Matrix_XY,0x00,sizeof(double)*(num_degree+1));


	//- make matrix -//
	for(kk=0; kk<num_sample; kk++)
	{
		double x_dummy = Xi[kk] - out->bias_x;
		for(jj=0; jj<=num_degree; jj++)
		{
			Matrix_XY[jj] += Yi[kk]*pow(x_dummy,(double)jj);
			for(ii=0; ii<=num_degree; ii++)	Matrix_XPOW[jj][ii] += pow(x_dummy,(double)(jj+ii));
		}
	}
/*
	for(jj=0; jj<=num_degree; jj++)
	{
		for(ii=0; ii<=num_degree; ii++)	printf("%e ",Matrix_XPOW[jj][ii]);
		printf("\n");
	}
	for(jj=0; jj<=num_degree; jj++)		printf("%e ",Matrix_XY[jj]);
	printf("\n");
*/


	//- inverse matrix -//
	PR_MATHMATRIX* fMatrix_XPow = PR_NewMathMatrix(num_degree+1,num_degree+1);

	for(jj=0; jj<=num_degree; jj++)
	for(ii=0; ii<=num_degree; ii++)
		fMatrix_XPow->dataMatrix[jj][ii] = (float)Matrix_XPOW[jj][ii];

	PR_MATHMATRIX* InvMatrix = PR_InverseMatrix(fMatrix_XPow);

	for(jj=0; jj<=num_degree; jj++)
	for(ii=0; ii<=num_degree; ii++)
		Matrix_XPOW[jj][ii] = (double)InvMatrix->dataMatrix[jj][ii];

	PR_DeleteMathMatrix(fMatrix_XPow);
	PR_DeleteMathMatrix(InvMatrix);
/*
	for(jj=0; jj<=num_degree; jj++)
	{
		for(ii=0; ii<=num_degree; ii++)	printf("%e ",Matrix_XPOW[jj][ii]);
		printf("\n");
	}
*/

	//- multiply -//
	for(jj=0; jj<=num_degree; jj++)
	for(ii=0; ii<=num_degree; ii++)
		out->Coefficient[jj] += Matrix_XPOW[jj][ii]*Matrix_XY[ii];
/*
	printf("ANS; \n");
	for(jj=0; jj<=num_degree; jj++)		printf("%e ",out->Coefficient[jj]);
	printf("\n");
*/

	//- print -//
	fprintf(stderr,"y = ");
	for(jj=0; jj<=num_degree; jj++)
	{
		fprintf(stderr,"a%d*X^%d",jj,jj);
		if(jj!=num_degree)	fprintf(stderr," + ");
		else				fprintf(stderr,"\n");
	}
	fprintf(stderr,"(X=x-%f)\n",out->bias_x);

	for(jj=0; jj<=num_degree; jj++)
		fprintf(stderr,"a%d = %+e\n",jj,out->Coefficient[jj]);


	//- delete -//
	delete [] *Matrix_XPOW;
	delete [] Matrix_XPOW;
	delete [] Matrix_XY;


	return out;
}


int 
PR_LUDecompose(PR_MATHMATRIX* matrix)
{
	unsigned int ii, jj, kk;

	PR_MATHMATRIX* mL = PR_NewMathMatrix(matrix->width,matrix->height);
	PR_MATHMATRIX* mU = PR_NewMathMatrix(matrix->width,matrix->height);


	for(jj=0; jj<matrix->height; jj++)
	{
		mL->dataMatrix[jj][jj]=1.0;
		mU->dataMatrix[0][jj]=matrix->dataMatrix[0][jj];
	}

	for(ii=0; ii<matrix->width; ii++)
	{
		mL->dataMatrix[ii][ii]=1.0;
	//	mU->dataMatrix[0][ii]=matrix->dataMatrix[0][ii];

		for(jj=0; jj<=ii; jj++)
		{
			mU->dataMatrix[jj][ii] = matrix->dataMatrix[jj][ii];

			if(ii>0)
			for(kk=0; kk<=ii-1; kk++)
			if(mL->dataMatrix[jj][kk]!=0.0 && mU->dataMatrix[kk][ii]!=0.0)
				mU->dataMatrix[jj][ii] -= mL->dataMatrix[jj][kk]*mU->dataMatrix[kk][ii];
		}

		for(jj=ii+1; jj<matrix->height; jj++)
		{
			mL->dataMatrix[jj][ii] = matrix->dataMatrix[jj][ii];

			if(jj>0)
			for(kk=0; kk<=jj-1; kk++)
			if(mL->dataMatrix[jj][kk]!=0.0 && mU->dataMatrix[kk][ii]!=0.0)
				mL->dataMatrix[jj][ii] -= mL->dataMatrix[jj][kk]*mU->dataMatrix[kk][ii];

			mL->dataMatrix[jj][ii] /= mU->dataMatrix[ii][ii];
		}
	}


	for(jj=0; jj<matrix->height; jj++)
	{
		for(ii=0; ii<matrix->width; ii++)	printf("%.3e ",matrix->dataMatrix[jj][ii]);
		printf("\n");
	}
	printf("\n");

	for(jj=0; jj<matrix->height; jj++)
	{
		for(ii=0; ii<matrix->width; ii++)	printf("%.3e ",mL->dataMatrix[jj][ii]);
		printf("\n");
	}
	printf("\n");

	for(jj=0; jj<matrix->height; jj++)
	{
		for(ii=0; ii<matrix->width; ii++)	printf("%.3e ",mU->dataMatrix[jj][ii]);
		printf("\n");
	}
	printf("\n");

	getchar();
	

	return 0;
}


int PR_PrintMathMatrix(PR_MATHMATRIX* data)
{
	unsigned int i, j;

	for(j=0; j<data->height; j++)
	{
		for(i=0; i<data->width; i++)
		{
			printf("%.3e ",data->dataMatrix[j][i]);
		}
		printf("\n");
	}

	return PR_EOT;
}

