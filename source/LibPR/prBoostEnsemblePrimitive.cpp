
#pragma warning(disable: 4996)


#include "prBoostEnsemblePrimitive.h"
#include "prBoostEnsemblePrimitive.private.h"

#include "../LibPR.h"


PR_BOOSTWEAKCLASSIFIERPARAMS* PR_NewBoostWeakClassifierParams()
{
//	printf("in");

	PR_BOOSTWEAKCLASSIFIERPARAMS* nodeParam = new PR_BOOSTWEAKCLASSIFIERPARAMS;
	if(nodeParam==NULL)		return NULL;
//	printf("1");

	nodeParam->featureTag = new PR_FEATTAG;
	if(nodeParam->featureTag==NULL)
	{
		delete nodeParam;
		return NULL;
	}
//	printf("2");

	nodeParam->featureTag->group = 0xffff;
	nodeParam->featureTag->element = 0xffff;
//	printf("3");

	nodeParam->mode = 0;
	nodeParam->nodeIndex = 0;
	nodeParam->nodeWeight = 0.0;
	nodeParam->threshold = 0.0;
	nodeParam->trainError = 10.0;
//	printf("4");

	return nodeParam;
}

PR_BOOSTWEAKCLASSIFIERPARAMS* PR_NewAddBoostWeakClassifierParams(PR_ENSEMBLENODE* node)
{
	if(node==NULL)	return NULL;

	PR_BOOSTWEAKCLASSIFIERPARAMS* nodeParam;
	nodeParam = PR_NewBoostWeakClassifierParams();
	node->data = nodeParam;

	return nodeParam;
}

void PR_DeleteBoostWeakClassifierParams(PR_BOOSTWEAKCLASSIFIERPARAMS* nodeParam)
{
	if(nodeParam==NULL)
	{
		printf("null nodeParam");		getchar();
		return;
	}
	else if(nodeParam->featureTag==NULL)
	{
		printf("null featureTag");		getchar();
		return;
	}

	delete nodeParam->featureTag;
	delete nodeParam;
//	printf("delete complete1!\n");
}

int PR_CopyBoostWeakClassifierParams(PR_BOOSTWEAKCLASSIFIERPARAMS* dest, PR_BOOSTWEAKCLASSIFIERPARAMS* src)
{
	PR_CopyFeatTag(dest->featureTag,src->featureTag);
	dest->mode = src->mode;
	dest->nodeIndex = src->nodeIndex;
	dest->nodeWeight = src->nodeWeight;
	dest->threshold = src->threshold;
	dest->trainError = src->trainError;
	return PR_EOT;
}

int PR_DeleteEnsembleNodeWithBoostWeakClassifierParams(PR_ENSEMBLENODE* node)
{
	if(node==NULL)
	{
//		printf("null node");		getchar();
		return PR_EOT;
	}

	PR_BOOSTWEAKCLASSIFIERPARAMS* nodeParam = (PR_BOOSTWEAKCLASSIFIERPARAMS*)node->data;
//	printf("%d[%04x.%04x],",nodeParam->nodeIndex,nodeParam->featureTag->group,nodeParam->featureTag->element);

	delete nodeParam->featureTag;	//	printf("tag,");
	delete nodeParam;				//	printf("nodeParam\n");
	node->data = NULL;

	int r=PR_DeleteEnsembleNode(node);
//	printf("node!\n");
	return r;
}



PR_ENSEMBLE* PR_NewBoostEnsemble(unsigned int numNode)
{
	PR_ENSEMBLE* ensemble = PR_NewEnsembleWithNode(numNode);
	PR_NewBoostWeakClassifierParamsAddToEnsemble(ensemble);
	return ensemble;
}

int PR_DeleteBoostEnsemble(PR_ENSEMBLE* ensemble)
{
	if(ensemble==NULL)	return PR_EOT;

	if(ensemble->data!=NULL)
		return PR_ErrorMessageInt("remain ensemble->data (@PR_DeleteEnsemble)");

	int resReturn = PR_EOT;
	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_ENSEMBLENODE* nextNode = currentNode->next;
	int cnt=1;

	while(currentNode!=NULL)
	{
	//	printf("cnt%d(%d)\n",cnt++,currentNode);

		int resCheck = PR_DeleteEnsembleNodeWithBoostWeakClassifierParams(currentNode);
		if(resCheck==PR_AVEND)
			return PR_ErrorMessageInt("cannot delete ENSEMBLENODE (@PR_DeleteEnsemble)");

		if(resReturn>resCheck)	resReturn=resCheck;

		currentNode=ensemble->firstNode;
	}

	return resReturn;
}



