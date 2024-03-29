/* Driver for routine pcshft */

#include <stdio.h>
#include <math.h>
#define NRANSI
#include "nr.h"

#define NVAL 40
#define PIO2 1.5707963

float func(float x)
{
	return x*x*(x*x-2.0)*sin(x);
}

int main(void)
{
	int i,j,mval;
	float a=(-PIO2),b=PIO2,poly,x;
	float c[NVAL],d[NVAL];

	chebft(a,b,c,NVAL,func);
	for (;;) {
		printf("\nHow many terms in Chebyshev evaluation?\n");
		printf("Enter n between 6 and %2d. (n=0 to end).\n",NVAL);
		scanf("%d",&mval);
		if ((mval <= 0) || (mval > NVAL)) break;
		chebpc(c,d,mval);
		pcshft(a,b,d,mval);
		/* Test shifted polynomial */
		printf("\n%9s %14s %14s\n","x","actual","polynomial");
		for (i = -8;i<=8;i++) {
			x=i*PIO2/10.0;
			poly=d[mval-1];
			for (j=mval-2;j>=0;j--) poly=poly*x+d[j];
			printf("%12.6f %12.6f %12.6f\n",x,func(x),poly);
		}
	}
	return 0;
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
