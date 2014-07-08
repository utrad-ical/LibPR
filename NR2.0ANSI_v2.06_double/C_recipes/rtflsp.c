#include <math.h>
#define MAXIT 30

double rtflsp(double (*func)(double), double x1, double x2, double xacc)
{
	void nrerror(char error_text[]);
	int j;
	double fl,fh,xl,xh,swap,dx,del,f,rtf;

	fl=(*func)(x1);
	fh=(*func)(x2);
	if (fl*fh > 0.0) nrerror("Root must be bracketed in rtflsp");
	if (fl < 0.0) {
		xl=x1;
		xh=x2;
	} else {
		xl=x2;
		xh=x1;
		swap=fl;
		fl=fh;
		fh=swap;
	}
	dx=xh-xl;
	for (j=1;j<=MAXIT;j++) {
		rtf=xl+dx*fl/(fl-fh);
		f=(*func)(rtf);
		if (f < 0.0) {
			del=xl-rtf;
			xl=rtf;
			fl=f;
		} else {
			del=xh-rtf;
			xh=rtf;
			fh=f;
		}
		dx=xh-xl;
		if (fabs(del) < xacc || f == 0.0) return rtf;
	}
	nrerror("Maximum number of iterations exceeded in rtflsp");
	return 0.0;
}
#undef MAXIT
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
