
#include <algorithm>
#include <functional>
#include <iostream>
#include <pthread.h>
#include "./prUBoost.private.h"
#include "../LibPR.h"

//#define GET_LAPTIME



int PR_UBoostEssentialFuncMT_LowMemory(
PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int channelBoost, double eta, int nThreads);

PR_ENSEMBLE* PR_UBoostMT_LowMemory(
	PR_CASELIST* caselist, int chBoost, unsigned int numClassifier, int chAddCost, double eta, int numThreads);

int PR_GetWeightOfWeakClassifierMT(PR_CANDDATAARRAY* aryCand, int boostType, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, double eta, int nThreads);


PR_ENSEMBLE* PR_SimpleAdaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_ADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0,numThreads);
}

PR_ENSEMBLE* PR_SimpleMadaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_MADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0,numThreads);
}

PR_ENSEMBLE* PR_SimpleLogitBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_LOGITBOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0,numThreads);
}

PR_ENSEMBLE* PR_SimpleEtaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_ETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,eta,numThreads);
}

PR_ENSEMBLE* PR_SimpleRobustEtaBoostMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_ROBUSTETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,eta,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveAdaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_ADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveMadaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_MADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveLogitBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_LOGITBOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveEtaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_ETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,eta,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveRobustEtaBoostForImbalancedDatasetMT_LowMemory(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT_LowMemory(caseList,PR_CHANNEL_ROBUSTETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,eta,numThreads);
}


PR_ENSEMBLE* PR_UBoostMT_LowMemory(
	PR_CASELIST* caselist, int chBoost, unsigned int numClassifier, int chAddCost, double eta, int numThreads)
{
	int rawCand=0;

	if(caselist->childNodeFirstCase->childNodeFirstCand->optionParam==NULL) {
		// データ重み格納メモリ領域ない場合、確保
		PR_NewAddBoostOptionToAllCands(caselist);
		rawCand=1;
	}

	// 事前コストの初期化
	PR_InitializeCostBoost(caselist,chAddCost);

	// アンサンブルのメモリ領域宣言
	PR_ENSEMBLE* ensemble = PR_NewBoostEnsemble(numClassifier);

	// 識別器アンサンブルの学習
	PR_UBoostEssentialFuncMT_LowMemory(caselist, ensemble, chBoost, eta, numThreads);

	// データ重みメモリ開放
	if(rawCand)		PR_DeleteBoostOptionInAllCands(caselist);

	return ensemble;
}


typedef struct _common_UBoostThread_2_
{
	PR_CANDDATAARRAY* dataAry;
	unsigned int** dataSorted;
	unsigned int lastFeat;
	char lastMode;
}
COMMONUBOOSTTHREAS2;

typedef struct _UBoostThread_2_
{
	COMMONUBOOSTTHREAS2* common;
	unsigned int forTopFeat, forEndFeat;
	float localBestErr;
	unsigned int localBestFeat;
	char localBestMode;
	float localBestThreshold;
}
UBOOSTTHREAD2;

UBOOSTTHREAD2* NewUBoostThread2(void)
{
	UBOOSTTHREAD2* output = new UBOOSTTHREAD2;
	output->common = NULL;
	output->forEndFeat = output->forTopFeat = 0;
	output->localBestErr = 10.0f;
	output->localBestFeat = INT_MAX;
	output->localBestMode = 0;
	output->localBestThreshold = 0.0f;
	return output;
}

void DeleteUBoostThread2(UBOOSTTHREAD2* input)
{
	delete input;
}

int PR_CountNumCandsForCaseList(PR_CASELIST* caseList);

