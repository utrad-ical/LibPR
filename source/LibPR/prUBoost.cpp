
#include "./prUBoost.h"
#include "./prUBoost.private.h"



PR_ENSEMBLE* PR_SimpleAdaBoost(PR_CASELIST* caseList, unsigned int numClassifier)
{
	return PR_UBoost(caseList,PR_CHANNEL_ADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0);
}

PR_ENSEMBLE* PR_SimpleMadaBoost(PR_CASELIST* caseList, unsigned int numClassifier)
{
	return PR_UBoost(caseList,PR_CHANNEL_MADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0);
}

PR_ENSEMBLE* PR_SimpleLogitBoost(PR_CASELIST* caseList, unsigned int numClassifier)
{
	return PR_UBoost(caseList,PR_CHANNEL_LOGITBOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0);
}

PR_ENSEMBLE* PR_SimpleEtaBoost(PR_CASELIST* caseList, unsigned int numClassifier, double eta)
{
	return PR_UBoost(caseList,PR_CHANNEL_ETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,eta);
}

PR_ENSEMBLE* PR_SimpleRobustEtaBoost(PR_CASELIST* caseList, unsigned int numClassifier, double eta)
{
	return PR_UBoost(caseList,PR_CHANNEL_ROBUSTETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,eta);
}

PR_ENSEMBLE* PR_CostSensitiveAdaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier)
{
	return PR_UBoost(caseList,PR_CHANNEL_ADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0);
}

PR_ENSEMBLE* PR_CostSensitiveMadaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier)
{
	return PR_UBoost(caseList,PR_CHANNEL_MADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0);
}

PR_ENSEMBLE* PR_CostSensitiveLogitBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier)
{
	return PR_UBoost(caseList,PR_CHANNEL_LOGITBOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0);
}

PR_ENSEMBLE* PR_CostSensitiveEtaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier, double eta)
{
	return PR_UBoost(caseList,PR_CHANNEL_ETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,eta);
}

PR_ENSEMBLE* PR_CostSensitiveRobustEtaBoostForImbalancedDataset(PR_CASELIST* caseList, unsigned int numClassifier, double eta)
{
	return PR_UBoost(caseList,PR_CHANNEL_ROBUSTETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,eta);
}

PR_ENSEMBLE* PR_UBoost(
	PR_CASELIST* caselist, int chBoost, unsigned int numClassifier, int chAddCost, double eta)
{
	int rawCand=0;

	if(caselist->childNodeFirstCase->childNodeFirstCand->optionParam==NULL)
	{
		// データ重み格納メモリ領域ない場合、確保
		PR_NewAddBoostOptionToAllCands(caselist);
		rawCand=1;
	}

	// 事前コストの初期化
	PR_InitializeCostBoost(caselist,chAddCost);

	// アンサンブルのメモリ領域宣言
	PR_ENSEMBLE* ensemble = PR_NewBoostEnsemble(numClassifier);

	// 識別器アンサンブルの学習
	PR_UBoostEssentialFunc(caselist,ensemble,chBoost,eta);

	// データ重みメモリ開放
	if(rawCand)		PR_DeleteBoostOptionInAllCands(caselist);

	return ensemble;
}


//#define MODE_USING_TEMPORAL_FP	// with bag -- cannot fwrite tmpfile (= sort result)
//#define GET_LAPTIME

int PR_DisplayDataWeightBoost(PR_CANDDATAARRAY* ary, unsigned long nDisplay)
{
	for(unsigned long i=0; i<ary->num; )
	{
		PR_BOOSTOPTION* boostOption = (PR_BOOSTOPTION*)(ary->cands[i]->optionParam);
		printf("w[%d]=%e\n",i,boostOption->dataWeight);
		i += ary->num/nDisplay;
	}
	return 0;
}


