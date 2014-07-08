#ifndef _NR_UTILS_H_
#define _NR_UTILS_H_

static double sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

static double dsqrarg;
#define DSQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)

static double dmaxarg1,dmaxarg2;
#define DMAX(a,b) (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1) > (dmaxarg2) ?\
        (dmaxarg1) : (dmaxarg2))

static double dminarg1,dminarg2;
#define DMIN(a,b) (dminarg1=(a),dminarg2=(b),(dminarg1) < (dminarg2) ?\
        (dminarg1) : (dminarg2))

static double maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
        (maxarg1) : (maxarg2))

static double minarg1,minarg2;
#define FMIN(a,b) (minarg1=(a),minarg2=(b),(minarg1) < (minarg2) ?\
        (minarg1) : (minarg2))

static int lmaxarg1,lmaxarg2;
#define LMAX(a,b) (lmaxarg1=(a),lmaxarg2=(b),(lmaxarg1) > (lmaxarg2) ?\
        (lmaxarg1) : (lmaxarg2))

static int lminarg1,lminarg2;
#define LMIN(a,b) (lminarg1=(a),lminarg2=(b),(lminarg1) < (lminarg2) ?\
        (lminarg1) : (lminarg2))

static int imaxarg1,imaxarg2;
#define IMAX(a,b) (imaxarg1=(a),imaxarg2=(b),(imaxarg1) > (imaxarg2) ?\
        (imaxarg1) : (imaxarg2))

static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
        (iminarg1) : (iminarg2))

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

#if defined(__STDC__) || defined(ANSI) || defined(NRANSI) /* ANSI */

void f64_nrerror(char error_text[]);
double *f64_vector(int nl, int nh);
int *f64_ivector(int nl, int nh);
unsigned char *f64_cvector(int nl, int nh);
unsigned int *f64_lvector(int nl, int nh);
double *f64_dvector(int nl, int nh);
double **f64_matrix(int nrl, int nrh, int ncl, int nch);
double **f64_dmatrix(int nrl, int nrh, int ncl, int nch);
int **f64_imatrix(int nrl, int nrh, int ncl, int nch);
double **f64_submatrix(double **a, int oldrl, int oldrh, int oldcl, int oldch,
	int newrl, int newcl);
double **f64_convert_matrix(double *a, int nrl, int nrh, int ncl, int nch);
double ***f64_f3tensor(int nrl, int nrh, int ncl, int nch, int ndl, int ndh);
void f64_free_vector(double *v, int nl, int nh);
void f64_free_ivector(int *v, int nl, int nh);
void f64_free_cvector(unsigned char *v, int nl, int nh);
void f64_free_lvector(unsigned int *v, int nl, int nh);
void f64_free_dvector(double *v, int nl, int nh);
void f64_free_matrix(double **m, int nrl, int nrh, int ncl, int nch);
void f64_free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch);
void f64_free_imatrix(int **m, int nrl, int nrh, int ncl, int nch);
void f64_free_submatrix(double **b, int nrl, int nrh, int ncl, int nch);
void f64_free_convert_matrix(double **b, int nrl, int nrh, int ncl, int nch);
void f64_free_f3tensor(double ***t, int nrl, int nrh, int ncl, int nch,
	int ndl, int ndh);

#else /* ANSI */
/* traditional - K&R */

void f64_nrerror();
double *vector();
double **matrix();
double **submatrix();
double **convert_matrix();
double ***f3tensor();
double *dvector();
double **dmatrix();
int *ivector();
int **imatrix();
unsigned char *cvector();
unsigned int *lvector();
void free_vector();
void free_dvector();
void free_ivector();
void free_cvector();
void free_lvector();
void free_matrix();
void free_submatrix();
void free_convert_matrix();
void free_dmatrix();
void free_imatrix();
void free_f3tensor();

#endif /* ANSI */

#endif /* _NR_UTILS_H_ */
