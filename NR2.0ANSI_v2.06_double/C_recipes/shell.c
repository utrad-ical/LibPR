void shell(unsigned long n, double a[])
{
	unsigned long i,j,inc;
	double v;
	inc=1;
	do {
		inc *= 3;
		inc++;
	} while (inc <= n);
	do {
		inc /= 3;
		for (i=inc+1;i<=n;i++) {
			v=a[i];
			j=i;
			while (a[j-inc] > v) {
				a[j]=a[j-inc];
				j -= inc;
				if (j <= inc) break;
			}
			a[j]=v;
		}
	} while (inc > 1);
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