int PR_SaveTextBoostEnsemble(PR_ENSEMBLE* ensemble, char* filename)
{
	if(PR_CheckNumEnsembleNode(ensemble)==PR_AVEND)
	{
		return PR_ErrorMessageInt("numNode error (@PR_SaveTextBoostEnsemble)");
	}

	FILE* fp = fopen(filename,"w");
	if(fp==NULL)
	{
		return PR_ErrorMessageInt("cannot open fp (@PR_SaveTextBoostEnsemble)");
	}


	fprintf(fp,"PR_BoostEnsemble\n");
	fprintf(fp,"%lu\n",ensemble->numNode);


	PR_ENSEMBLENODE* current = ensemble->firstNode;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param;

	for(unsigned int nmodel=0; nmodel<ensemble->numNode; nmodel++)
	{
		param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)current->data;

		fprintf(fp,"%lu ",param->nodeIndex);
		fprintf(fp,"%04x ",param->featureTag->group);
		fprintf(fp,"%04x ",param->featureTag->element);
		fprintf(fp,"%d ",param->mode);
		fprintf(fp,"%e ",param->threshold);
		fprintf(fp,"%le ",param->trainError);
		fprintf(fp,"%le\n",param->nodeWeight);

		current = current->next;
	}
	
	fclose(fp);
	return PR_EOT;
}

int PR_SaveBinaryBoostEnsemble(PR_ENSEMBLE* ensemble, char* filename)
{
	if(PR_CheckNumEnsembleNode(ensemble)==PR_AVEND)
		return PR_ErrorMessageInt("numNode error (@PR_SaveEnsembleWithDiscreteAdaBoostTxt)");

	FILE* fp = fopen(filename,"wb");
	if(fp==NULL)	return PR_ErrorMessageInt("cannot open fp (@PR_SaveEnsembleWithDiscreteAdaBoostTxt)");

	char Identifier[32];
	sprintf(Identifier,"PR_DiscreteAdaBoostEnsembleData");
	fwrite(Identifier, sizeof(char), 32, fp);

	fwrite(&ensemble->numNode, sizeof(unsigned int), 1, fp);

	PR_ENSEMBLENODE* current = ensemble->firstNode;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param;

	for(unsigned int nmodel=0; nmodel<ensemble->numNode; nmodel++)
	{
		param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)current->data;
		fwrite(&param->nodeIndex, sizeof(unsigned int), 1, fp);
		fwrite(&param->featureTag->group, sizeof(unsigned short), 1, fp);
		fwrite(&param->featureTag->element, sizeof(unsigned short), 1, fp);
		fwrite(&param->mode, sizeof(char), 1, fp);
		fwrite(&param->threshold, sizeof(float), 1, fp);
		fwrite(&param->trainError, sizeof(double), 1, fp);
		fwrite(&param->nodeWeight, sizeof(double), 1, fp);
		current = current->next;
	}
	
	fclose(fp);
	return PR_EOT;
}


PR_ENSEMBLE* PR_ReadTextBoostEnsemble(char* filename)
{
	
	FILE* fp=fopen(filename,"r");
	
	if(fp==NULL)
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL("fileName error (@PR_ReadTextBoostEnsemble)");

	char strdmy[256];
	fscanf(fp,"%s",strdmy);
	int res1 = strcmp(strdmy,"PR_DiscreteAdaBoostEnsembleData");
	int res2 = strcmp(strdmy,"PR_BoostEnsemble");

	if(res1!=0 && res2!=0)
	{
		sprintf(strdmy,"Not Boost-Ensemble; %s (@PR_ReadTextBoostEnsemble)",filename);
		fclose(fp);
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL(strdmy);
	}

	unsigned int numModel;
	fscanf(fp,"%lu\n",&numModel);

	PR_ENSEMBLE* ensemble = PR_NewEnsembleWithNode(numModel);
	PR_NewBoostWeakClassifierParamsAddToEnsemble(ensemble);

	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;


	char dmy_str[32];

	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param;

	for(unsigned int cnt=0; cnt<numModel; cnt++)
	{
		if(currentNode==NULL)
			currentNode = PR_NewNodeAddToEnsembleLast(ensemble);

		if(currentNode==NULL)
		{
			fclose(fp);
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create ensembleNode (@PR_ReadTextDiscreteAdaBoostEnsemble)");
		}

		param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentNode->data;
		if(param==NULL)
			param = PR_NewAddBoostWeakClassifierParams(currentNode);

		if(param==NULL)
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create nodeParam (@PR_ReadTextDiscreteAdaBoostEnsemble)");

		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%lu",&param->nodeIndex);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%04x",&param->featureTag->group);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%04x",&param->featureTag->element);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%d",&param->mode);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%e",&param->threshold);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%le",&param->trainError);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%le",&param->nodeWeight);

		currentNode = currentNode->next;
	}

	fclose(fp);
	return ensemble;

}

