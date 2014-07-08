#include <math.h>
#include "complex.h"
#define NRANSI
#include "nrutil.h"
#define EPS 1.0e-6

fcomplex aa,bb,cc,z0,dz;

int kmax,kount;
double *xp,**yp,dxsav;

fcomplex hypgeo(fcomplex a, fcomplex b, fcomplex c, fcomplex z)
{
	void bsstep(double y[], double dydx[], int nv, double *xx, double htry,
		double eps, double yscal[], double *hdid, double *hnext,
		void (*derivs)(double, double [], double []));
	void hypdrv(double s, double yy[], double dyyds[]);
	void hypser(fcomplex a, fcomplex b, fcomplex c, fcomplex z,
		fcomplex *series, fcomplex *deriv);
	void odeint(double ystart[], int nvar, double x1, double x2,
		double eps, double h1, double hmin, int *nok, int *nbad,
		void (*derivs)(double, double [], double []),
		void (*rkqs)(double [], double [], int, double *, double, double,
		double [], double *, double *, void (*)(double, double [], double [])));
	int nbad,nok;
	fcomplex ans,y[3];
	double *yy;

	kmax=0;
	if (z.r*z.r+z.i*z.i <= 0.25) {
		hypser(a,b,c,z,&ans,&y[2]);
		return ans;
	}
	else if (z.r < 0.0) z0=Complex(-0.5,0.0);
	else if (z.r <= 1.0) z0=Complex(0.5,0.0);
	else z0=Complex(0.0,z.i >= 0.0 ? 0.5 : -0.5);
	aa=a;
	bb=b;
	cc=c;
	dz=Csub(z,z0);
	hypser(aa,bb,cc,z0,&y[1],&y[2]);
	yy=vector(1,4);
	yy[1]=y[1].r;
	yy[2]=y[1].i;
	yy[3]=y[2].r;
	yy[4]=y[2].i;
	odeint(yy,4,0.0,1.0,EPS,0.1,0.0001,&nok,&nbad,hypdrv,bsstep);
	y[1]=Complex(yy[1],yy[2]);
	free_vector(yy,1,4);
	return y[1];
}
#undef EPS
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
