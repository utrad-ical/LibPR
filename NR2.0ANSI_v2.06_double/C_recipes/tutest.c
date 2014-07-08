#include <math.h>
#define NRANSI
#include "nrutil.h"

void tutest(double data1[], unsigned long n1, double data2[], unsigned long n2,
	double *t, double *prob)
{
	void avevar(double data[], unsigned long n, double *ave, double *var);
	double betai(double a, double b, double x);
	double var1,var2,df,ave1,ave2;

	avevar(data1,n1,&ave1,&var1);
	avevar(data2,n2,&ave2,&var2);
	*t=(ave1-ave2)/sqrt(var1/n1+var2/n2);
	df=SQR(var1/n1+var2/n2)/(SQR(var1/n1)/(n1-1)+SQR(var2/n2)/(n2-1));
	*prob=betai(0.5*df,0.5,df/(df+SQR(*t)));
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