PR_ENSEMBLE* PR_ReadBinaryBoostEnsemble(char* filename)
{
	FILE* fp=fopen(filename,"rb");
	
	if(fp==NULL)
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL("fileName error (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");

	char Identifier[32];
	fread(Identifier,sizeof(char),32,fp);
	if(strcmp(Identifier,"PR_DiscreteAdaBoostEnsembleData")!=0)
	{
		char strdmy[64];
		sprintf(strdmy,"%s is not for DiscreteAdaBoost (@PR_ReadBinaryDiscreteAdaBoostEnsemble)",filename);
		fclose(fp);
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL(strdmy);
	}
	
	unsigned int numModel;
	fread(&numModel,sizeof(unsigned int),1,fp);

	PR_ENSEMBLE* ensemble = PR_NewEnsembleWithNode(numModel);
	PR_NewBoostWeakClassifierParamsAddToEnsemble(ensemble);
	
	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;
	
	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param;

	for(unsigned int cnt=0; cnt<numModel; cnt++)
	{
		if(currentNode==NULL)
			currentNode = PR_NewNodeAddToEnsembleLast(ensemble);

		if(currentNode==NULL)
		{
			fclose(fp);
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create ensembleNode (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");
		}

		param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)currentNode->data;
		if(param==NULL)
			param = PR_NewAddBoostWeakClassifierParams(currentNode);

		if(param==NULL)
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create nodeParam (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");

		int aaa=0;
		aaa += (int)fread(&param->nodeIndex, sizeof(unsigned int), 1, fp);
		aaa += (int)fread(&param->featureTag->group, sizeof(unsigned short), 1, fp);
		aaa += (int)fread(&param->featureTag->element, sizeof(unsigned short), 1, fp);
		aaa += (int)fread(&param->mode, sizeof(char), 1, fp);
		aaa += (int)fread(&param->threshold, sizeof(float), 1, fp);
		aaa += (int)fread(&param->trainError, sizeof(double), 1, fp);
		aaa += (int)fread(&param->nodeWeight, sizeof(double), 1, fp);

		if(aaa<7)
		{
			fclose(fp);
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("file error (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");
		}

		currentNode=currentNode->next;
	}

	fclose(fp);
	return ensemble;
}



int PR_NewBoostWeakClassifierParamsAddToEnsemble(PR_ENSEMBLE* ensemble)
{
	if(ensemble==NULL)	return PR_AVEND;

	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;

	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	unsigned int numNode = 0;
	
	while(currentNode!=NULL)
	{
		PR_BOOSTWEAKCLASSIFIERPARAMS* nodeParam;
		nodeParam = PR_NewAddBoostWeakClassifierParams(currentNode);
		if(nodeParam==NULL)		return PR_AVEND;

		PR_ENSEMBLENODE* dmy = currentNode->next;
		currentNode = dmy;
		numNode++;
	}

	if(numNode!=ensemble->numNode)
	{
		ensemble->numNode = numNode;
		return PR_EOT;
	}

	return PR_EOT;
}



PR_BOOSTOPTION* PR_NewAddBoostOptionToCand(PR_CANDDATA* candData)
{
	if(candData==NULL)	return NULL;
	PR_BOOSTOPTION* opt = new PR_BOOSTOPTION;
	candData->optionParam = opt;
	opt->dataWeight=1.0;
	opt->cost=1.0;
//	opt->onProcCost=1.0;
	return opt;
}

int PR_NewAddBoostOptionToAllCands(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return 1;

	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			candData->optionParam = PR_NewAddBoostOptionToCand(candData);
			candData = candData->nodeNextCand; 
		}
		caseData = caseData->nodeNextCase;
	}

	return 0;
}

void PR_DeleteBoostOptionOfCand(PR_CANDDATA* candData)
{
	if(candData==NULL || candData->optionParam==NULL)	return;
	delete (PR_BOOSTOPTION*)(candData->optionParam);
	candData->optionParam = NULL;
}

