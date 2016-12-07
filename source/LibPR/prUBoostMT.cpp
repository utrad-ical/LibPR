
#include <iostream>
#include <float.h>
#include <limits.h>
#include <pthread.h>
#include "./prUBoost.private.h"
#include "../LibPR.h"

//#define GET_LAPTIME


int PR_UBoostEssentialFuncMT(
PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int channelBoost, double eta, int nThreads);

PR_ENSEMBLE* PR_UBoostMT(
	PR_CASELIST* caselist, int chBoost, unsigned int numClassifier, int chAddCost, double eta, int numThreads);

int PR_GetWeightOfWeakClassifierMT(PR_CANDDATAARRAY* aryCand, int boostType, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, double eta, int nThreads);


PR_ENSEMBLE* PR_SimpleAdaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_ADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0,numThreads);
}

PR_ENSEMBLE* PR_SimpleMadaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_MADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0,numThreads);
}

PR_ENSEMBLE* PR_SimpleLogitBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_LOGITBOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,0.0,numThreads);
}

PR_ENSEMBLE* PR_SimpleEtaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_ETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,eta,numThreads);
}

PR_ENSEMBLE* PR_SimpleRobustEtaBoostMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_ROBUSTETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_EVEN,eta,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveAdaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_ADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveMadaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_MADABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveLogitBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_LOGITBOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,0.0,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveEtaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_ETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,eta,numThreads);
}

PR_ENSEMBLE* PR_CostSensitiveRobustEtaBoostForImbalancedDatasetMT(PR_CASELIST* caseList, unsigned int numClassifier, double eta, int numThreads)
{
	return PR_UBoostMT(caseList,PR_CHANNEL_ROBUSTETABOOST,numClassifier,PR_DWEIGHTINITIALIZE_CLASS,eta,numThreads);
}


PR_ENSEMBLE* PR_UBoostMT(
	PR_CASELIST* caselist, int chBoost, unsigned int numClassifier, int chAddCost, double eta, int numThreads)
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
	if(numThreads==1)
		PR_UBoostEssentialFunc(caselist,ensemble,chBoost,eta);
	else
		PR_UBoostEssentialFuncMT(caselist,ensemble,chBoost,eta,numThreads);

	// データ重みメモリ開放
	if(rawCand)		PR_DeleteBoostOptionInAllCands(caselist);

	return ensemble;
}

/*
int PR_TrainAdaBoostEnsembleMT(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int numThreads)
{
	if(numThreads==1)
		PR_UBoostEssentialFunc(caseList,ensemble,PR_CHANNEL_ADABOOST,0.0,0);
	else if(numThreads>1)
		PR_UBoostEssentialFuncMT(caseList,ensemble,PR_CHANNEL_ADABOOST,0.0,numThreads);
	else
		return PR_AVEND;
	return PR_EOT;
}

int PR_TrainMadaBoostEnsembleMT(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int numThreads)
{
	if(numThreads==1)
		PR_UBoostEssentialFunc(caseList,ensemble,PR_CHANNEL_ADABOOST,0.0,0);
	else if(numThreads>1)
		PR_UBoostEssentialFuncMT(caseList,ensemble,PR_CHANNEL_MADABOOST,0.0,numThreads);
	else
		return PR_AVEND;
	return PR_EOT;
}

int PR_TrainLogitBoostEnsembleMT(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, int numThreads)
{
	if(numThreads==1)
		PR_UBoostEssentialFunc(caseList,ensemble,PR_CHANNEL_ADABOOST,0.0,0);
	else if(numThreads>1)
		PR_UBoostEssentialFuncMT(caseList,ensemble,PR_CHANNEL_LOGITBOOST,0.0,numThreads);
	else
		return PR_AVEND;
	return PR_EOT;
}

int PR_TrainEtaBoostEnsembleMT(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double eta, int numThreads)
{
	if(numThreads==1)
		PR_UBoostEssentialFunc(caseList,ensemble,PR_CHANNEL_ADABOOST,0.0,0);
	else if(numThreads>1)
		PR_UBoostEssentialFuncMT(caseList,ensemble,PR_CHANNEL_ETABOOST,eta,numThreads);
	else
		return PR_AVEND;
	return PR_EOT;
}

int PR_TrainRobustEtaBoostEnsembleMT(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double eta, int numThreads)
{
	if(numThreads==1)
		PR_UBoostEssentialFunc(caseList,ensemble,PR_CHANNEL_ADABOOST,0.0,0);
	else if(numThreads>1)
		PR_UBoostEssentialFuncMT(caseList,ensemble,PR_CHANNEL_ROBUSTETABOOST,eta,numThreads);
	else
		return PR_AVEND;
	return PR_EOT;
}
*/

