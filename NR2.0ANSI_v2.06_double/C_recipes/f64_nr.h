#ifdef __cplusplus
extern "C" {
#endif

#ifndef _NR_H_
#define _NR_H_

#ifndef _FCOMPLEX_DECLARE_T_
typedef struct FCOMPLEX {double r,i;} fcomplex;
#define _FCOMPLEX_DECLARE_T_
#endif /* _FCOMPLEX_DECLARE_T_ */

#ifndef _ARITHCODE_DECLARE_T_
typedef struct {
	unsigned int *ilob,*iupb,*ncumfq,jdif,nc,minint,nch,ncum,nrad;
} arithcode;
#define _ARITHCODE_DECLARE_T_
#endif /* _ARITHCODE_DECLARE_T_ */

#ifndef _HUFFCODE_DECLARE_T_
typedef struct {
	unsigned int *icod,*ncod,*left,*right,nch,nodemax;
} huffcode;
#define _HUFFCODE_DECLARE_T_
#endif /* _HUFFCODE_DECLARE_T_ */

#include <stdio.h>

#if defined(__STDC__) || defined(ANSI) || defined(NRANSI) /* ANSI */

void f64_addint(double **uf, double **uc, double **res, int nf);
void f64_airy(double x, double *ai, double *bi, double *aip, double *bip);
void f64_amebsa(double **p, double y[], int ndim, double pb[],	double *yb, double ftol, double (*funk)(double []), int *iter, double temptr);
void f64_amoeba(double **p, double y[], int ndim, double ftol, double (*funk)(double []), int *iter);
double f64_amotry(double **p, double y[], double psum[], int ndim, double (*funk)(double []), int ihi, double fac);
double f64_amotsa(double **p, double y[], double psum[], int ndim, double pb[], double *yb, double (*funk)(double []), int ihi, double *yhi, double fac);
void f64_anneal(double x[], double y[], int iorder[], int ncity);
double f64_anorm2(double **a, int n);
void f64_arcmak(unsigned int nfreq[], unsigned int nchh, unsigned int nradd, arithcode *acode);
void f64_arcode(unsigned int *ich, unsigned char **codep, unsigned int *lcode, unsigned int *lcd, int isign, arithcode *acode);
void f64_arcsum(unsigned int iin[], unsigned int iout[], unsigned int ja, int nwk, unsigned int nrad, unsigned int nc);
void f64_asolve(unsigned int n, double b[], double x[], int itrnsp);
void f64_atimes(unsigned int n, double x[], double r[], int itrnsp);
void f64_avevar(double data[], unsigned int n, double *ave, double *var);
void f64_balanc(double **a, int n);
void f64_banbks(double **a, unsigned int n, int m1, int m2, double **al, unsigned int indx[], double b[]);
void f64_bandec(double **a, unsigned int n, int m1, int m2, double **al, unsigned int indx[], double *d);
void f64_banmul(double **a, unsigned int n, int m1, int m2, double x[], double b[]);
void f64_bcucof(double y[], double y1[], double y2[], double y12[], double d1, double d2, double **c);
void f64_bcuint(double y[], double y1[], double y2[], double y12[], double x1l, double x1u, double x2l, double x2u, double x1, double x2, double *ansy, double *ansy1, double *ansy2);
void f64_beschb(double x, double *gam1, double *gam2, double *gampl, double *gammi);
double f64_bessi(int n, double x);
double f64_bessi0(double x);
double f64_bessi1(double x);
void f64_bessik(double x, double xnu, double *ri, double *rk, double *rip, double *rkp);
double f64_bessj(int n, double x);
double f64_bessj0(double x);
double f64_bessj1(double x);
void f64_bessjy(double x, double xnu, double *rj, double *ry, double *rjp, double *ryp);
double f64_bessk(int n, double x);
double f64_bessk0(double x);
double f64_bessk1(double x);
double f64_bessy(int n, double x);
double f64_bessy0(double x);
double f64_bessy1(double x);
double f64_beta(double z, double w);
double f64_betacf(double a, double b, double x);
double f64_betai(double a, double b, double x);
double f64_bico(int n, int k);
void f64_bksub(int ne, int nb, int jf, int k1, int k2, double ***c);
double f64_bnldev(double pp, int n, int *idum);
double f64_brent(double ax, double bx, double cx, double (*f)(double), double tol, double *xmin);
void f64_broydn(double x[], int n, int *check, void (*vecfunc)(int, double [], double []));
void f64_bsstep(double y[], double dydx[], int nv, double *xx, double htry, double eps, double yscal[], double *hdid, double *hnext, void (*derivs)(double, double [], double []));
void f64_caldat(int julian, int *mm, int *id, int *iyyy);
void f64_chder(double a, double b, double c[], double cder[], int n);
double f64_chebev(double a, double b, double c[], int m, double x);
void f64_chebft(double a, double b, double c[], int n, double (*func)(double));
void f64_chebpc(double c[], double d[], int n);
void f64_chint(double a, double b, double c[], double cint[], int n);
double f64_chixy(double bang);
void f64_choldc(double **a, int n, double p[]);
void f64_cholsl(double **a, int n, double p[], double b[], double x[]);
void f64_chsone(double bins[], double ebins[], int nbins, int knstrn, double *df, double *chsq, double *prob);
void f64_chstwo(double bins1[], double bins2[], int nbins, int knstrn, double *df, double *chsq, double *prob);
void f64_cisi(double x, double *ci, double *si);
void f64_cntab1(int **nn, int ni, int nj, double *chisq, double *df, double *prob, double *cramrv, double *ccc);
void f64_cntab2(int **nn, int ni, int nj, double *h, double *hx, double *hy, double *hygx, double *hxgy, double *uygx, double *uxgy, double *uxy);
void f64_convlv(double data[], unsigned int n, double respns[], unsigned int m, int isign, double ans[]);
void f64_copy(double **aout, double **ain, int n);
void f64_correl(double data1[], double data2[], unsigned int n, double ans[]);
void f64_cosft(double y[], int n, int isign);
void f64_cosft1(double y[], int n);
void f64_cosft2(double y[], int n, int isign);
void f64_covsrt(double **covar, int ma, int ia[], int mfit);
void f64_crank(unsigned int n, double w[], double *s);
void f64_cyclic(double a[], double b[], double c[], double alpha, double beta, double r[], double x[], unsigned int n);
void f64_daub4(double a[], unsigned int n, int isign);
double f64_dawson(double x);
double f64_dbrent(double ax, double bx, double cx, double (*f)(double), double (*df)(double), double tol, double *xmin);
void f64_ddpoly(double c[], int nc, double x, double pd[], int nd);
int f64_decchk(char string[], int n, char *ch);
void f64_derivs(double x, double y[], double dydx[]);
double f64_df1dim(double x);
void f64_dfour1(double data[], unsigned int nn, int isign);
void f64_dfpmin(double p[], int n, double gtol, int *iter, double *fret, double (*func)(double []), void (*dfunc)(double [], double []));
double f64_dfridr(double (*func)(double), double x, double h, double *err);
void f64_dftcor(double w, double delta, double a, double b, double endpts[], double *corre, double *corim, double *corfac);
void f64_dftint(double (*func)(double), double a, double b, double w, double *cosint, double *sinint);
void f64_difeq(int k, int k1, int k2, int jsf, int is1, int isf, int indexv[], int ne, double **s, double **y);
void f64_dlinmin(double p[], double xi[], int n, double *fret, double (*func)(double []), void (*dfunc)(double [], double[]));
double f64_dpythag(double a, double b);
void f64_drealft(double data[], unsigned int n, int isign);
void f64_dsprsax(double sa[], unsigned int ija[], double x[], double b[], unsigned int n);
void f64_dsprstx(double sa[], unsigned int ija[], double x[], double b[], unsigned int n);
void f64_dsvbksb(double **u, double w[], double **v, int m, int n, double b[], double x[]);
void f64_dsvdcmp(double **a, int m, int n, double w[], double **v);
void f64_eclass(int nf[], int n, int lista[], int listb[], int m);
void f64_eclazz(int nf[], int n, int (*equiv)(int, int));
double f64_ei(double x);
void f64_eigsrt(double d[], double **v, int n);
double f64_elle(double phi, double ak);
double f64_ellf(double phi, double ak);
double f64_ellpi(double phi, double en, double ak);
void f64_elmhes(double **a, int n);
double f64_erfcc(double x);
double f64_erff(double x);
double f64_erffc(double x);
void f64_eulsum(double *sum, double term, int jterm, double wksp[]);
double f64_evlmem(double fdt, double d[], int m, double xms);
double f64_expdev(int *idum);
double f64_expint(int n, double x);
double f64_f1(double x);
double f64_f1dim(double x);
double f64_f2(double y);
double f64_f3(double z);
double f64_factln(int n);
double f64_factrl(int n);
void f64_fasper(double x[], double y[], unsigned int n, double ofac, double hifac, double wk1[], double wk2[], unsigned int nwk, unsigned int *nout, unsigned int *jmax, double *prob);
void f64_fdjac(int n, double x[], double fvec[], double **df, void (*vecfunc)(int, double [], double []));
void f64_fgauss(double x, double a[], double *y, double dyda[], int na);
void f64_fill0(double **u, int n);
void f64_fit(double x[], double y[], int ndata, double sig[], int mwt, double *a, double *b, double *siga, double *sigb, double *chi2, double *q);
void f64_fitexy(double x[], double y[], int ndat, double sigx[], double sigy[], double *a, double *b, double *siga, double *sigb, double *chi2, double *q);
void f64_fixrts(double d[], int m);
void f64_fleg(double x, double pl[], int nl);
void f64_flmoon(int n, int nph, int *jd, double *frac);
double f64_fmin(double x[]);
void f64_four1(double data[], unsigned int nn, int isign);
void f64_fourew(FILE *file[5], int *na, int *nb, int *nc, int *nd);
void f64_fourfs(FILE *file[5], unsigned int nn[], int ndim, int isign);
void f64_fourn(double data[], unsigned int nn[], int ndim, int isign);
void f64_fpoly(double x, double p[], int np);
void f64_fred2(int n, double a, double b, double t[], double f[], double w[], double (*g)(double), double (*ak)(double, double));
double fredin(double x, int n, double a, double b, double t[], double f[], double w[], double (*g)(double), double (*ak)(double, double));
void f64_frenel(double x, double *s, double *c);
void f64_frprmn(double p[], int n, double ftol, int *iter, double *fret, double (*func)(double []), void (*dfunc)(double [], double []));
void f64_ftest(double data1[], unsigned int n1, double data2[], unsigned int n2, double *f, double *prob);
double f64_gamdev(int ia, int *idum);
double f64_gammln(double xx);
double f64_gammp(double a, double x);
double f64_gammq(double a, double x);
double f64_gasdev(int *idum);
void f64_gaucof(int n, double a[], double b[], double amu0, double x[], double w[]);
void f64_gauher(double x[], double w[], int n);
void f64_gaujac(double x[], double w[], int n, double alf, double bet);
void f64_gaulag(double x[], double w[], int n, double alf);
void f64_gauleg(double x1, double x2, double x[], double w[], int n);
void f64_gaussj(double **a, int n, double **b, int m);
void f64_gcf(double *gammcf, double a, double x, double *gln);
double f64_golden(double ax, double bx, double cx, double (*f)(double), double tol, double *xmin);
void f64_gser(double *gamser, double a, double x, double *gln);
void f64_hpsel(unsigned int m, unsigned int n, double arr[], double heap[]);
void f64_hpsort(unsigned int n, double ra[]);
void f64_hqr(double **a, int n, double wr[], double wi[]);
void f64_hufapp(unsigned int index[], unsigned int nprob[], unsigned int n, unsigned int i);
void f64_hufdec(unsigned int *ich, unsigned char *code, unsigned int lcode, unsigned int *nb, huffcode *hcode);
void f64_hufenc(unsigned int ich, unsigned char **codep, unsigned int *lcode, unsigned int *nb, huffcode *hcode);
void f64_hufmak(unsigned int nfreq[], unsigned int nchin, unsigned int *ilong, unsigned int *nlong, huffcode *hcode);
void f64_hunt(double xx[], unsigned int n, double x, unsigned int *jlo);
void f64_hypdrv(double s, double yy[], double dyyds[]);
fcomplex f64_hypgeo(fcomplex a, fcomplex b, fcomplex c, fcomplex z);
void f64_hypser(fcomplex a, fcomplex b, fcomplex c, fcomplex z, fcomplex *series, fcomplex *deriv);
unsigned short f64_icrc(unsigned short crc, unsigned char *bufptr, unsigned int len, short jinit, int jrev);
unsigned short f64_icrc1(unsigned short crc, unsigned char onech);
unsigned int f64_igray(unsigned int n, int is);
void f64_iindexx(unsigned int n, int arr[], unsigned int indx[]);
void f64_indexx(unsigned int n, double arr[], unsigned int indx[]);
void f64_interp(double **uf, double **uc, int nf);
int f64_irbit1(unsigned int *iseed);
int f64_irbit2(unsigned int *iseed);
void f64_jacobi(double **a, int n, double d[], double **v, int *nrot);
void f64_jacobn(double x, double y[], double dfdx[], double **dfdy, int n);
int f64_julday(int mm, int id, int iyyy);
void f64_kendl1(double data1[], double data2[], unsigned int n, double *tau, double *z, double *prob);
void f64_kendl2(double **tab, int i, int j, double *tau, double *z, double *prob);
void f64_kermom(double w[], double y, int m);
void f64_ks2d1s(double x1[], double y1[], unsigned int n1, void (*quadvl)(double, double, double *, double *, double *, double *), double *d1, double *prob);
void f64_ks2d2s(double x1[], double y1[], unsigned int n1, double x2[], double y2[], unsigned int n2, double *d, double *prob);
void f64_ksone(double data[], unsigned int n, double (*func)(double), double *d, double *prob);
void f64_kstwo(double data1[], unsigned int n1, double data2[], unsigned int n2, double *d, double *prob);
void f64_laguer(fcomplex a[], int m, fcomplex *x, int *its);
void f64_lfit(double x[], double y[], double sig[], int ndat, double a[], int ia[], int ma, double **covar, double *chisq, void (*funcs)(double, double [], int));
void f64_linbcg(unsigned int n, double b[], double x[], int itol, double tol,  int itmax, int *iter, double *err);
void f64_linmin(double p[], double xi[], int n, double *fret, double (*func)(double []));
void f64_lnsrch(int n, double xold[], double fold, double g[], double p[], double x[],  double *f, double stpmax, int *check, double (*func)(double []));
void f64_load(double x1, double v[], double y[]);
void f64_load1(double x1, double v1[], double y[]);
void f64_load2(double x2, double v2[], double y[]);
void f64_locate(double xx[], unsigned int n, double x, unsigned int *j);
void f64_lop(double **out, double **u, int n);
void f64_lubksb(double **a, int n, int *indx, double b[]);
void f64_ludcmp(double **a, int n, int *indx, double *d);
void f64_machar(int *ibeta, int *it, int *irnd, int *ngrd, int *machep, int *negep, int *iexp, int *minexp, int *maxexp, double *eps, double *epsneg, double *xmin, double *xmax);
void f64_matadd(double **a, double **b, double **c, int n);
void f64_matsub(double **a, double **b, double **c, int n);
void f64_medfit(double x[], double y[], int ndata, double *a, double *b, double *abdev);
void f64_memcof(double data[], int n, int m, double *xms, double d[]);
int f64_metrop(double de, double t);
void f64_mgfas(double **u, int n, int maxcyc);
void f64_mglin(double **u, int n, int ncycle);
double f64_midexp(double (*funk)(double), double aa, double bb, int n);
double f64_midinf(double (*funk)(double), double aa, double bb, int n);
double f64_midpnt(double (*func)(double), double a, double b, int n);
double f64_midsql(double (*funk)(double), double aa, double bb, int n);
double f64_midsqu(double (*funk)(double), double aa, double bb, int n);
void f64_miser(double (*func)(double []), double regn[], int ndim, unsigned int npts, double dith, double *ave, double *var);
void f64_mmid(double y[], double dydx[], int nvar, double xs, double htot, int nstep, double yout[], void (*derivs)(double, double[], double[]));
void f64_mnbrak(double *ax, double *bx, double *cx, double *fa, double *fb, double *fc, double (*func)(double));
void f64_mnewt(int ntrial, double x[], int n, double tolx, double tolf);
void f64_moment(double data[], int n, double *ave, double *adev, double *sdev, double *var, double *skew, double *curt);
void f64_mp2dfr(unsigned char a[], unsigned char s[], int n, int *m);
void f64_mpadd(unsigned char w[], unsigned char u[], unsigned char v[], int n);
void f64_mpdiv(unsigned char q[], unsigned char r[], unsigned char u[], unsigned char v[], int n, int m);
void f64_mpinv(unsigned char u[], unsigned char v[], int n, int m);
void f64_mplsh(unsigned char u[], int n);
void f64_mpmov(unsigned char u[], unsigned char v[], int n);
void f64_mpmul(unsigned char w[], unsigned char u[], unsigned char v[], int n, int m);
void f64_mpneg(unsigned char u[], int n);
void f64_mppi(int n);
void f64_mprove(double **a, double **alud, int n, int indx[], double b[], double x[]);
void f64_mpsad(unsigned char w[], unsigned char u[], int n, int iv);
void f64_mpsdv(unsigned char w[], unsigned char u[], int n, int iv, int *ir);
void f64_mpsmu(unsigned char w[], unsigned char u[], int n, int iv);
void f64_mpsqrt(unsigned char w[], unsigned char u[], unsigned char v[], int n, int m);
void f64_mpsub(int *is, unsigned char w[], unsigned char u[], unsigned char v[], int n);
void f64_mrqcof(double x[], double y[], double sig[], int ndata, double a[], int ia[], int ma, double **alpha, double beta[], double *chisq, void (*funcs)(double, double [], double *, double [], int));
void f64_mrqmin(double x[], double y[], double sig[], int ndata, double a[], int ia[], int ma, double **covar, double **alpha, double *chisq, void (*funcs)(double, double [], double *, double [], int), double *alamda);
void f64_newt(double x[], int n, int *check, void (*vecfunc)(int, double [], double []));
void f64_odeint(double ystart[], int nvar, double x1, double x2, double eps, double h1, double hmin, int *nok, int *nbad, void (*derivs)(double, double [], double []), void (*rkqs)(double [], double [], int, double *, double, double, double [], double *, double *, void (*)(double, double [], double [])));
void f64_orthog(int n, double anu[], double alpha[], double beta[], double a[], double b[]);
void f64_pade(double cof[], int n, double *resid);
void f64_pccheb(double d[], double c[], int n);
void f64_pcshft(double a, double b, double d[], int n);
void f64_pearsn(double x[], double y[], unsigned int n, double *r, double *prob, double *z);
void f64_period(double x[], double y[], int n, double ofac, double hifac, double px[], double py[], int np, int *nout, int *jmax, double *prob);
void f64_piksr2(int n, double arr[], double brr[]);
void f64_piksrt(int n, double arr[]);
void f64_pinvs(int ie1, int ie2, int je1, int jsf, int jc1, int k, double ***c, double **s);
double f64_plgndr(int l, int m, double x);
double f64_poidev(double xm, int *idum);
void f64_polcoe(double x[], double y[], int n, double cof[]);
void f64_polcof(double xa[], double ya[], int n, double cof[]);
void f64_poldiv(double u[], int n, double v[], int nv, double q[], double r[]);
void f64_polin2(double x1a[], double x2a[], double **ya, int m, int n, double x1, double x2, double *y, double *dy);
void f64_polint(double xa[], double ya[], int n, double x, double *y, double *dy);
void f64_powell(double p[], double **xi, int n, double ftol, int *iter, double *fret, double (*func)(double []));
void f64_predic(double data[], int ndata, double d[], int m, double future[], int nfut);
double f64_probks(double alam);
void f64_psdes(unsigned int *lword, unsigned int *irword);
void f64_pwt(double a[], unsigned int n, int isign);
void f64_pwtset(int n);
double f64_pythag(double a, double b);
void f64_pzextr(int iest, double xest, double yest[], double yz[], double dy[], int nv);
double f64_qgaus(double (*func)(double), double a, double b);
void f64_qrdcmp(double **a, int n, double *c, double *d, int *sing);
double f64_qromb(double (*func)(double), double a, double b);
double f64_qromo(double (*func)(double), double a, double b, double (*choose)(double (*)(double), double, double, int));
void f64_qroot(double p[], int n, double *b, double *c, double eps);
void f64_qrsolv(double **a, int n, double c[], double d[], double b[]);
void f64_qrupdt(double **r, double **qt, int n, double u[], double v[]);
double f64_qsimp(double (*func)(double), double a, double b);
double f64_qtrap(double (*func)(double), double a, double b);
double f64_quad3d(double (*func)(double, double, double), double x1, double x2);
void f64_quadct(double x, double y, double xx[], double yy[], unsigned int nn, double *fa, double *fb, double *fc, double *fd);
void f64_quadmx(double **a, int n);
void f64_quadvl(double x, double y, double *fa, double *fb, double *fc, double *fd);
double f64_ran0(int *idum);
double f64_ran1(int *idum);
double f64_ran2(int *idum);
double f64_ran3(int *idum);
double f64_ran4(int *idum);
void f64_rank(unsigned int n, unsigned int indx[], unsigned int irank[]);
void f64_ranpt(double pt[], double regn[], int n);
void f64_ratint(double xa[], double ya[], int n, double x, double *y, double *dy);
void f64_ratlsq(double (*fn)(double), double a, double b, int mm, int kk, double cof[], double *dev);
double f64_ratval(double x, double cof[], int mm, int kk);
double f64_rc(double x, double y);
double f64_rd(double x, double y, double z);
void f64_realft(double data[], unsigned int n, int isign);
void f64_rebin(double rc, int nd, double r[], double xin[], double xi[]);
void f64_red(int iz1, int iz2, int jz1, int jz2, int jm1, int jm2, int jmf, int ic1, int jc1, int jcf, int kc, double ***c, double **s);
void f64_relax(double **u, double **rhs, int n);
void f64_relax2(double **u, double **rhs, int n);
void f64_resid(double **res, double **u, double **rhs, int n);
double f64_revcst(double x[], double y[], int iorder[], int ncity, int n[]);
void f64_reverse(int iorder[], int ncity, int n[]);
double f64_rf(double x, double y, double z);
double f64_rj(double x, double y, double z, double p);
void f64_rk4(double y[], double dydx[], int n, double x, double h, double yout[], void (*derivs)(double, double [], double []));
void f64_rkck(double y[], double dydx[], int n, double x, double h, double yout[], double yerr[], void (*derivs)(double, double [], double []));
void f64_rkdumb(double vstart[], int nvar, double x1, double x2, int nstep, void (*derivs)(double, double [], double []));
void f64_rkqs(double y[], double dydx[], int n, double *x, double htry, double eps, double yscal[], double *hdid, double *hnext, void (*derivs)(double, double [], double []));
void f64_rlft3(double ***data, double **speq, unsigned int nn1, unsigned int nn2, unsigned int nn3, int isign);
double f64_rofunc(double b);
void f64_rotate(double **r, double **qt, int n, int i, double a, double b);
void f64_rsolv(double **a, int n, double d[], double b[]);
void f64_rstrct(double **uc, double **uf, int nc);
double f64_rtbis(double (*func)(double), double x1, double x2, double xacc);
double f64_rtflsp(double (*func)(double), double x1, double x2, double xacc);
double f64_rtnewt(void (*funcd)(double, double *, double *), double x1, double x2, double xacc);
double f64_rtsafe(void (*funcd)(double, double *, double *), double x1, double x2, double xacc);
double f64_rtsec(double (*func)(double), double x1, double x2, double xacc);
void f64_rzextr(int iest, double xest, double yest[], double yz[], double dy[], int nv);
void f64_savgol(double c[], int np, int nl, int nr, int ld, int m);
void f64_score(double xf, double y[], double f[]);
void f64_scrsho(double (*fx)(double));
double f64_select(unsigned int k, unsigned int n, double arr[]);
double f64_selip(unsigned int k, unsigned int n, double arr[]);
void f64_shell(unsigned int n, double a[]);
void f64_shoot(int n, double v[], double f[]);
void f64_shootf(int n, double v[], double f[]);
void f64_simp1(double **a, int mm, int ll[], int nll, int iabf, int *kp, double *bmax);
void f64_simp2(double **a, int n, int l2[], int nl2, int *ip, int kp, double *q1);
void f64_simp3(double **a, int i1, int k1, int ip, int kp);
void f64_simplx(double **a, int m, int n, int m1, int m2, int m3, int *icase, int izrov[], int iposv[]);
void f64_simpr(double y[], double dydx[], double dfdx[], double **dfdy, int n, double xs, double htot, int nstep, double yout[], void (*derivs)(double, double [], double []));
void f64_sinft(double y[], int n);
void f64_slvsm2(double **u, double **rhs);
void f64_slvsml(double **u, double **rhs);
void f64_sncndn(double uu, double emmc, double *sn, double *cn, double *dn);
double f64_snrm(unsigned int n, double sx[], int itol);
void f64_sobseq(int *n, double x[]);
void f64_solvde(int itmax, double conv, double slowc, double scalv[], int indexv[], int ne, int nb, int m, double **y, double ***c, double **s);
void f64_sor(double **a, double **b, double **c, double **d, double **e, double **f, double **u, int jmax, double rjac);
void f64_sort(unsigned int n, double arr[]);
void f64_sort2(unsigned int n, double arr[], double brr[]);
void f64_sort3(unsigned int n, double ra[], double rb[], double rc[]);
void f64_spctrm(FILE *fp, double p[], int m, int k, int ovrlap);
void f64_spear(double data1[], double data2[], unsigned int n, double *d, double *zd, double *probd, double *rs, double *probrs);
void f64_sphbes(int n, double x, double *sj, double *sy, double *sjp, double *syp);
void f64_splie2(double x1a[], double x2a[], double **ya, int m, int n, double **y2a);
void f64_splin2(double x1a[], double x2a[], double **ya, double **y2a, int m, int n, double x1, double x2, double *y);
void f64_spline(double x[], double y[], int n, double yp1, double ypn, double y2[]);
void f64_splint(double xa[], double ya[], double y2a[], int n, double x, double *y);
void f64_spread(double y, double yy[], unsigned int n, double x, int m);
void f64_sprsax(double sa[], unsigned int ija[], double x[], double b[], unsigned int n);
void f64_sprsin(double **a, int n, double thresh, unsigned int nmax, double sa[], unsigned int ija[]);
void f64_sprspm(double sa[], unsigned int ija[], double sb[], unsigned int ijb[], double sc[], unsigned int ijc[]);
void f64_sprstm(double sa[], unsigned int ija[], double sb[], unsigned int ijb[], double thresh, unsigned int nmax, double sc[], unsigned int ijc[]);
void f64_sprstp(double sa[], unsigned int ija[], double sb[], unsigned int ijb[]);
void f64_sprstx(double sa[], unsigned int ija[], double x[], double b[], unsigned int n);
void f64_stifbs(double y[], double dydx[], int nv, double *xx, double htry, double eps, double yscal[], double *hdid, double *hnext, void (*derivs)(double, double [], double []));
void f64_stiff(double y[], double dydx[], int n, double *x, double htry, double eps, double yscal[], double *hdid, double *hnext, void (*derivs)(double, double [], double []));
void f64_stoerm(double y[], double d2y[], int nv, double xs, double htot, int nstep, double yout[], void (*derivs)(double, double [], double []));
void f64_svbksb(double **u, double w[], double **v, int m, int n, double b[], double x[]);
void f64_svdcmp(double **a, int m, int n, double w[], double **v);
void f64_svdfit(double x[], double y[], double sig[], int ndata, double a[], int ma, double **u, double **v, double w[], double *chisq, void (*funcs)(double, double [], int));
void f64_svdvar(double **v, int ma, double w[], double **cvm);
void f64_toeplz(double r[], double x[], double y[], int n);
void f64_tptest(double data1[], double data2[], unsigned int n, double *t, double *prob);
void f64_tqli(double d[], double e[], int n, double **z);
double f64_trapzd(double (*func)(double), double a, double b, int n);
void f64_tred2(double **a, int n, double d[], double e[]);
void f64_tridag(double a[], double b[], double c[], double r[], double u[], unsigned int n);
double f64_trncst(double x[], double y[], int iorder[], int ncity, int n[]);
void f64_trnspt(int iorder[], int ncity, int n[]);
void f64_ttest(double data1[], unsigned int n1, double data2[], unsigned int n2, double *t, double *prob);
void f64_tutest(double data1[], unsigned int n1, double data2[], unsigned int n2, double *t, double *prob);
void f64_twofft(double data1[], double data2[], double fft1[], double fft2[], unsigned int n);
void f64_vander(double x[], double w[], double q[], int n);
void f64_vegas(double regn[], int ndim, double (*fxn)(double [], double), int init, unsigned int ncall, int itmx, int nprn, double *tgral, double *sd, double *chi2a);
void f64_voltra(int n, int m, double t0, double h, double *t, double **f, double (*g)(int, double), double (*ak)(int, int, double, double));
void f64_wt1(double a[], unsigned int n, int isign, void (*wtstep)(double [], unsigned int, int));
void f64_wtn(double a[], unsigned int nn[], int ndim, int isign, void (*wtstep)(double [], unsigned int, int));
void f64_wwghts(double wghts[], int n, double h, void (*kermom)(double [], double ,int));
int f64_zbrac(double (*func)(double), double *x1, double *x2);
void f64_zbrak(double (*fx)(double), double x1, double x2, int n, double xb1[], double xb2[], int *nb);
double f64_zbrent(double (*func)(double), double x1, double x2, double tol);
void f64_zrhqr(double a[], int m, double rtr[], double rti[]);
double f64_zriddr(double (*func)(double), double x1, double x2, double xacc);
void f64_zroots(fcomplex a[], int m, fcomplex roots[], int polish);
/*
void addint(double **uf, double **uc, double **res, int nf);
void airy(double x, double *ai, double *bi, double *aip, double *bip);
void amebsa(double **p, double y[], int ndim, double pb[],	double *yb,
	double ftol, double (*funk)(double []), int *iter, double temptr);
void amoeba(double **p, double y[], int ndim, double ftol,
	double (*funk)(double []), int *iter);
double amotry(double **p, double y[], double psum[], int ndim,
	double (*funk)(double []), int ihi, double fac);
double amotsa(double **p, double y[], double psum[], int ndim, double pb[],
	double *yb, double (*funk)(double []), int ihi, double *yhi, double fac);
void anneal(double x[], double y[], int iorder[], int ncity);
double anorm2(double **a, int n);
void arcmak(unsigned int nfreq[], unsigned int nchh, unsigned int nradd,
	arithcode *acode);
void arcode(unsigned int *ich, unsigned char **codep, unsigned int *lcode,
	unsigned int *lcd, int isign, arithcode *acode);
void arcsum(unsigned int iin[], unsigned int iout[], unsigned int ja,
	int nwk, unsigned int nrad, unsigned int nc);
void asolve(unsigned int n, double b[], double x[], int itrnsp);
void atimes(unsigned int n, double x[], double r[], int itrnsp);
void avevar(double data[], unsigned int n, double *ave, double *var);
void balanc(double **a, int n);
void banbks(double **a, unsigned int n, int m1, int m2, double **al,
	unsigned int indx[], double b[]);
void bandec(double **a, unsigned int n, int m1, int m2, double **al,
	unsigned int indx[], double *d);
void banmul(double **a, unsigned int n, int m1, int m2, double x[], double b[]);
void bcucof(double y[], double y1[], double y2[], double y12[], double d1,
	double d2, double **c);
void bcuint(double y[], double y1[], double y2[], double y12[],
	double x1l, double x1u, double x2l, double x2u, double x1,
	double x2, double *ansy, double *ansy1, double *ansy2);
void beschb(double x, double *gam1, double *gam2, double *gampl,
	double *gammi);
double bessi(int n, double x);
double bessi0(double x);
double bessi1(double x);
void bessik(double x, double xnu, double *ri, double *rk, double *rip,
	double *rkp);
double bessj(int n, double x);
double bessj0(double x);
double bessj1(double x);
void bessjy(double x, double xnu, double *rj, double *ry, double *rjp,
	double *ryp);
double bessk(int n, double x);
double bessk0(double x);
double bessk1(double x);
double bessy(int n, double x);
double bessy0(double x);
double bessy1(double x);
double beta(double z, double w);
double betacf(double a, double b, double x);
double betai(double a, double b, double x);
double bico(int n, int k);
void bksub(int ne, int nb, int jf, int k1, int k2, double ***c);
double bnldev(double pp, int n, int *idum);
double brent(double ax, double bx, double cx,
	double (*f)(double), double tol, double *xmin);
void broydn(double x[], int n, int *check,
	void (*vecfunc)(int, double [], double []));
void bsstep(double y[], double dydx[], int nv, double *xx, double htry,
	double eps, double yscal[], double *hdid, double *hnext,
	void (*derivs)(double, double [], double []));
void caldat(int julian, int *mm, int *id, int *iyyy);
void chder(double a, double b, double c[], double cder[], int n);
double chebev(double a, double b, double c[], int m, double x);
void chebft(double a, double b, double c[], int n, double (*func)(double));
void chebpc(double c[], double d[], int n);
void chint(double a, double b, double c[], double cint[], int n);
double chixy(double bang);
void choldc(double **a, int n, double p[]);
void cholsl(double **a, int n, double p[], double b[], double x[]);
void chsone(double bins[], double ebins[], int nbins, int knstrn,
	double *df, double *chsq, double *prob);
void chstwo(double bins1[], double bins2[], int nbins, int knstrn,
	double *df, double *chsq, double *prob);
void cisi(double x, double *ci, double *si);
void cntab1(int **nn, int ni, int nj, double *chisq,
	double *df, double *prob, double *cramrv, double *ccc);
void cntab2(int **nn, int ni, int nj, double *h, double *hx, double *hy,
	double *hygx, double *hxgy, double *uygx, double *uxgy, double *uxy);
void convlv(double data[], unsigned int n, double respns[], unsigned int m,
	int isign, double ans[]);
void copy(double **aout, double **ain, int n);
void correl(double data1[], double data2[], unsigned int n, double ans[]);
void cosft(double y[], int n, int isign);
void cosft1(double y[], int n);
void cosft2(double y[], int n, int isign);
void covsrt(double **covar, int ma, int ia[], int mfit);
void crank(unsigned int n, double w[], double *s);
void cyclic(double a[], double b[], double c[], double alpha, double beta,
	double r[], double x[], unsigned int n);
void daub4(double a[], unsigned int n, int isign);
double dawson(double x);
double dbrent(double ax, double bx, double cx,
	double (*f)(double), double (*df)(double), double tol, double *xmin);
void ddpoly(double c[], int nc, double x, double pd[], int nd);
int decchk(char string[], int n, char *ch);
void derivs(double x, double y[], double dydx[]);
double df1dim(double x);
void dfour1(double data[], unsigned int nn, int isign);
void dfpmin(double p[], int n, double gtol, int *iter, double *fret,
	double (*func)(double []), void (*dfunc)(double [], double []));
double dfridr(double (*func)(double), double x, double h, double *err);
void dftcor(double w, double delta, double a, double b, double endpts[],
	double *corre, double *corim, double *corfac);
void dftint(double (*func)(double), double a, double b, double w,
	double *cosint, double *sinint);
void difeq(int k, int k1, int k2, int jsf, int is1, int isf,
	int indexv[], int ne, double **s, double **y);
void dlinmin(double p[], double xi[], int n, double *fret,
	double (*func)(double []), void (*dfunc)(double [], double[]));
double dpythag(double a, double b);
void drealft(double data[], unsigned int n, int isign);
void dsprsax(double sa[], unsigned int ija[], double x[], double b[],
	unsigned int n);
void dsprstx(double sa[], unsigned int ija[], double x[], double b[],
	unsigned int n);
void dsvbksb(double **u, double w[], double **v, int m, int n, double b[],
	double x[]);
void dsvdcmp(double **a, int m, int n, double w[], double **v);
void eclass(int nf[], int n, int lista[], int listb[], int m);
void eclazz(int nf[], int n, int (*equiv)(int, int));
double ei(double x);
void eigsrt(double d[], double **v, int n);
double elle(double phi, double ak);
double ellf(double phi, double ak);
double ellpi(double phi, double en, double ak);
void elmhes(double **a, int n);
double erfcc(double x);
double erff(double x);
double erffc(double x);
void eulsum(double *sum, double term, int jterm, double wksp[]);
double evlmem(double fdt, double d[], int m, double xms);
double expdev(int *idum);
double expint(int n, double x);
double f1(double x);
double f1dim(double x);
double f2(double y);
double f3(double z);
double factln(int n);
double factrl(int n);
void fasper(double x[], double y[], unsigned int n, double ofac, double hifac,
	double wk1[], double wk2[], unsigned int nwk, unsigned int *nout,
	unsigned int *jmax, double *prob);
void fdjac(int n, double x[], double fvec[], double **df,
	void (*vecfunc)(int, double [], double []));
void fgauss(double x, double a[], double *y, double dyda[], int na);
void fill0(double **u, int n);
void fit(double x[], double y[], int ndata, double sig[], int mwt,
	double *a, double *b, double *siga, double *sigb, double *chi2, double *q);
void fitexy(double x[], double y[], int ndat, double sigx[], double sigy[],
	double *a, double *b, double *siga, double *sigb, double *chi2, double *q);
void fixrts(double d[], int m);
void fleg(double x, double pl[], int nl);
void flmoon(int n, int nph, int *jd, double *frac);
double fmin(double x[]);
void four1(double data[], unsigned int nn, int isign);
void fourew(FILE *file[5], int *na, int *nb, int *nc, int *nd);
void fourfs(FILE *file[5], unsigned int nn[], int ndim, int isign);
void fourn(double data[], unsigned int nn[], int ndim, int isign);
void fpoly(double x, double p[], int np);
void fred2(int n, double a, double b, double t[], double f[], double w[],
	double (*g)(double), double (*ak)(double, double));
double fredin(double x, int n, double a, double b, double t[], double f[], double w[],
	double (*g)(double), double (*ak)(double, double));
void frenel(double x, double *s, double *c);
void frprmn(double p[], int n, double ftol, int *iter, double *fret,
	double (*func)(double []), void (*dfunc)(double [], double []));
void ftest(double data1[], unsigned int n1, double data2[], unsigned int n2,
	double *f, double *prob);
double gamdev(int ia, int *idum);
double gammln(double xx);
double gammp(double a, double x);
double gammq(double a, double x);
double gasdev(int *idum);
void gaucof(int n, double a[], double b[], double amu0, double x[], double w[]);
void gauher(double x[], double w[], int n);
void gaujac(double x[], double w[], int n, double alf, double bet);
void gaulag(double x[], double w[], int n, double alf);
void gauleg(double x1, double x2, double x[], double w[], int n);
void gaussj(double **a, int n, double **b, int m);
void gcf(double *gammcf, double a, double x, double *gln);
double golden(double ax, double bx, double cx, double (*f)(double), double tol,
	double *xmin);
void gser(double *gamser, double a, double x, double *gln);
void hpsel(unsigned int m, unsigned int n, double arr[], double heap[]);
void hpsort(unsigned int n, double ra[]);
void hqr(double **a, int n, double wr[], double wi[]);
void hufapp(unsigned int index[], unsigned int nprob[], unsigned int n,
	unsigned int i);
void hufdec(unsigned int *ich, unsigned char *code, unsigned int lcode,
	unsigned int *nb, huffcode *hcode);
void hufenc(unsigned int ich, unsigned char **codep, unsigned int *lcode,
	unsigned int *nb, huffcode *hcode);
void hufmak(unsigned int nfreq[], unsigned int nchin, unsigned int *ilong,
	unsigned int *nlong, huffcode *hcode);
void hunt(double xx[], unsigned int n, double x, unsigned int *jlo);
void hypdrv(double s, double yy[], double dyyds[]);
fcomplex hypgeo(fcomplex a, fcomplex b, fcomplex c, fcomplex z);
void hypser(fcomplex a, fcomplex b, fcomplex c, fcomplex z,
	fcomplex *series, fcomplex *deriv);
unsigned short icrc(unsigned short crc, unsigned char *bufptr,
	unsigned int len, short jinit, int jrev);
unsigned short icrc1(unsigned short crc, unsigned char onech);
unsigned int igray(unsigned int n, int is);
void iindexx(unsigned int n, int arr[], unsigned int indx[]);
void indexx(unsigned int n, double arr[], unsigned int indx[]);
void interp(double **uf, double **uc, int nf);
int irbit1(unsigned int *iseed);
int irbit2(unsigned int *iseed);
void jacobi(double **a, int n, double d[], double **v, int *nrot);
void jacobn(double x, double y[], double dfdx[], double **dfdy, int n);
int julday(int mm, int id, int iyyy);
void kendl1(double data1[], double data2[], unsigned int n, double *tau, double *z,
	double *prob);
void kendl2(double **tab, int i, int j, double *tau, double *z, double *prob);
void kermom(double w[], double y, int m);
void ks2d1s(double x1[], double y1[], unsigned int n1,
	void (*quadvl)(double, double, double *, double *, double *, double *),
	double *d1, double *prob);
void ks2d2s(double x1[], double y1[], unsigned int n1, double x2[], double y2[],
	unsigned int n2, double *d, double *prob);
void ksone(double data[], unsigned int n, double (*func)(double), double *d,
	double *prob);
void kstwo(double data1[], unsigned int n1, double data2[], unsigned int n2,
	double *d, double *prob);
void laguer(fcomplex a[], int m, fcomplex *x, int *its);
void lfit(double x[], double y[], double sig[], int ndat, double a[], int ia[],
	int ma, double **covar, double *chisq, void (*funcs)(double, double [], int));
void linbcg(unsigned int n, double b[], double x[], int itol, double tol,
	 int itmax, int *iter, double *err);
void linmin(double p[], double xi[], int n, double *fret,
	double (*func)(double []));
void lnsrch(int n, double xold[], double fold, double g[], double p[], double x[],
	 double *f, double stpmax, int *check, double (*func)(double []));
void load(double x1, double v[], double y[]);
void load1(double x1, double v1[], double y[]);
void load2(double x2, double v2[], double y[]);
void locate(double xx[], unsigned int n, double x, unsigned int *j);
void lop(double **out, double **u, int n);
void lubksb(double **a, int n, int *indx, double b[]);
void ludcmp(double **a, int n, int *indx, double *d);
void machar(int *ibeta, int *it, int *irnd, int *ngrd,
	int *machep, int *negep, int *iexp, int *minexp, int *maxexp,
	double *eps, double *epsneg, double *xmin, double *xmax);
void matadd(double **a, double **b, double **c, int n);
void matsub(double **a, double **b, double **c, int n);
void medfit(double x[], double y[], int ndata, double *a, double *b, double *abdev);
void memcof(double data[], int n, int m, double *xms, double d[]);
int metrop(double de, double t);
void mgfas(double **u, int n, int maxcyc);
void mglin(double **u, int n, int ncycle);
double midexp(double (*funk)(double), double aa, double bb, int n);
double midinf(double (*funk)(double), double aa, double bb, int n);
double midpnt(double (*func)(double), double a, double b, int n);
double midsql(double (*funk)(double), double aa, double bb, int n);
double midsqu(double (*funk)(double), double aa, double bb, int n);
void miser(double (*func)(double []), double regn[], int ndim, unsigned int npts,
	double dith, double *ave, double *var);
void mmid(double y[], double dydx[], int nvar, double xs, double htot,
	int nstep, double yout[], void (*derivs)(double, double[], double[]));
void mnbrak(double *ax, double *bx, double *cx, double *fa, double *fb,
	double *fc, double (*func)(double));
void mnewt(int ntrial, double x[], int n, double tolx, double tolf);
void moment(double data[], int n, double *ave, double *adev, double *sdev,
	double *var, double *skew, double *curt);
void mp2dfr(unsigned char a[], unsigned char s[], int n, int *m);
void mpadd(unsigned char w[], unsigned char u[], unsigned char v[], int n);
void mpdiv(unsigned char q[], unsigned char r[], unsigned char u[],
	unsigned char v[], int n, int m);
void mpinv(unsigned char u[], unsigned char v[], int n, int m);
void mplsh(unsigned char u[], int n);
void mpmov(unsigned char u[], unsigned char v[], int n);
void mpmul(unsigned char w[], unsigned char u[], unsigned char v[], int n,
	int m);
void mpneg(unsigned char u[], int n);
void mppi(int n);
void mprove(double **a, double **alud, int n, int indx[], double b[],
	double x[]);
void mpsad(unsigned char w[], unsigned char u[], int n, int iv);
void mpsdv(unsigned char w[], unsigned char u[], int n, int iv, int *ir);
void mpsmu(unsigned char w[], unsigned char u[], int n, int iv);
void mpsqrt(unsigned char w[], unsigned char u[], unsigned char v[], int n,
	int m);
void mpsub(int *is, unsigned char w[], unsigned char u[], unsigned char v[],
	int n);
void mrqcof(double x[], double y[], double sig[], int ndata, double a[],
	int ia[], int ma, double **alpha, double beta[], double *chisq,
	void (*funcs)(double, double [], double *, double [], int));
void mrqmin(double x[], double y[], double sig[], int ndata, double a[],
	int ia[], int ma, double **covar, double **alpha, double *chisq,
	void (*funcs)(double, double [], double *, double [], int), double *alamda);
void newt(double x[], int n, int *check,
	void (*vecfunc)(int, double [], double []));
void odeint(double ystart[], int nvar, double x1, double x2,
	double eps, double h1, double hmin, int *nok, int *nbad,
	void (*derivs)(double, double [], double []),
	void (*rkqs)(double [], double [], int, double *, double, double,
	double [], double *, double *, void (*)(double, double [], double [])));
void orthog(int n, double anu[], double alpha[], double beta[], double a[],
	double b[]);
void pade(double cof[], int n, double *resid);
void pccheb(double d[], double c[], int n);
void pcshft(double a, double b, double d[], int n);
void pearsn(double x[], double y[], unsigned int n, double *r, double *prob,
	double *z);
void period(double x[], double y[], int n, double ofac, double hifac,
	double px[], double py[], int np, int *nout, int *jmax, double *prob);
void piksr2(int n, double arr[], double brr[]);
void piksrt(int n, double arr[]);
void pinvs(int ie1, int ie2, int je1, int jsf, int jc1, int k,
	double ***c, double **s);
double plgndr(int l, int m, double x);
double poidev(double xm, int *idum);
void polcoe(double x[], double y[], int n, double cof[]);
void polcof(double xa[], double ya[], int n, double cof[]);
void poldiv(double u[], int n, double v[], int nv, double q[], double r[]);
void polin2(double x1a[], double x2a[], double **ya, int m, int n,
	double x1, double x2, double *y, double *dy);
void polint(double xa[], double ya[], int n, double x, double *y, double *dy);
void powell(double p[], double **xi, int n, double ftol, int *iter, double *fret,
	double (*func)(double []));
void predic(double data[], int ndata, double d[], int m, double future[], int nfut);
double probks(double alam);
void psdes(unsigned int *lword, unsigned int *irword);
void pwt(double a[], unsigned int n, int isign);
void pwtset(int n);
double pythag(double a, double b);
void pzextr(int iest, double xest, double yest[], double yz[], double dy[],
	int nv);
double qgaus(double (*func)(double), double a, double b);
void qrdcmp(double **a, int n, double *c, double *d, int *sing);
double qromb(double (*func)(double), double a, double b);
double qromo(double (*func)(double), double a, double b,
	double (*choose)(double (*)(double), double, double, int));
void qroot(double p[], int n, double *b, double *c, double eps);
void qrsolv(double **a, int n, double c[], double d[], double b[]);
void qrupdt(double **r, double **qt, int n, double u[], double v[]);
double qsimp(double (*func)(double), double a, double b);
double qtrap(double (*func)(double), double a, double b);
double quad3d(double (*func)(double, double, double), double x1, double x2);
void quadct(double x, double y, double xx[], double yy[], unsigned int nn,
	double *fa, double *fb, double *fc, double *fd);
void quadmx(double **a, int n);
void quadvl(double x, double y, double *fa, double *fb, double *fc, double *fd);
double ran0(int *idum);
double ran1(int *idum);
double ran2(int *idum);
double ran3(int *idum);
double ran4(int *idum);
void rank(unsigned int n, unsigned int indx[], unsigned int irank[]);
void ranpt(double pt[], double regn[], int n);
void ratint(double xa[], double ya[], int n, double x, double *y, double *dy);
void ratlsq(double (*fn)(double), double a, double b, int mm, int kk,
	double cof[], double *dev);
double ratval(double x, double cof[], int mm, int kk);
double rc(double x, double y);
double rd(double x, double y, double z);
void realft(double data[], unsigned int n, int isign);
void rebin(double rc, int nd, double r[], double xin[], double xi[]);
void red(int iz1, int iz2, int jz1, int jz2, int jm1, int jm2, int jmf,
	int ic1, int jc1, int jcf, int kc, double ***c, double **s);
void relax(double **u, double **rhs, int n);
void relax2(double **u, double **rhs, int n);
void resid(double **res, double **u, double **rhs, int n);
double revcst(double x[], double y[], int iorder[], int ncity, int n[]);
void reverse(int iorder[], int ncity, int n[]);
double rf(double x, double y, double z);
double rj(double x, double y, double z, double p);
void rk4(double y[], double dydx[], int n, double x, double h, double yout[],
	void (*derivs)(double, double [], double []));
void rkck(double y[], double dydx[], int n, double x, double h,
	double yout[], double yerr[], void (*derivs)(double, double [], double []));
void rkdumb(double vstart[], int nvar, double x1, double x2, int nstep,
	void (*derivs)(double, double [], double []));
void rkqs(double y[], double dydx[], int n, double *x,
	double htry, double eps, double yscal[], double *hdid, double *hnext,
	void (*derivs)(double, double [], double []));
void rlft3(double ***data, double **speq, unsigned int nn1,
	unsigned int nn2, unsigned int nn3, int isign);
double rofunc(double b);
void rotate(double **r, double **qt, int n, int i, double a, double b);
void rsolv(double **a, int n, double d[], double b[]);
void rstrct(double **uc, double **uf, int nc);
double rtbis(double (*func)(double), double x1, double x2, double xacc);
double rtflsp(double (*func)(double), double x1, double x2, double xacc);
double rtnewt(void (*funcd)(double, double *, double *), double x1, double x2,
	double xacc);
double rtsafe(void (*funcd)(double, double *, double *), double x1, double x2,
	double xacc);
double rtsec(double (*func)(double), double x1, double x2, double xacc);
void rzextr(int iest, double xest, double yest[], double yz[], double dy[], int nv);
void savgol(double c[], int np, int nl, int nr, int ld, int m);
void score(double xf, double y[], double f[]);
void scrsho(double (*fx)(double));
double select(unsigned int k, unsigned int n, double arr[]);
double selip(unsigned int k, unsigned int n, double arr[]);
void shell(unsigned int n, double a[]);
void shoot(int n, double v[], double f[]);
void shootf(int n, double v[], double f[]);
void simp1(double **a, int mm, int ll[], int nll, int iabf, int *kp,
	double *bmax);
void simp2(double **a, int n, int l2[], int nl2, int *ip, int kp, double *q1);
void simp3(double **a, int i1, int k1, int ip, int kp);
void simplx(double **a, int m, int n, int m1, int m2, int m3, int *icase,
	int izrov[], int iposv[]);
void simpr(double y[], double dydx[], double dfdx[], double **dfdy,
	int n, double xs, double htot, int nstep, double yout[],
	void (*derivs)(double, double [], double []));
void sinft(double y[], int n);
void slvsm2(double **u, double **rhs);
void slvsml(double **u, double **rhs);
void sncndn(double uu, double emmc, double *sn, double *cn, double *dn);
double snrm(unsigned int n, double sx[], int itol);
void sobseq(int *n, double x[]);
void solvde(int itmax, double conv, double slowc, double scalv[],
	int indexv[], int ne, int nb, int m, double **y, double ***c, double **s);
void sor(double **a, double **b, double **c, double **d, double **e,
	double **f, double **u, int jmax, double rjac);
void sort(unsigned int n, double arr[]);
void sort2(unsigned int n, double arr[], double brr[]);
void sort3(unsigned int n, double ra[], double rb[], double rc[]);
void spctrm(FILE *fp, double p[], int m, int k, int ovrlap);
void spear(double data1[], double data2[], unsigned int n, double *d, double *zd,
	double *probd, double *rs, double *probrs);
void sphbes(int n, double x, double *sj, double *sy, double *sjp, double *syp);
void splie2(double x1a[], double x2a[], double **ya, int m, int n, double **y2a);
void splin2(double x1a[], double x2a[], double **ya, double **y2a, int m, int n,
	double x1, double x2, double *y);
void spline(double x[], double y[], int n, double yp1, double ypn, double y2[]);
void splint(double xa[], double ya[], double y2a[], int n, double x, double *y);
void spread(double y, double yy[], unsigned int n, double x, int m);
void sprsax(double sa[], unsigned int ija[], double x[], double b[],
	unsigned int n);
void sprsin(double **a, int n, double thresh, unsigned int nmax, double sa[],
	unsigned int ija[]);
void sprspm(double sa[], unsigned int ija[], double sb[], unsigned int ijb[],
	double sc[], unsigned int ijc[]);
void sprstm(double sa[], unsigned int ija[], double sb[], unsigned int ijb[],
	double thresh, unsigned int nmax, double sc[], unsigned int ijc[]);
void sprstp(double sa[], unsigned int ija[], double sb[], unsigned int ijb[]);
void sprstx(double sa[], unsigned int ija[], double x[], double b[],
	unsigned int n);
void stifbs(double y[], double dydx[], int nv, double *xx,
	double htry, double eps, double yscal[], double *hdid, double *hnext,
	void (*derivs)(double, double [], double []));
void stiff(double y[], double dydx[], int n, double *x,
	double htry, double eps, double yscal[], double *hdid, double *hnext,
	void (*derivs)(double, double [], double []));
void stoerm(double y[], double d2y[], int nv, double xs,
	double htot, int nstep, double yout[],
	void (*derivs)(double, double [], double []));
void svbksb(double **u, double w[], double **v, int m, int n, double b[],
	double x[]);
void svdcmp(double **a, int m, int n, double w[], double **v);
void svdfit(double x[], double y[], double sig[], int ndata, double a[],
	int ma, double **u, double **v, double w[], double *chisq,
	void (*funcs)(double, double [], int));
void svdvar(double **v, int ma, double w[], double **cvm);
void toeplz(double r[], double x[], double y[], int n);
void tptest(double data1[], double data2[], unsigned int n, double *t, double *prob);
void tqli(double d[], double e[], int n, double **z);
double trapzd(double (*func)(double), double a, double b, int n);
void tred2(double **a, int n, double d[], double e[]);
void tridag(double a[], double b[], double c[], double r[], double u[],
	unsigned int n);
double trncst(double x[], double y[], int iorder[], int ncity, int n[]);
void trnspt(int iorder[], int ncity, int n[]);
void ttest(double data1[], unsigned int n1, double data2[], unsigned int n2,
	double *t, double *prob);
void tutest(double data1[], unsigned int n1, double data2[], unsigned int n2,
	double *t, double *prob);
void twofft(double data1[], double data2[], double fft1[], double fft2[],
	unsigned int n);
void vander(double x[], double w[], double q[], int n);
void vegas(double regn[], int ndim, double (*fxn)(double [], double), int init,
	unsigned int ncall, int itmx, int nprn, double *tgral, double *sd,
	double *chi2a);
void voltra(int n, int m, double t0, double h, double *t, double **f,
	double (*g)(int, double), double (*ak)(int, int, double, double));
void wt1(double a[], unsigned int n, int isign,
	void (*wtstep)(double [], unsigned int, int));
void wtn(double a[], unsigned int nn[], int ndim, int isign,
	void (*wtstep)(double [], unsigned int, int));
void wwghts(double wghts[], int n, double h,
	void (*kermom)(double [], double ,int));
int zbrac(double (*func)(double), double *x1, double *x2);
void zbrak(double (*fx)(double), double x1, double x2, int n, double xb1[],
	double xb2[], int *nb);
double zbrent(double (*func)(double), double x1, double x2, double tol);
void zrhqr(double a[], int m, double rtr[], double rti[]);
double zriddr(double (*func)(double), double x1, double x2, double xacc);
void zroots(fcomplex a[], int m, fcomplex roots[], int polish);
*/

#else /* ANSI */
/* traditional - K&R */

void f64_addint();
void f64_airy();
void f64_amebsa();
void f64_amoeba();
double f64_amotry();
double f64_amotsa();
void f64_anneal();
double f64_anorm2();
void f64_arcmak();
void f64_arcode();
void f64_arcsum();
void f64_asolve();
void f64_atimes();
void f64_avevar();
void f64_balanc();
void f64_banbks();
void f64_bandec();
void f64_banmul();
void f64_bcucof();
void f64_bcuint();
void f64_beschb();
double f64_bessi();
double f64_bessi0();
double f64_bessi1();
void f64_bessik();
double f64_bessj();
double f64_bessj0();
double f64_bessj1();
void f64_bessjy();
double f64_bessk();
double f64_bessk0();
double f64_bessk1();
double f64_bessy();
double f64_bessy0();
double f64_bessy1();
double f64_beta();
double f64_betacf();
double f64_betai();
double f64_bico();
void f64_bksub();
double f64_bnldev();
double f64_brent();
void f64_broydn();
void f64_bsstep();
void f64_caldat();
void f64_chder();
double f64_chebev();
void f64_chebft();
void f64_chebpc();
void f64_chint();
double f64_chixy();
void f64_choldc();
void f64_cholsl();
void f64_chsone();
void f64_chstwo();
void f64_cisi();
void f64_cntab1();
void f64_cntab2();
void f64_convlv();
void f64_copy();
void f64_correl();
void f64_cosft();
void f64_cosft1();
void f64_cosft2();
void f64_covsrt();
void f64_crank();
void f64_cyclic();
void f64_daub4();
double f64_dawson();
double f64_dbrent();
void f64_ddpoly();
int f64_decchk();
void f64_derivs();
double f64_df1dim();
void f64_dfour1();
void f64_dfpmin();
double f64_dfridr();
void f64_dftcor();
void f64_dftint();
void f64_difeq();
void f64_dlinmin();
double f64_dpythag();
void f64_drealft();
void f64_dsprsax();
void f64_dsprstx();
void f64_dsvbksb();
void f64_dsvdcmp();
void f64_eclass();
void f64_eclazz();
double f64_ei();
void f64_eigsrt();
double f64_elle();
double f64_ellf();
double f64_ellpi();
void f64_elmhes();
double f64_erfcc();
double f64_erff();
double f64_erffc();
void f64_eulsum();
double f64_evlmem();
double f64_expdev();
double f64_expint();
double f64_f1();
double f64_f1dim();
double f64_f2();
double f64_f3();
double f64_factln();
double f64_factrl();
void f64_fasper();
void f64_fdjac();
void f64_fgauss();
void f64_fill0();
void f64_fit();
void f64_fitexy();
void f64_fixrts();
void f64_fleg();
void f64_flmoon();
double f64_fmin();
void f64_four1();
void f64_fourew();
void f64_fourfs();
void f64_fourn();
void f64_fpoly();
void f64_fred2();
double f64_fredin();
void f64_frenel();
void f64_frprmn();
void f64_ftest();
double f64_gamdev();
double f64_gammln();
double f64_gammp();
double f64_gammq();
double f64_gasdev();
void f64_gaucof();
void f64_gauher();
void f64_gaujac();
void f64_gaulag();
void f64_gauleg();
void f64_gaussj();
void f64_gcf();
double f64_golden();
void f64_gser();
void f64_hpsel();
void f64_hpsort();
void f64_hqr();
void f64_hufapp();
void f64_hufdec();
void f64_hufenc();
void f64_hufmak();
void f64_hunt();
void f64_hypdrv();
fcomplex f64_hypgeo();
void f64_hypser();
unsigned short f64_icrc();
unsigned short f64_icrc1();
unsigned long f64_igray();
void f64_iindexx();
void f64_indexx();
void f64_interp();
int f64_irbit1();
int f64_irbit2();
void f64_jacobi();
void f64_jacobn();
int f64_julday();
void f64_kendl1();
void f64_kendl2();
void f64_kermom();
void f64_ks2d1s();
void f64_ks2d2s();
void f64_ksone();
void f64_kstwo();
void f64_laguer();
void f64_lfit();
void f64_linbcg();
void f64_linmin();
void f64_lnsrch();
void f64_load();
void f64_load1();
void f64_load2();
void f64_locate();
void f64_lop();
void f64_lubksb();
void f64_ludcmp();
void f64_machar();
void f64_matadd();
void f64_matsub();
void f64_medfit();
void f64_memcof();
int f64_metrop();
void f64_mgfas();
void f64_mglin();
double f64_midexp();
double f64_midinf();
double f64_midpnt();
double f64_midsql();
double f64_midsqu();
void f64_miser();
void f64_mmid();
void f64_mnbrak();
void f64_mnewt();
void f64_moment();
void f64_mp2dfr();
void f64_mpadd();
void f64_mpdiv();
void f64_mpinv();
void f64_mplsh();
void f64_mpmov();
void f64_mpmul();
void f64_mpneg();
void f64_mppi();
void f64_mprove();
void f64_mpsad();
void f64_mpsdv();
void f64_mpsmu();
void f64_mpsqrt();
void f64_mpsub();
void f64_mrqcof();
void f64_mrqmin();
void f64_newt();
void f64_odeint();
void f64_orthog();
void f64_pade();
void f64_pccheb();
void f64_pcshft();
void f64_pearsn();
void f64_period();
void f64_piksr2();
void f64_piksrt();
void f64_pinvs();
double f64_plgndr();
double f64_poidev();
void f64_polcoe();
void f64_polcof();
void f64_poldiv();
void f64_polin2();
void f64_polint();
void f64_powell();
void f64_predic();
double f64_probks();
void f64_psdes();
void f64_pwt();
void f64_pwtset();
double f64_pythag();
void f64_pzextr();
double f64_qgaus();
void f64_qrdcmp();
double f64_qromb();
double f64_qromo();
void f64_qroot();
void f64_qrsolv();
void f64_qrupdt();
double f64_qsimp();
double f64_qtrap();
double f64_quad3d();
void f64_quadct();
void f64_quadmx();
void f64_quadvl();
double f64_ran0();
double f64_ran1();
double f64_ran2();
double f64_ran3();
double f64_ran4();
void f64_rank();
void f64_ranpt();
void f64_ratint();
void f64_ratlsq();
double f64_ratval();
double f64_rc();
double f64_rd();
void f64_realft();
void f64_rebin();
void f64_red();
void f64_relax();
void f64_relax2();
void f64_resid();
double f64_revcst();
void f64_reverse();
double f64_rf();
double f64_rj();
void f64_rk4();
void f64_rkck();
void f64_rkdumb();
void f64_rkqs();
void f64_rlft3();
double f64_rofunc();
void f64_rotate();
void f64_rsolv();
void f64_rstrct();
double f64_rtbis();
double f64_rtflsp();
double f64_rtnewt();
double f64_rtsafe();
double f64_rtsec();
void f64_rzextr();
void f64_savgol();
void f64_score();
void f64_scrsho();
double f64_select();
double f64_selip();
void f64_shell();
void f64_shoot();
void f64_shootf();
void f64_simp1();
void f64_simp2();
void f64_simp3();
void f64_simplx();
void f64_simpr();
void f64_sinft();
void f64_slvsm2();
void f64_slvsml();
void f64_sncndn();
double f64_snrm();
void f64_sobseq();
void f64_solvde();
void f64_sor();
void f64_sort();
void f64_sort2();
void f64_sort3();
void f64_spctrm();
void f64_spear();
void f64_sphbes();
void f64_splie2();
void f64_splin2();
void f64_spline();
void f64_splint();
void f64_spread();
void f64_sprsax();
void f64_sprsin();
void f64_sprspm();
void f64_sprstm();
void f64_sprstp();
void f64_sprstx();
void f64_stifbs();
void f64_stiff();
void f64_stoerm();
void f64_svbksb();
void f64_svdcmp();
void f64_svdfit();
void f64_svdvar();
void f64_toeplz();
void f64_tptest();
void f64_tqli();
double f64_trapzd();
void f64_tred2();
void f64_tridag();
double f64_trncst();
void f64_trnspt();
void f64_ttest();
void f64_tutest();
void f64_twofft();
void f64_vander();
void f64_vegas();
void f64_voltra();
void f64_wt1();
void f64_wtn();
void f64_wwghts();
int f64_zbrac();
void f64_zbrak();
double f64_zbrent();
void f64_zrhqr();
double f64_zriddr();
void f64_zroots();
/*
void addint();
void airy();
void amebsa();
void amoeba();
double amotry();
double amotsa();
void anneal();
double anorm2();
void arcmak();
void arcode();
void arcsum();
void asolve();
void atimes();
void avevar();
void balanc();
void banbks();
void bandec();
void banmul();
void bcucof();
void bcuint();
void beschb();
double bessi();
double bessi0();
double bessi1();
void bessik();
double bessj();
double bessj0();
double bessj1();
void bessjy();
double bessk();
double bessk0();
double bessk1();
double bessy();
double bessy0();
double bessy1();
double beta();
double betacf();
double betai();
double bico();
void bksub();
double bnldev();
double brent();
void broydn();
void bsstep();
void caldat();
void chder();
double chebev();
void chebft();
void chebpc();
void chint();
double chixy();
void choldc();
void cholsl();
void chsone();
void chstwo();
void cisi();
void cntab1();
void cntab2();
void convlv();
void copy();
void correl();
void cosft();
void cosft1();
void cosft2();
void covsrt();
void crank();
void cyclic();
void daub4();
double dawson();
double dbrent();
void ddpoly();
int decchk();
void derivs();
double df1dim();
void dfour1();
void dfpmin();
double dfridr();
void dftcor();
void dftint();
void difeq();
void dlinmin();
double dpythag();
void drealft();
void dsprsax();
void dsprstx();
void dsvbksb();
void dsvdcmp();
void eclass();
void eclazz();
double ei();
void eigsrt();
double elle();
double ellf();
double ellpi();
void elmhes();
double erfcc();
double erff();
double erffc();
void eulsum();
double evlmem();
double expdev();
double expint();
double f1();
double f1dim();
double f2();
double f3();
double factln();
double factrl();
void fasper();
void fdjac();
void fgauss();
void fill0();
void fit();
void fitexy();
void fixrts();
void fleg();
void flmoon();
double fmin();
void four1();
void fourew();
void fourfs();
void fourn();
void fpoly();
void fred2();
double fredin();
void frenel();
void frprmn();
void ftest();
double gamdev();
double gammln();
double gammp();
double gammq();
double gasdev();
void gaucof();
void gauher();
void gaujac();
void gaulag();
void gauleg();
void gaussj();
void gcf();
double golden();
void gser();
void hpsel();
void hpsort();
void hqr();
void hufapp();
void hufdec();
void hufenc();
void hufmak();
void hunt();
void hypdrv();
fcomplex hypgeo();
void hypser();
unsigned short icrc();
unsigned short icrc1();
unsigned int igray();
void iindexx();
void indexx();
void interp();
int irbit1();
int irbit2();
void jacobi();
void jacobn();
int julday();
void kendl1();
void kendl2();
void kermom();
void ks2d1s();
void ks2d2s();
void ksone();
void kstwo();
void laguer();
void lfit();
void linbcg();
void linmin();
void lnsrch();
void load();
void load1();
void load2();
void locate();
void lop();
void lubksb();
void ludcmp();
void machar();
void matadd();
void matsub();
void medfit();
void memcof();
int metrop();
void mgfas();
void mglin();
double midexp();
double midinf();
double midpnt();
double midsql();
double midsqu();
void miser();
void mmid();
void mnbrak();
void mnewt();
void moment();
void mp2dfr();
void mpadd();
void mpdiv();
void mpinv();
void mplsh();
void mpmov();
void mpmul();
void mpneg();
void mppi();
void mprove();
void mpsad();
void mpsdv();
void mpsmu();
void mpsqrt();
void mpsub();
void mrqcof();
void mrqmin();
void newt();
void odeint();
void orthog();
void pade();
void pccheb();
void pcshft();
void pearsn();
void period();
void piksr2();
void piksrt();
void pinvs();
double plgndr();
double poidev();
void polcoe();
void polcof();
void poldiv();
void polin2();
void polint();
void powell();
void predic();
double probks();
void psdes();
void pwt();
void pwtset();
double pythag();
void pzextr();
double qgaus();
void qrdcmp();
double qromb();
double qromo();
void qroot();
void qrsolv();
void qrupdt();
double qsimp();
double qtrap();
double quad3d();
void quadct();
void quadmx();
void quadvl();
double ran0();
double ran1();
double ran2();
double ran3();
double ran4();
void rank();
void ranpt();
void ratint();
void ratlsq();
double ratval();
double rc();
double rd();
void realft();
void rebin();
void red();
void relax();
void relax2();
void resid();
double revcst();
void reverse();
double rf();
double rj();
void rk4();
void rkck();
void rkdumb();
void rkqs();
void rlft3();
double rofunc();
void rotate();
void rsolv();
void rstrct();
double rtbis();
double rtflsp();
double rtnewt();
double rtsafe();
double rtsec();
void rzextr();
void savgol();
void score();
void scrsho();
double select();
double selip();
void shell();
void shoot();
void shootf();
void simp1();
void simp2();
void simp3();
void simplx();
void simpr();
void sinft();
void slvsm2();
void slvsml();
void sncndn();
double snrm();
void sobseq();
void solvde();
void sor();
void sort();
void sort2();
void sort3();
void spctrm();
void spear();
void sphbes();
void splie2();
void splin2();
void spline();
void splint();
void spread();
void sprsax();
void sprsin();
void sprspm();
void sprstm();
void sprstp();
void sprstx();
void stifbs();
void stiff();
void stoerm();
void svbksb();
void svdcmp();
void svdfit();
void svdvar();
void toeplz();
void tptest();
void tqli();
double trapzd();
void tred2();
void tridag();
double trncst();
void trnspt();
void ttest();
void tutest();
void twofft();
void vander();
void vegas();
void voltra();
void wt1();
void wtn();
void wwghts();
int zbrac();
void zbrak();
double zbrent();
void zrhqr();
double zriddr();
void zroots();
*/
#endif /* ANSI */

#endif /* _NR_H_ */


#ifdef __cplusplus
}
#endif