void PR_DeleteBoostOptionInAllCands(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return;

	PR_CASEDATA* casedata = caseList->childNodeFirstCase;
	PR_CANDDATA* canddata;

	while(casedata!=NULL)
	{
		canddata = casedata->childNodeFirstCand;
		while(canddata!=NULL)
		{
			PR_DeleteBoostOptionOfCand(canddata);
			canddata= canddata->nodeNextCand;
		}
		casedata = casedata->nodeNextCase;
	}
}


int PR_InitializeCostBoost(PR_CASELIST* caseList, int mode)
{
	if(caseList==NULL || (mode!=1 && mode!=2))	return 1;

	PR_CountNumCandsForCaseList(caseList);
	
	float* intCostValue = new float [2];
	if(mode==1)
	{
		intCostValue[0] = intCostValue[1] = 1.0;
	}
	else
	{
		intCostValue[0] = (float)(0.5/(double)caseList->totalNumCandOfClass[0]);
		intCostValue[1] = (float)(0.5/(double)(caseList->totalNumCand-caseList->totalNumCandOfClass[0]));
	}
	
	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	PR_CANDDATA* currentCand;

	double* sumPreCost = new double [2];
	sumPreCost[0] = sumPreCost[1] = 0.0;

	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;
		while(currentCand!=NULL)
		{
			unsigned char cls = currentCand->classLabel;
			if(cls>1)	cls = 1;

			PR_BOOSTOPTION* opt = (PR_BOOSTOPTION*)currentCand->optionParam;
			
			if(opt==NULL)
			{
				printf("error; unexist BOOSTOPTION!!\n");
				getchar();
				return PR_AVEND;
			}

			opt->cost = intCostValue[cls];
			sumPreCost[cls] += opt->cost;

			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}

	delete [] intCostValue;
	delete [] sumPreCost;

	return PR_EOT;
}

/*
int PR_ReguralizeCostBoost(PR_CASELIST* caseList)
{
	if(caseList==NULL)		return PR_AVEND;

	double sumDataWeight=0.0;
	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	PR_CANDDATA* currentCand;
	PR_BOOSTOPTION* pParam;

	while(currentCase!=NULL)
	{
		currentCand = currentCase->childNodeFirstCand;
		double sumDmy=0.0;

		while(currentCand!=NULL)
		{
			if(currentCand->optionParam==NULL)		return PR_AVEND;

			pParam = (PR_BOOSTOPTION*)currentCand->optionParam;
			sumDmy += pParam->dataWeight;

			PR_CANDDATA* dmyCand = currentCand;
			currentCand = dmyCand->nodeNextCand;
		}

		PR_CASEDATA* dmyCase = currentCase;
		currentCase = dmyCase->nodeNextCase;
		sumDataWeight += sumDmy;
	}

	if(sumDataWeight!=1.0)
	{
		while(currentCase!=NULL)
		{
			currentCand = currentCase->childNodeFirstCand;

			while(currentCand!=NULL)
			{
				pParam = (PR_BOOSTOPTION*)currentCand->optionParam;
				pParam->dataWeight /= sumDataWeight;

				PR_CANDDATA* dmyCand = currentCand;
				currentCand = dmyCand->nodeNextCand;
			}

			PR_CASEDATA* dmyCase = currentCase;
			currentCase = dmyCase->nodeNextCase;
		}
	}

	return PR_EOT;
}
*/


