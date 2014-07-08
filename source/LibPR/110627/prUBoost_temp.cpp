
//#include "./prUBoost_temp.h"
#include "./prUBoost_temp.private.h"


int PR_TrainEnsembleUBoost_Temp(
PR_ARRAYCASEDATATEMP* tableMeta,
PR_ENSEMBLE* ensemble,			// list of weak-classifiers for ensemble
int channelBoost,
double eta,
char bDataWeightLogOutput
){
	fprintf(stderr,"boosting - maxNum=%d\n",ensemble->numNode);
	
	PR_CountNumCandidateInClassOfArrayCaseDataTemp(tableMeta);

	if(tableMeta==NULL || ensemble==NULL)
		return PR_ErrorMessageInt("unknown inputs (@PR_TrainDiscreteAdaBoostEnsemble)");

	unsigned int nmodel, nfeat;
	unsigned int numFeature = tableMeta->tableFeature->numFeat;

	if(channelBoost!=PR_MODELCODE_ETABOOST && channelBoost!=PR_MODELCODE_ROBUSTETABOOST)	eta=0.0;

	// get results of ascending sort for feature value
//	FILE** tempRank = new FILE* [numFeature];
	FILE* rankTemp = tmpfile();

	for(unsigned int i=0; i<numFeature; i++)
	{
	//	printf("feat[%d/%d];\n",i,numFeature);

		float* aryFeature = PR_ExtractFullArrayOfOneFeature(tableMeta, i);
		unsigned int* rankDmy = PR_QuickSortAscendingWithRank(aryFeature, tableMeta->numCandidate);
		/*
		if(i%100==0)
		{
			char strdmy[128];
			sprintf(strdmy,"temp_debug_rankFeat%d_nCand%d.txt",i,tableMeta->numCandidate);
			FILE* fp = fopen(strdmy,"w");
			for(unsigned int cnt=0; cnt<tableMeta->numCandidate; cnt++)
				if(cnt%100==0 || cnt==tableMeta->numCandidate-1)	fprintf(fp,"#%d %d\n",cnt,rankDmy[cnt]);
			fclose(fp);
		}
		*/
		size_t sz = fwrite(rankDmy,sizeof(unsigned int),tableMeta->numCandidate,rankTemp);
		delete [] aryFeature;
		delete [] rankDmy;

	//	getchar();
	}
//	printf("\n");
	// **tempRank(+), *tempRank(+)


	unsigned int idxMinErrFeat;

	// boorean: 1=target_of_analysis	// **tempRank(+), *tempRank(+), *booAnalyze(+)
	char* booAnalyze = new char [numFeature];
	
	// num of weak-classifier within ensemble user want to create
	unsigned int finalNumModel = ensemble->numNode;	

	PR_ENSEMBLENODE* currentModel = ensemble->firstNode;
	PR_BOOSTWEAKCLASSIFIERPARAMS* dmyWeakClassifier = PR_NewBoostWeakClassifierParams();

	int response;

	unsigned char* aryClass = PR_ExtractFullArrayOfClassLabel(tableMeta);

	double* dataWeight = new double [tableMeta->numCandidate];
	for(unsigned int i=0; i<tableMeta->numCandidate; i++)
		dataWeight[i] = 1.0/(double)tableMeta->numCandidate;

	double* outEnsemble;
	FILE* tempEnsembleOut = NULL;

	// **tempRank(+), *tempRank(+), *booAnalyze(+),
	// *aryClass(+), *dataWeight(+), *outEnsemble(-), *tempEnsembleOut(-)

	unsigned int* rankDmy = new unsigned int [tableMeta->numCandidate];


	fprintf(stderr,"boost(c) - maxNumWeakClassifier=%d\n",finalNumModel);
	for(nmodel=0; nmodel<finalNumModel; nmodel++)
	{
		PR_BOOSTWEAKCLASSIFIERPARAMS* weakModel = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentModel->data;
		PR_BOOSTWEAKCLASSIFIERPARAMS* prev1 = NULL;
		if(nmodel)	prev1 = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentModel->prev->data;

		// set data weight
		PR_IncludeCostInDataWeightForBoost_Temp(tableMeta,dataWeight);
		PR_NormalizeValueInDataAry(dataWeight,tableMeta->numCandidate);
		
		/*
		if(nmodel%5==0)
		{
			printf("#%d/%d.. setWeight;\n",nmodel,finalNumModel);
			double sumW=0.0;
			double* aryCost = PR_ExtractFullArrayOfCost(tableMeta);
			unsigned char* aryClass = PR_ExtractFullArrayOfClassLabel(tableMeta);
			for(unsigned int a=0; a<tableMeta->numCandidate; a++)
			{
				sumW+=dataWeight[a];
				if(a%100==0 || aryClass[a])
					printf("[%d(%d)](c%.3e,w%.3e), ",a,aryClass[a],aryCost[a],dataWeight[a]);
			}
			printf("(sumW%.3e)\n",sumW);
			delete [] aryCost;
			delete [] aryClass;
		}
		/**/

		// define features which will be tested
		if(weakModel->mode==0 && weakModel->featureTag->group==0xffff && weakModel->featureTag->element==0xffff)
		{
			for(nfeat=0; nfeat<numFeature; nfeat++)	booAnalyze[nfeat]=1;
			if(nmodel>0)
			{
				unsigned int pf1=PR_ExtractFeatNumberInFeatTagTable(tableMeta->tableFeature,prev1->featureTag);
				if(prev1->mode==+1)		booAnalyze[pf1]=10;
				else					booAnalyze[pf1]=11;
			}
		}
		else
		{
			for(nfeat=0; nfeat<numFeature; nfeat++)		booAnalyze[nfeat]=0;
			response = PR_CheckIfFeatTagIsMemberOfFeatTagTable(weakModel->featureTag,tableMeta->tableFeature,&idxMinErrFeat);

			if(response!=PR_NG)
			{
				if(weakModel->mode==-1)		booAnalyze[idxMinErrFeat]=10;
				else						booAnalyze[idxMinErrFeat]=11;
			}
			else
				return PR_ErrorMessageInt("error featTagTable (@PR_TrainDiscreteAdaBoostEnsemble)");
		}
	//	printf("setB\n");


		// find out feature provides minimum weighted error
		unsigned int idxFeatOfMinEra;
		double minEra=10.0;
		for(nfeat=0; nfeat<numFeature; nfeat++)
		{
			printf("model#%d/%d; feat%d/%d..",nmodel+1,finalNumModel,nfeat,tableMeta->tableFeature->numFeat);
			dmyWeakClassifier->trainError = 2.0;

			if(booAnalyze[nfeat])
			{	
				char mode=0;
				if(booAnalyze[nfeat]==10)		mode=-1;
				else if(booAnalyze[nfeat]==11)	mode=1;

			//	rewind(tempRank[nfeat]);
			//	fread(rankDmy,sizeof(unsigned int),tableMeta->numCandidate,tempRank[nfeat]);

				fseek(rankTemp,sizeof(unsigned int)*tableMeta->numCandidate*nfeat,SEEK_SET);
				fread(rankDmy,sizeof(unsigned int),tableMeta->numCandidate,rankTemp);

				PR_EvaluateWeightedMinErrorOfFeatureThresholding_Temp(tableMeta,nfeat,rankDmy,aryClass,dataWeight,dmyWeakClassifier,mode);
			}

			if(minEra>dmyWeakClassifier->trainError)
			{
				// update (nmodel+1)-th weak classifier
				idxFeatOfMinEra = nfeat;
				minEra = weakModel->trainError = dmyWeakClassifier->trainError;

				PR_CopyFeatTag(weakModel->featureTag,dmyWeakClassifier->featureTag);
				weakModel->mode = dmyWeakClassifier->mode;
				weakModel->threshold = dmyWeakClassifier->threshold;
				weakModel->nodeIndex = nmodel+1;
			}

			printf("\r");
			/*{
				FILE* fp=fopen("debug_tmp_trainLog.txt","a");
				fprintf(fp,"model#%d/%d; feat%d/%d,",nmodel+1,finalNumModel,nfeat,tableMeta->tableFeature->numFeat);
				fprintf(fp," mode%d, idx%d, thd%.3e, era%.3e\n",dmyWeakClassifier->mode,dmyWeakClassifier->nodeIndex,
																dmyWeakClassifier->threshold,dmyWeakClassifier->trainError);
				fclose(fp);
			}*/
		}


		// to be temporal "dataWeight"
		FILE* tempDataWeight = tmpfile();
		fwrite(dataWeight,sizeof(double),tableMeta->numCandidate,tempDataWeight);
		delete [] dataWeight;

		// **tempRank(+), *tempRank(+), *booAnalyze(+),
		// *aryClass(+), *dataWeight(-), *outEnsemble(-), *tempEnsembleOut(-),
		// *tempDataWeight(+)


		// reflect new-add weak classifier for ensemble output 
		printf("> model#%d/%d; #%d[%04x %04x] ",nmodel+1,finalNumModel,idxFeatOfMinEra,weakModel->featureTag->group, weakModel->featureTag->element);

		outEnsemble = new double [tableMeta->numCandidate];
		if(nmodel)	
		{
			rewind(tempEnsembleOut);
			fread(outEnsemble,sizeof(double),tableMeta->numCandidate,tempEnsembleOut);
		}
		else
			memset(outEnsemble,0x00,sizeof(double)*tableMeta->numCandidate);
		
		char* aryOutWeak = PR_CalculateOutputOfWeakClassifier_Temp(tableMeta,weakModel);		
		PR_GetWeightOfWeakClassifier_Temp(tableMeta,outEnsemble,aryOutWeak,aryClass,weakModel,channelBoost,eta);
		for(unsigned int i=0; i<tableMeta->numCandidate; i++)
			outEnsemble[i] += (double)aryOutWeak[i]*weakModel->nodeWeight;
		delete [] aryOutWeak;

		printf("thd%.3e(%+d) err%.3e weight%.3e\n",weakModel->threshold,weakModel->mode,weakModel->trainError,weakModel->nodeWeight);
		// **tempRank(+), *tempRank(+), *booAnalyze(+),
		// *aryClass(+), *dataWeight(-), *outEnsemble(+), *tempEnsembleOut(-),
		// *tempDataWeight(+)


		// update data weights
		dataWeight = new double [tableMeta->numCandidate];
		rewind(tempDataWeight);
		fread(dataWeight,sizeof(double),tableMeta->numCandidate,tempDataWeight);
		fclose(tempDataWeight);

		UpdateWeightOfCandidateByObtainedEnsembleOutput_Temp(dataWeight,outEnsemble,aryClass,tableMeta->numCandidate,channelBoost,eta);


		// to be temporal "outEnsemble"
		if(nmodel)	fclose(tempEnsembleOut);
		tempEnsembleOut = tmpfile();
		fwrite(outEnsemble,sizeof(double),tableMeta->numCandidate,tempEnsembleOut);
		delete [] outEnsemble;

		// **tempRank(+), *tempRank(+), *booAnalyze(+),
		// *aryClass(+), *dataWeight(+), *outEnsemble(-), *tempEnsembleOut(+),
		// *tempDataWeight(-)


		if(bDataWeightLogOutput)
		{
			char strDmy[128];
			sprintf(strDmy,"logDataWeight_%d.txt",nmodel+1);
			FILE* fpLog = fopen(strDmy,"w");

			fprintf(fpLog,"%04x %04x <-_selected_feature\n",weakModel->featureTag->group,weakModel->featureTag->element);
			fprintf(fpLog,"%le <-_weight_for_feature\n",weakModel->nodeWeight);
			fprintf(fpLog,"%le <-_weight_for_feature\n",weakModel->nodeWeight);
			fprintf(fpLog,"(case_id,cand_id,class,data_weight)\n");

			unsigned int i,j,cntSum=0;
			for(j=0; j<tableMeta->numCase; j++)
			{
				unsigned int idCase = tableMeta->arrayCase[j]->idCase;
			//	unsigned int* idCandidate = new unsigned int [tableMeta->arrayCase[j]->numCandidate];
			//	rewind(tableMeta->arrayCase[j]->tempCandidateID);
			//	fread(idCandidate,sizeof(unsigned int),tableMeta->arrayCase[j]->numCandidate,tableMeta->arrayCase[j]->tempCandidateID);
				unsigned int* idCandidate = PR_ExtractOneCaseArrayOfCandidateID(tableMeta->arrayCase[j]);

				for(i=0; i<tableMeta->arrayCase[j]->numCandidate; i++)
					fprintf(fpLog,"%d %d %u %le\n",idCase,idCandidate[i],aryClass[i+cntSum],dataWeight[i+cntSum]);

				delete [] idCandidate;
				cntSum+=tableMeta->arrayCase[j]->numCandidate;
			}

			fclose(fpLog);
		}

		// shift target ensemble node to next
		currentModel = currentModel->next;
	}


//	for(unsigned int i=0; i<numFeature; i++)	fclose(tempRank[i]);
//	delete [] tempRank;
	delete [] rankDmy;
	fclose(rankTemp);

	delete [] booAnalyze;
	PR_DeleteBoostWeakClassifierParams(dmyWeakClassifier);
	delete [] aryClass;
	delete [] dataWeight;

	fclose(tempEnsembleOut);


	return PR_OK;
}



