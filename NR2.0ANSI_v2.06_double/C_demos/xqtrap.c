/* Driver for routine qtrap */

#include <stdio.h>
#include <math.h>
#define NRANSI
#include "nr.h"

#define PIO2 1.5707963

/* Test function */
float func(float x)
{
	return x*x*(x*x-2.0)*sin(x);
}

/* Integral of test function */
float fint(float x)
{
	return 4.0*x*(x*x-7.0)*sin(x)-(pow(x,4.0)-14.0*x*x+28.0)*cos(x);
}

int main(void)
{
	float a=0.0,b=PIO2,s;

	printf("Integral of func computed with QTRAP\n\n");
	printf("Actual value of integral is %12.6f\n",fint(b)-fint(a));
	s=qtrap(func,a,b);
	printf("Result from routine QTRAP is %12.6f\n",s);
	return 0;
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
