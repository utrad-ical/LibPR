double erff(double x)
{
	double gammp(double a, double x);

	return x < 0.0 ? -gammp(0.5,x*x) : gammp(0.5,x*x);
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