typedef struct common_UBoostThread
{
	PR_CANDDATAARRAY* dataAry;
	unsigned long** dataSorted;
	unsigned int lastFeat;
	char lastMode;
}
COMMONUBOOSTTHREAS;

typedef struct UBoostThread
{
	COMMONUBOOSTTHREAS* common;
	unsigned int forTopFeat, forEndFeat;
	double localBestErr;
	unsigned int localBestFeat;
	char localBestMode;
	float localBestThreshold;
}
UBOOSTTHREAD;

UBOOSTTHREAD* NewUBoostThread(void)
{
	UBOOSTTHREAD* output = new UBOOSTTHREAD;
	output->common = NULL;
	output->forEndFeat = output->forTopFeat = 0;
	output->localBestErr = 10.0;
	output->localBestFeat = LONG_MAX;
	output->localBestMode = 0;
	output->localBestThreshold = 0.0f;
	return output;
}

void DeleteUBoostThread(UBOOSTTHREAD* input)
{
	delete input;
}

static void* threaded_UBoostLearning(void* arg)
{
	///--- arguments ---///
	UBOOSTTHREAD* argThread = (UBOOSTTHREAD*)arg;
	
	PR_CANDDATAARRAY* dataAry = argThread->common->dataAry;
	PR_CASELIST* caseList = (PR_CASELIST*)((PR_CASEDATA*)(dataAry->cands[0]->rootCaseData))->rootCaseList;
	unsigned int nFeat = caseList->featTagTable->numFeat;
	unsigned int top = argThread->forTopFeat;
	unsigned int end = argThread->forEndFeat;
	unsigned long** dataSorted = argThread->common->dataSorted;
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
#ifndef MODE_USING_TEMPORAL_FP
		PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,iFeat,dataSorted[iFeat],mode,dmyModel);
#else
		fseek(fpTemp,sizeof(unsigned int)*dataAry->num*iFeat,SEEK_SET);
		fread(rankDmy,sizeof(unsigned long),dataAry->num,fpTemp);
		PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,iFeat,rankDmy,mode,dmyModel);
#endif

		///--- find weak-model with the best performance ---///
		if(bestTrainError>dmyModel->trainError)
		{
			bestFeat = iFeat;
			bestThreshold = dmyModel->threshold;
			bestTrainError = dmyModel->trainError;
			bestMode = dmyModel->mode;
//			printf("[%d]; th=%e, e=%e, m=(%d)\n",bestFeat,bestThreshold,bestTrainError,bestMode);
		}

		
	}

	///--- copy parameters of local best weak-model ---///
	argThread->localBestFeat = bestFeat;
	argThread->localBestMode = bestMode;
	argThread->localBestErr = bestTrainError;
	argThread->localBestThreshold = bestThreshold;

	///--- delete ---///
	PR_DeleteBoostWeakClassifierParams(dmyModel);

	return NULL;
}