int 
PR_UBoostEssentialFunc(
	PR_CASELIST* caseList,
	PR_ENSEMBLE* ensemble,		// list of weak-classifiers for ensemble
	int channelBoost,
	double eta)
{
	fprintf(stderr,"UBoost; nModel=%d, type=%d, eta=%.4f\n",ensemble->numNode,channelBoost,eta);
	printf("0");

	if(caseList==NULL || ensemble==NULL || 
		caseList->childNodeFirstCase->childNodeFirstCand->optionParam==NULL)
	{
		printf("aaaaa!");
		return PR_ErrorMessageInt("unknown inputs (PR_UBoostEssentialFunc)");
	}
//	if(PR_ReguralizeCostBoost(caseList)==PR_AVEND)
//		return PR_ErrorMessageInt("unset data-weight (PR_UBoostEssentialFunc)");
	printf("1");


	///--- utilities ---/// 
//	char strDmy[256];
	unsigned int iFeat, nFeat = caseList->featTagTable->numFeat;
	unsigned int iModel, nModel = ensemble->numNode;
	printf("a");

	///--- count number of candidate ---///
	PR_CountNumCandsForCaseList(caseList);
	printf("b");


	///--- make cand-data-array for learning ---///
	PR_CANDDATAARRAY* dataAry = PR_NewCandDataArray(caseList,eta);
	printf("c");

	
	///--- ascending-sorting of each feature ---///
#ifndef MODE_USING_TEMPORAL_FP
	unsigned long** dataSorted = new unsigned long* [nFeat];
	for(iFeat=0; iFeat<nFeat; iFeat++)
	{
		dataSorted[iFeat] = PR_GetAscendSortResultOfFeature(dataAry,caseList->featTagTable->tags[iFeat]);
	}
//	unsigned int** dataSorted32 = new unsigned int* [caseList->featTagTable->numFeat];
//	for(unsigned int i=0; i<nFeat; i++)
//	{
//		dataSorted32[i] = PR_GetAscendSortResultOfFeatureOld(dataAry,caseList->featTagTable->tags[i]);
//	}
#else
	FILE* fpTemp;
	errno_t err = tmpfile_s(&fpTemp);
	unsigned long* rankDmy;
	printf("temporal(%d);",err);
	for(iFeat=0; iFeat<nFeat; iFeat++)
	{
		printf("%d,",iFeat);
		rankDmy = PR_GetAscendSortResultOfFeature(dataAry,caseList->featTagTable->tags[iFeat]);
		printf("sort,");
		fwrite(rankDmy,sizeof(unsigned long),dataAry->num,fpTemp);
		printf("write,");
		delete [] rankDmy;
		printf("del,");
	}
#endif
	printf("d");

	
	///--- output of optimization for weak models by single feature thresholding ---///
	PR_BOOSTWEAKCLASSIFIERPARAMS* dmyModel = PR_NewBoostWeakClassifierParams();
	printf("e\n");

	///--- best weak model ---///
	unsigned int bestFeat;
	float bestThreshold;
	double bestTrainError;
	char bestMode;

	///--- last seleted weak model ---///
	unsigned int lastFeat = nFeat+1;
	char lastMode = 0;

	///--- modifying eta ---///
	if(channelBoost!=13 && channelBoost!=14)	eta=0.0;

#ifdef GET_LAPTIME
	GETLAPTIME* laptimePre = NewGetLapTime();
	GETLAPTIME* laptimeProc = NewGetLapTime();
	GETLAPTIME* laptimePost = NewGetLapTime();
#endif


	///--- U-Boost main ---///
	PR_ENSEMBLENODE* currentModel = ensemble->firstNode;

	for(iModel=0; iModel<nModel; iModel++)
	{
#ifdef GET_LAPTIME
		GetLocaStartTime(laptimePre);
#endif
	//	printf("< w/o cost >\n");
	//	PR_DisplayDataWeightBoost(dataAry, 10);

		// model parameters
		PR_BOOSTWEAKCLASSIFIERPARAMS* weakModel = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentModel->data;

		// initialize data-weight
	//	printf("< cost sensitive >\n");
		PR_CostSensitiveNormalizeDataWeightBoost(dataAry);
	//	PR_DisplayDataWeightBoost(dataAry, 10);

#ifdef MODE_USING_TEMPORAL_FP
		rankDmy = new unsigned long [dataAry->num];
#endif	

#ifdef GET_LAPTIME
		GetLocalEndTime(laptimePre);
		GetLocaStartTime(laptimeProc);
#endif

		// selection of weak-model (single feature thresholding)
		if(weakModel->mode==0)
		{
			bestTrainError = 10.0;

			for(iFeat=0; iFeat<nFeat; iFeat++)
		//	if(iFeat==54)
			{
				// avoid sequentially selection of identical weak-model
				char mode = 0;
				if(iFeat==lastFeat)		mode = -1*lastMode;

				// performance evaluation of each feature
#ifndef MODE_USING_TEMPORAL_FP
				PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,iFeat,dataSorted[iFeat],mode,dmyModel);
#else
				fseek(fpTemp,sizeof(unsigned int)*dataAry->num*iFeat,SEEK_SET);
				fread(rankDmy,sizeof(unsigned long),dataAry->num,fpTemp);
				PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,iFeat,rankDmy,mode,dmyModel);
#endif

				// find weak-model with the best performance
				if(bestTrainError>dmyModel->trainError)
				{
					bestFeat = iFeat;
					bestThreshold = dmyModel->threshold;
					bestTrainError = dmyModel->trainError;
					bestMode = dmyModel->mode;
				}
			}
		}
		// optimization for determined weak-model
		else
		{
			// get index of the feature
			PR_CheckIfFeatTagIsMemberOfFeatTagTable(weakModel->featureTag,caseList->featTagTable,&bestFeat);
			
			// evaluation of the performance
#ifndef MODE_USING_TEMPORAL_FP
			PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,bestFeat,dataSorted[bestFeat],weakModel->mode,dmyModel);
	//		PR_ThresholdingOptimizationByWeightedDataBoostOld(dataAry,bestFeat,dataSorted32[bestFeat],weakModel->mode,dmyModel);
#else
			fseek(fpTemp,sizeof(unsigned int)*dataAry->num*bestFeat,SEEK_SET);
			fread(rankDmy,sizeof(unsigned long),dataAry->num,fpTemp);
			PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,bestFeat,rankDmy,weakModel->mode,dmyModel);
#endif

			// get the performance
			bestThreshold = dmyModel->threshold;
			bestTrainError = dmyModel->trainError;
			bestMode = dmyModel->mode;
		}

#ifdef MODE_USING_TEMPORAL_FP
		delete [] rankDmy;
#endif

#ifdef GET_LAPTIME
		GetLocalEndTime(laptimeProc);
		GetLocaStartTime(laptimePost);
#endif

		///--- copy parameters of the current weak-model ---///
		weakModel->mode = bestMode;
		weakModel->nodeIndex = iModel+1;
		weakModel->threshold = bestThreshold;
		weakModel->trainError = bestTrainError;
		PR_CopyFeatTag(weakModel->featureTag,caseList->featTagTable->tags[bestFeat]);

	//	double wsum = PR_SumDataWeight(dataAry);
	//	printf("dat_weight_sum=%.16e\n", wsum);

		///--- calculate weight of the current weak-model (cost sensitive) ---///
		PR_GetWeightOfWeakClassifier(dataAry,channelBoost,weakModel,eta);

		///--- reflection of current weak-model output on final output ---///
		PR_ReflectWeakClassifierOutputOnLikelihood(dataAry,weakModel);

		///--- update data-weight ---///
		PR_UpdateDataWeight(dataAry,channelBoost,eta);

	//	printf("< updated >\n");
	//	PR_DisplayDataWeightBoost(dataAry, 10);

		///--- std output ---///
		printf("%d[%d], thd%f(%+d), err%e, w%e\n",
			iModel+1,bestFeat,bestThreshold,bestMode,bestTrainError,weakModel->nodeWeight);

		///--- (option) debug output ---///
		/*
		if(bDataWeightLogOutput)
		{
			sprintf(strDmy,"logDataWeight_%d.txt",iModel+1);
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
		*/

		///--- avoid sequentially selection of identical weak-model ---///
		lastFeat = bestFeat;
		lastMode = bestMode;

		///--- shift poiter to next model ---///
		currentModel = currentModel->next;

