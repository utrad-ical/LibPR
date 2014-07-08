

#include "prBoostEnsemblePrimitive.h"


PR_BOOSTWEAKCLASSIFIERPARAMS* PR_NewAddBoostWeakClassifierParams(PR_ENSEMBLENODE* node);
int PR_DeleteEnsembleNodeWithBoostWeakClassifierParams(PR_ENSEMBLENODE* node);

int PR_NewBoostWeakClassifierParamsAddToEnsemble(PR_ENSEMBLE* ensemble);

PR_BOOSTOPTION* PR_NewAddBoostOptionToCand(PR_CANDDATA* candData);


