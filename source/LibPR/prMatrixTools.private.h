////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			xxx-2.private.h : for function2
//
//			main coder: Y. Masutani
//
//			update history
//
//			2007.05.04	start coding
//			2007.05.05	bug fixed for aaa
//
//
////////////////////////////////////////////////////////////////////////////////////////


#include "../LibPR.h"
#include "./prSortTools.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PI 3.141592653589793832

#define ANSI
#include "../../NR2.0ANSI_v2.06_double/C_others/f64_nr.h"
#include "../../NR2.0ANSI_v2.06_double/C_recipes/f64_nr.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */


static float	parameter=0.0f;
double* PR_GetEigenVectorNR(double** matrix, unsigned int msize, double lambda);

