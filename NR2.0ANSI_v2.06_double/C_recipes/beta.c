#include <math.h>

double beta(double z, double w)
{
	double gammln(double xx);

	return exp(gammln(z)+gammln(w)-gammln(z+w));
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