int PR_NormalizeValueInDataAry(double* ary, unsigned int num)
{
	double sum=0.0;
	for(unsigned int i=0; i<num; i++)	sum+=ary[i];
	for(unsigned int i=0; i<num; i++)	ary[i]/=sum;
	return PR_OK;
}


int PR_IncludeCostInDataWeightForBoost_Temp(PR_ARRAYCASEDATATEMP* tableMeta, double* aryDataWeight)
{
	if(tableMeta->temp->classLabel==NULL)		return PR_NG;

	double* aryCost = PR_ExtractFullArrayOfCost(tableMeta);
	if(aryCost==NULL)
	{
		printf("error: IncludeCostInDataWeightForBoost_Temp\n");
		return PR_NG;
	}

	unsigned int i, num=tableMeta->numCandidate;
	double sumNewWeight=0.0;

	for(i=0; i<num; i++)
	{
		if(aryCost[i]!=1.0)		aryDataWeight[i] *= aryCost[i];
		sumNewWeight += aryDataWeight[i];
	}

	if(sumNewWeight!=1.0)
	{
		for(i=0; i<num; i++)
			aryDataWeight[i] /= sumNewWeight;
	}

	delete [] aryCost;
	return PR_OK;
}

int PR_InitializeCostBoostTwoClassTemporal(PR_ARRAYCASEDATATEMP* tableMeta, int mode)
{
	if(tableMeta==NULL || (mode!=1 && mode!=2))	return PR_NG;

	PR_CountNumCandidateInClassOfArrayCaseDataTemp(tableMeta);
	
	double* intCostValue = new double [2];
	if(mode==1)
	{
		intCostValue[0] = intCostValue[1] = 1.0/(double)tableMeta->numCandidate;
	}
	else
	{
		intCostValue[0] = 0.5/(double)tableMeta->numCandidateInClass[0];
		intCostValue[1] = 0.5/(double)(tableMeta->numCandidate-tableMeta->numCandidateInClass[0]);
	}
	
	double* aryCost = PR_ExtractFullArrayOfCost(tableMeta);
	unsigned char* aryClass = PR_ExtractFullArrayOfClassLabel(tableMeta);

	for(unsigned int i=0; i<tableMeta->numCandidate; i++)
	{
		if(aryClass[i]==0)	aryCost[i] = intCostValue[0];
		else				aryCost[i] = intCostValue[1];
	}

	rewind(tableMeta->temp->cost);
	fwrite(aryCost,sizeof(double),tableMeta->numCandidate,tableMeta->temp->cost);

	delete [] intCostValue;
	delete [] aryCost;
	delete [] aryClass;

	return PR_GOOD;
}

