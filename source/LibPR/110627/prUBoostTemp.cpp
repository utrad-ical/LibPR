
#include <float.h>
#include "./prUBoost.private.h"
#include "../LibPR.h"

int 
PR_TrainEnsembleUBoostTemp(
	PR_CASELIST* caseList,PR_ENSEMBLE* ensemble,int channelBoost,double eta,char bDataWeightLogOutput);


int PR_TrainAdaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	PR_TrainEnsembleUBoostTemp(caseList,ensemble,PR_MODELCODE_ADABOOST,0.0,0);
	return PR_EOT;
}

int PR_TrainMadaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	PR_TrainEnsembleUBoostTemp(caseList,ensemble,PR_MODELCODE_MADABOOST,0.0,0);
	return PR_EOT;
}

int PR_TrainLogitBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	PR_TrainEnsembleUBoost(caseList,ensemble,PR_MODELCODE_LOGITBOOST,0.0,0);
	return PR_EOT;
}

int PR_TrainEtaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double eta)
{
	PR_TrainEnsembleUBoost(caseList,ensemble,PR_MODELCODE_ETABOOST,eta,0);
	return PR_EOT;
}

int PR_TrainRobustEtaBoostEnsembleTemp(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double eta)
{
	PR_TrainEnsembleUBoost(caseList,ensemble,PR_MODELCODE_ROBUSTETABOOST,eta,0);
	return PR_EOT;
}


