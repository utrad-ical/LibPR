#include <math.h>
#define EPS 1.0e-6
#define JMAX 20

double qsimp(double (*func)(double), double a, double b)
{
	double trapzd(double (*func)(double), double a, double b, int n);
	void nrerror(char error_text[]);
	int j;
	double s,st,ost,os;

	ost = os = -1.0e30;
	for (j=1;j<=JMAX;j++) {
		st=trapzd(func,a,b,j);
		s=(4.0*st-ost)/3.0;
		if (fabs(s-os) < EPS*fabs(os)) return s;
		if (s == 0.0 && os == 0.0 && j > 6) return s;
		os=s;
		ost=st;
	}
	nrerror("Too many steps in routine qsimp");
	return 0.0;
}
#undef EPS
#undef JMAX
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