float* ExtractSingleFeatureArray(PR_CASELIST* caselist, PR_FEATTAG* tag)
{
	unsigned int fidx = PR_ExtractFeatNumberInFeatTagTable(caselist->featTagTable, tag);
	unsigned int num = caselist->totalNumCand;
	float* out_array = new float [num];
	PR_CASEDATA* casedata = caselist->childNodeFirstCase;
	unsigned int count = 0;

	for(unsigned int j=0; j<caselist->numCase; j++) {
		PR_CANDDATA* cand = casedata->childNodeFirstCand;
		for(unsigned int i=0; i<casedata->numCand; i++) {
			out_array[count++] = cand->featValues[fidx];
			cand = cand->nodeNextCand;
		}
		casedata = casedata->nodeNextCase;
	}

	return out_array;
}

int PR_ThresholdingOptimizationByWeightedDataBoost2(
	PR_CANDDATAARRAY* dataAry, unsigned int iFeature, unsigned int* ascendSorted, char mode, 
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

		if(minweight>weightAry[i])	minweight = weightAry[i];
		
		if(cands[j]->classLabel) {
			sumWeightTP += weightAry[i];
			weightAry[i] *= -1.0;
		} else {
			sumWeightFP += weightAry[i];
		}
	}

	// high-pass
	double dmySumWgt=sumWeightTP;

	char optMode;
	float feat1, feat2, optThd;
	double optErr=2.0;

	if(mode==0 || mode==+1) for(i=numCands1; i>0; i--) {
		weightDmy = weightAry[i];
		feat1 = featAry[i];
		feat2 = featAry[i-1];
		dmySumWgt += weightDmy;
		if(feat1!=feat2 && optErr >= dmySumWgt) {
			optMode = +1;
			optThd = (feat1+feat2)*0.5f;
			optErr = dmySumWgt;
		}
	}

	// low-pass
	dmySumWgt=sumWeightTP;

	if(mode==0 || mode==-1) for(i=0; i<numCands1; i++) {
		weightDmy = weightAry[i];
		feat1 = featAry[i];
		feat2 = featAry[i+1];
		dmySumWgt += weightDmy;
		if(feat1!=feat2	&& optErr >= dmySumWgt) {
			optMode = -1;
			optThd = (feat1+feat2)*0.5f;
			optErr = dmySumWgt;
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

int 
PR_GetWeightOfWeakClassifierMT2(PR_CANDDATAARRAY* aryCand, int boostType, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, double eta, int nThreads)
{
	unsigned int num=aryCand->num;
	double wOut;
	int nIteration=10000;

	PR_CALCLOSSBOOST* data = new PR_CALCLOSSBOOST;
	data->nThreads = nThreads;
	data->dataAry = aryCand;
	data->eta = eta;
	if(boostType!=PR_CHANNEL_ADABOOST)
	{
		data->outWeakModel = PR_CalculateOutputOfWeakClassifier(aryCand, weakClassifier);
		PR_CalculateOutputOfWeakClassifier(aryCand, weakClassifier);
	}
	else
	{
		data->outWeakModel = NULL;
	}

	double error = weakClassifier->trainError;
	double alpha = 0.5f*log((1.0f-error)/error);

	double vmin=DBL_MIN, vmax=alpha*2.0;
//	fprintf(stderr,"alpha=%f ", alpha);

//	PR_BrentMethodGetArgMinForFunction(min, alpha, max, PR_OutputSumOfLossMadaBoost, data, nIteration, &wOut);
//	fprintf(stderr," -> wOut=%f ", wOut);

	switch(boostType)
	{
	case PR_CHANNEL_ADABOOST:
		wOut = alpha;
		break;
	case PR_CHANNEL_MADABOOST:
		PR_BrentMethodGetArgMinForFunction(vmin, alpha, vmax, PR_OutputSumOfLossMadaBoost, data, nIteration, &wOut);
//		PR_BrentMethodGetArgMinForFunction(vmin, 1.0, 10.0, PR_OutputSumOfLossMadaBoost, data, nIteration, &wOut);
		break;
	case PR_CHANNEL_LOGITBOOST:
		PR_BrentMethodGetArgMinForFunction(vmin, alpha, vmax, PR_OutputSumOfLossLogitBoost, data, nIteration, &wOut);
		break;
	case PR_CHANNEL_ETABOOST:
		PR_BrentMethodGetArgMinForFunction(vmin, alpha, vmax, PR_OutputSumOfLossEtaBoost, data, nIteration, &wOut);
		break;
	case PR_CHANNEL_ROBUSTETABOOST:
		PR_BrentMethodGetArgMinForFunction(vmin, alpha, vmax, PR_OutputSumOfLossMostBRobustEtaBoost, data, nIteration, &wOut);
		break;
	default:
		fprintf(stderr,"error boostChannel=%d\n",boostType);
		return PR_AVEND;
	}

//	int e = (int)log10(wOut);
//	int intweight = (int)(wOut*pow(10.0,5-e)+0.5);
//	wOut = (double)intweight*pow(10.0,e-5);

//	fprintf(stderr,"-> %f", wOut);
	weakClassifier->nodeWeight = wOut;

	if(data->outWeakModel!=NULL)	delete [] data->outWeakModel;
	delete data;
//	fprintf(stderr,"\n");
	
	return PR_EOT;
}


static void* threaded_UBoostLearning2(void* arg)
{
	///--- arguments ---///
	UBOOSTTHREAD2* argThread = (UBOOSTTHREAD2*)arg;
	
	PR_CANDDATAARRAY* dataAry = argThread->common->dataAry;
	PR_CASELIST* caseList = (PR_CASELIST*)((PR_CASEDATA*)(dataAry->cands[0]->rootCaseData))->rootCaseList;
	unsigned int nFeat = caseList->featTagTable->numFeat;
	unsigned int top = argThread->forTopFeat;
	unsigned int end = argThread->forEndFeat;
	unsigned int** dataSorted = argThread->common->dataSorted;
	unsigned int lastFeat = argThread->common->lastFeat;
	char lastMode = argThread->common->lastMode;

	if(top>end && top>=nFeat && end>=nFeat)		return NULL;
		
	///--- utilities ---/// 
	unsigned int iFeat;
	PR_BOOSTWEAKCLASSIFIERPARAMS* dmyModel = PR_NewBoostWeakClassifierParams();

	///--- best weak model ---///
	unsigned int bestFeat;
	float bestThreshold;
	double bestTrainError = 10.0;
	char bestMode;
	
	///--- larning main ---///
//	printf("\n");
	for(iFeat=top; iFeat<=end; iFeat++)
	{
		///--- avoid sequentially selection of identical weak-model ---///
		char mode = 0;
		if(iFeat==lastFeat)		mode = -1*lastMode;

		///--- performance evaluation of each feature ---///
		PR_ThresholdingOptimizationByWeightedDataBoost2(dataAry,iFeat,dataSorted[iFeat],mode,dmyModel);

		///--- find weak-model with the best performance ---///
		if(bestTrainError>dmyModel->trainError)
		{
			bestFeat = iFeat;
			bestThreshold = dmyModel->threshold;
			bestTrainError = dmyModel->trainError;
			bestMode = dmyModel->mode;
		}
	}

	///--- copy parameters of local best weak-model ---///
	argThread->localBestFeat = bestFeat;
	argThread->localBestMode = bestMode;
	argThread->localBestErr = (float)bestTrainError;
	argThread->localBestThreshold = bestThreshold;

	///--- delete ---///
	PR_DeleteBoostWeakClassifierParams(dmyModel);

	return NULL;
}

unsigned int FindBestPartialResultUBoostMT2(UBOOSTTHREAD2** argThreads, int num)
{
	int i, bestMember;
	double bestError=10.0;

	for(i=0; i<num; i++) {
	if(bestError>argThreads[i]->localBestErr) {
		bestMember = i;
		bestError = argThreads[i]->localBestErr;
	}}

	return bestMember;
}


int PR_UBoostEssentialFuncMT_LowMemory(
		PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int channelBoost, double eta, int nThreads)
{
	fprintf(stderr,"UBoost; nModel=%d, type=%d, eta=%.4f\n",ensemble->numNode,channelBoost,eta);

	if(caseList==NULL || ensemble==NULL || 
		caseList->childNodeFirstCase->childNodeFirstCand->optionParam==NULL)
		return PR_ErrorMessageInt("unknown inputs (PR_UBoostEssentialFunc)");


	///--- utilities ---/// 
	int i;
	unsigned int iFeat, nFeat = caseList->featTagTable->numFeat;
	unsigned int iModel, nModel = ensemble->numNode;


	///--- count number of candidate ---///
	PR_CountNumCandsForCaseList(caseList);
	fprintf(stdout, "n_data=%d,", caseList->totalNumCand);

	PR_FEATTAGTABLE* fTable = PR_NewFeatTagTable(nFeat);
	PR_CopyFeatTagTable(fTable, caseList->featTagTable);

	///--- make cand-data-array for learning ---///
	PR_CANDDATAARRAY* dataAry = PR_NewCandDataArray(caseList,eta);
	fprintf(stdout, "b,");
	
	///--- ascending-sorting of each feature ---///
	unsigned int** dataSorted = new unsigned int* [nFeat];

	for(iFeat=0; iFeat<nFeat; iFeat++) {
		fprintf(stdout, "sorting_f[%d] (tag:%04x.%04x)\r", 
			iFeat,caseList->featTagTable->tags[iFeat]->group,caseList->featTagTable->tags[iFeat]->element);
		unsigned long* temp = PR_GetAscendSortResultOfFeature(dataAry,caseList->featTagTable->tags[iFeat]);
		dataSorted[iFeat] = new unsigned int [caseList->totalNumCand];
		for(unsigned int i=0; i<caseList->totalNumCand; i++)	dataSorted[iFeat][i] = (unsigned int)temp[i];
		delete [] temp;
	}
	fprintf(stdout, "c,");

	
	///--- output of optimization for weak models by single feature thresholding ---///
	PR_BOOSTWEAKCLASSIFIERPARAMS* dmyModel = PR_NewBoostWeakClassifierParams();
	fprintf(stdout, "d,");

	///--- best weak model ---///
	unsigned int bestFeat;
	float bestThreshold;
	double bestTrainError;
	char bestMode;

	///--- last seleted weak model ---///

	///--- modifying eta ---///
	if(channelBoost!=13 && channelBoost!=14)	eta=0.0;

#ifdef GET_LAPTIME
	GETLAPTIME* laptimePre = NewGetLapTime();
	GETLAPTIME* laptimeProc = NewGetLapTime();
	GETLAPTIME* laptimePost = NewGetLapTime();
#endif

	///--- for multi-thread processing  ---///
	if(nThreads>(int)nFeat)		nThreads = (int)nFeat;
	unsigned int bestThreaded;

	COMMONUBOOSTTHREAS2* common = new COMMONUBOOSTTHREAS2;
	common->dataAry = dataAry;
	common->dataSorted = dataSorted;
	common->lastFeat = nFeat+1;
	common->lastMode = 0;

	UBOOSTTHREAD2** data = new UBOOSTTHREAD2* [nThreads];
	double nForThread=(double)nFeat/(double)nThreads;
	printf("nThreads=%d\n",nThreads);
	for(i=0; i<nThreads; i++)
	{
		data[i] = NewUBoostThread2();
		data[i]->common = common;
		data[i]->forTopFeat = (unsigned int)((double)i*nForThread+0.5);
		data[i]->forEndFeat = (unsigned int)((double)(i+1)*nForThread+0.5)-1;
		printf("thread[%d]; f%d~f%d\n",i,data[i]->forTopFeat,data[i]->forEndFeat);
	}
	printf("nFeat=%d\n",nFeat);

	// initialize data-weight
	PR_CostSensitiveNormalizeDataWeightBoost(dataAry);


	///--- U-Boost main ---///
	PR_ENSEMBLENODE* currentModel = ensemble->firstNode;

	pthread_t* threads = new pthread_t [nThreads];

	for(iModel=0; iModel<nModel; iModel++)
	{
#ifdef GET_LAPTIME
		GetLocaStartTime(laptimePre);
#endif

		// model parameters
		PR_BOOSTWEAKCLASSIFIERPARAMS* weakModel = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentModel->data;
		

#ifdef MODE_USING_TEMPORAL_FP
		rankDmy = new unsigned long [dataAry->num];
#endif	

#ifdef GET_LAPTIME
		GetLocalEndTime(laptimePre);
		GetLocaStartTime(laptimeProc);
#endif
		
		// multi-thread selection of weak-model (single feature thresholding)
		if(weakModel->mode==0)
		{
		//	fprintf(stderr,"iModel[%d];\n",iModel);

			for(i=0; i<nThreads; i++) {
				pthread_create(&threads[i], NULL, threaded_UBoostLearning2, (void *)data[i]);
			}

			for(i=0; i<nThreads; i++) {
				pthread_join(threads[i],NULL);
			}

			bestThreaded = FindBestPartialResultUBoostMT2(data,nThreads);

			bestTrainError = data[bestThreaded]->localBestErr;
			bestFeat = data[bestThreaded]->localBestFeat;
			bestMode = data[bestThreaded]->localBestMode;
			bestThreshold = data[bestThreaded]->localBestThreshold;
		}
		// optimization for determined weak-model
		else
		{
			// get index of the feature
			PR_CheckIfFeatTagIsMemberOfFeatTagTable(weakModel->featureTag,caseList->featTagTable,&bestFeat);
			
			// evaluation of the performance
			PR_ThresholdingOptimizationByWeightedDataBoost2(dataAry,bestFeat,dataSorted[bestFeat],weakModel->mode,dmyModel);

			// get the performance
			bestThreshold = dmyModel->threshold;
			bestTrainError = dmyModel->trainError;
			bestMode = dmyModel->mode;
		}

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

		PR_GetWeightOfWeakClassifierMT2(dataAry, channelBoost, weakModel, eta, nThreads);

		///--- reflection of current weak-model output on final output ---///
		PR_ReflectWeakClassifierOutputOnLikelihood(dataAry,weakModel);

		///--- update data-weight ---///
		PR_UpdateDataWeight(dataAry,channelBoost,eta);

		// initialize data-weight
		PR_CostSensitiveNormalizeDataWeightBoost(dataAry);


		///--- std output ---///
		printf("%03d [%03d]=[%04x.%04x] thd%f(%+d), err%e, w%e\n",
			iModel+1,bestFeat,caseList->featTagTable->tags[bestFeat]->group,caseList->featTagTable->tags[bestFeat]->element,
			bestThreshold,bestMode,bestTrainError,weakModel->nodeWeight);

		///--- avoid sequentially selection of identical weak-model ---///
		common->lastFeat = bestFeat;
		common->lastMode = bestMode;

		///--- shift poiter to next model ---///
		currentModel = currentModel->next;

#ifdef GET_LAPTIME
		GetLocalEndTime(laptimePost);
		ShowLapTime(laptimePre);
		ShowLapTime(laptimeProc);
		ShowLapTime(laptimePost);
#endif
	}

	delete [] threads;


	///--- delete ---///
	PR_DeleteCandDataArray(dataAry);
	PR_DeleteBoostWeakClassifierParams(dmyModel);

	for(i=0; i<nThreads; i++)	DeleteUBoostThread2(data[i]);
	delete [] data;
	delete common;

	for(iFeat=0; iFeat<nFeat; iFeat++)	delete [] dataSorted[iFeat];
	delete [] dataSorted;

	return PR_EOT;
}

