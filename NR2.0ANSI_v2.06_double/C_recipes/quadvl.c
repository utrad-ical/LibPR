#define NRANSI
#include "nrutil.h"

void quadvl(double x, double y, double *fa, double *fb, double *fc, double *fd)
{
	double qa,qb,qc,qd;

	qa=FMIN(2.0,FMAX(0.0,1.0-x));
	qb=FMIN(2.0,FMAX(0.0,1.0-y));
	qc=FMIN(2.0,FMAX(0.0,x+1.0));
	qd=FMIN(2.0,FMAX(0.0,y+1.0));
	*fa=0.25*qa*qb;
	*fb=0.25*qb*qc;
	*fc=0.25*qc*qd;
	*fd=0.25*qd*qa;
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
