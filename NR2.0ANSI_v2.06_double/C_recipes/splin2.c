#define NRANSI
#include "nrutil.h"

void splin2(double x1a[], double x2a[], double **ya, double **y2a, int m, int n,
	double x1, double x2, double *y)
{
	void spline(double x[], double y[], int n, double yp1, double ypn, double y2[]);
	void splint(double xa[], double ya[], double y2a[], int n, double x, double *y);
	int j;
	double *ytmp,*yytmp;

	ytmp=vector(1,m);
	yytmp=vector(1,m);
	for (j=1;j<=m;j++)
		splint(x2a,ya[j],y2a[j],n,x2,&yytmp[j]);
	spline(x1a,yytmp,m,1.0e30,1.0e30,ytmp);
	splint(x1a,yytmp,ytmp,m,x1,y);
	free_vector(yytmp,1,m);
	free_vector(ytmp,1,m);
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