int 
PR_EvaluateWeightedMinErrorOfFeatureThresholding_Temp(
	PR_ARRAYCASEDATATEMP* tableMeta, unsigned int idxUseFeature, unsigned int* aryRank, 
	unsigned char* classLabel, double* dataWeight,
	PR_BOOSTWEAKCLASSIFIERPARAMS* param,
	char modeEvaluation
){
	unsigned int i;
	
	float* aryFeature = PR_ExtractFullArrayOfOneFeature(tableMeta,idxUseFeature);
	unsigned int numCandidate = tableMeta->numCandidate;

//	unsigned int* rankAscend = new unsigned int [numCandidate];
//	rewind(tempRank);
//	fread(rankAscend,sizeof(unsigned int),numCandidate,tempRank);
//	rewind(tempRank);
	unsigned int* rankAscend = aryRank;

	/*
	if(idxUseFeature%200==0)
	{
		printf("\n>feat[%d];",idxUseFeature);
		for(i=0; i<numCandidate; i++)
			if(i%50==0)	printf("[%d]%.3e, ",i,aryFeature[i]);
		printf("\n>rank[%d];",idxUseFeature);
		for(i=0; i<numCandidate; i++)
			if(i%50==0)	printf("[%d]%d, ",i,rankAscend[i]);
		printf("\n>feat[%d];",idxUseFeature);
		for(i=0; i<numCandidate; i++)
			if(i%50==0)	printf("[%d]%.3e, ",i,dataWeight[i]);
	}
	*/


   ///-- get sum weight of TP and FP --///
	double sumWeightTP=0.0, sumWeightFP=0.0;
	for(i=0; i<numCandidate; i++)
	{
		if(classLabel[i])	sumWeightTP += dataWeight[i];
		else				sumWeightFP += dataWeight[i];
	}

	
	param->mode = 0;
	param->trainError = 2.0;
	PR_CopyFeatTag(param->featureTag, tableMeta->tableFeature->tags[idxUseFeature]);
	

	unsigned char clsCand;
	unsigned int iCand, iCandNext, iCandPrev;
	double dmySumWgtFN=10.0, dmySumWgtFP=10.0, dmyWeight;


   ///-- high-pass --///
//	printf("High\n");
	dmySumWgtFN=sumWeightTP;
	dmySumWgtFP=0.0;

	if(modeEvaluation==0 || modeEvaluation==+1)
	{
		for(i=numCandidate-1; i>0; i--)
		{
			iCand = rankAscend[i];
			iCandPrev = rankAscend[i-1];
			clsCand = classLabel[iCand];
			dmyWeight = dataWeight[iCand];

			if(clsCand)		dmySumWgtFN -= dmyWeight;
			else			dmySumWgtFP += dmyWeight;

			if(aryFeature[iCand]!=aryFeature[iCandPrev] && param->trainError>dmySumWgtFP+dmySumWgtFN)
			{
				param->mode = +1;
				param->threshold = (aryFeature[iCand]+aryFeature[iCandPrev])*0.5f;
				param->trainError = dmySumWgtFP+dmySumWgtFN;
			}
		}
	}


   ///-- low-pass --///
//	printf("Low\n");
	dmySumWgtFN=sumWeightTP;
	dmySumWgtFP=0.0;

	if(modeEvaluation==0 || modeEvaluation==-1)
	for(i=0; i<numCandidate-1; i++)
	{
		iCand = rankAscend[i];
		iCandNext = rankAscend[i+1];
		clsCand = classLabel[iCand];
		dmyWeight = dataWeight[iCand];
		
		if(clsCand)		dmySumWgtFN -= dmyWeight;
		else			dmySumWgtFP += dmyWeight;

		if(aryFeature[iCand]!=aryFeature[iCandNext] && param->trainError>dmySumWgtFP+dmySumWgtFN)
		{
			param->mode = -1;
			param->threshold = (aryFeature[iCand]+aryFeature[iCandNext])*0.5f;
			param->trainError = dmySumWgtFP+dmySumWgtFN;
		}
	}

	delete [] aryFeature;
//	delete [] rankAscend;

	return PR_OK;
}



