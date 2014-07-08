extern long idum;

void ranpt(double pt[], double regn[], int n)
{
	double ran1(long *idum);
	int j;

	for (j=1;j<=n;j++)
		pt[j]=regn[j]+(regn[n+j]-regn[j])*ran1(&idum);
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