#ifdef GET_LAPTIME
		GetLocalEndTime(laptimePost);
		ShowLapTime(laptimePre);
		ShowLapTime(laptimeProc);
		ShowLapTime(laptimePost);
#endif
	}


	PR_DeleteCandDataArray(dataAry);
	PR_DeleteBoostWeakClassifierParams(dmyModel);


#ifndef MODE_USING_TEMPORAL_FP
	for(iFeat=0; iFeat<nFeat; iFeat++)	delete [] dataSorted[iFeat];
	delete [] dataSorted;
//	for(iFeat=0; iFeat<nFeat; iFeat++)	delete [] dataSorted32[iFeat];
//	delete [] dataSorted32;
#else
//	fclose(fpTemp);
	_rmtmp();
#endif


	return PR_EOT;
}



// private function

PR_CANDDATAARRAY* 
PR_NewCandDataArray(PR_CASELIST* caseList, double eta)
{
	PR_CANDDATAARRAY* ary = new PR_CANDDATAARRAY;
	ary->cands = new PR_CANDDATA* [caseList->totalNumCand];
//	ary->eta = eta;
	ary->num = caseList->totalNumCand;
//	ary->outputWeakClassifier = new char [caseList->totalNumCand];
//	memset(ary->outputWeakClassifier,0x00,sizeof(char)*caseList->totalNumCand);
	
	unsigned long i=0;
	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			ary->cands[i++] = candData;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}

	return ary;
}

void
PR_DeleteCandDataArray(PR_CANDDATAARRAY* aryCand)
{
	delete [] aryCand->cands;
//	delete [] aryCand->outputWeakClassifier;
	delete aryCand;
}


unsigned long PR_MeasureDataSizeOfCandidates(PR_CASELIST* caseList)
{
	unsigned long numCandidates = caseList->totalNumCand;
	unsigned long numFeatures = caseList->featTagTable->numFeat;
	unsigned long numClass = (unsigned long)caseList->numClass;
	return 	numCandidates*(numFeatures+numClass)*sizeof(double)+numCandidates*numClass*sizeof(unsigned char);
}


unsigned long* PR_GetAscendSortResultOfFeature(PR_CANDDATAARRAY* args, PR_FEATTAG* tag)
{
	PR_CASELIST* caseList = (PR_CASELIST*)((PR_CASEDATA*)args->cands[0]->rootCaseData)->rootCaseList;
	unsigned int idxFeat = PR_ExtractFeatNumberInFeatTagTable(caseList->featTagTable, tag);

	float* featArray = new float [args->num];

	for(unsigned long i=0; i<args->num; i++)
		featArray[i] = args->cands[i]->featValues[idxFeat];

	unsigned long* rankCand = PR_QuickSortAscendingWithRank(featArray,args->num);
	
	delete [] featArray;
	return rankCand;
}

unsigned int* PR_GetAscendSortResultOfFeatureOld(PR_CANDDATAARRAY* args, PR_FEATTAG* tag)
{
	PR_CASELIST* caseList = (PR_CASELIST*)((PR_CASEDATA*)args->cands[0]->rootCaseData)->rootCaseList;
	unsigned int idxFeat = PR_ExtractFeatNumberInFeatTagTable(caseList->featTagTable, tag);

	float* featArray = new float [args->num];

	for(unsigned long i=0; i<args->num; i++)
		featArray[i] = args->cands[i]->featValues[idxFeat];

	unsigned int* rankCand = PR_QuickSortAscendingWithRankOld(featArray,args->num);
	
	delete [] featArray;
	return rankCand;
}


int PR_IncludeCostInDataWeightForBoost(PR_CANDDATAARRAY* ary)
{
	PR_BOOSTOPTION* boostOption;
	for(unsigned long i=0; i<ary->num; i++)
	{
		boostOption = (PR_BOOSTOPTION*)(ary->cands[i]->optionParam);
		if(boostOption->cost!=1.0)	boostOption->dataWeight *= boostOption->cost;
	}
	return PR_EOT;
}


double PR_NormalizeDataWeightForBoost(PR_CANDDATAARRAY* ary)
{
	unsigned long nCand = ary->num;
	float sumWeight=0.0f;

	for(unsigned long i=0; i<nCand; i++)
		sumWeight += ((PR_BOOSTOPTION*)(ary->cands[i]->optionParam))->dataWeight;

	if(sumWeight==1.0)	return sumWeight;

	for(unsigned long i=0; i<nCand; i++)
		((PR_BOOSTOPTION*)(ary->cands[i]->optionParam))->dataWeight /= (float)sumWeight;

	return sumWeight;
}

int PR_CostSensitiveNormalizeDataWeightBoost(PR_CANDDATAARRAY* ary)
{
	unsigned long i, nCand = ary->num;
	PR_BOOSTOPTION* boostOption;
	float sumWeight=0.0f;

	for(i=0; i<nCand; i++)
	{
		boostOption = (PR_BOOSTOPTION*)(ary->cands[i]->optionParam);
		boostOption->dataWeight *= boostOption->cost;
		sumWeight += boostOption->dataWeight;

//		if(i%5==0 && i<100)	printf("weight'[%d]=%e\n",i,boostOption->dataWeight);
	}

	if(sumWeight==1.0)	return PR_EOT;

//	FILE* fpo_debug = fopen("debug_weight.csv","w");
//	fprintf(fpo_debug,"sum=%e\n",sumWeight);

	for(i=0; i<nCand; i++)
	{
		boostOption = (PR_BOOSTOPTION*)(ary->cands[i]->optionParam);
		boostOption->dataWeight /= sumWeight;

//		fprintf(fpo_debug,"cost,\t%e,\tweight,\t%e,\n",boostOption->cost,boostOption->dataWeight);
//		if(i%5==0 && i<100)	printf("weight''[%d]=%e\n",i,boostOption->dataWeight);
	}

//	fclose(fpo_debug);
//	fprintf(stderr,"output debug_weight.csv\n");
//	getchar();

	return PR_EOT;
}