int 
PR_GetWeightOfWeakClassifier_Temp(
	PR_ARRAYCASEDATATEMP* tableMeta, double* aryOutEnsemble, char* aryOutWeak, unsigned char* classLabel,
	PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier, int channelBoost, double eta
){
	int numIteration=10000;
	unsigned int numCandidate=tableMeta->numCandidate;
	double error=weakClassifier->trainError;
	double alpha=0.5*log((1.0-error)/error), min=1.0e-100, max=alpha*2.0;
//	printf("min%.3e~%.3e~max%.3e\n",min,alpha,max);
//	printf("\n");

	PR_ARRAYCANDIDATEFORTRAININGBYTEMP* ary;
	if(channelBoost!=PR_MODELCODE_ADABOOST)
	{
		ary = new PR_ARRAYCANDIDATEFORTRAININGBYTEMP;
		ary->classLabel = classLabel;
		ary->cost = PR_ExtractFullArrayOfCost(tableMeta);
		ary->num = tableMeta->numCandidate;
		ary->outEnsemble = aryOutEnsemble;
		ary->outWeakClassifier = aryOutWeak;
		
		if(channelBoost==PR_MODELCODE_ETABOOST
			|| channelBoost==PR_MODELCODE_ROBUSTETABOOST)	ary->paramEta = eta;
		else												ary->paramEta = 0.0;
	}

	switch(channelBoost)
	{
	case PR_MODELCODE_ADABOOST:
	//	weakClassifier->nodeWeight = 0.5*log((1.0-error)/error);
		weakClassifier->nodeWeight = alpha;
		break;
	case PR_MODELCODE_MADABOOST:
//		printf("    Fmada(%.3e)=%le..",alpha,PR_OutputSumOfLossMadaBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossMadaBoost_Temp,ary,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Fmada(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossMadaBoost(weakClassifier->nodeWeight,aryCand));
		break;
	case PR_MODELCODE_LOGITBOOST:
//		printf("    Flog(%.3e)=%le..",alpha,PR_OutputSumOfLossLogitBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossLogitBoost_Temp,ary,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Flog(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossLogitBoost(weakClassifier->nodeWeight,aryCand));
		break;
	case PR_MODELCODE_ETABOOST:
//		printf("    Feta(%.3e)=%le..",alpha,PR_OutputSumOfLossEtaBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossEtaBoost_Temp,ary,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Feta(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossEtaBoost(weakClassifier->nodeWeight,aryCand));
		break;
	case PR_MODELCODE_ROBUSTETABOOST:
//		printf("    Freta(%.3e)=%le..",alpha,PR_OutputSumOfLossMostBRobustEtaBoost(alpha,aryCand));
		PR_BrentMethodGetArgMinForFunction(min,alpha,max,PR_OutputSumOfLossMostBRobustEtaBoost_Temp,ary,
										   numIteration,&weakClassifier->nodeWeight);
//		printf(" Freta(%.3e)=%le\n",weakClassifier->nodeWeight,PR_OutputSumOfLossMostBRobustEtaBoost(weakClassifier->nodeWeight,aryCand));
		break;
	default:
		fprintf(stderr,"error boostChannel=%d\n",channelBoost);
		return PR_NG;
	}

	if(channelBoost!=PR_MODELCODE_ADABOOST)
	{
		delete [] ary->cost;
		delete ary;
	}
	
	return PR_OK;
}


