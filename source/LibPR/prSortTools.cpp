
#include "prSortTools.h"
#include "prSortTools.private.h"

#include <stdio.h>
#include <string.h>


/* pivotting */
unsigned long PR_Pivot(float *a, char mode, unsigned long i, unsigned long j)
{
	unsigned long k=i+1;

	while(k<=j && a[i]==a[k]) k++;

	if(k>j)			return -1;
	if(mode==1 && a[i]>=a[k])	return i;
	if(mode==2 && a[i]<=a[k])	return i;	
	return k;
}

unsigned long PR_DataSwapAndSprit(float* Dat, char mode, unsigned long top, unsigned long end, float ref)
{
	unsigned long ii=top, jj=end;
//	printf("swap_range:%d~%d, ref=%e\n",ii,jj,ref);

	while(ii<=jj)
	{
		while(ii<=end && ((mode==1 && Dat[ii]<ref) || (mode==2 && Dat[ii]>ref)))	ii++;
		while(jj>=top && ((mode==1 && Dat[jj]>=ref) || (mode==2 && Dat[jj]<=ref)))	jj--;
//		printf("(%d) dat[i%d]=%e, dat[j%d]=%e",mode,ii,Dat[ii],jj,Dat[jj]);	getchar();
		if(ii>jj)	break;

		float dmy=Dat[ii];
		Dat[ii]=Dat[jj];	Dat[jj]=dmy;
	}
	return ii;
}

void PR_QSortRegression(float* Dat, char mode, unsigned long top, unsigned long end)
{
	if(top>=end)	return;

	unsigned long posi = PR_Pivot(Dat,mode,top,end);
	unsigned long divi;
//	printf("\n> mode=%d, ref_posi=%d, ",mode,posi);

	if(posi!=-1)
	{
		divi = PR_DataSwapAndSprit(Dat,mode,top,end,Dat[posi]);
		PR_QSortRegression(Dat,mode,top,divi-1);
		PR_QSortRegression(Dat,mode,divi,end);
	}
}

void PR_QuickSortRecurrence(float* data, unsigned long num, char mode)
{
	PR_QSortRegression(data,mode,0,num-1);
}

void PR_QuickSortAscending(float* data, unsigned long num)
{
	PR_QuickSortRecurrence(data, num, QSORT_MODE_ASCENDING);
}

void PR_QuickSortDescending(float *data, unsigned long num)
{
	PR_QuickSortRecurrence(data, num, QSORT_MODE_DESCENDING);
}



/* partition */
unsigned long PR_Partition(float *a, unsigned long *b, unsigned long i, unsigned long j, float target)
{
	unsigned long l=i,r=j;

	// åüçıÇ™åç∑Ç∑ÇÈÇ‹Ç≈åJÇËï‘ÇµÇ‹Ç∑
	while(l<=r)
	{
		while(l<=j && a[l]<target)  l++;	// é≤óvëfà»è„ÇÃÉfÅ[É^ÇíTÇµÇ‹Ç∑
		while(r>=i && a[r]>=target) r--;	// é≤óvëfñ¢ñûÇÃÉfÅ[É^ÇíTÇµÇ‹Ç∑
		if(l>r)	break;

		float fdmy=a[l];
		a[l]=a[r];		a[r]=fdmy;
		unsigned long uldmy=b[l];
		b[l]=b[r];		b[r]=uldmy;
		l++; r--;
	}
	return l;
}

/* quick sort recurrence edition */
void PR_QuickSortRecurrenceWithRank(float *a, unsigned long *b, unsigned long i, unsigned long j)
{
	if(i>=j)	return;

	unsigned long p = PR_Pivot(a,1,i,j);
//	printf("%d~%d->p[%d]=%e, ",i,j,p,a[p]);

	if(p>=0 && p<=j)
	{
		unsigned int k = PR_Partition(a,b,i,j,a[p]);
		PR_QuickSortRecurrenceWithRank(a,b,i,k-1);
		PR_QuickSortRecurrenceWithRank(a,b,k,j);
	}
//	printf("bb,");
}

/* quick sort (ascending order) mein */
unsigned long* PR_QuickSortAscendingWithRank(float *a, unsigned long n)
{
	unsigned long i;
	unsigned long *b = new unsigned long [n];
	for(i=0; i<n; i++)	b[i]=i;

	PR_QuickSortRecurrenceWithRank(a,b,0,n-1);
	return b;
}

/* quick sort (descending order) mein */
unsigned long* PR_QuickSortDescendingWithRank(float *a, unsigned long n)
{
	float *a_dmy = new float [n];
	memcpy(a_dmy,a,sizeof(float)*n);

	unsigned long *b_dmy = PR_QuickSortAscendingWithRank(a_dmy,n);
	unsigned long *b = new unsigned long [n];

	unsigned long i;
	for(i=0; i<n; i++)
	{
		a[i]=a_dmy[(n-1)-i];
		b[i]=b_dmy[(n-1)-i];
	}

	delete [] a_dmy;	delete [] b_dmy;

	return b;
}



