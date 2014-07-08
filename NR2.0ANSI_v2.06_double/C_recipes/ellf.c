#include <math.h>
#define NRANSI
#include "nrutil.h"

double ellf(double phi, double ak)
{
	double rf(double x, double y, double z);
	double s;

	s=sin(phi);
	return s*rf(SQR(cos(phi)),(1.0-s*ak)*(1.0+s*ak),1.0);
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
