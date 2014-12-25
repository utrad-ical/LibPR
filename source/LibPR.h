////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR-1.0 : library for Pattern Recognition
//
//			LibPR.h : main header file
//
//			main coder: M. Nemoto
//
//			update history
//
//			2007.08.27	start coding
//			
//
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef LIB_PR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#include "./LibPR/prBoostEnsemblePrimitive.h"
#include "./LibPR/prBoostEnsemblePrimitiveMT.h"
#include "./LibPR/prCandDataSet.h"
#include "./LibPR/prCaseDataIO.h"
#include "./LibPR/prErrorMessage.h"
#include "./LibPR/prEvaluation.h"
#include "./LibPR/prFeatureData_temp.h"
#include "./LibPR/prFeatTag.h"
#include "./LibPR/prHistogram.h"
#include "./LibPR/prKernelLDA.h"
#include "./LibPR/prKernelMethodTools.h"
#include "./LibPR/prMahalanobis.h"
#include "./LibPR/prMahalanobis_temp.h"
#include "./LibPR/prMatrixTools.h"
#include "./LibPR/prPCA.h"
#include "./LibPR/prSortTools.h"
#include "./LibPR/prStatistics.h"
#include "./LibPR/prTableInputFile.h"
#include "./LibPR/prUBoost.h"
#include "./LibPR/prUBoostMT.h"
#include "./LibPR/prUBoostMT2.h"


//#include "../NR2.0ANSI_v2.06_double/f64_nr_functions.h"
//#include "../NR2.0ANSI_v2.06_double/C_recipes/f64_nr.h"


#define	PR_EOT		0
#define PR_AVEND	1


/*-- Classifier Model Code --*/

//-- 0<code<10 : traditional classifiers --//
#define PR_CHANNEL_KNEIGHBOR	0
#define PR_CHANNEL_PCA			1
#define PR_CHANNEL_LDA			2
#define PR_CHANNEL_MAHALANOBIS	3

//-- 10<=code<20 : boosting ensembles --//
#define PR_CHANNEL_ADABOOST			10
#define PR_CHANNEL_MADABOOST		11
#define PR_CHANNEL_LOGITBOOST		12
#define PR_CHANNEL_ETABOOST			13
#define PR_CHANNEL_ROBUSTETABOOST	14

//-- 20<=code<30 : kernel machines --//
#define PR_CHANNEL_KPCA		20
#define PR_CHANNEL_KLDA		21
#define PR_CHANNEL_SVM		22



#define LIB_PR_H
#endif