int PR_ThresholdingOptimizationByWeightedDataBoost(
	PR_CANDDATAARRAY* dataAry, unsigned int iFeature, unsigned long* ascendSorted, char mode, 
	PR_BOOSTWEAKCLASSIFIERPARAMS* output)
{
	PR_CANDDATA** cands = dataAry->cands;
	unsigned long numCands = dataAry->num;

	// get index of target feat
	PR_CASELIST* caseList = (PR_CASELIST*)((PR_CASEDATA*)cands[0]->rootCaseData)->rootCaseList;
	output->featureTag->group = caseList->featTagTable->tags[iFeature]->group;
	output->featureTag->element = caseList->featTagTable->tags[iFeature]->element;

	unsigned long i,j,numCands1=numCands-1;

	float* featAry = new float [numCands];
	double* weightAry = new double [numCands];

	double weightDmy, sumWeightTP=0.0, sumWeightFP=0.0;
	double minweight=10.0;

	for(i=0; i<numCands; i++)
	{
		j = ascendSorted[i];
		featAry[i] = cands[j]->featValues[iFeature];
		weightAry[i] = ((PR_BOOSTOPTION*)(cands[j]->optionParam))->dataWeight;

		if(minweight>weightAry[i])
		{
			minweight = weightAry[i];
		}
		
		if(cands[j]->classLabel)
		{
			sumWeightTP += weightAry[i];
			weightAry[i] *= -1.0;
		}
		else
		{
			sumWeightFP += weightAry[i];
		}
	}

	// high-pass
	double dmySumWgt=sumWeightTP;

	char optMode;
	float feat1, feat2, optThd;
	double optErr=2.0;

	if(mode==0 || mode==+1)
	for(i=numCands1; i>0; i--)
	{
		weightDmy = weightAry[i];
		feat1 = featAry[i];
		feat2 = featAry[i-1];
		dmySumWgt += weightDmy;
		
		if(feat1!=feat2 && optErr >= dmySumWgt)
		{
			optMode = +1;
			optThd = (feat1+feat2)*0.5f;
			optErr = dmySumWgt;

	//		if(iFeature==0)
	//			fprintf(stderr,"icand%d/%d; mode%d, thd%e, err%e (%e)\n",i,numCands1,optMode,optThd,optErr,weightAry[i]);
		}
	}

	// low-pass
	dmySumWgt=sumWeightTP;

	if(mode==0 || mode==-1)
	for(i=0; i<numCands1; i++)
	{
		weightDmy = weightAry[i];
		feat1 = featAry[i];
		feat2 = featAry[i+1];
		dmySumWgt += weightDmy;

		if(feat1!=feat2	&& optErr >= dmySumWgt)
		{
			optMode = -1;
			optThd = (feat1+feat2)*0.5f;
			optErr = dmySumWgt;

	//		if(iFeature==0)
	//			fprintf(stderr,"icand%d/%d; mode%d, thd%e, err%e (%e)\n",i,numCands1,optMode,optThd,optErr,weightAry[i]);
		}
	}

	if(abs(optErr)<minweight/10.0)	optErr = minweight/10.0;

	delete [] featAry;
	delete [] weightAry;

	output->mode = optMode;
	output->threshold = optThd;
	output->trainError = optErr;

	return PR_EOT;
}
/*
int PR_ThresholdingOptimizationByWeightedDataBoost(
	PR_CANDDATAARRAY* dataAry, unsigned int iFeature, unsigned long* ascendSorted, char mode, 
	PR_BOOSTWEAKCLASSIFIERPARAMS* output)
{
//	PR_BOOSTWEAKCLASSIFIERPARAMS* output = PR_NewBoostWeakClassifierParams();

	PR_CANDDATA** cands = dataAry->cands;
	unsigned long numCands = dataAry->num;

	// get index of target feat
	PR_CASELIST* caseList = (PR_CASELIST*)((PR_CASEDATA*)cands[0]->rootCaseData)->rootCaseList;
	output->featureTag->group = caseList->featTagTable->tags[iFeature]->group;
	output->featureTag->element = caseList->featTagTable->tags[iFeature]->element;

//	printf("[%04x %04x](sm%+d) era..",param->featureTag->group,param->featureTag->element,modeEvaluation);
//	printf("\n");

//	unsigned int iFeat = iFeature;
//	output.mode = 0;
//	output->trainError = 2.0;

	unsigned long i,j,numCands1=numCands-1;
//	unsigned char clsCand;

	float* featAry = new float [numCands];
	double* weightAry = new double [numCands];
//	unsigned char* clsAry = new unsigned char [numCands];

	double weightDmy, sumWeightTP=0.0, sumWeightFP=0.0;

	for(i=0; i<numCands; i++)
	{
		j = ascendSorted[i];
		featAry[i] = cands[j]->featValues[iFeature];
		weightAry[i] = ((PR_BOOSTOPTION*)(cands[j]->optionParam))->dataWeight;
//		clsAry[i] = cands[j]->classLabel;

//		weightDmy = ((PR_BOOSTOPTION*)(cands[i]->optionParam))->dataWeight;
//		if(dataAry->cands[i]->classLabel)	sumWeightTP += weightDmy;
//		else								sumWeightFP += weightDmy;

		if(cands[j]->classLabel)
		{
			sumWeightTP += weightAry[i];
			weightAry[i] *= -1.0;
		}
		else
		{
			sumWeightFP += weightAry[i];
		}
	}
	
//	unsigned long iCand, iCandNext, iCandPrev;
//	double dmySumWgtFN=10.0, dmySumWgtFP=10.0;

	// high-pass
//	double dmySumWgtFN=sumWeightTP;
//	double dmySumWgtFP=0.0;
	double dmySumWgt=sumWeightTP;

	char optMode;
	float feat1, feat2, optThd;
	double optErr=2.0;

	if(mode==0 || mode==+1)
	for(i=numCands1; i>0; i--)
	{
	//	iCand = ascendSorted[i];
	//	iCandPrev = ascendSorted[i-1];
	//	clsCand = cands[iCand]->classLabel;
	//	weightDmy = ((PR_BOOSTOPTION*)(cands[iCand]->optionParam))->dataWeight;
//		clsCand = clsAry[i];
		weightDmy = weightAry[i];
		feat1 = featAry[i];
		feat2 = featAry[i-1];
		
	//	if(clsCand)		dmySumWgtFN -= weightDmy;
	//	else			dmySumWgtFP += weightDmy;
	//	if(clsCand)		dmySumWgt -= weightDmy;
	//	else			dmySumWgt += weightDmy;
		dmySumWgt += weightDmy;

	//	if(cands[iCand]->featValues[iFeature]!=cands[iCandPrev]->featValues[iFeature] && optErr > dmySumWgtFP+dmySumWgtFN)
		if(feat1!=feat2 && optErr > dmySumWgt)
		{
			optMode = +1;
	//		optThd = (dataAry->cands[iCand]->featValues[iFeature]+dataAry->cands[iCandPrev]->featValues[iFeature])*0.5f;
			optThd = (feat1+feat2)*0.5f;
	//		optErr = dmySumWgtFP+dmySumWgtFN;
			optErr = dmySumWgt;

			printf("%d; err=%e, thd=(%e+%e)/2=%e",i,)
		}
	}


	// low-pass
//	printf("Low\n");
//	dmySumWgtFN=sumWeightTP;
//	dmySumWgtFP=0.0;
	dmySumWgt=sumWeightTP;

	if(mode==0 || mode==-1)
	for(i=0; i<numCands1; i++)
	{
	//	iCand = ascendSorted[i];
	//	iCandNext = ascendSorted[i+1];
	//	clsCand = cands[iCand]->classLabel;
	//	weightDmy = ((PR_BOOSTOPTION*)(cands[iCand]->optionParam))->dataWeight;
//		clsCand = clsAry[i];
		weightDmy = weightAry[i];
		feat1 = featAry[i];
		feat2 = featAry[i+1];
		
	//	if(clsCand)		dmySumWgtFN -= weightDmy;
	//	else			dmySumWgtFP += weightDmy;
	//	if(clsCand)		dmySumWgt -= weightDmy;
	//	else			dmySumWgt += weightDmy;
		dmySumWgt += weightDmy;

	//	if(cands[iCand]->featValues[iFeature]!=cands[iCandNext]->featValues[iFeature] && optErr > dmySumWgtFP+dmySumWgtFN)
		if(feat1!=feat2	&& optErr > dmySumWgt)
		{
			optMode = -1;
	//		optThd = (cands[iCand]->featValues[iFeature]+cands[iCandNext]->featValues[iFeature])*0.5f;
			optThd = (feat1+feat2)*0.5f;
	//		optErr = dmySumWgtFP+dmySumWgtFN;
			optErr = dmySumWgt;
		}
	}

	delete [] featAry;
	delete [] weightAry;
//	delete [] clsAry;

	output->mode = optMode;
	output->threshold = optThd;
	output->trainError = optErr;

	return PR_EOT;
}
*/