int 
PR_TrainEnsembleUBoostTemp(
	PR_CASELIST* caseList,
	PR_ENSEMBLE* ensemble,		// list of weak-classifiers for ensemble
	int channelBoost,
	double eta,
	char bDataWeightLogOutput)
{

	fprintf(stderr,"boost(a) - maxNumWeakClassifier=%d\n",ensemble->numNode);


	char dmyfilebase[128];
	sprintf(dmyfilebase,"dmyTrainBoost_");

	unsigned int nmodel, iFeat;

	if(caseList==NULL || ensemble==NULL)
		return PR_ErrorMessageInt("unknown inputs (@PR_TrainDiscreteAdaBoostEnsemble)");
	if(PR_ReguralizeCostBoost(caseList)==PR_AVEND)
		return PR_ErrorMessageInt("not set dataWeight (@PR_TrainDiscreteAdaBoostEnsemble)");


	// count number of candidate
	PR_CountNumCandsForCaseList(caseList);


	if(channelBoost!=13 && channelBoost!=14)	eta=0.0;

	// make candidates array for ensemble training
	PR_CANDDATAARRAY* dataAry = PR_NewCandDataArray(caseList,eta);


	char strDmy[256];


	// get results of ascending sort for feature value
	FILE* fpTemp = tmpfile();
	unsigned long* rankDmy;
	printf("temporal,");
	for(unsigned int i=0; i<caseList->featTagTable->numFeat; i++)
	{
		rankDmy = PR_GetAscendSortResultOfFeature(dataAry,caseList->featTagTable->tags[i]);
		fwrite(rankDmy,sizeof(unsigned int),dataAry->num,fpTemp);
		delete [] rankDmy;
	}

	PR_ENSEMBLENODE* currentModel = ensemble->firstNode;
	double denominatorNormalize;
	int response;


	unsigned int iBestFeat;
	PR_BOOSTWEAKCLASSIFIERPARAMS* dmyModel;
	PR_BOOSTWEAKCLASSIFIERPARAMS* bestModel;


//	fprintf(stderr,"boost(b) - maxNumWeakClassifier=%d\n",ensemble->numNode);

	for(nmodel=0; nmodel<ensemble->numNode; nmodel++)
	{
		dmyModel = bestModel = NULL;
		PR_BOOSTWEAKCLASSIFIERPARAMS* weakModel = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentModel->data;
		PR_BOOSTWEAKCLASSIFIERPARAMS* prevModel = NULL;
		if(nmodel>0)	prevModel = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentModel->prev->data;

		// set data-weight
		PR_IncludeCostInDataWeightForBoost(dataAry);
		denominatorNormalize = PR_NormalizeDataWeightForBoost(dataAry);

		rankDmy = new unsigned long [dataAry->num];

		// define features which will be tested
		if(weakModel->mode==0 && weakModel->featureTag->group==0xffff && weakModel->featureTag->element==0xffff)
		{
			unsigned int iPrevFeat = PR_ExtractFeatNumberInFeatTagTable(caseList->featTagTable,prevModel->featureTag);
			bestModel->trainError = 10.0;

			for(iFeat=0; iFeat<caseList->featTagTable->numFeat; iFeat++)
			{
				char mode = 0;
				if(iFeat==iPrevFeat)
				{
					if(prevModel->mode==-1)		mode = 1;
					else						mode = -1;
				}

				fseek(fpTemp,sizeof(unsigned int)*dataAry->num*iFeat,SEEK_SET);
				fread(rankDmy,sizeof(unsigned long),dataAry->num,fpTemp);
				dmyModel = PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,iFeat,rankDmy,mode);

				if(bestModel->trainError>dmyModel->trainError)
				{
					PR_CopyBoostWeakClassifierParams(bestModel,dmyModel);
					iBestFeat = iFeat;
				}
			}
		}
		else
		{
			printf("[%04x.%04x],",weakModel->featureTag->group,weakModel->featureTag->element);
			response = PR_CheckIfFeatTagIsMemberOfFeatTagTable(weakModel->featureTag,caseList->featTagTable,&iBestFeat);
			
			fseek(fpTemp,sizeof(unsigned int)*dataAry->num*iBestFeat,SEEK_SET);
			fread(rankDmy,sizeof(unsigned long),dataAry->num,fpTemp);
			bestModel = PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,iBestFeat,rankDmy,weakModel->mode);
		}

		delete [] rankDmy;
		bestModel->nodeIndex = nmodel+1;



		// update (nmodel+1)-th weak classifier
		PR_CopyBoostWeakClassifierParams(weakModel,bestModel);

		PR_GetWeightOfWeakClassifier(dataAry,channelBoost,weakModel,denominatorNormalize);

		printf("> model#%d/%d; feat#.%d ",nmodel+1,ensemble->numNode,iBestFeat);
		printf("Thd%.3e(%+d) ",weakModel->threshold,weakModel->mode);
		printf("Err%.3e a%.3e\n",weakModel->trainError,weakModel->nodeWeight);

		PR_GenerateEsembleOutput(dataAry,weakModel);

		// update data weights
		PR_UpdateDataWeight(dataAry,channelBoost,eta);


		if(bDataWeightLogOutput)
		{
			sprintf(strDmy,"logDataWeight_%d.txt",nmodel+1);
			FILE* fpLog = fopen(strDmy,"w");

			fprintf(fpLog,"%04x %04x <-_selected_feature\n",weakModel->featureTag->group,weakModel->featureTag->element);
			fprintf(fpLog,"%le <-_weight_for_feature\n",weakModel->nodeWeight);
			fprintf(fpLog,"%le <-_weight_for_feature\n",weakModel->nodeWeight);
			fprintf(fpLog,"(case_id,cand_id,class,data_weight)\n");
			for(unsigned long i=0; i<dataAry->num; i++)
			{
				PR_CASEDATA* caseData = (PR_CASEDATA*)dataAry->cands[i]->rootCaseData;
				PR_CANDDATA* candData = dataAry->cands[i];
				double dWeight = ((PR_BOOSTOPTION*)(candData->optionParam))->dataWeight;
				fprintf(fpLog,"%d %d %u %le\n",caseData->idCase,candData->idCand,candData->classLabel,dWeight);
			}
			fclose(fpLog);
		}

		if(dmyModel!=NULL)		PR_DeleteBoostWeakClassifierParams(dmyModel);
		if(bestModel!=NULL)		PR_DeleteBoostWeakClassifierParams(bestModel);


		// shift target ensemble node to next
		currentModel = currentModel->next;
	}

	PR_DeleteCandDataArray(dataAry);
	fclose(fpTemp);

	return PR_EOT;
}