double
PR_OutputSumOfLossMadaBoost_Temp(double alpha, void* option)
{
	unsigned char* classLabel = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->classLabel;
	double* cost = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->cost;
	double* outEnsemble = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outEnsemble;
	char* outWeak = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outWeakClassifier;

	unsigned int num = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->num;

	unsigned int i;
	double dOutWeak, z, loss, sum=0.0;
	for(i=0; i<num; i++)
	{
		dOutWeak = (double)outWeak[i];
		if(classLabel[i])	z = -outEnsemble[i]-alpha*dOutWeak;
		else				z = outEnsemble[i]+alpha*dOutWeak;

		if(z>=0.0)	loss = z;
		else		loss = 0.5*exp(2.0*z)-0.5;
		if(cost!=NULL && cost[i]!=1.0)	loss *= cost[i];

		sum += loss;
	}

	return sum/(double)num;
}

double
PR_OutputSumOfLossLogitBoost_Temp(double alpha, void* option)
{
	unsigned char* classLabel = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->classLabel;
	double* cost = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->cost;
	double* outEnsemble = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outEnsemble;
	char* outWeak = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outWeakClassifier;

	unsigned int num = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->num;

	double dOutWeak, z, loss, sum=0.0;
	for(unsigned int i=0; i<num; i++)
	{
		dOutWeak = (double)outWeak[i];
		if(classLabel[i])	z = -outEnsemble[i]-alpha*dOutWeak;
		else				z = outEnsemble[i]+alpha*dOutWeak;

		loss = log(1.0+exp(2.0*z));
		if(cost!=NULL && cost[i]!=1.0)	loss *= cost[i];

		sum += loss;
	}
	return sum/(double)num;
}