unsigned int FindBestPartialResultUBoostMT(UBOOSTTHREAD** argThreads, int num)
{
	int i, bestMember;
	double bestError=10.0;

//	fprintf(stderr,"\n");
	for(i=0; i<num; i++)
	{
//		fprintf(stderr,"thread-id[%d]; feat%d, threshold%e err%e\n",
//				i,argThreads[i]->localBestFeat,argThreads[i]->localBestThreshold,argThreads[i]->localBestErr);

		if(bestError>argThreads[i]->localBestErr)
		{
			bestMember = i;
			bestError = argThreads[i]->localBestErr;
		}
	}
//	fprintf(stderr,"\n");

	return bestMember;
}

int 
PR_UBoostEssentialFuncMT(
	PR_CASELIST* caseList,
	PR_ENSEMBLE* ensemble,
	int channelBoost,
	double eta,
	int nThreads)
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
	fprintf(stdout, "a,");

	///--- make cand-data-array for learning ---///
	PR_CANDDATAARRAY* dataAry = PR_NewCandDataArray(caseList,eta);
	fprintf(stdout, "b,");
	
	///--- ascending-sorting of each feature ---///
#ifndef MODE_USING_TEMPORAL_FP
	unsigned long** dataSorted = new unsigned long* [nFeat];
	for(iFeat=0; iFeat<nFeat; iFeat++)
	{
		fprintf(stdout, "sorting_f[%d] (tag:%x.%x)\r", 
			iFeat,caseList->featTagTable->tags[iFeat]->group,caseList->featTagTable->tags[iFeat]->element);
		dataSorted[iFeat] = PR_GetAscendSortResultOfFeature(dataAry,caseList->featTagTable->tags[iFeat]);
	}
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

	COMMONUBOOSTTHREAS* common = new COMMONUBOOSTTHREAS;
	common->dataAry = dataAry;
	common->dataSorted = dataSorted;
	common->lastFeat = nFeat+1;
	common->lastMode = 0;

	UBOOSTTHREAD** data = new UBOOSTTHREAD* [nThreads];
	double nForThread=(double)nFeat/(double)nThreads;
	printf("nThreads=%d\n",nThreads);
	for(i=0; i<nThreads; i++)
	{
		data[i] = NewUBoostThread();
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

			for(i=0; i<nThreads; i++)
			{
			//	printf("thread#.%d; %d~%d\n",i,data[i]->forTopFeat,data[i]->forEndFeat);
				pthread_create(&threads[i],NULL,threaded_UBoostLearning,(void *)data[i]);
			}

			for(i=0; i<nThreads; i++)
			{
				pthread_join(threads[i],NULL);
			}

			bestThreaded = FindBestPartialResultUBoostMT(data,nThreads);

			bestTrainError = data[bestThreaded]->localBestErr;
			bestFeat = data[bestThreaded]->localBestFeat;
			bestMode = data[bestThreaded]->localBestMode;
			bestThreshold = data[bestThreaded]->localBestThreshold;

		//	fprintf(stderr,"best; err%e, feat%d, mode%d, threshold%e\n",bestTrainError,bestFeat,bestMode,bestThreshold);
		//	getchar();

/*			bestTrainError = 10.0;

			for(iFeat=0; iFeat<nFeat; iFeat++)
			{
				// avoid sequentially selection of identical weak-model
				char mode = 0;
				if(iFeat==lastFeat)		mode = -1*lastMode;

				// performance evaluation of each feature
#ifndef MODE_USING_TEMPORAL_FP
				PR_ThresholdingOptimizationByWeightedDataBoost(dataAry,iFeat,dataSorted[iFeat],mode,dmyModel);
	//			PR_ThresholdingOptimizationByWeightedDataBoostOld(dataAry,iFeat,dataSorted32[iFeat],mode,dmyModel);
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
*/		}
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

	//	int nSignificance = 15;
	//	int ee = (int)(log10(weakModel->trainError)+0.5);
	//	int interror = (int)(weakModel->trainError*pow(10.0,nSignificance-ee)+0.5);
	//	weakModel->trainError = (double)interror*pow(10.0,ee-nSignificance);

		///--- calculate weight of the current weak-model (cost sensitive) ---///
		PR_GetWeightOfWeakClassifierMT(dataAry, channelBoost, weakModel, eta, nThreads);
		
	//	int ew = (int)(log10(weakModel->nodeWeight)+0.5);
	//	int intweight = (int)(weakModel->nodeWeight*pow(10.0,nSignificance-ew)+0.5);
	//	weakModel->nodeWeight = (double)intweight*pow(10.0,ew-nSignificance);

		///--- reflection of current weak-model output on final output ---///
		PR_ReflectWeakClassifierOutputOnLikelihood(dataAry,weakModel);

		///--- update data-weight ---///
		PR_UpdateDataWeight(dataAry,channelBoost,eta);

		// initialize data-weight
		PR_CostSensitiveNormalizeDataWeightBoost(dataAry);


		///--- std output ---///
		printf("%d[%d], thd%f(%+d), err%e, w%e\n",
			iModel+1,bestFeat,bestThreshold,bestMode,bestTrainError,weakModel->nodeWeight);

		/*
		unsigned long interval = caseList->totalNumCand/10;
		for(unsigned long i=0; i<caseList->totalNumCand; i++)
		if(i%interval==0 ||(dataAry->cands[i]->classLabel && i%2==0))
		{
			PR_BOOSTOPTION* option = (PR_BOOSTOPTION*)(dataAry->cands[i]->optionParam);
			printf(">> [%5d] H=%e w=%e c=%e\n", i,dataAry->cands[i]->likelihood[1],option->dataWeight,option->cost);
		}
		printf("\n");
		*/


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

	for(i=0; i<nThreads; i++)	DeleteUBoostThread(data[i]);
	delete [] data;
	delete common;


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


