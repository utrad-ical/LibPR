# Makefile

LIB = libPR-1.04.a

SRC_PR  = source/LibPR/
SRC_NR  = NR2.0ANSI_v2.06_double/C_recipes/
SRC_GEOMETRY  = source/Geometry/
SRC_TRANSFORM = source/Transform/
SRC_UTILITY   = source/Utility/

DST_HEADDER = /usr/include/libPR-1.04/LibPR

#----------------------------------------------------------------------------
#       Macros
#----------------------------------------------------------------------------
AR            = ar
CXX           = g++
CXXFLAGS      = -c -O2 -lpthreads
RM            = rm -rf
OBJS          = $(SRC_NR)f64_balanc.o $(SRC_NR)f64_elmhes.o $(SRC_NR)f64_gaussj.o $(SRC_NR)f64_hqr.o $(SRC_NR)f64_jacobi.o $(SRC_NR)f64_lubksb.o $(SRC_NR)f64_ludcmp.o $(SRC_NR)f64_nrutil.o $(SRC_NR)f64_pythag.o $(SRC_NR)f64_tqli.o $(SRC_NR)f64_tred2.o $(SRC_PR)GetLapTime.o $(SRC_PR)prBoostEnsemblePrimitive.o $(SRC_PR)prBoostEnsemblePrimitiveMT.o $(SRC_PR)prCandDataSet.o $(SRC_PR)prCaseDataIO.o $(SRC_PR)prErrorMessage.o $(SRC_PR)prErrorOfClassifier.o $(SRC_PR)prEvaluation.o $(SRC_PR)prFeatTag.o $(SRC_PR)prFeatureData_temp.o $(SRC_PR)prHistogram.o $(SRC_PR)prIsomap.o $(SRC_PR)prKernelLDA.o $(SRC_PR)prKernelMethodTools.o $(SRC_PR)prListEnsemble.o $(SRC_PR)prMahalanobis.o $(SRC_PR)prMahalanobis_temp.o $(SRC_PR)prMatrixTools.o $(SRC_PR)prPCA.o $(SRC_PR)prRealAdaBoost.o $(SRC_PR)prSortTools.o $(SRC_PR)prStatistics.o $(SRC_PR)prTableInputFile.o $(SRC_PR)prThresholdingForFeats.o $(SRC_PR)prUBoost.o $(SRC_PR)prUBoostMT.o


#----------------------------------------------------------------------------
#       Implicit rules
#----------------------------------------------------------------------------
.SUFFIXES: .cxx .cpp .c

.cxx.o:
	$(CXX) $(CXXFLAGS) -o $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ $^

.c.o:
	$(CXX) $(CXXFLAGS) -o $@ $^


#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	
$(SRC_NR)f64_balanc.o:                  $(SRC_NR)f64_balanc.c
$(SRC_NR)f64_elmhes.o:                  $(SRC_NR)f64_elmhes.c
$(SRC_NR)f64_gaussj.o:                  $(SRC_NR)f64_gaussj.c
$(SRC_NR)f64_hqr.o:                     $(SRC_NR)f64_hqr.c
$(SRC_NR)f64_jacobi.o:                  $(SRC_NR)f64_jacobi.c
$(SRC_NR)f64_lubksb.o:                  $(SRC_NR)f64_lubksb.c
$(SRC_NR)f64_ludcmp.o:                  $(SRC_NR)f64_ludcmp.c
$(SRC_NR)f64_nrutil.o:                  $(SRC_NR)f64_nrutil.c
$(SRC_NR)f64_pythag.o:                  $(SRC_NR)f64_pythag.c
$(SRC_NR)f64_tqli.o:                    $(SRC_NR)f64_tqli.c
$(SRC_NR)f64_tred2.o:                   $(SRC_NR)f64_tred2.c

