#include <math.h>

void tptest(double data1[], double data2[], unsigned long n, double *t,
	double *prob)
{
	void avevar(double data[], unsigned long n, double *ave, double *var);
	double betai(double a, double b, double x);
	unsigned long j;
	double var1,var2,ave1,ave2,sd,df,cov=0.0;

	avevar(data1,n,&ave1,&var1);
	avevar(data2,n,&ave2,&var2);
	for (j=1;j<=n;j++)
		cov += (data1[j]-ave1)*(data2[j]-ave2);
	cov /= df=n-1;
	sd=sqrt((var1+var2-2.0*cov)/n);
	*t=(ave1-ave2)/sd;
	*prob=betai(0.5*df,0.5,df/(df+(*t)*(*t)));
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