double
PR_OutputSumOfLossEtaBoost_Temp(double alpha, void* option)
{
	unsigned char* classLabel = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->classLabel;
	double* cost = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->cost;
	double* outEnsemble = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outEnsemble;
	char* outWeak = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outWeakClassifier;

	unsigned int num = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->num;

	double eta = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->paramEta;
	double etaS = 1.0-eta;

	double dOutWeak, z, loss, sum=0.0;
	for(unsigned int i=0; i<num; i++)
	{
		dOutWeak = (double)outWeak[i];
		if(classLabel[i])	z = -outEnsemble[i]-alpha*dOutWeak;
		else				z = outEnsemble[i]+alpha*dOutWeak;

		loss = etaS*exp(z)+eta*z;
		if(cost!=NULL && cost[i]!=1.0)	loss *= cost[i];

		sum += loss;
	}
	return sum/(double)num;
}

double
PR_OutputSumOfLossMostBRobustEtaBoost_Temp(double alpha, void* option)
{
	unsigned char* classLabel = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->classLabel;
	double* cost = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->cost;
	double* outEnsemble = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outEnsemble;
	char* outWeak = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->outWeakClassifier;

	unsigned int num = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->num;

	double eta = ((PR_ARRAYCANDIDATEFORTRAININGBYTEMP*)option)->paramEta;
	double etaS = 1.0-eta, eta2p=eta*eta, rLog=2.0*eta-1.0;

	double dOutWeak, z, zExpPlus, loss, sum=0.0;

	for(unsigned int i=0; i<num; i++)
	{
		dOutWeak = (double)outWeak[i];
		if(classLabel[i])	z = -outEnsemble[i]-alpha*dOutWeak;
		else				z = outEnsemble[i]+alpha*dOutWeak;

		if(z>=0.0)
			loss = z;
		else
		{
			zExpPlus = (exp(z)-1.0)*eta;
			loss = (etaS*zExpPlus+rLog*log(1.0+zExpPlus))/eta2p;
		}
		if(cost!=NULL && cost[i]!=1.0)	loss *= cost[i];

		sum += loss;
	}
	return sum/(double)num;
}