int PR_CalcOutputBoostEnsembleForCand(PR_ENSEMBLE* ensemble, PR_CANDDATA* candData)
{
	if(candData==NULL || ensemble==NULL)								return PR_AVEND;
	if(candData->rootCaseData==NULL)									return PR_AVEND;
	if(((PR_CASEDATA*)(candData->rootCaseData))->rootCaseList==NULL)	return PR_AVEND;
	if(ensemble->codeEnsembleType!=PR_ENSEMBLETYPE_DISCRETEADABOOST)	return PR_AVEND;
	
	int res;
	unsigned int nfeat, targetFeat;
	PR_CASELIST* root = ((PR_CASELIST*)((PR_CASEDATA*)(candData->rootCaseData))->rootCaseList);
	float fvalue;
	double sumOutput = 0.0;

	PR_ENSEMBLENODE* current;
	current = ensemble->firstNode;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param = ((PR_BOOSTWEAKCLASSIFIERPARAMS*)(ensemble->firstNode->data));

	for(nfeat=0; nfeat<ensemble->numNode; nfeat++) {
		param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)(current->data);
		res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(param->featureTag, root->featTagTable, &targetFeat);
		if(res==PR_AVEND)	return PR_AVEND;
		fvalue = candData->featValues[targetFeat];

		if(ensemble->numNode==1) {
			sumOutput = (double)(fvalue-param->threshold);
			if(param->mode==-1)		sumOutput *= -1.0;
		} else {
			if(param->mode==-1) {
				if(fvalue<=param->threshold)	sumOutput += param->nodeWeight;
				else							sumOutput -= param->nodeWeight;
			} else {
				if(fvalue>=param->threshold)	sumOutput += param->nodeWeight;
				else							sumOutput -= param->nodeWeight;
			}
		}
		current = current->next;
	}

	candData->likelihood[1] = sumOutput;
	candData->likelihood[0] = -1.0 * candData->likelihood[1];

	return PR_EOT;
}

int PR_CalcOutputBoostEnsembleForCase(PR_ENSEMBLE* ensemble, PR_CASEDATA* caseData)
{
	if(caseData==NULL || ensemble==NULL)	return PR_AVEND;

	PR_CANDDATA* current = caseData->childNodeFirstCand;
	PR_CANDDATA* dmy;
	
	while(current!=NULL)
	{
		int res = PR_CalcOutputBoostEnsembleForCand(ensemble, current);
		if(res==PR_AVEND)	return PR_AVEND;
		dmy = current->nodeNextCand;
		current = dmy;
	}

	return PR_EOT;
}

int PR_CalcOutputBoostEnsembleForCases(PR_ENSEMBLE* ensemble, PR_CASELIST* caseList)
{
	if(caseList==NULL || ensemble==NULL)	return PR_AVEND;

	PR_CASEDATA* current = caseList->childNodeFirstCase;
	PR_CASEDATA* dmy;

	while(current!=NULL)
	{
		int res = PR_CalcOutputBoostEnsembleForCase(ensemble, current);
		if(res==PR_AVEND)	return PR_AVEND;
		dmy = current->nodeNextCase;
		current = dmy;
	}

	return PR_EOT;
}


double PR_CalcSumOfWeightForWeakClassifiersBoost(PR_ENSEMBLE* ensemble)
{
	double wsumModel=0.0;
	PR_ENSEMBLENODE* enode = ensemble->firstNode;
	PR_BOOSTWEAKCLASSIFIERPARAMS* param;

	for(unsigned int i=0; i<ensemble->numNode; i++)
	{
		param = (PR_BOOSTWEAKCLASSIFIERPARAMS*)(enode->data);
		wsumModel += param->nodeWeight;
		enode = enode->next;
	}

	return wsumModel;
}


int PR_NormalizeOutputBoostEnsembleForCand(PR_ENSEMBLE* ensemble, PR_CANDDATA* canddata)
{
	double wsumModel = PR_CalcSumOfWeightForWeakClassifiersBoost(ensemble);

	canddata->likelihood[0] /= wsumModel;
	canddata->likelihood[1] /= wsumModel;

	return PR_EOT;
}

int PR_NormalizeOutputBoostEnsembleForCase(PR_ENSEMBLE* ensemble, PR_CASEDATA* casedata)
{
	double wsumModel = PR_CalcSumOfWeightForWeakClassifiersBoost(ensemble);

	PR_CANDDATA* canddata = casedata->childNodeFirstCand;

	while(canddata!=NULL)
	{
		canddata->likelihood[0] /= wsumModel;
		canddata->likelihood[1] /= wsumModel;
		canddata = canddata->nodeNextCand;
	}

	return PR_EOT;
}

int PR_NormalizeOutputBoostEnsembleForCases(PR_ENSEMBLE* ensemble, PR_CASELIST* caselist)
{
	double wsumModel = PR_CalcSumOfWeightForWeakClassifiersBoost(ensemble);

	PR_CASEDATA* casedata = caselist->childNodeFirstCase;

	while(casedata!=NULL)
	{
		PR_CANDDATA* canddata = casedata->childNodeFirstCand;
	
		while(canddata!=NULL)
		{
			canddata->likelihood[0] /= wsumModel;
			canddata->likelihood[1] /= wsumModel;
			canddata = canddata->nodeNextCand;
		}

		casedata = casedata->nodeNextCase;
	}

	return PR_EOT;
}