int PR_ThresholdingOptimizationByWeightedDataBoostOld(
	PR_CANDDATAARRAY* dataAry, unsigned int iFeature, unsigned int* ascendSorted, char mode, 
	PR_BOOSTWEAKCLASSIFIERPARAMS* output)
{
//	PR_BOOSTWEAKCLASSIFIERPARAMS* output = PR_NewBoostWeakClassifierParams();

	unsigned int numCands = (unsigned int)dataAry->num;

	// get index of target feat
	PR_CASELIST* caseList = (PR_CASELIST*)((PR_CASEDATA*)dataAry->cands[0]->rootCaseData)->rootCaseList;

	output->featureTag->group = caseList->featTagTable->tags[iFeature]->group;
	output->featureTag->element = caseList->featTagTable->tags[iFeature]->element;

//	printf("[%04x %04x](sm%+d) era..",param->featureTag->group,param->featureTag->element,modeEvaluation);
//	printf("\n");

//	unsigned int iFeat = iFeature;
//	output.mode = 0;
	output->trainError = 2.0;

	unsigned int i;
	unsigned char clsCand;

	double weightDmy, sumWeightTP=0.0, sumWeightFP=0.0;
	for(i=0; i<numCands; i++)
	{
		weightDmy = ((PR_BOOSTOPTION*)(dataAry->cands[i]->optionParam))->dataWeight;
		if(dataAry->cands[i]->classLabel)	sumWeightTP += weightDmy;
		else								sumWeightFP += weightDmy;
	}
	
	unsigned int iCand, iCandNext, iCandPrev;
	double dmySumWgtFN=10.0, dmySumWgtFP=10.0;

	// high-pass
	dmySumWgtFN=sumWeightTP;
	dmySumWgtFP=0.0;

	if(mode==0 || mode==+1)
	for(i=numCands-1; i>0; i--)
	{
		iCand = ascendSorted[i];
		iCandPrev = ascendSorted[i-1];
		clsCand = dataAry->cands[iCand]->classLabel;
		weightDmy = ((PR_BOOSTOPTION*)(dataAry->cands[iCand]->optionParam))->dataWeight;
		
		if(clsCand)		dmySumWgtFN -= weightDmy;
		else			dmySumWgtFP += weightDmy;

		if(dataAry->cands[iCand]->featValues[iFeature]!=dataAry->cands[iCandPrev]->featValues[iFeature]
			&& output->trainError > dmySumWgtFP+dmySumWgtFN)
		{
			output->mode = +1;
			output->threshold = (dataAry->cands[iCand]->featValues[iFeature]+dataAry->cands[iCandPrev]->featValues[iFeature])*0.5f;
			output->trainError = dmySumWgtFP+dmySumWgtFN;
		}
	}


	// low-pass
//	printf("Low\n");
	dmySumWgtFN=sumWeightTP;
	dmySumWgtFP=0.0;

	if(mode==0 || mode==-1)
	for(i=0; i<numCands-1; i++)
	{
		iCand = ascendSorted[i];
		iCandNext = ascendSorted[i+1];
		clsCand = dataAry->cands[iCand]->classLabel;
		weightDmy = ((PR_BOOSTOPTION*)(dataAry->cands[iCand]->optionParam))->dataWeight;
		
		if(clsCand)		dmySumWgtFN -= weightDmy;
		else			dmySumWgtFP += weightDmy;

		if(dataAry->cands[iCand]->featValues[iFeature]!=dataAry->cands[iCandNext]->featValues[iFeature]
			&& output->trainError > dmySumWgtFP+dmySumWgtFN)
		{
			output->mode = -1;
			output->threshold = (dataAry->cands[iCand]->featValues[iFeature]+dataAry->cands[iCandNext]->featValues[iFeature])*0.5f;
			output->trainError = dmySumWgtFP+dmySumWgtFN;
		}
	}

	return PR_EOT;
}


