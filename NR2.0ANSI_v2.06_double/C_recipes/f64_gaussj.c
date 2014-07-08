#include <math.h>
#include <stdlib.h>

#define NRANSI
#include "f64_nrutil.h"
//#include "../C_others/f64_nrutil.h"

#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

double** f64_malloc_2d_double(int n)
{
	int i;
	double **dd2 = (double **)malloc(sizeof(double *)*n);
	double *dd1 = (double *)malloc(sizeof(double *)*n*n);

	for(i=0;i<n;i++)
		dd2[i]=dd1+i*n;

	for(i=0;i<n*n;i++)
		dd1[i]=0.0;
	
	return dd2;
}

void f64_free_2d_double(double** dd2)
{
	free(*dd2);
	free(dd2);
}


void f64_gaussj(double **a, int n, double **b, int m)
{
	int *indxc,*indxr,*ipiv;
	int i,icol,irow,j,k,l,ll;
	double big,dum,pivinv,temp;

	indxc=f64_ivector(1,n);
	indxr=f64_ivector(1,n);
	ipiv=f64_ivector(1,n);

	for (j=1;j<=n;j++) ipiv[j]=0;
	for (i=1;i<=n;i++) {
		big=0.0;
		for (j=1;j<=n;j++)
			if (ipiv[j] != 1)
				for (k=1;k<=n;k++) {
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big=fabs(a[j][k]);
							irow=j;
							icol=k;
						}
					} else if (ipiv[k] > 1) f64_nrerror("gaussj: Singular Matrix-1");
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l=1;l<=n;l++) SWAP(a[irow][l],a[icol][l])
			for (l=1;l<=m;l++) SWAP(b[irow][l],b[icol][l])
		}
		indxr[i]=irow;
		indxc[i]=icol;
		if (a[icol][icol] == 0.0) f64_nrerror("gaussj: Singular Matrix-2");
		pivinv=1.0/a[icol][icol];
		a[icol][icol]=1.0;
		for (l=1;l<=n;l++) a[icol][l] *= pivinv;
		for (l=1;l<=m;l++) b[icol][l] *= pivinv;
		for (ll=1;ll<=n;ll++)
			if (ll != icol) {
				dum=a[ll][icol];
				a[ll][icol]=0.0;
				for (l=1;l<=n;l++) a[ll][l] -= a[icol][l]*dum;
				for (l=1;l<=m;l++) b[ll][l] -= b[icol][l]*dum;
			}
	}
	for (l=n;l>=1;l--) {
		if (indxr[l] != indxc[l])
			for (k=1;k<=n;k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	}
	f64_free_ivector(ipiv,1,n);
	f64_free_ivector(indxr,1,n);
	f64_free_ivector(indxc,1,n);
}
#undef SWAP
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