$(SRC_PR)GetLapTime.o:                  $(SRC_PR)GetLapTime.cpp
$(SRC_PR)prBoostEnsemblePrimitive.o:    $(SRC_PR)prBoostEnsemblePrimitive.cpp
$(SRC_PR)prBoostEnsemblePrimitiveMT.o:  $(SRC_PR)prBoostEnsemblePrimitiveMT.cpp
$(SRC_PR)prCandDataSet.o:               $(SRC_PR)prCandDataSet.cpp
$(SRC_PR)prCaseDataIO.o:                $(SRC_PR)prCaseDataIO.cpp
$(SRC_PR)prErrorMessage.o:              $(SRC_PR)prErrorMessage.cpp
$(SRC_PR)prErrorOfClassifier.o:         $(SRC_PR)prErrorOfClassifier.cpp
$(SRC_PR)prEvaluation.o:                $(SRC_PR)prEvaluation.cpp
$(SRC_PR)prFeatTag.o:                   $(SRC_PR)prFeatTag.cpp
$(SRC_PR)prFeatureData_temp.o:          $(SRC_PR)prFeatureData_temp.cpp
$(SRC_PR)prHistogram.o:                 $(SRC_PR)prHistogram.cpp
$(SRC_PR)prIsomap.o:                    $(SRC_PR)prIsomap.cpp
$(SRC_PR)prKernelLDA.o:                 $(SRC_PR)prKernelLDA.cpp
$(SRC_PR)prKernelMethodTools.o:         $(SRC_PR)prKernelMethodTools.cpp
$(SRC_PR)prListEnsemble.o:              $(SRC_PR)prListEnsemble.cpp
$(SRC_PR)prMahalanobis.o:               $(SRC_PR)prMahalanobis.cpp
$(SRC_PR)prMahalanobis_temp.o:          $(SRC_PR)prMahalanobis_temp.cpp
$(SRC_PR)prMatrixTools.o:               $(SRC_PR)prMatrixTools.cpp
$(SRC_PR)prPCA.o:                       $(SRC_PR)prPCA.cpp
$(SRC_PR)prRealAdaBoost.o:              $(SRC_PR)prRealAdaBoost.cpp
$(SRC_PR)prSortTools.o:                 $(SRC_PR)prSortTools.cpp
$(SRC_PR)prStatistics.o:                $(SRC_PR)prStatistics.cpp
$(SRC_PR)prTableInputFile.o:            $(SRC_PR)prTableInputFile.cpp
$(SRC_PR)prThresholdingForFeats.o:      $(SRC_PR)prThresholdingForFeats.cpp
$(SRC_PR)prUBoost.o:                    $(SRC_PR)prUBoost.cpp
$(SRC_PR)prUBoostMT.o:                  $(SRC_PR)prUBoostMT.cpp

.PHONY: clean realclean install

clean:
	-$(RM) $(OBJS)

realclean: clean
	-$(RM) $(SHLIB) $(LIB)

install:
	install -m 644 -o root -g root -p $(LIB) /usr/lib
	install -d $(DST_HEADDER)
	install -m 644 -o root -g root -p source/LibPR.h /usr/include/libVOLbinary-1.04
	install -m 644 -o root -g root -p $(SRC_PR)GetLapTime.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prBoostEnsemblePrimitive.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prBoostEnsemblePrimitiveMT.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prCaseDataIO.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prCandDataSet.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prErrorMessage.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prErrorOfClassifier.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prEvaluation.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prFeatTag.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prFeatureData_temp.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prHistogram.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prIsomap.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prKernelLDA.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prKernelMethodTools.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prListEnsemble.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prMahalanobis.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prMahalanobis_temp.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prMatrixTools.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prPCA.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prRealAdaBoost.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prSortTools.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prStatistics.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prTableInputFile.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prThresholdingForFeats.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prUBoost.h $(DST_HEADDER)
	install -m 644 -o root -g root -p $(SRC_PR)prUBoostMT.h $(DST_HEADDER)