int 
PR_GetWeightOfWeakClassifier(
	PR_CANDDATAARRAY* aryCand, int channelBoost, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, double eta//,double Zt
){
	unsigned int num = aryCand->num;
//	double eta = aryCand->eta;
	double error = weakClassifier->trainError;
//	double sqrtError, Kt, etaKt, alpha;
	double alpha;
	int numIteration=10000;

//	printf("\n   eta%.4e,err%.3e, ",eta,error);

	if(channelBoost!=PR_CHANNEL_ADABOOST)
		PR_CalculateOutputOfWeakClassifier(aryCand, weakClassifier);

	PR_CALCLOSSBOOST* params = new PR_CALCLOSSBOOST;
	params->nThreads = 1;
	params->dataAry = aryCand;
	params->eta = eta;
	if(channelBoost!=PR_CHANNEL_ADABOOST)
		params->outWeakModel = PR_CalculateOutputOfWeakClassifier(aryCand, weakClassifier);
	else
		params->outWeakModel = NULL;

	alpha = 0.5*log((1.0-error)/error);
	double min=DBL_MIN, max=alpha*2.0;

	switch(channelBoost)
	{
	case PR_CHANNEL_ADABOOST:
	//	weakClassifier->nodeWeight = 0.5*log((1.0-error)/error);
		weakClassifier->nodeWeight = alpha;
		break;
	case PR_CHANNEL_MADABOOST:
//		printf("    Fmada(%.3e)=%le..",alpha,PR_OutputSumOfLossMadaBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossMadaBoost,params,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Fmada(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossMadaBoost(weakClassifier->nodeWeight,aryCand));
		break;
	case PR_CHANNEL_LOGITBOOST:
//		printf("    Flog(%.3e)=%le..",alpha,PR_OutputSumOfLossLogitBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossLogitBoost,params,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Flog(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossLogitBoost(weakClassifier->nodeWeight,aryCand));
		break;
	case PR_CHANNEL_ETABOOST:
//		printf("    Feta(%.3e)=%le..",alpha,PR_OutputSumOfLossEtaBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossEtaBoost,params,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Feta(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossEtaBoost(weakClassifier->nodeWeight,aryCand));
		break;
	case PR_CHANNEL_ROBUSTETABOOST:
//		printf("    Freta(%.3e)=%le..",alpha,PR_OutputSumOfLossMostBRobustEtaBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossMostBRobustEtaBoost,params,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Freta(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossMostBRobustEtaBoost(weakClassifier->nodeWeight,aryCand));
		break;
	default:
		fprintf(stderr,"error boostChannel=%d\n",channelBoost);
		return PR_AVEND;
	}

	if(params->outWeakModel!=NULL)	delete [] params->outWeakModel;
	delete params;
	
	return PR_EOT;
}



int
PR_ReflectWeakClassifierOutputOnLikelihood(
	PR_CANDDATAARRAY* args, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier)
{
	PR_CANDDATA** cands = args->cands;
	unsigned int i, num = args->num;

	float threshold = weakClassifier->threshold;
	double nodeWeight = weakClassifier->nodeWeight;
//	char mode = weakClassifier->mode;

	PR_CASELIST* root = (PR_CASELIST*)((PR_CASEDATA*)(cands[0]->rootCaseData))->rootCaseList;
	unsigned int targetFeat = PR_ExtractFeatNumberInFeatTagTable(root->featTagTable, weakClassifier->featureTag);

	if(weakClassifier->nodeIndex==1)
	{
		for(i=0; i<num; i++)	cands[i]->likelihood[1]=0.0;
	}

	if(weakClassifier->mode==-1)
	{
		for(i=0; i<num; i++)
		{
			float fValue = cands[i]->featValues[targetFeat];
			double* lhod = cands[i]->likelihood;
			if(fValue<=threshold)	lhod[1] += nodeWeight;
			else					lhod[1] -= nodeWeight;
//			lhod[0] = -1.0*lhod[1];
//			if(i%5==0 && i<50)	printf("out[%d]=%e\n",i,lhod[1]);
		}
	}
	else
	{
		for(i=0; i<num; i++)
		{
			float fValue = cands[i]->featValues[targetFeat];
			double* lhod = cands[i]->likelihood;
			if(fValue>=threshold)	lhod[1] += nodeWeight;
			else					lhod[1] -= nodeWeight;
//			lhod[0] = -1.0*lhod[1];
//			if(i%5==0 && i<50)	printf("out[%d]=%e\n",i,lhod[1]);
		}
	}

	return PR_EOT;
}


double PR_SumDataWeight(PR_CANDDATAARRAY* args)
{
	PR_CANDDATA** cands = args->cands;
	double sum = 0.0;
	for(unsigned int i=0; i<args->num; i++)
	{
		PR_BOOSTOPTION* option = (PR_BOOSTOPTION*)(cands[i]->optionParam);
		sum += option->dataWeight;
	}
	return sum;
}