/* pivotting */
unsigned long PR_Pivot64(double *a, char mode, unsigned long i, unsigned long j)
{
	unsigned long k=i+1;

	while(k<=j && a[i]==a[k]) k++;

	if(k>j)			return -1;
	if(mode==1 && a[i]>=a[k])	return i;
	if(mode==2 && a[i]<=a[k])	return i;	
	return k;
}

unsigned long PR_DataSwapAndSprit64(double* Dat, char mode, unsigned long top, unsigned long end, double ref)
{
	unsigned long ii=top, jj=end;
//	printf("swap_range:%d~%d, ref=%e\n",ii,jj,ref);

	while(ii<=jj)
	{
		while(ii<=end && ((mode==1 && Dat[ii]<ref) || (mode==2 && Dat[ii]>ref)))	ii++;
		while(jj>=top && ((mode==1 && Dat[jj]>=ref) || (mode==2 && Dat[jj]<=ref)))	jj--;
//		printf("(%d) dat[i%d]=%e, dat[j%d]=%e",mode,ii,Dat[ii],jj,Dat[jj]);	getchar();
		if(ii>jj)	break;

		double dmy=Dat[ii];
		Dat[ii]=Dat[jj];	Dat[jj]=dmy;
	}
	return ii;
}

void PR_QSortRegression64(double* Dat, char mode, unsigned long top, unsigned long end)
{
	if(top>=end)	return;

	unsigned long posi = PR_Pivot64(Dat,mode,top,end);
	unsigned long divi;
//	printf("\n> mode=%d, ref_posi=%d, ",mode,posi);

	if(posi!=-1)
	{
		divi = PR_DataSwapAndSprit64(Dat,mode,top,end,Dat[posi]);
		PR_QSortRegression64(Dat,mode,top,divi-1);
		PR_QSortRegression64(Dat,mode,divi,end);
	}
}

void PR_QuickSortRecurrence64(double* data, unsigned long num, char mode)
{
	PR_QSortRegression64(data,mode,0,num-1);
}

void PR_QuickSortAscending64(double* data, unsigned long num)
{
	PR_QuickSortRecurrence64(data, num, QSORT_MODE_ASCENDING);
}

void PR_QuickSortDescending64(double *data, unsigned long num)
{
	PR_QuickSortRecurrence64(data, num, QSORT_MODE_DESCENDING);
}

/* partition */
unsigned long PR_Partition64(double *a, unsigned long *b, unsigned long i, unsigned long j, double target)
{
	unsigned long l=i,r=j;

	// åüçıÇ™åç∑Ç∑ÇÈÇ‹Ç≈åJÇËï‘ÇµÇ‹Ç∑
	while(l<=r)
	{
		while(l<=j && a[l]<target)  l++;	// é≤óvëfà»è„ÇÃÉfÅ[É^ÇíTÇµÇ‹Ç∑
		while(r>=i && a[r]>=target) r--;	// é≤óvëfñ¢ñûÇÃÉfÅ[É^ÇíTÇµÇ‹Ç∑
		if(l>r)	break;

		double fdmy=a[l];
		a[l]=a[r];		a[r]=fdmy;
		unsigned long uldmy=b[l];
		b[l]=b[r];		b[r]=uldmy;
		l++; r--;
	}
	return l;
}

/* quick sort recurrence edition */
void PR_QuickSortRecurrenceWithRank64(double *a, unsigned long *b, unsigned long i, unsigned long j)
{
	if(i>=j)	return;

	unsigned long p = PR_Pivot64(a,1,i,j);
//	printf("%d~%d->p[%d]=%e, ",i,j,p,a[p]);

	if(p>=0 && p<=j)
	{
		unsigned int k = PR_Partition64(a,b,i,j,a[p]);
		PR_QuickSortRecurrenceWithRank64(a,b,i,k-1);
		PR_QuickSortRecurrenceWithRank64(a,b,k,j);
	}
//	printf("bb,");
}

/* quick sort (ascending order) mein */
unsigned long* PR_QuickSortAscendingWithRank64(double *a, unsigned long n)
{
	unsigned long i;
	unsigned long *b = new unsigned long [n];
	for(i=0; i<n; i++)	b[i]=i;

	PR_QuickSortRecurrenceWithRank64(a,b,0,n-1);
	return b;
}

/* quick sort (descending order) mein */
unsigned long* PR_QuickSortDescendingWithRank64(double *a, unsigned long n)
{
	double *a_dmy = new double [n];
	memcpy(a_dmy,a,sizeof(double)*n);

	unsigned long *b_dmy = PR_QuickSortAscendingWithRank64(a_dmy,n);
	unsigned long *b = new unsigned long [n];

	unsigned long i;
	for(i=0; i<n; i++)
	{
		a[i]=a_dmy[(n-1)-i];
		b[i]=b_dmy[(n-1)-i];
	}

	delete [] a_dmy;	delete [] b_dmy;

	return b;
}