int 
UpdateWeightOfCandidateByObtainedEnsembleOutput_Temp(
	double* dataWeight, double* outEnsemble, unsigned char* classLabel, unsigned int numCandidate, 
	int channelBoost, double eta
){
//	PR_CANDDATA** arrayCandidate = args->arrayCandidate;
//	unsigned int num = args->num;

	double z, z2Exp;
//	PR_BOOSTOPTION* option;

	for(unsigned int i=0; i<numCandidate; i++)
	{
		if(classLabel[i])	z = -outEnsemble[i];
		else				z = outEnsemble[i];
	//	option = (PR_BOOSTOPTION*)(arrayCandidate[i]->optionParam);

		switch(channelBoost)
		{
		case PR_MODELCODE_ADABOOST:
			dataWeight[i] = exp(z);
			break;
		case PR_MODELCODE_MADABOOST:
			if(z>=0.0)		dataWeight[i] = 1.0;
			else			dataWeight[i] = exp(2.0*z);
			break;
		case PR_MODELCODE_LOGITBOOST:
			z2Exp = exp(2.0*z);
			dataWeight[i] = 2.0*z2Exp/(1.0+z2Exp);
			break;
		case PR_MODELCODE_ETABOOST:
			dataWeight[i] = (1.0-eta)*exp(z)+eta;
			break;
		case PR_MODELCODE_ROBUSTETABOOST:
			if(z>=0.0)		dataWeight[i] = 1.0;
			else			dataWeight[i] = ((1.0-eta)*exp(z)+eta)/((1.0-eta)*exp(-z)+eta);
			break;
		default:
			fprintf(stderr,"error boostChannel=%d\n",channelBoost);
			return PR_NG;
		}
	}

	return PR_OK;
}


char*
PR_CalculateOutputOfWeakClassifier_Temp(PR_ARRAYCASEDATATEMP* tableMeta, PR_BOOSTWEAKCLASSIFIERPARAMS* weakClassifier)
{
	unsigned int fidx = PR_ExtractFeatNumberInFeatTagTable(tableMeta->tableFeature,weakClassifier->featureTag);
	float* feature = PR_ExtractFullArrayOfOneFeature(tableMeta,fidx);
	if(feature==NULL)
	{
		printf("error: PR_CalculateOutputOfWeakClassifier_Temp;");
		return NULL;
	}
	
	unsigned int i, num = tableMeta->numCandidate;
	float threshold = weakClassifier->threshold;
	char* output = new char [num];
	for(i=0; i<num; i++)
	{
		if(weakClassifier->mode==-1)
		{
			if(feature[i]<=threshold)	output[i] = 1;
			else						output[i] = -1;
		}
		else
		{
			if(feature[i]>=threshold)	output[i] = 1;
			else						output[i] = -1;
		}
	}

	delete [] feature;
	return output;
}


int 
PR_CalcEnsembleOutputOfCaseDataTemp(PR_CASEDATATEMP* dataTemp, PR_ENSEMBLE* ensemble)
{
	PR_ARRAYCASEDATATEMP* root = (PR_ARRAYCASEDATATEMP*)dataTemp->rootTable;
	if(root->temp->likelihood[1]==NULL)
	{
		printf("error: PR_CalcEnsembleOutputOfCaseDataTemp\n");
		return PR_NG;
	}

	unsigned int nc,nf,nw;

	PR_FEATTAGTABLE* ftable = root->tableFeature;
	PR_ENSEMBLENODE* node;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param;

	float* aryFeature;// = new float [dataTemp->numCandidate];
	double* aryOutput = new double [dataTemp->numCandidate];
	memset(aryOutput,0x00,sizeof(double)*dataTemp->numCandidate);
	
	for(nf=0; nf<ftable->numFeat; nf++)
	{
	//	printf("#%d(%04x.%04x);",nf,ftable->tags[nf]->group,ftable->tags[nf]->element);
		aryFeature = PR_ExtractOneCaseArrayOfOneFeature(dataTemp,nf);

		node=ensemble->firstNode;
		for(nw=0; nw<ensemble->numNode; nw++)
		{
			param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)node->data;
			if(PR_CompareFeatTag(param->featureTag,ftable->tags[nf])!=PR_NG)
			{
	//			printf("@node%d(w%.3f) ",nw+1,param->nodeWeight);
				for(nc=0; nc<dataTemp->numCandidate; nc++)
				{
	//				if(nc==0)	printf("#%d feat[%d]=%.3e; out%.3e",nc,nf,aryFeature[nc],aryOutput[nc]);
					if(param->mode==1)
					{
						if(aryFeature[nc]>=param->threshold)	aryOutput[nc]+=param->nodeWeight;
						else									aryOutput[nc]-=param->nodeWeight;
					}
					else
					{
						if(aryFeature[nc]<=param->threshold)	aryOutput[nc]+=param->nodeWeight;
						else									aryOutput[nc]-=param->nodeWeight;
					}
	//				if(nc==0)	printf("->%.3e\n",aryOutput[nc]);
				}
			}
			node=node->next;
		}
	//	printf("\n");
		delete [] aryFeature;
	}

	double sumWeight=0.0;
	node=ensemble->firstNode;
	for(nw=0; nw<ensemble->numNode; nw++)
	{
		sumWeight += ((PR_BOOSTWEAKCLASSIFIERPARAMS*)node->data)->nodeWeight;
		node = node->next;
	}
