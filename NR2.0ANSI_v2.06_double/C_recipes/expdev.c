#include <math.h>

double expdev(long *idum)
{
	double ran1(long *idum);
	double dum;

	do
		dum=ran1(idum);
	while (dum == 0.0);
	return -log(dum);
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