/* pivotting */
unsigned int PR_PivotOld(float *a, char mode, unsigned int i, unsigned int j)
{
	unsigned int k=i+1;

	while(k<=j && a[i]==a[k]) k++;

	if(k>j)			return -1;
	if(mode==1 && a[i]>=a[k])	return i;
	if(mode==2 && a[i]<=a[k])	return i;	
	return k;
}

unsigned int PR_DataSwapAndSpritOld(float* Dat, char mode, unsigned int top, unsigned int end, float ref)
{
	unsigned int ii=top, jj=end;
//	printf("swap_range:%d~%d, ref=%e\n",ii,jj,ref);

	while(ii<=jj)
	{
		while(ii<=end && ((mode==1 && Dat[ii]<ref) || (mode==2 && Dat[ii]>ref)))	ii++;
		while(jj>=top && ((mode==1 && Dat[jj]>=ref) || (mode==2 && Dat[jj]<=ref)))	jj--;
//		printf("(%d) dat[i%d]=%e, dat[j%d]=%e",mode,ii,Dat[ii],jj,Dat[jj]);	getchar();
		if(ii>jj)	break;

		float dmy=Dat[ii];
		Dat[ii]=Dat[jj];	Dat[jj]=dmy;
	}
	return ii;
}

void PR_QSortRegressionOld(float* Dat, char mode, unsigned int top, unsigned int end)
{
	if(top>=end)	return;

	unsigned int posi = PR_Pivot(Dat,mode,top,end);
	unsigned int divi;
//	printf("\n> mode=%d, ref_posi=%d, ",mode,posi);

	if(posi!=-1)
	{
		divi = PR_DataSwapAndSpritOld(Dat,mode,top,end,Dat[posi]);
		PR_QSortRegressionOld(Dat,mode,top,divi-1);
		PR_QSortRegressionOld(Dat,mode,divi,end);
	}
}

void PR_QuickSortRecurrenceOld(float* data, unsigned int num, char mode)
{
	PR_QSortRegressionOld(data,mode,0,num-1);
}

void PR_QuickSortAscendingOld(float* data, unsigned int num)
{
	PR_QuickSortRecurrenceOld(data, num, QSORT_MODE_ASCENDING);
}

void PR_QuickSortDescendingOld(float *data, unsigned int num)
{
	PR_QuickSortRecurrenceOld(data, num, QSORT_MODE_DESCENDING);
}


/* partition */
unsigned int PR_PartitionOld(float *a, unsigned int *b, unsigned int i, unsigned int j, float target)
{
	unsigned int l=i,r=j;

	// åüçıÇ™åç∑Ç∑ÇÈÇ‹Ç≈åJÇËï‘ÇµÇ‹Ç∑
	while(l<=r)
	{
		while(l<=j && a[l]<target)  l++;	// é≤óvëfà»è„ÇÃÉfÅ[É^ÇíTÇµÇ‹Ç∑
		while(r>=i && a[r]>=target) r--;	// é≤óvëfñ¢ñûÇÃÉfÅ[É^ÇíTÇµÇ‹Ç∑
		if(l>r)	break;

		float fdmy=a[l];
		a[l]=a[r];		a[r]=fdmy;
		unsigned int uldmy=b[l];
		b[l]=b[r];		b[r]=uldmy;
		l++; r--;
	}
	return l;
}

/* quick sort recurrence edition */
void PR_QuickSortRecurrenceWithRankOld(float *a, unsigned int *b, unsigned int i, unsigned int j)
{
	if(i>=j)	return;

	unsigned int p = PR_Pivot(a,1,i,j);
//	printf("%d~%d->p[%d]=%e, ",i,j,p,a[p]);

	if(p>=0 && p<=j)
	{
		unsigned int k = PR_PartitionOld(a,b,i,j,a[p]);
		PR_QuickSortRecurrenceWithRankOld(a,b,i,k-1);
		PR_QuickSortRecurrenceWithRankOld(a,b,k,j);
	}
//	printf("bb,");
}

/* quick sort (ascending order) mein */
unsigned int* PR_QuickSortAscendingWithRankOld(float *a, unsigned int n)
{
	unsigned int i;
	unsigned int *b = new unsigned int [n];
	for(i=0; i<n; i++)	b[i]=i;

	PR_QuickSortRecurrenceWithRankOld(a,b,0,n-1);
	return b;
}

/* quick sort (descending order) mein */
unsigned int* PR_QuickSortDescendingWithRankOld(float *a, unsigned int n)
{
	float *a_dmy = new float [n];
	memcpy(a_dmy,a,sizeof(float)*n);

	unsigned int *b_dmy = PR_QuickSortAscendingWithRankOld(a_dmy,n);
	unsigned int *b = new unsigned int [n];

	unsigned int i;
	for(i=0; i<n; i++)
	{
		a[i]=a_dmy[(n-1)-i];
		b[i]=b_dmy[(n-1)-i];
	}

	delete [] a_dmy;	delete [] b_dmy;

	return b;
}


