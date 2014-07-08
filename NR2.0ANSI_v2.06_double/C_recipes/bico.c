#include <math.h>

double bico(int n, int k)
{
	double factln(int n);

	return floor(0.5+exp(factln(n)-factln(k)-factln(n-k)));
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