int 
PR_UpdateDataWeight(
	PR_CANDDATAARRAY* args, int channelBoost, double eta)
{
	PR_CANDDATA** cands = args->cands;
	unsigned int num = args->num;
//	double eta = args->eta;

	if((channelBoost==13 || channelBoost==14) && (eta<0.0 || eta>=1.0))
	{
		fprintf(stderr,"error eta=%e <-> 0.0<=eta<1.0\n",eta);
		return PR_AVEND;
	}

	double dClass, z, z2Exp;
	PR_BOOSTOPTION* option;

	for(unsigned int i=0; i<num; i++)
	{
		if(cands[i]->classLabel)	dClass = 1.0;
		else						dClass = -1.0;
		z = -1.0*cands[i]->likelihood[1]*dClass;
		option = (PR_BOOSTOPTION*)(cands[i]->optionParam);

		switch(channelBoost)
		{
		case 10: // PR_CHANNEL_ADABOOST:
			option->dataWeight = (float)exp(z);
			break;
		case 11: // PR_CHANNEL_MADABOOST:
			if(z>=0.0)		option->dataWeight = 1.0f;
			else			option->dataWeight = (float)exp(2.0*z);
			break;
		case 12: // PR_CHANNEL_LOGITBOOST:
			z2Exp = exp(2.0*z);
			option->dataWeight = (float)(2.0*z2Exp/(1.0+z2Exp));
			break;
		case 13: // PR_CHANNEL_ETABOOST:
			option->dataWeight = (float)((1.0-eta)*exp(z)+eta);
			break;
		case 14: // PR_CHANNEL_ROBUSTETABOOST:
			if(z>=0.0)		option->dataWeight = 1.0f;
			else			option->dataWeight = (float)(((1.0-eta)*exp(z)+eta)/((1.0-eta)*exp(-z)+eta));
			break;
		default:
			fprintf(stderr,"error boostChannel=%d\n",channelBoost);
			return PR_AVEND;
		}

//		if(i%5==0 && i<100)	printf("[%d] l%e -> w%e\n",i,cands[i]->likelihood[1],option->dataWeight);
	}

	return PR_EOT;
}


double prim_CalcLossAda(double z)
{
	return exp(z);
}

double prim_CalcLossMada(double z)
{
	if(z>=0.0)	return z;
	else		return (0.5*exp(2.0*z)-0.5);
}

double prim_CalcLossLogit(double z)
{
	return log(1.0+exp(2.0*z));
}

double prim_CalcLossEta(double z , double eta)
{
	return (1.0-eta)*exp(z)+eta*z;
}

double prim_CalcLossRobustEta(double z , double eta, double etaS, double eta2p, double rLog)
{
	double zExpPlus=(exp(z)-1.0)*eta;
	if(z>=0.0)	return z;
	else		return (etaS*zExpPlus+rLog*log(1.0+zExpPlus))/eta2p;
}

double
basis_CalcLossSumBoost(
	double wCurrent, unsigned int iTop, unsigned int iEnd, PR_CANDDATA** cands, char* outWeakModel, double eta, int boostType)
{
	double loss, sum=0.0;
	double etaS=1.0-eta, eta2p=eta*eta, rLog=2.0*eta-1.0;
	
	for(unsigned int i=iTop; i<=iEnd; i++)
	{
		double label = -1.0;
		if(cands[i]->classLabel)	label = 1.0;
		double oPrev = cands[i]->likelihood[1];
		double oCurrent = (double)outWeakModel[i];
		double z = -label*(oPrev+wCurrent*oCurrent);

		if(boostType==PR_CHANNEL_ADABOOST)				loss = prim_CalcLossAda(z);
		else if(boostType==PR_CHANNEL_MADABOOST)		loss = prim_CalcLossMada(z);
		else if(boostType==PR_CHANNEL_LOGITBOOST)		loss = prim_CalcLossLogit(z);
		else if(boostType==PR_CHANNEL_ETABOOST)			loss = prim_CalcLossEta(z, eta);
		else if(boostType==PR_CHANNEL_ROBUSTETABOOST)	loss = prim_CalcLossRobustEta(z, eta, etaS, eta2p, rLog);

		double cost = ((PR_BOOSTOPTION*)cands[i]->optionParam)->cost;
		sum += loss*cost;
	}
	return sum;
}

double
PublicCalcLossSumBoost(double alpha, void* option, int boostType)
{
	PR_CANDDATA** cands = ((PR_CALCLOSSBOOST*)option)->dataAry->cands;
	unsigned int num = ((PR_CALCLOSSBOOST*)option)->dataAry->num;
	char* outWeakModel = ((PR_CALCLOSSBOOST*)option)->outWeakModel;
	double eta = ((PR_CALCLOSSBOOST*)option)->eta;

	return basis_CalcLossSumBoost(alpha, 0, num-1, cands, outWeakModel, eta, boostType);
	// return basis_CalcLossSumBoost()/(double)num;
}

double PublicCalcLossSumBoostMT(double alpha, void* option, int boostType);		// from prUBoostMT.cpp

double
PR_OutputSumOfLossAdaBoost(double alpha, void* option)
{
	int nThreads = ((PR_CALCLOSSBOOST*)option)->nThreads;
	if(nThreads==1)	return PublicCalcLossSumBoost(alpha, option, PR_CHANNEL_ADABOOST);
	else			return PublicCalcLossSumBoostMT(alpha, option, PR_CHANNEL_ADABOOST);
}

double
PR_OutputSumOfLossMadaBoost(double alpha, void* option)
{
	int nThreads = ((PR_CALCLOSSBOOST*)option)->nThreads;
	if(nThreads==1)	return PublicCalcLossSumBoost(alpha, option, PR_CHANNEL_MADABOOST);
	else			return PublicCalcLossSumBoostMT(alpha, option, PR_CHANNEL_MADABOOST);
}

double
PR_OutputSumOfLossLogitBoost(double alpha, void* option)
{
	int nThreads = ((PR_CALCLOSSBOOST*)option)->nThreads;
	if(nThreads==1)	return PublicCalcLossSumBoost(alpha, option, PR_CHANNEL_LOGITBOOST);
	else			return PublicCalcLossSumBoostMT(alpha, option, PR_CHANNEL_LOGITBOOST);
}

double
PR_OutputSumOfLossEtaBoost(double alpha, void* option)
{
	int nThreads = ((PR_CALCLOSSBOOST*)option)->nThreads;
	if(nThreads==1)	return PublicCalcLossSumBoost(alpha, option, PR_CHANNEL_ETABOOST);
	else			return PublicCalcLossSumBoostMT(alpha, option, PR_CHANNEL_ETABOOST);
}