typedef struct _CALC_LOSS_THREAD_ARGS_
{
	double alpha;
	int boostType;
	unsigned int iTop, iEnd;
	PR_CALCLOSSBOOST* option;
	double out;
}
CALCLOSS_THREADARGS;

static void* threaded_CalcLossSumBoost(void* args)
{
	CALCLOSS_THREADARGS* data = (CALCLOSS_THREADARGS*)args;
	PR_CALCLOSSBOOST* option = data->option;
	double lossSum = basis_CalcLossSumBoost(data->alpha, data->iTop, data->iEnd, option->dataAry->cands, option->outWeakModel, option->eta, data->boostType);
	data->out = lossSum;
	return NULL;
}

double PublicCalcLossSumBoostMT(double alpha, void* option, int boostType)
{
	int nThreads = ((PR_CALCLOSSBOOST*)option)->nThreads;
	unsigned int num = ((PR_CALCLOSSBOOST*)option)->dataAry->num;

	CALCLOSS_THREADARGS** args = new CALCLOSS_THREADARGS* [num];
	for(int i=0; i<nThreads; i++)
	{
		args[i] = new CALCLOSS_THREADARGS;
		args[i]->alpha = alpha;
		args[i]->boostType = boostType;
		args[i]->iTop = (unsigned int)((float)num/(float)nThreads*(float)i+0.5f);
		args[i]->iEnd = (unsigned int)((float)num/(float)nThreads*(float)(i+1)+0.5f)-1;
		args[i]->option = (PR_CALCLOSSBOOST*)option;
	}

	pthread_t* threads = new pthread_t [nThreads];
	double lossSum=0.0;

	for(int i=0; i<nThreads; i++)
		pthread_create(&threads[i], NULL, threaded_CalcLossSumBoost, (void*)args[i]);

	for(int i=0; i<nThreads; i++)
		pthread_join(threads[i], NULL);

	for(int i=0; i<nThreads; i++)
		lossSum += args[i]->out;

	for(int i=0; i<nThreads; i++)	delete args[i];
	delete [] args;
	delete [] threads;

	return lossSum;
}


int 
PR_GetWeightOfWeakClassifierMT(PR_CANDDATAARRAY* aryCand, int boostType, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, double eta, int nThreads)
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

