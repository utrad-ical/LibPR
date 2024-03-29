#include <math.h>

void choldc(double **a, int n, double p[])
{
	void nrerror(char error_text[]);
	int i,j,k;
	double sum;

	for (i=1;i<=n;i++) {
		for (j=i;j<=n;j++) {
			for (sum=a[i][j],k=i-1;k>=1;k--) sum -= a[i][k]*a[j][k];
			if (i == j) {
				if (sum <= 0.0)
					nrerror("choldc failed");
				p[i]=sqrt(sum);
			} else a[j][i]=sum/p[i];
		}
	}
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