double
PR_OutputSumOfLossMostBRobustEtaBoost(double alpha, void* option)
{
	int nThreads = ((PR_CALCLOSSBOOST*)option)->nThreads;
	if(nThreads==1)	return PublicCalcLossSumBoost(alpha, option, PR_CHANNEL_ROBUSTETABOOST);
	else			return PublicCalcLossSumBoostMT(alpha, option, PR_CHANNEL_ROBUSTETABOOST);
}

char* 
PR_CalculateOutputOfWeakClassifier(PR_CANDDATAARRAY* ary, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier)
{
	PR_FEATTAGTABLE* table = ((PR_CASELIST*)((PR_CASEDATA*)ary->cands[0]->rootCaseData)->rootCaseList)->featTagTable;
	unsigned int idx = PR_ExtractFeatNumberInFeatTagTable(table, weakClassifier->featureTag);

	char* output = new char [ary->num];

	for(unsigned long i=0; i<ary->num; i++)
	{
		double value = ary->cands[i]->featValues[idx];
		double threshold = weakClassifier->threshold;
		if(weakClassifier->mode==-1)
		{
			if(value<=threshold)	output[i]=1;
			else					output[i]=-1;
		}
		else
		{
			if(value>=threshold)	output[i]=1;
			else					output[i]=-1;
		}
	}

	return output;
}


float
PR_BrentMethodGetArgMinForFunction_Float32(
	float ax, float bx, float cx, 
	float (*function)(double, void*), void* param, 
	int numIteration,
	float *xmin
){
	float tol=sqrt(FLT_MIN);
//	double tol=sqrt(DBL_MIN);

	int iter;
	float a,b,d,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
	float e=0.0;

	a=(ax < cx ? ax : cx);
	b=(ax > cx ? ax : cx);
	x=w=v=bx;
	fw=fv=fx=(*function)(x,param);

	float tempMin=fw;

	for(iter=1; iter<=numIteration; iter++)
	{
		xm=0.5*(a+b);
		tol2=2.0*(tol1=tol*fabs(x)+ZEPS);

		if(tempMin>x)
		{
			tempMin=x;
		}

		if(fabs(x-xm)<=(tol2-0.5*(b-a)))
		{
			*xmin=x;
			return fx;
		}

		if(fabs(e)>tol1)
		{
			r=(x-w)*(fx-fv);
			q=(x-v)*(fx-fw);
			p=(x-v)*q-(x-w)*r;
			q=2.0*(q-r);

			if(q>0.0)	p=-p;

			q=fabs(q);
			etemp=e;
			e=d;

			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
			{
				d=CGOLD*(e=(x >= xm ? a-x : b-x));
			}
			else 
			{
				d=p/q;	u=x+d;
				if(u-a<tol2 || b-u<tol2)	d=SIGN(tol1,xm-x);
			}
		}
		else
		{
			d=CGOLD*(e=(x >= xm ? a-x : b-x));
		}

		u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
		fu=(*function)((double)u,param);

		if(fu<=fx)
		{
			if(u>=x)	a=x;
			else		b=x;
			SHFT(v,w,x,u)
			SHFT(fv,fw,fx,fu)
		} 
		else
		{
			if(u<x)		a=u;
			else		b=u;
			if(fu<=fw || w==x)
			{
				v=w;	w=u;	fv=fw;		fw=fu;
			}
			else if(fu<=fv || v==x || v==w)
			{
				v=u;	fv=fu;
			}
		}
	}

	fprintf(stderr,"Too many iterations in BrentMethod..\n");
	*xmin=tempMin;
	return fx;

//	getchar();
//	exit(1);
//	f64_nrerror("Too many iterations in brent");
}

double
PR_BrentMethodGetArgMinForFunction(
	double ax, double bx, double cx, 
	double (*function)(double, void*), void* param, 
	int numIteration,
	double *xmin
){
//	double tol=sqrt(FLT_MIN);
	double tol=sqrt(DBL_MIN);

	int iter;
	double a,b,d,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
	double e=0.0;

	a=(ax < cx ? ax : cx);
	b=(ax > cx ? ax : cx);
	x=w=v=bx;
	fw=fv=fx=(*function)(x,param);

	double tempMin=fw;

	for(iter=1; iter<=numIteration; iter++)
	{
		xm=0.5*(a+b);
		tol2=2.0*(tol1=tol*fabs(x)+ZEPS);

		if(tempMin>x)
		{
			tempMin=x;
		}

		if(fabs(x-xm)<=(tol2-0.5*(b-a)))
		{
			*xmin=x;
			return fx;
		}

		if(fabs(e)>tol1)
		{
			r=(x-w)*(fx-fv);
			q=(x-v)*(fx-fw);
			p=(x-v)*q-(x-w)*r;
			q=2.0*(q-r);

			if(q>0.0)	p=-p;

			q=fabs(q);
			etemp=e;
			e=d;

			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
			{
				d=CGOLD*(e=(x >= xm ? a-x : b-x));
			}
			else 
			{
				d=p/q;	u=x+d;
				if(u-a<tol2 || b-u<tol2)	d=SIGN(tol1,xm-x);
			}
		}
		else
		{
			d=CGOLD*(e=(x >= xm ? a-x : b-x));
		}

		u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
		fu=(*function)(u,param);

		if(fu<=fx)
		{
			if(u>=x)	a=x;
			else		b=x;
			SHFT(v,w,x,u)
			SHFT(fv,fw,fx,fu)
		} 
		else
		{
			if(u<x)		a=u;
			else		b=u;
			if(fu<=fw || w==x)
			{
				v=w;	w=u;	fv=fw;		fw=fu;
			}
			else if(fu<=fv || v==x || v==w)
			{
				v=u;	fv=fu;
			}
		}
	}

	fprintf(stderr,"Too many iterations in BrentMethod..\n");
	*xmin=tempMin;
	return fx;

//	getchar();
//	exit(1);
//	f64_nrerror("Too many iterations in brent");
}
#undef ITMAX
#undef CGOLD
#undef ZEPS
#undef SHFT
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */



