#define NRANSI
#include "nrutil.h"

void sort3(unsigned long n, double ra[], double rb[], double rc[])
{
	void indexx(unsigned long n, double arr[], unsigned long indx[]);
	unsigned long j,*iwksp;
	double *wksp;

	iwksp=lvector(1,n);
	wksp=vector(1,n);
	indexx(n,ra,iwksp);
	for (j=1;j<=n;j++) wksp[j]=ra[j];
	for (j=1;j<=n;j++) ra[j]=wksp[iwksp[j]];
	for (j=1;j<=n;j++) wksp[j]=rb[j];
	for (j=1;j<=n;j++) rb[j]=wksp[iwksp[j]];
	for (j=1;j<=n;j++) wksp[j]=rc[j];
	for (j=1;j<=n;j++) rc[j]=wksp[iwksp[j]];
	free_vector(wksp,1,n);
	free_lvector(iwksp,1,n);
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