//	printf("sumW%.3f,",sumWeight);

	for(nc=0; nc<dataTemp->numCandidate; nc++)		aryOutput[nc]/=sumWeight;
//	printf("margin,");

	
//	printf("cls%d,",root->numClass);
	unsigned int addsum=0;
	double* dmylhod = PR_ExtractFullArrayOfLikelihood(root,1);
	if(dmylhod==NULL)	dmylhod = new double [root->numCandidate];
//	printf("setLhod,");

	for(nc=0; nc<root->numCase; nc++)
	{
//		printf("[%d]sum%d,",nc,addsum);
		if(root->arrayCase[nc]==dataTemp)
		{
			for(unsigned int i=0; i<dataTemp->numCandidate; i++)
				dmylhod[addsum+i] = aryOutput[i];
			break;
		}
		addsum += root->arrayCase[nc]->numCandidate;
	}
//	printf("ary,");

	fclose(root->temp->likelihood[1]);
	root->temp->likelihood[1] = tmpfile();
	fwrite(dmylhod,sizeof(double),root->numCandidate,root->temp->likelihood[1]);
//	printf("rewrite\n");

	delete [] dmylhod;
	delete [] aryOutput;

	return PR_OK;
}

int 
PR_CalcEnsembleOutputOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* aryData, PR_ENSEMBLE* ensemble)
{
	if(aryData->temp->likelihood[1]==NULL)
	{
		printf("error: PR_CalcEnsembleOutputOfCaseDataTemp\n");
		return PR_NG;
	}

	for(unsigned int i=0; i<aryData->numCase; i++)
	{
		PR_CalcEnsembleOutputOfCaseDataTemp(aryData->arrayCase[i], ensemble);
	}

	return PR_OK;
}


/*
int 
PR_CalcEnsembleOutputOfArrayCaseDataTemp(PR_ARRAYCASEDATATEMP* data, PR_ENSEMBLE* ensemble)
{
	if(data->temp->likelihood[1]==NULL)
	{
		printf("error: PR_CalcEnsembleOutputOfCaseDataTemp\n");
		return PR_NG;
	}

	unsigned int i, j, k;
	double weight;

	PR_FEATTAGTABLE* ftable = data->tableFeature;
	PR_ENSEMBLENODE* node;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param;

	float* aryFeature;// = new float [dataTemp->numCandidate];
	double* aryOutput = new double [data->numCandidate];
	memset(aryOutput,0x00,sizeof(double)*data->numCandidate);
	
	for(k=0; k<ftable->numFeat; k++)
	{
		aryFeature = PR_ExtractFullArrayOfOneFeature(data,k);
		node=ensemble->firstNode;

		for(j=0; j<ensemble->numNode; j++)
		{
			param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)node->data;
			weight = ((PR_BOOSTWEAKCLASSIFIERPARAMS*)node->data)->nodeWeight;

			if(PR_CompareFeatTag(param->featureTag,ftable->tags[k])!=PR_NG)
			{
				for(i=0; i<data->numCandidate; i++)
				{
					if(param->mode==1)
					{
						if(aryFeature[i]>=param->threshold)		aryOutput[i] += weight;
						else									aryOutput[i] -= weight;
					}
					else
					{
						if(aryFeature[i]<=param->threshold)		aryOutput[i] += weight;
						else									aryOutput[i] -= weight;
					}
				}
			}

			node=node->next;
		}

		delete [] aryFeature;
	}

	fclose(data->temp->likelihood[1]);
	data->temp->likelihood[1] = tmpfile();
	fwrite(aryOutput,sizeof(double),data->numCandidate,data->temp->likelihood[1]);
	
	delete [] aryOutput;
	return PR_OK;
}
*/

