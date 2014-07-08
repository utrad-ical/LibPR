
#pragma warning(disable: 4996)

#include "prDiscreteAdaBoost.h"
#include "prDiscreteAdaBoost.private.h"

#include "../LibPR.h"


PR_BOOSTOPTION* PR_NewBoostOptionAddToCandData(PR_CANDDATA* candData)
{
	if(candData==NULL)	return NULL;
	PR_BOOSTOPTION* opt = new PR_BOOSTOPTION;
	candData->optionParam = opt;
	opt->dataWeight=1.0;
	opt->initialCost=1.0;
	opt->onProcCost=1.0;
	return opt;
}

int PR_NewBoostOptionAddToCandDataOfCaseList(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return PR_NG;

	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			candData->optionParam = PR_NewBoostOptionAddToCandData(candData);
			candData = candData->nodeNextCand; 
		}
		caseData = caseData->nodeNextCase;
	}

	return PR_GOOD;
}

void PR_DeleteBoostOptionOfCandData(PR_CANDDATA* candData)
{
	if(candData==NULL || candData->optionParam==NULL)	return;
	delete [] (PR_BOOSTOPTION*)(candData->optionParam);
//	delete [] candData->optionParam;
	candData->optionParam = NULL;
}

void PR_DeleteBoostOptionOfCaseData(PR_CASEDATA* caseData)
{
	if(caseData==NULL)	return;

	PR_CANDDATA* candData = caseData->childNodeFirstCand;
	
	while(candData!=NULL)
	{
		PR_DeleteBoostOptionOfCandData(candData);
		candData = candData->nodeNextCand;
	}
}

void PR_DeleteBoostOptionOfCaseList(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return;

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;

	while(caseData!=NULL)
	{
		PR_DeleteBoostOptionOfCaseData(caseData);
		caseData = caseData->nodeNextCase;
	}
}

int PR_InitializeDataWeightTwoClass(PR_CASELIST* caseList, int mode)
{
	if(caseList==NULL || (mode!=1 && mode!=2))	return PR_NG;

	PR_CountNumCandsForCaseList(caseList);
	
	double* intWeightValue = new double [2];
	if(mode==1)
	{
		intWeightValue[0] = intWeightValue[1] = 1.0/(double)caseList->totalNumCand;
	}
	else
	{
		intWeightValue[0] = 0.5/(double)caseList->totalNumCandOfClass[0];
		intWeightValue[1] = 0.5/(double)(caseList->totalNumCand-caseList->totalNumCandOfClass[0]);
	}
	
	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
	PR_CANDDATA* currentCand;

	double* sumWeight = new double [2];
	sumWeight[0] = sumWeight[1] = 0.0;

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
				printf("error! unexist BOOSTOPTION!!\n");
				getchar();
			}
			
			opt->dataWeight = intWeightValue[cls];
			sumWeight[cls] += opt->dataWeight;

			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}
	printf("initialize weight; sum(cls0)=%e + sum(cls1)=%e = %e\n",
			sumWeight[0],sumWeight[1],sumWeight[0]+sumWeight[1]);

	delete [] intWeightValue;
	delete [] sumWeight;

	return PR_GOOD;
}

int PR_SetInitialCost(PR_CASELIST* caseList, int mode)
{
	if(caseList==NULL || (mode!=1 && mode!=2))	return PR_NG;

	PR_CountNumCandsForCaseList(caseList);
	
	double* intCostValue = new double [2];
	if(mode==1)
	{
		intCostValue[0] = intCostValue[1] = 1.0/(double)caseList->totalNumCand;
	}
	else
	{
		intCostValue[0] = 0.5/(double)caseList->totalNumCandOfClass[0];
		intCostValue[1] = 0.5/(double)(caseList->totalNumCand-caseList->totalNumCandOfClass[0]);
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
				printf("error! unexist BOOSTOPTION!!\n");
				getchar();
			}

		//	printf("initialCost%f->%f,",opt->initialCost,intCostValue[cls]);
			opt->initialCost = intCostValue[cls];
			sumPreCost[cls] += opt->initialCost;

			currentCand = currentCand->nodeNextCand;
		}
		currentCase = currentCase->nodeNextCase;
	}
	printf("\n");
	printf("iniPreCost; sum(cls0)=%e + sum(cls1)=%e = %e\n",
			sumPreCost[0],sumPreCost[1],sumPreCost[0]+sumPreCost[1]);

	delete [] intCostValue;
	delete [] sumPreCost;

	return PR_GOOD;
}


PR_DISCRETEADABOOST_NODEPARAM* PR_NewDiscreteAdaBoostNodeParam()
{
//	printf("in");

	PR_DISCRETEADABOOST_NODEPARAM* nodeParam = new PR_DISCRETEADABOOST_NODEPARAM;
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

PR_DISCRETEADABOOST_NODEPARAM* PR_NewDiscreteAdaBoostNodeParamOfEnsembleNode(PR_ENSEMBLENODE* node)
{
	if(node==NULL)	return NULL;

	PR_DISCRETEADABOOST_NODEPARAM* nodeParam;
	nodeParam = PR_NewDiscreteAdaBoostNodeParam();
	node->data = nodeParam;

	return nodeParam;
}

void PR_DeleteDiscreteAdaBoostNodeParam(PR_DISCRETEADABOOST_NODEPARAM* nodeParam)
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

int PR_DeleteEnsembleNodeWithDiscreteAdaBoostNodeParam(PR_ENSEMBLENODE* node)
{
	if(node==NULL)
	{
//		printf("null node");		getchar();
		return PR_OK;
	}

	PR_DISCRETEADABOOST_NODEPARAM* nodeParam = (PR_DISCRETEADABOOST_NODEPARAM*)node->data;
//	printf("%d[%04x.%04x],",nodeParam->nodeIndex,nodeParam->featureTag->group,nodeParam->featureTag->element);

	delete nodeParam->featureTag;	//	printf("tag,");
	delete nodeParam;				//	printf("nodeParam\n");
	node->data = NULL;

	int r=PR_DeleteEnsembleNode(node);
//	printf("node!\n");
	return r;
}

int PR_DeleteDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble)
{
	if(ensemble==NULL)	return PR_OK;

	if(ensemble->data!=NULL)
		return PR_ErrorMessageInt("remain ensemble->data (@PR_DeleteEnsemble)");

	int resReturn = PR_GOOD;
	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_ENSEMBLENODE* nextNode = currentNode->next;
	int cnt=1;

	while(currentNode!=NULL)
	{
	//	printf("cnt%d(%d)\n",cnt++,currentNode);

		int resCheck = PR_DeleteEnsembleNodeWithDiscreteAdaBoostNodeParam(currentNode);
		if(resCheck==PR_NG)
			return PR_ErrorMessageInt("cannot delete ENSEMBLENODE (@PR_DeleteEnsemble)");

		if(resReturn>resCheck)	resReturn=resCheck;

		currentNode=ensemble->firstNode;
	}

	return resReturn;
}

int PR_SaveTextDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName)
{
//	printf("a");

	if(PR_CheckNumEnsembleNode(ensemble)==PR_NG)
		return PR_ErrorMessageInt("numNode error (@PR_SaveEnsembleWithDiscreteAdaBoostTxt)");

//	printf("b");

//	PR_CheckAndModifyFileNamesExtensionToLibPR(fileName, PR_FILE__EXTENSION_TEXT);
	
//	printf("c");

	FILE* fp = fopen(fileName,"w");
	if(fp==NULL)
		return PR_ErrorMessageInt("cannot open fp (@PR_SaveEnsembleWithDiscreteAdaBoostTxt)");

//	printf("d");

	fprintf(fp,"PR_DiscreteAdaBoostEnsembleData\n");
	fprintf(fp,"%lu\n",ensemble->numNode);
	
//	printf("e");    

	PR_ENSEMBLENODE* current = ensemble->firstNode;
//	PR_ENSEMBLENODE* next;
	PR_DISCRETEADABOOST_NODEPARAM* param;

//	printf("f\n");

	for(unsigned int nmodel=0; nmodel<ensemble->numNode; nmodel++)
	{
		param = (PR_DISCRETEADABOOST_NODEPARAM*)current->data;
		
//		fprintf(stderr,"%lu %04x %04x %+d %e %le %le\n",
//					param->nodeIndex, param->featureTag->group, param->featureTag->element,
//					param->mode, param->threshold, param->trainError, param->nodeWeight);
		fprintf(fp,"%lu %04x %04x %+d %e %le %le\n",
					param->nodeIndex, param->featureTag->group, param->featureTag->element,
					param->mode, param->threshold, param->trainError, param->nodeWeight);
		
		current = current->next;
	//	if(current->next!=NULL)		next = current->next;
	}

//	printf("g!\n");
	
	fclose(fp);
	return PR_OK;
}

int PR_SaveBinaryDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName)
{
	if(PR_CheckNumEnsembleNode(ensemble)==PR_NG)
		return PR_ErrorMessageInt("numNode error (@PR_SaveEnsembleWithDiscreteAdaBoostTxt)");
//	printf("a");

//	PR_CheckAndModifyFileNamesExtensionToLibPR(fileName, PR_FILE__EXTENSION_BINARY);
//	printf("b");

	FILE* fp = fopen(fileName,"wb");
	if(fp==NULL)
		return PR_ErrorMessageInt("cannot open fp (@PR_SaveEnsembleWithDiscreteAdaBoostTxt)");
//	printf("c");

	char Identifier[32];
	sprintf(Identifier,"PR_DiscreteAdaBoostEnsembleData");
	fwrite(Identifier, sizeof(char), 32, fp);
//	printf("d");

	fwrite(&ensemble->numNode, sizeof(unsigned int), 1, fp);
//	printf("e");

	PR_ENSEMBLENODE* current = ensemble->firstNode;
//	PR_ENSEMBLENODE* next = current->next;
	PR_DISCRETEADABOOST_NODEPARAM* param;

	for(unsigned int nmodel=0; nmodel<ensemble->numNode; nmodel++)
	{
//		printf("%d",nmodel);
		param = (PR_DISCRETEADABOOST_NODEPARAM*)current->data;
		fwrite(&param->nodeIndex, sizeof(unsigned int), 1, fp);
		fwrite(&param->featureTag->group, sizeof(unsigned short), 1, fp);
		fwrite(&param->featureTag->element, sizeof(unsigned short), 1, fp);
		fwrite(&param->mode, sizeof(char), 1, fp);
		fwrite(&param->threshold, sizeof(float), 1, fp);
		fwrite(&param->trainError, sizeof(double), 1, fp);
		fwrite(&param->nodeWeight, sizeof(double), 1, fp);

		current = current->next;
	//	if(current->next!=NULL)		next = current->next;
//		printf(",");
	}
//	printf("f");
	
	fclose(fp);
	return PR_OK;
}

int PR_SaveTxtFileDiscrateAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName)
{
	return PR_SaveTextDiscreteAdaBoostEnsemble(ensemble, fileName);
}

int PR_SaveBinFileDiscrateAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName)
{
	return PR_SaveBinaryDiscreteAdaBoostEnsemble(ensemble, fileName);
}


int PR_ReadTextDiscreteAdaBoostEnsemble(PR_ENSEMBLE* ensemble, char* fileName)
{
//	printf("read discreteAB \"#s\"\n",fileName);

	FILE* fp=fopen(fileName,"r");
	
	if(fp==NULL)
		return PR_ErrorMessageInt("fileName error (@PR_ReadTextDiscreteAdaBoostEnsemble)");

	char strdmy[256];
	fscanf(fp,"%s",strdmy);
	if(strcmp(strdmy,"PR_DiscreteAdaBoostEnsembleData")!=0)
	{
		sprintf(strdmy,"%s is not for DiscreteAdaBoost (@PR_ReadTextDiscreteAdaBoostEnsemble)",fileName);
		fclose(fp);
		return PR_ErrorMessageInt(strdmy);
	}

	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;

	unsigned int numModel;
	fscanf(fp,"%lu\n",&numModel);

	char dmy_str[32];

	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_DISCRETEADABOOST_NODEPARAM* param;

	for(unsigned int cnt=0; cnt<numModel; cnt++)
	{
		if(currentNode==NULL)
			currentNode = PR_NewNodeAddToEnsembleLast(ensemble);

		if(currentNode==NULL)
			return PR_ErrorMessageInt("cannot create ensembleNode (@PR_ReadTextDiscreteAdaBoostEnsemble)");

		param = (PR_DISCRETEADABOOST_NODEPARAM*)currentNode->data;
		if(param==NULL)
			param = PR_NewDiscreteAdaBoostNodeParamOfEnsembleNode(currentNode);

		if(param==NULL)
			return PR_ErrorMessageInt("cannot create nodeParam (@PR_ReadTextDiscreteAdaBoostEnsemble)");

		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%lu",&param->nodeIndex);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%04x",&param->featureTag->group);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%04x",&param->featureTag->element);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%d",&param->mode);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%e",&param->threshold);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%le",&param->trainError);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%le",&param->nodeWeight);

	//	int aaa;
	//	aaa = fscanf(fp,"%lu %04x %04x %+d %e %le %le\n",
	//					&param->nodeIndex, &param->featureTag->group, &param->featureTag->element,
	//					&param->mode, &param->threshold, &param->trainError, &param->nodeWeight);
	//	if(aaa==EOF)
	//	{
	//		fclose(fp);
	//		return PR_ErrorMessageInt("file error (@PR_ReadTextDiscreteAdaBoostEnsemble)");
	//	}

		/*
		fprintf(stderr,"(%d)'s Param.. ",currentNode);
		fprintf(stderr,"%lu %04x %04x %+d %e %le %le\n",
					param->nodeIndex, param->featureTag->group, param->featureTag->element,
					param->mode, param->threshold, param->trainError, param->nodeWeight);
		/**/

		PR_ENSEMBLENODE* dummy = currentNode->next;
		currentNode = dummy;
	}

	fclose(fp);
	return PR_OK;
}

int PR_ReadBinaryDiscreteAdaBoostEnsemble(char* fileName, PR_ENSEMBLE* ensemble)
{
	FILE* fp=fopen(fileName,"rb");
	
	if(fp==NULL)
		return PR_ErrorMessageInt("fileName error (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");

	char Identifier[32];
	fread(Identifier,sizeof(char),32,fp);
	if(strcmp(Identifier,"PR_DiscreteAdaBoostEnsembleData")!=0)
	{
		char strdmy[64];
		sprintf(strdmy,"%s is not for DiscreteAdaBoost (@PR_ReadBinaryDiscreteAdaBoostEnsemble)",fileName);
		fclose(fp);
		return PR_ErrorMessageInt(strdmy);
	}
	
	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;

	unsigned int numModel;
	fread(&numModel,sizeof(unsigned int),1,fp);
	printf("numModel=%d\n",numModel);
	
	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_DISCRETEADABOOST_NODEPARAM* param;

	for(unsigned int cnt=0; cnt<numModel; cnt++)
	{
		if(currentNode==NULL)	currentNode = PR_NewNodeAddToEnsembleLast(ensemble);
		if(currentNode==NULL)	return PR_ErrorMessageInt("cannot create ensembleNode (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");

		param = (PR_DISCRETEADABOOST_NODEPARAM*)currentNode->data;
		if(param==NULL)		param = PR_NewDiscreteAdaBoostNodeParamOfEnsembleNode(currentNode);
		if(param==NULL)		return PR_ErrorMessageInt("cannot create nodeParam (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");

		int aaa=0;
		aaa += fread(&param->nodeIndex, sizeof(unsigned int), 1, fp);
		aaa += fread(&param->featureTag->group, sizeof(unsigned short), 1, fp);
		aaa += fread(&param->featureTag->element, sizeof(unsigned short), 1, fp);
		aaa += fread(&param->mode, sizeof(char), 1, fp);
		aaa += fread(&param->threshold, sizeof(float), 1, fp);
		aaa += fread(&param->trainError, sizeof(double), 1, fp);
		aaa += fread(&param->nodeWeight, sizeof(double), 1, fp);

		if(aaa<7)
		{
			fclose(fp);
			return PR_ErrorMessageInt("file error (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");
		}
	//	fprintf(stderr,"%lu %04x %04x %+d %e %le %le\n",
	//				param->nodeIndex, param->featureTag->group, param->featureTag->element,
	//				param->mode, param->threshold, param->trainError, param->nodeWeight);

		currentNode=currentNode->next;
	}

	fclose(fp);
	return PR_OK;
}

PR_ENSEMBLE* PR_ReadTxtFileDiscreteAdaBoostEnsemble(char* fileName)
{
	
	FILE* fp=fopen(fileName,"r");
	
	if(fp==NULL)
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL("fileName error (@PR_ReadTextDiscreteAdaBoostEnsemble)");


	char strdmy[256];
	fscanf(fp,"%s",strdmy);
	if(strcmp(strdmy,"PR_DiscreteAdaBoostEnsembleData")!=0)
	{
		sprintf(strdmy,"%s is not for DiscreteAdaBoost (@PR_ReadTextDiscreteAdaBoostEnsemble)",fileName);
		fclose(fp);
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL(strdmy);
	}


	unsigned int numModel;
	fscanf(fp,"%lu\n",&numModel);

	PR_ENSEMBLE* ensemble = PR_NewEnsembleWithNode(numModel);
	PR_NewDiscreteAdaBoostNodeParamAddToEnsembleList(ensemble);

	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;


	char dmy_str[32];

	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_DISCRETEADABOOST_NODEPARAM* param;

	for(unsigned int cnt=0; cnt<numModel; cnt++)
	{
		if(currentNode==NULL)
			currentNode = PR_NewNodeAddToEnsembleLast(ensemble);

		if(currentNode==NULL)
		{
			fclose(fp);
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create ensembleNode (@PR_ReadTextDiscreteAdaBoostEnsemble)");
		}

		param = (PR_DISCRETEADABOOST_NODEPARAM*)currentNode->data;
		if(param==NULL)
			param = PR_NewDiscreteAdaBoostNodeParamOfEnsembleNode(currentNode);

		if(param==NULL)
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create nodeParam (@PR_ReadTextDiscreteAdaBoostEnsemble)");

		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%lu",&param->nodeIndex);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%04x",&param->featureTag->group);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%04x",&param->featureTag->element);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%d",&param->mode);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%e",&param->threshold);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%le",&param->trainError);
		fscanf(fp,"%s",dmy_str);	sscanf(dmy_str,"%le",&param->nodeWeight);

	//	fprintf(stderr,"%lu %04x %04x %+d %e %le %le\n",
	//				param->nodeIndex, param->featureTag->group, param->featureTag->element,
	//				param->mode, param->threshold, param->trainError, param->nodeWeight);

		currentNode = currentNode->next;
	}

	fclose(fp);
	return ensemble;

}

PR_ENSEMBLE* PR_ReadBinFileDiscreteAdaBoostEnsemble(char* fileName)
{
	FILE* fp=fopen(fileName,"rb");
	
	if(fp==NULL)
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL("fileName error (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");

	char Identifier[32];
	fread(Identifier,sizeof(char),32,fp);
	if(strcmp(Identifier,"PR_DiscreteAdaBoostEnsembleData")!=0)
	{
		char strdmy[64];
		sprintf(strdmy,"%s is not for DiscreteAdaBoost (@PR_ReadBinaryDiscreteAdaBoostEnsemble)",fileName);
		fclose(fp);
		return (PR_ENSEMBLE*)PR_ErrorMessageNULL(strdmy);
	}
	
	unsigned int numModel;
	fread(&numModel,sizeof(unsigned int),1,fp);
	printf("numModel=%d\n",numModel);

	PR_ENSEMBLE* ensemble = PR_NewEnsembleWithNode(numModel);
	PR_NewDiscreteAdaBoostNodeParamAddToEnsembleList(ensemble);
	
	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;
	
	
	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_DISCRETEADABOOST_NODEPARAM* param;

	for(unsigned int cnt=0; cnt<numModel; cnt++)
	{
		if(currentNode==NULL)
			currentNode = PR_NewNodeAddToEnsembleLast(ensemble);

		if(currentNode==NULL)
		{
			fclose(fp);
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create ensembleNode (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");
		}

		param = (PR_DISCRETEADABOOST_NODEPARAM*)currentNode->data;
		if(param==NULL)
			param = PR_NewDiscreteAdaBoostNodeParamOfEnsembleNode(currentNode);

		if(param==NULL)
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("cannot create nodeParam (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");

		int aaa=0;
		aaa += fread(&param->nodeIndex, sizeof(unsigned int), 1, fp);
		aaa += fread(&param->featureTag->group, sizeof(unsigned short), 1, fp);
		aaa += fread(&param->featureTag->element, sizeof(unsigned short), 1, fp);
		aaa += fread(&param->mode, sizeof(char), 1, fp);
		aaa += fread(&param->threshold, sizeof(float), 1, fp);
		aaa += fread(&param->trainError, sizeof(double), 1, fp);
		aaa += fread(&param->nodeWeight, sizeof(double), 1, fp);

		if(aaa<7)
		{
			fclose(fp);
			return (PR_ENSEMBLE*)PR_ErrorMessageNULL("file error (@PR_ReadBinaryDiscreteAdaBoostEnsemble)");
		}

	//	fprintf(stderr,"%lu %04x %04x %+d %e %le %le\n",
	//				param->nodeIndex, param->featureTag->group, param->featureTag->element,
	//				param->mode, param->threshold, param->trainError, param->nodeWeight);

		currentNode=currentNode->next;
	}

	fclose(fp);
	return ensemble;
}


int PR_ReguralizeDataWeightForBoost(PR_CASELIST* caseList)
{
	if(caseList==NULL)		return PR_NG;

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
			if(currentCand->optionParam==NULL)		return PR_NG;

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

	return PR_OK;
}


PR_TRAINCANDARRAY* PR_PickUpTrainCandDataDiscreteAdaBoost(PR_CASELIST* caseList)
{
	if(caseList==NULL)	return NULL;

	unsigned int cntTrainCand=0;
	
//	PR_CASEDATA* currentCase = caseList->childNodeFirstCase;
//	PR_CANDDATA* currentCand;
	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;

//	FILE* fp;

	/**/
//	fp=fopen("pre.csv","w");
//	printf("\n");
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		unsigned int cnt=0;

		while(candData!=NULL)
		{
			if(candData->optionParam==NULL)		return NULL;
			
			double weight = ((PR_BOOSTOPTION*)candData->optionParam)->dataWeight;
			if(candData->booOutlier==0 && weight>0.0)
			{
				cntTrainCand++;

		//		if(caseData->idCase==1330)
		//		fprintf(fp,"%d,%d,%d,%d,%d,%e\n",cnt,candData->nodePrevCand,candData,candData->nodeNextCand,
		//											candData->booOutlier,((PR_BOOSTOPTION*)candData->optionParam)->dataWeight);
			}
		//	else
		//	{
		//		if(caseData->idCase==1330)
		//		fprintf(fp,"%d,%d,%d,%d,%d,%e\n",cnt,candData->nodePrevCand,candData,candData->nodeNextCand,
		//											candData->booOutlier,((PR_BOOSTOPTION*)candData->optionParam)->dataWeight);
		//		fprintf(stderr,"\n%d,%d,%d,%d,%d,%e\n",cnt,candData->nodePrevCand,candData,candData->nodeNextCand,
		//											candData->booOutlier,((PR_BOOSTOPTION*)candData->optionParam)->dataWeight);
		//	}

			candData = candData->nodeNextCand;
			cnt++;
		}
//		printf("id%d:%d(%d),",caseData->idCase,cntTrainCand,cnt);
		caseData = caseData->nodeNextCase;
	}
//	printf("\n");
//	printf("cntTrain%d,",cntTrainCand);
//	fclose(fp);
	/**/
//	cntTrainCand = caseList->totalNumCand;
	
	unsigned int numTrainCand = cntTrainCand;
	cntTrainCand=0;

	PR_TRAINCANDARRAY* trainCand = PR_NewTrainCandArray(caseList->numClass, numTrainCand);
//	printf("numTrain%d",numTrainCand);

//	fp=fopen("post.csv","w");
//	printf("\n");
	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		unsigned int cnt=0;

		while(candData!=NULL)
		{
			double weight = ((PR_BOOSTOPTION*)candData->optionParam)->dataWeight;
		//	printf("case:%d cand:%d %e ",currentCase->idCase,candData->idCand,weight);

			if(candData->booOutlier==0 && weight>0.0)
			{
				trainCand->data[cntTrainCand] = candData;
		//		if(cntTrainCand<10)
		//		{
		//			printf("cnt%d: adress(%d=%d)",cntTrainCand,trainCand->data[cntTrainCand],currentCand);
		//			printf("class %d=%d\n",candData->classLabel,trainCand->data[cntTrainCand]->classLabel);
		//		}
		//		if(caseData->idCase==1330)
		//		fprintf(fp,"%d,%d,%d,%d,%d,%e\n",cnt,candData->nodePrevCand,candData,candData->nodeNextCand,
		//											candData->booOutlier,((PR_BOOSTOPTION*)candData->optionParam)->dataWeight);

				trainCand->numDataOfClass[candData->classLabel]++;
				cntTrainCand++;
			//	getchar();
			}
		//	else
		//	{
		//		if(caseData->idCase==1330)
		//		fprintf(fp,"%d,%d,%d,%d,%d,%e\n",cnt,candData->nodePrevCand,candData,candData->nodeNextCand,
		//											candData->booOutlier,((PR_BOOSTOPTION*)candData->optionParam)->dataWeight);
		//		fprintf(stderr,"\n%d,%d,%d,%d,%d,%e\n",cnt,candData->nodePrevCand,candData,candData->nodeNextCand,
		//											candData->booOutlier,((PR_BOOSTOPTION*)candData->optionParam)->dataWeight);
		//	}

		//	printf("\r");
			candData = candData->nodeNextCand;
			cnt++;
		}
//		printf("id%d:%d(%d),",caseData->idCase,cntTrainCand,cnt);
		caseData = caseData->nodeNextCase;
	}
//	fclose(fp);
//	printf("\n");

	if(cntTrainCand!=numTrainCand)
	{
		printf("wrong numCand: 1st%d <-> 2nd%d..\n",numTrainCand,cntTrainCand);
		return NULL;
	}
//	getchar();

	return trainCand;
}

int PR_CalcMinErrorAndThresholdDiscreteAdaBoost(
PR_TRAINCANDARRAY* trainCand,
PR_DISCRETEADABOOST_NODEPARAM* abParam,
char searchMode
){
//	printf("[%04x %04x](sm%+d) era...",abParam->featureTag->group,abParam->featureTag->element,searchMode);
//	printf("\n");

	if(trainCand==NULL || abParam==NULL)
		return PR_ErrorMessageInt("unknown inputs (@PR_CalcMinErrorAndThresholdDiscreteAdaBoost)");
//	printf(" aa");

	// get index of target feat
	unsigned int indexTargetFeatInTable;

	PR_CASEDATA* rootCase = (PR_CASEDATA*)trainCand->data[1]->rootCaseData;
	PR_CASELIST* rootList = (PR_CASELIST*)rootCase->rootCaseList;
//	printf(" bb(nFeat%d)",rootList->featTagTable->numFeat);
	int res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(abParam->featureTag, rootList->featTagTable, &indexTargetFeatInTable);
//	printf(" cc(%d) ",res);
	
	if(res == PR_NG)
		return PR_ErrorMessageInt("error feat (@PR_CalcMinErrorAndThresholdDiscreteAdaBoost)");


	unsigned char clsCand;
	unsigned int idxCand, ncand;
	double wgtCand, sumWgtCandTP=0.0, sumWgtCandFP=0.0;

	// copy value of feat[#.indexTargetFeatInTable]
	float* featValues = new float [trainCand->numData];
	if(featValues==NULL)
		return PR_ErrorMessageInt("error mallic featValues (@PR_CalcMinErrorAndThresholdDiscreteAdaBoost)"); 
//	printf("d\n");
	
	for(ncand=0; ncand<trainCand->numData; ncand++)
	{
//		if(ncand>trainCand->numData*9/10)	printf("cand%d/%d,",ncand,trainCand->numData);

		if(trainCand->data[ncand]==NULL)
		{
			printf("era!!!!!");
			getchar();
		}

		// calc weight sum of classes
		clsCand = trainCand->data[ncand]->classLabel;
//		if(ncand>trainCand->numData*9/10)	printf("cls%d,",clsCand);

		wgtCand = ((PR_BOOSTOPTION*)(trainCand->data[ncand]->optionParam))->dataWeight;
//		if(ncand>trainCand->numData*9/10)	printf("wgt%f,",wgtCand);

		// copy main
		featValues[ncand] = trainCand->data[ncand]->featValues[indexTargetFeatInTable];
//		if(ncand>trainCand->numData*9/10)	printf("value%f,",featValues[ncand]);

		if(clsCand==0)	sumWgtCandFP += wgtCand;
		else			sumWgtCandTP += wgtCand;
//		if(ncand>trainCand->numData*9/10)	printf("sum(tp%f,fp%f)\n",sumWgtCandTP,sumWgtCandFP);
	}
//	printf("e");

	// sort feat value
	unsigned int* rankCand = PR_QuickSortAscendingWithRank(featValues, trainCand->numData);
	if(rankCand==NULL)
	{
		delete [] featValues;
		return PR_ErrorMessageInt("error mallic rankCand (@PR_CalcMinErrorAndThresholdDiscreteAdaBoost)"); 
	}
//	printf("(min%+.3e~max%+.3e) ",featValues[0],featValues[trainCand->numData-1]);

	abParam->mode = 0;
	abParam->trainError = 2.0;

	// low-pass
//	printf("Low\n");
	double dmySumWgtFN=sumWgtCandTP, dmySumWgtFP=0.0;
	if(searchMode==0 || searchMode==-1)
	for(ncand=0; ncand<trainCand->numData-1; ncand++)
	{
		idxCand = rankCand[ncand];
		clsCand = trainCand->data[idxCand]->classLabel;
		wgtCand = ((PR_BOOSTOPTION*)(trainCand->data[idxCand]->optionParam))->dataWeight;
		
		if(clsCand==0)	dmySumWgtFP += wgtCand;
		else			dmySumWgtFN -= wgtCand;

		if(featValues[ncand]!=featValues[ncand+1] && abParam->trainError > dmySumWgtFP+dmySumWgtFN)
		{
	//		printf("#%06d(%06d); %e=%.3e,wgt%.2e,wFn%.2e,wFp%.2e,Era%.4e",
	//			ncand,idxCand,featValues[ncand],
	//			trainCand->data[idxCand]->featValues[indexTargetFeatInTable],
	//			wgtCand,dmySumWgtFN,dmySumWgtFP,dmySumWgtFP+dmySumWgtFN);

			abParam->mode = -1;
			abParam->threshold = (featValues[ncand]+featValues[ncand+1])*0.5f;
			abParam->trainError = dmySumWgtFP+dmySumWgtFN;
	//		printf("*\n");
	//		getchar();
		}
	//	printf("\n");
	}
//	printf("end LowPass: Era=%e, Thd=%e\n",abParam->trainError,abParam->threshold);

	// high-pass
//	printf("High\n");
	dmySumWgtFN=sumWgtCandTP;	dmySumWgtFP=0.0;
	if(searchMode==0 || searchMode==+1)
	for(ncand=trainCand->numData-1; ncand>0; ncand--)
	{
		idxCand = rankCand[ncand];
		clsCand = trainCand->data[idxCand]->classLabel;
		wgtCand = ((PR_BOOSTOPTION*)(trainCand->data[idxCand]->optionParam))->dataWeight;
		
		if(clsCand==0)	dmySumWgtFP += wgtCand;
		else			dmySumWgtFN -= wgtCand;

		if(featValues[ncand]!=featValues[ncand-1] && abParam->trainError > dmySumWgtFP+dmySumWgtFN)
		{
	//		printf("#%06d(%06d); %e=%.3e,wgt%.2e,wFn%.2e,wFp%.2e,Era%.4e",
	//			ncand,idxCand,featValues[ncand],
	//			trainCand->data[idxCand]->featValues[indexTargetFeatInTable],
	//			wgtCand,dmySumWgtFN,dmySumWgtFP,dmySumWgtFP+dmySumWgtFN);

			abParam->mode = +1;
			abParam->threshold = (featValues[ncand]+featValues[ncand-1])*0.5f;
			abParam->trainError = dmySumWgtFP+dmySumWgtFN;
	//		printf("*\n");
	//		getchar();
		}
	//	printf("\n");
	}
//	printf("Era%.4e,Thd%.4e(%+d)\r",abParam->trainError,abParam->threshold,abParam->mode);
//	printf("Era%.4e,Thd%.4e(%+d)\n",abParam->trainError,abParam->threshold,abParam->mode);
	
	delete [] featValues;
	delete [] rankCand;

	return PR_OK;
}

int AddWeakClassifierOutputToEnsembleOutputDiscreteAdaBoost(
PR_TRAINCANDARRAY* cands, 
PR_DISCRETEADABOOST_NODEPARAM* param
){
	if(cands==NULL || param==NULL)	return PR_NG;

	unsigned int targetFeat;
	PR_CASELIST* root = (PR_CASELIST*)((PR_CASEDATA*)(cands->data[0]->rootCaseData))->rootCaseList;
	int res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(param->featureTag, root->featTagTable, &targetFeat);
	if(res==PR_NG)	return PR_NG;

	unsigned int ncand;
	for(ncand=0; ncand<cands->numData; ncand++)
	{
		float fvalue = cands->data[ncand]->featValues[targetFeat];
		if(param->mode==-1)
		{
			if(fvalue<=param->threshold)	cands->data[ncand]->likelihood[1] += param->nodeWeight;
			else							cands->data[ncand]->likelihood[1] -= param->nodeWeight;
		}
		else
		{
			if(fvalue>=param->threshold)	cands->data[ncand]->likelihood[1] += param->nodeWeight;
			else							cands->data[ncand]->likelihood[1] -= param->nodeWeight;
		}
		cands->data[ncand]->likelihood[0] = -1.0*cands->data[ncand]->likelihood[1];
	}

	return PR_OK;
}

int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForTrainCandArray(
PR_TRAINCANDARRAY* cands, 
PR_ENSEMBLE* ensemble	// list of weak-classifiers for ensemble
){
	if(cands==NULL || ensemble==NULL)	return PR_NG;

	int res;
	unsigned int ncand, nfeat;
	unsigned int targetFeat;
	PR_CASELIST* root = (PR_CASELIST*)((PR_CASEDATA*)(cands->data[0]->rootCaseData))->rootCaseList;
	float fvalue;
	double sumOutput;

	PR_ENSEMBLENODE *current, *dmy;
	PR_DISCRETEADABOOST_NODEPARAM* param;
	
	for(ncand=0; ncand<cands->numData; ncand++)
	{
		current = ensemble->firstNode;
		param = (PR_DISCRETEADABOOST_NODEPARAM*)(ensemble->firstNode->data);
		sumOutput = 0.0f;

		for(nfeat=0; nfeat<ensemble->numNode; nfeat++)
		{
			param = (PR_DISCRETEADABOOST_NODEPARAM*)(current->data); 

			res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(param->featureTag, root->featTagTable, &targetFeat);
			if(res==PR_NG)	return PR_NG;

			fvalue = cands->data[ncand]->featValues[targetFeat];
			if(param->mode==-1)
			{
				if(fvalue<=param->threshold)	sumOutput += param->nodeWeight;
				else							sumOutput -= param->nodeWeight;
			}
			else if(param->mode==1)
			{
				if(fvalue>=param->threshold)	sumOutput += param->nodeWeight;
				else							sumOutput -= param->nodeWeight;
			}

			dmy = current;
			current = dmy->next;
		}

		cands->data[ncand]->likelihood[1] = sumOutput;
		cands->data[ncand]->likelihood[0] = -1.0 * cands->data[ncand]->likelihood[1];
	}

	return PR_OK;
}

int PR_UpdataDataWeightDiscreteAdaBoost(
PR_TRAINCANDARRAY* cands, 
PR_DISCRETEADABOOST_NODEPARAM* param
){
	if(cands==NULL || param==NULL)	return PR_NG;

	unsigned int targetFeat;
	PR_CASELIST* root = (PR_CASELIST*)((PR_CASEDATA*)(cands->data[0]->rootCaseData))->rootCaseList;
	int res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(param->featureTag, root->featTagTable, &targetFeat);
	if(res==PR_NG)	return PR_NG;
	
	unsigned int ncand;
	float fvalue;
	unsigned char judge, label;
	PR_BOOSTOPTION* pParam;
	double penaltyForWeight = (1.0-param->trainError)/param->trainError;
	double sumNextDataWeight = 0.0;

	printf("> index%02d [%04x %04x]: trainEra=%e, penalty=%e\n",
		targetFeat,param->featureTag->group,param->featureTag->element,
			param->trainError,penaltyForWeight);
	
	double sumTpWgt=0.0, sumFpWgt=0.0, sumMissWgt=0.0;

	for(ncand=0; ncand<cands->numData; ncand++)
	{
		fvalue = cands->data[ncand]->featValues[targetFeat];
		pParam = (PR_BOOSTOPTION*)(cands->data[ncand]->optionParam);
		label = cands->data[ncand]->classLabel;
		if(label)	label=1;

		if(param->mode==-1)
		{
			if(fvalue<=param->threshold)	judge=1;
			else							judge=0;
		}
		else
		{
			if(fvalue>=param->threshold)	judge=1;
			else							judge=0;
		}
		
		if(cands->data[ncand]->classLabel==0)	sumFpWgt+=pParam->dataWeight;
		else									sumTpWgt+=pParam->dataWeight;
		
		if(judge!=label)
		{
			sumMissWgt += pParam->dataWeight;
			pParam->dataWeight *= penaltyForWeight;
		}
		sumNextDataWeight += pParam->dataWeight;
	}
	printf("> wUpdate; sumTp=%e, sumFp=%e (miss%e)-> ",sumTpWgt,sumFpWgt,sumMissWgt);

	sumTpWgt=sumFpWgt=0.0;

	if(sumNextDataWeight!=1.0)
	for(ncand=0; ncand<cands->numData; ncand++)
	{
		pParam = (PR_BOOSTOPTION*)(cands->data[ncand]->optionParam);
		pParam->dataWeight /= sumNextDataWeight;

		if(cands->data[ncand]->classLabel==0)	sumFpWgt+=pParam->dataWeight;
		else									sumTpWgt+=pParam->dataWeight;
	}
	printf("sumTp=%e, sumFp=%e\n",sumTpWgt,sumFpWgt);

	return PR_OK;
}


int PR_UpdataDataWeightUBoost(
PR_CASELIST* caseList,		// candidate data
PR_ENSEMBLE* ensemble,		// ensemble data
unsigned int numModel,		// num of weak classifier 
char modeBoost				// selection swich of boost loss func. (1=ada,2=mada,3=logit)
)
// !! causion !!
// Data weight normalization is not processed in this function! 
{
	if(caseList==NULL || ensemble==NULL)
	{
		fprintf(stderr,"disable data..");
		return PR_NG;
	}

	if(ensemble->numNode<numModel)
	{
		fprintf(stderr,"wrong number of weak classifier! (total%d<->input%d)..",ensemble->numNode,numModel);
		return PR_NG;
	}

	if(modeBoost<1 || modeBoost>3)
	{
		fprintf(stderr,"wrong mode:%d..",modeBoost);
		return PR_NG;
	}


	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;
	PR_BOOSTOPTION* bOption;

	PR_ENSEMBLENODE* weakModel;
	PR_DISCRETEADABOOST_NODEPARAM* param;

	unsigned int cntModel=0, fTarget;
	double output;
	unsigned int cntCand=0;

	caseData=caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData=caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			// get output of ensemble
			output=0.0;
	//		printf("[%d]@case%d:0.0->",cntCand,caseData->idCase);

			cntModel=0;
			weakModel=ensemble->firstNode;
			while(weakModel!=NULL && cntModel<numModel)
			{
	//			printf("(%d/%d)",cntModel,numModel);
				param = (PR_DISCRETEADABOOST_NODEPARAM*)(weakModel->data); 

				int res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(param->featureTag, caseList->featTagTable, &fTarget);
				if(res==PR_NG)	return PR_NG;
				
				float value=(float)candData->featValues[fTarget];
				if(param->mode==-1)
				{
					if(value<=param->threshold)	output += param->nodeWeight;
					else						output -= param->nodeWeight;
				}
				else
				{
					if(value>=param->threshold)	output += param->nodeWeight;
					else						output -= param->nodeWeight;
				}
	//			printf("%.3e",output);
	//			if(cntModel<numModel-1)		printf("->");

				cntModel++;
				weakModel=weakModel->next;
			}
	//		printf(" => ");

			bOption = (PR_BOOSTOPTION*)(candData->optionParam);
			if(candData->classLabel)	output = -output;
	//		printf("z%.3f,",output);

			switch(modeBoost)
			{
			case 1:		// AdaBoost
				bOption->dataWeight = exp(output);
				break;
			case 2:		// MadaBoost
				if(output>=0.0)		bOption->dataWeight = 1.0;
				else				bOption->dataWeight = exp(output*2.0);
				break;
			case 3:		// LogitBoost
				bOption->dataWeight = 2.0*exp(output*2.0)/(1.0+exp(output*2.0));
				break;
			}
	//		printf("wgt%e",bOption->dataWeight);
	//		getchar();

			cntCand++;
			candData=candData->nodeNextCand;
		}
		caseData=caseData->nodeNextCase;
	}

	return PR_OK;
}


unsigned int 
PR_GetMaxCandIDFromCaseList(PR_CASELIST* caseList)
{
	unsigned int max=0;

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;

	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			if(max<candData->idCand)
			{
				max=candData->idCand;
				printf("->%d",max);
			}
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
	
	printf("=max\n",max);
	return max;
}

unsigned int 
PR_GetNumCandIDFromCaseList(PR_CASELIST* caseList)
{
	return PR_GetMaxCandIDFromCaseList(caseList)+1;
}

unsigned int* 
PR_CountCandInIDGroupFromCaseList(PR_CASELIST* caseList, unsigned int numGroup)
{
	unsigned int* cnt = new unsigned int [numGroup];
	memset(cnt,0x00,sizeof(unsigned int)*numGroup);

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;

	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			cnt[candData->idCand]++;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
	
	return cnt;
}

int 
PR_CalcSumDataWeigntInIDGroupFromCaseList(
	PR_CASELIST* caseList, unsigned int numGroup, double* arrayWgt)
{
	memset(arrayWgt,0x00,sizeof(double)*numGroup);

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;

	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			arrayWgt[candData->idCand] += ((PR_BOOSTOPTION*)candData->optionParam)->dataWeight;
			candData = candData->nodeNextCand;
		}
		caseData = caseData->nodeNextCase;
	}
	
	return PR_GOOD;
}

int PR_TrainDiscreteAdaBoostEnsembleLogOutput(
PR_CASELIST*	caseList,
PR_ENSEMBLE*	ensemble,	// list of weak-classifiers for ensemble
char* logfileDataWeight,
char modeBoost
){
	unsigned int nmodel, nfeat;

	if(caseList==NULL || ensemble==NULL)
		return PR_ErrorMessageInt("unknown inputs (@PR_TrainDiscreteAdaBoostEnsemble)");
	if(PR_ReguralizeDataWeightForBoost(caseList)==PR_NG)
		return PR_ErrorMessageInt("not set dataWeight (@PR_TrainDiscreteAdaBoostEnsemble)");
//	printf("a");

	PR_CountNumCandsForCaseList(caseList);

	PR_TRAINCANDARRAY* trainCand = PR_PickUpTrainCandDataDiscreteAdaBoost(caseList);		// candidates for ensemble training
//	printf("b,");
	
	PR_DISCRETEADABOOST_NODEPARAM** eraCalcResult;			// weighted classification errors of all input features
	eraCalcResult = new PR_DISCRETEADABOOST_NODEPARAM* [caseList->featTagTable->numFeat];
//	printf("featNum=%d\n",caseList->featTagTable->numFeat);

	for(nfeat=0; nfeat<caseList->featTagTable->numFeat; nfeat++)
	{
//		printf("%02d",nfeat);
		eraCalcResult[nfeat] = PR_NewDiscreteAdaBoostNodeParam();
//		printf(": ");
//		if(eraCalcResult[nfeat]==NULL)
//		{
//			printf("aaaaa!!");	getchar();
//		}
//		printf("feat");
		PR_CopyFeatTag(eraCalcResult[nfeat]->featureTag, caseList->featTagTable->tags[nfeat]);
//		printf("[%04x %04x]\n",eraCalcResult[nfeat]->featureTag->group,eraCalcResult[nfeat]->featureTag->element);
	}
//	printf("c");

//	printf("featNum=%d\n",caseList->featTagTable->numFeat);
	PR_CASELIST* rootList = (PR_CASELIST*)caseList->childNodeFirstCase->rootCaseList;
//	printf("featNum=%d\n",rootList->featTagTable->numFeat);
	PR_CASEDATA* rootCase = (PR_CASEDATA*)trainCand->data[0]->rootCaseData;
	rootList = (PR_CASELIST*)rootCase->rootCaseList;
//	printf("featNum=%d\n",rootList->featTagTable->numFeat);
//	printf("d");


	PR_ERROROFCLASSIFIER_2C** errorData;	// ensembles accuracies (num of node in ensemble = 1~ensemble->numNode)
	errorData = new PR_ERROROFCLASSIFIER_2C* [ensemble->numNode];
//	printf("e");

	char* booAnalyzedFeat = new char [caseList->featTagTable->numFeat];	// boorean: 1=target_of_analysis
	unsigned int finalNumModel = ensemble->numNode;		// num of weak-classifier within ensemble user want to create

	unsigned int idxTargetFeatOfTable;

	FILE* fpLogWgt;
	unsigned int numCandGroup=0;
	unsigned int* cntCandInGroup;
	double* sumDatWgtInGroup;
	if(logfileDataWeight!=NULL)
	{
		fpLogWgt = fopen(logfileDataWeight,"w");
		numCandGroup = PR_GetMaxCandIDFromCaseList(caseList)+1;
		if(numCandGroup==1)		fclose(fpLogWgt);

		cntCandInGroup = PR_CountCandInIDGroupFromCaseList(caseList, numCandGroup);
		sumDatWgtInGroup = new double [numCandGroup];

		unsigned int ng;

		fprintf(fpLogWgt,"step,add-model,");
		for(ng=0; ng<numCandGroup; ng++)	fprintf(fpLogWgt,"group-%d,",ng);
		fprintf(fpLogWgt,"\n");

		fprintf(fpLogWgt,",,");
		for(ng=0; ng<numCandGroup; ng++)	fprintf(fpLogWgt,"%d,",cntCandInGroup[ng]);
		fprintf(fpLogWgt,"<-size,\n");

		PR_CalcSumDataWeigntInIDGroupFromCaseList(caseList, numCandGroup, sumDatWgtInGroup);

		fprintf(fpLogWgt,"0,[null],");
		for(ng=0; ng<numCandGroup; ng++)	fprintf(fpLogWgt,"%le,",sumDatWgtInGroup[ng]);
		fprintf(fpLogWgt,"\n");

		fclose(fpLogWgt);
	}
//	printf("f");

	PR_ENSEMBLENODE* currentModel = ensemble->firstNode;
	PR_ENSEMBLENODE* dmyModel;

	for(nmodel=0; nmodel<finalNumModel; nmodel++)
	{
		printf("#%d/%d: ",nmodel+1,finalNumModel);

		PR_IncludeCostInDataWeight(caseList);	//	printf("costInc,");
		PR_NormalizeDataWeight(caseList);		//	printf("wgtNor\n");

		PR_DISCRETEADABOOST_NODEPARAM* paramAB = (PR_DISCRETEADABOOST_NODEPARAM*)currentModel->data;

		// define features which will be tested
		if(paramAB->mode==0 && paramAB->featureTag->group==0xffff && paramAB->featureTag->element==0xffff)
		{
			for(nfeat=0; nfeat<caseList->featTagTable->numFeat; nfeat++)	booAnalyzedFeat[nfeat]=1;

			// need? : feature of last weak classifier is not selected for current weak classifier??
		//	if(nmodel>0)
		//	{
		//		PR_DISCRETEADABOOST_NODEPARAM* paramBefore = (PR_DISCRETEADABOOST_NODEPARAM*)currentModel->prev->data;
		//		PR_CheckIfFeatTagIsMemberOfFeatTagTable(paramBefore->featureTag, caseList->featTagTable, &idxTargetFeatOfTable);
		//		booAnalyzedFeat[idxTargetFeatOfTable]=0;
		//	}
		}
		else
		{
		//	printf("def[%04x.%04x](%+d) ",
		//			paramAB->featureTag->group,paramAB->featureTag->element,paramAB->mode);

			for(nfeat=0; nfeat<caseList->featTagTable->numFeat; nfeat++)	booAnalyzedFeat[nfeat]=0;
			int res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(paramAB->featureTag, caseList->featTagTable, &idxTargetFeatOfTable);
			if(res!=PR_NG)
			{
				if(paramAB->mode==-1)	booAnalyzedFeat[idxTargetFeatOfTable]=10;
				else					booAnalyzedFeat[idxTargetFeatOfTable]=11;
			}
			else
				return PR_ErrorMessageInt("error featTagTable (@PR_TrainDiscreteAdaBoostEnsemble)");
		}

		// find out feature provides minimum weighted error
		unsigned int idxFeatOfMinEra;
		double minEra=10.0;
		for(nfeat=0; nfeat<caseList->featTagTable->numFeat; nfeat++)
		{
			if(booAnalyzedFeat[nfeat])
			{
			//	printf("%d[%04x %04x] calcMinEra start\n",
			//		nfeat,eraCalcResult[nfeat]->featureTag->group,
			//		eraCalcResult[nfeat]->featureTag->element);
				char mode=0;
				if(booAnalyzedFeat[nfeat]==10)			mode=-1;
				else if(booAnalyzedFeat[nfeat]==11)		mode=1;
				PR_CalcMinErrorAndThresholdDiscreteAdaBoost(trainCand, eraCalcResult[nfeat], mode);
			}
			else
				eraCalcResult[nfeat]->trainError = 2.0;

			if(minEra>eraCalcResult[nfeat]->trainError)
			{
				idxFeatOfMinEra = nfeat;
				minEra = eraCalcResult[nfeat]->trainError;
			}
		}

		// update (nmodel+1)-th weak classifier
		PR_CopyFeatTag(paramAB->featureTag,eraCalcResult[idxFeatOfMinEra]->featureTag);
		paramAB->mode = eraCalcResult[idxFeatOfMinEra]->mode;
		paramAB->threshold = eraCalcResult[idxFeatOfMinEra]->threshold;
		paramAB->trainError = eraCalcResult[idxFeatOfMinEra]->trainError;
		paramAB->nodeIndex = nmodel+1;
		paramAB->nodeWeight = 0.5*log((1.0-paramAB->trainError)/paramAB->trainError);

		printf("%04x.%04x th%.3e(%+d),e=%.3e,w=%.3e \n",
				paramAB->featureTag->group,paramAB->featureTag->element,
				paramAB->threshold,paramAB->mode,paramAB->trainError,paramAB->nodeWeight);
		
		// update data weights
		PR_UpdataDataWeightUBoost(caseList, ensemble, nmodel+1, modeBoost);

		if(logfileDataWeight!=NULL && numCandGroup>1)
		{
			fpLogWgt = fopen(logfileDataWeight,"a");
			unsigned int ng;

			PR_CalcSumDataWeigntInIDGroupFromCaseList(caseList, numCandGroup, sumDatWgtInGroup);
			fprintf(fpLogWgt,"%d,",nmodel+1);
			fprintf(fpLogWgt,"[%04x %04x],",paramAB->featureTag->group,paramAB->featureTag->element);
			for(ng=0; ng<numCandGroup; ng++)	fprintf(fpLogWgt,"%le,",sumDatWgtInGroup[ng]);
			fprintf(fpLogWgt,"\n");

			fclose(fpLogWgt);
		}

		char booCalcEnsEra=0;
#ifdef CALCULATE_ENSEMBLE_ERROR

		// calculate outputs of classifier ensemble
	//	AddWeakClassifierOutputToEnsembleOutputDiscreteAdaBoost(trainCand, paramAB);
		ensemble->numNode = nmodel+1;
		PR_CalculateEnsembleOutputOfDiscreteAdaBoostForTrainCandArray(trainCand, ensemble);
		ensemble->numNode = finalNumModel;

		// calculate error parameters of classifier ensemble
		errorData[nmodel] = PR_CalculateErrorOfClassifier2CForTrainCandArray(trainCand,0.0f);
		printf("Ensemble#%d; Acc(Std)=%.4e, Avg(rTP,rFP)=%.4e, rTP=%.4e, rTN=%.4e\n>> Numbers #{tp}=%d, #{fn}=%d, #{tn}=%d, #{fp}=%d\n\n",
				nmodel+1,errorData[nmodel]->accuracy,
				(errorData[nmodel]->sensitivity+errorData[nmodel]->specificity)/2.0,
				errorData[nmodel]->sensitivity,errorData[nmodel]->specificity,
				errorData[nmodel]->numTP,errorData[nmodel]->numFN,
				errorData[nmodel]->numTN,errorData[nmodel]->numFP);
		booCalcEnsEra=1;
#endif

		/*
		if(booCalcEnsEra==0 && 
			((nmodel+1)%(finalNumModel/10)==0 || nmodel==0 || nmodel==finalNumModel-1))
		{
			// calculate outputs of classifier ensemble
			ensemble->numNode = nmodel+1;
			PR_CalculateEnsembleOutputOfDiscreteAdaBoostForTrainCandArray(trainCand, ensemble);
			ensemble->numNode = finalNumModel;
	
			// calculate error parameters of classifier ensemble
			PR_ERROROFCLASSIFIER_2C* eraEns = PR_CalculateErrorOfClassifier2CForTrainCandArray(trainCand,0.0f);
			printf("Ensemble#%d; Acc(Std)=%.4e, Avg(rTP,rFP)=%.4e, rTP=%.4e, rTN=%.4e\n",
				nmodel+1,eraEns->accuracy,
				(eraEns->sensitivity+eraEns->specificity)/2.0,eraEns->sensitivity,eraEns->specificity);
			printf(">> Numbers #{tp}=%d, #{fn}=%d, #{tn}=%d, #{fp}=%d\n\n",
				eraEns->numTP,eraEns->numFN,eraEns->numTN,eraEns->numFP);
			PR_DeleteErrorOfClassifier2C(eraEns);
		}
		*/

		// shift target ensemble node to next
		dmyModel = currentModel;
		currentModel = dmyModel->next;

	//	getchar();
	}

	PR_DeleteTrainCandArray(trainCand);

	for(nfeat=0; nfeat<caseList->featTagTable->numFeat; nfeat++)
		PR_DeleteDiscreteAdaBoostNodeParam(eraCalcResult[nfeat]);
	delete [] eraCalcResult;

#ifdef CALCULATE_ENSEMBLE_ERROR
	for(nmodel=0; nmodel<finalNumModel; nmodel++)
		PR_DeleteErrorOfClassifier2C(errorData[nmodel]);
#endif
	delete [] errorData;

	delete [] booAnalyzedFeat;

	if(logfileDataWeight!=NULL && numCandGroup>1)
	{
		delete [] cntCandInGroup;
		delete [] sumDatWgtInGroup;
	}

	return PR_OK;
}

int 
PR_TrainDiscreteAdaBoostEnsemble(
	PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	PR_TrainDiscreteAdaBoostEnsembleLogOutput(caseList,ensemble, NULL, 1);
	return PR_GOOD;
}

int PR_TrainMadaBoostEnsembleDiscrete2C(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	PR_TrainDiscreteAdaBoostEnsembleLogOutput(caseList,ensemble, NULL, 2);
	return PR_GOOD;
}

int PR_TrainLogitBoostEnsembleDiscrete2C(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	PR_TrainDiscreteAdaBoostEnsembleLogOutput(caseList,ensemble, NULL, 3);
	return PR_GOOD;
}



int PR_NewDiscreteAdaBoostNodeParamAddToEnsembleList(PR_ENSEMBLE* ensemble)
{
	if(ensemble==NULL)	return PR_NG;

	ensemble->codeEnsembleType = PR_ENSEMBLETYPE_DISCRETEADABOOST;

	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	unsigned int numNode = 0;
	
	while(currentNode!=NULL)
	{
		PR_DISCRETEADABOOST_NODEPARAM* nodeParam;
		nodeParam = PR_NewDiscreteAdaBoostNodeParamOfEnsembleNode(currentNode);
		if(nodeParam==NULL)		return PR_NG;

		PR_ENSEMBLENODE* dmy = currentNode->next;
		currentNode = dmy;
		numNode++;
	}

	if(numNode!=ensemble->numNode)
	{
		ensemble->numNode = numNode;
		return PR_OK;
	}

	return PR_GOOD;
}

unsigned int iCand;

int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCand(PR_ENSEMBLE* ensemble, PR_CANDDATA* candData)
{
	if(candData==NULL || ensemble==NULL)	return PR_NG;
	
	if(candData->rootCaseData==NULL)	return PR_NG;
	if(((PR_CASEDATA*)(candData->rootCaseData))->rootCaseList==NULL)	return PR_NG;

	if(ensemble->codeEnsembleType!=PR_ENSEMBLETYPE_DISCRETEADABOOST)	return PR_NG;

	
	int res;
	unsigned int nfeat, targetFeat;
	PR_CASELIST* root = ((PR_CASELIST*)((PR_CASEDATA*)(candData->rootCaseData))->rootCaseList);
	float fvalue;
	double sumOutput = 0.0;

	PR_ENSEMBLENODE* current;
	current = ensemble->firstNode;
	PR_DISCRETEADABOOST_NODEPARAM* param = ((PR_DISCRETEADABOOST_NODEPARAM*)(ensemble->firstNode->data));

//	if(iCand==0)	printf("\n");

//	printf("outTransition: ");
	for(nfeat=0; nfeat<ensemble->numNode; nfeat++)
	{
		param = (PR_DISCRETEADABOOST_NODEPARAM*)(current->data);
	//	printf("@node%d(w%.3f),",nfeat+1,param->nodeWeight);

		res = PR_CheckIfFeatTagIsMemberOfFeatTagTable(param->featureTag, root->featTagTable, &targetFeat);
		if(res==PR_NG)	return PR_NG;

		fvalue = candData->featValues[targetFeat];
//		if(iCand==0)	printf("#%d feat[%d]=%.3e; out%.3e",iCand,targetFeat,fvalue,sumOutput);

		if(param->mode==-1)
		{
			if(fvalue<=param->threshold)	sumOutput += param->nodeWeight;
			else							sumOutput -= param->nodeWeight;
		}
		else
		{
			if(fvalue>=param->threshold)	sumOutput += param->nodeWeight;
			else							sumOutput -= param->nodeWeight;
		}

//		if(iCand==0)	printf("->%.3e\n",sumOutput);
//		printf("(%d)%.3e->",nfeat+1,sumOutput);
		current = current->next;
	}
//	printf("\b\b [end]");
//	getchar();

	candData->likelihood[1] = sumOutput;
	candData->likelihood[0] = -1.0 * candData->likelihood[1];

	return PR_GOOD;
}

int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCase(PR_ENSEMBLE* ensemble, PR_CASEDATA* caseData)
{
	if(caseData==NULL || ensemble==NULL)	return PR_NG;

	PR_CANDDATA* current = caseData->childNodeFirstCand;
	PR_CANDDATA* dmy;
	
	iCand=0;
	while(current!=NULL)
	{
		int res = PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCand(ensemble, current);
		if(res==PR_NG)	return PR_NG;
		dmy = current->nodeNextCand;
		current = dmy;
		iCand++;
	}

	return PR_GOOD;
}

int PR_CalculateEnsembleOutputOfDiscreteAdaBoostForList(PR_ENSEMBLE* ensemble, PR_CASELIST* caseList)
{
	if(caseList==NULL || ensemble==NULL)	return PR_NG;

	PR_CASEDATA* current = caseList->childNodeFirstCase;
	PR_CASEDATA* dmy;

	while(current!=NULL)
	{
	//	printf("case%04d,",current->idCase);
		int res = PR_CalculateEnsembleOutputOfDiscreteAdaBoostForCase(ensemble, current);
		if(res==PR_NG)	return PR_NG;
		dmy = current->nodeNextCase;
		current = dmy;
	}

	return PR_GOOD;
}

PR_ENSEMBLE* 
PR_NewDiscreteAdaBoostEnsemble(unsigned int numNode)
{
	PR_ENSEMBLE* ensemble = PR_NewEnsembleWithNode(numNode);

	PR_NewDiscreteAdaBoostNodeParamAddToEnsembleList(ensemble);

	return ensemble;
}

int
PR_CopyWeakClassifierOfDiscreteAdaBoostEnsemble(PR_ENSEMBLE* dest, PR_ENSEMBLE* src)
{
	if(dest==NULL || src==NULL)									return PR_NG;
	if(dest->codeEnsembleType!=1 || src->codeEnsembleType!=1)	return PR_NG;
	if(src->numNode < dest->numNode)							return PR_NG;


	PR_ENSEMBLENODE* nSrc = src->firstNode;
	PR_ENSEMBLENODE* nDest = dest->firstNode;
	
	PR_DISCRETEADABOOST_NODEPARAM* pSrc;
	PR_DISCRETEADABOOST_NODEPARAM* pDest;
	
//	PR_FEATTAG* ftagD;
//	PR_FEATTAG* ftagS;

	for(unsigned int ii=0; ii<dest->numNode; ii++)
	{
//		ftagD = ((PR_DISCRETEADABOOST_NODEPARAM*)nodeD->data)->featureTag;
//		ftagS = ((PR_DISCRETEADABOOST_NODEPARAM*)nodeS->data)->featureTag;
//		PR_CopyFeatTag(ftagD, ftagS);

		pSrc = (PR_DISCRETEADABOOST_NODEPARAM*)nSrc->data;
		pDest = (PR_DISCRETEADABOOST_NODEPARAM*)nDest->data;
		
		pDest->featureTag->group = pSrc->featureTag->group;
		pDest->featureTag->element = pSrc->featureTag->element;
		pDest->mode = pSrc->mode;
		pDest->nodeIndex = pSrc->nodeIndex;
		pDest->nodeWeight = pSrc->nodeWeight;
		pDest->threshold = pSrc->threshold;
		pDest->trainError = pSrc->trainError;

		nDest = nDest->next;
		nSrc = nSrc->next;
	}
	
	if(src->numNode == dest->numNode)	return PR_GOOD;
	else								return PR_OK;
}


PR_LOSSFUNCBOOST*
PR_NewLossFuncBoost(unsigned int num)
{
	PR_LOSSFUNCBOOST* loss = new PR_LOSSFUNCBOOST;

	loss->lossFunc = new double [num];
	memset(loss->lossFunc,0x00,sizeof(double)*num);
	loss->lossFuncFP = new double [num];
	memset(loss->lossFuncFP,0x00,sizeof(double)*num);
	loss->lossFuncTP = new double [num];
	memset(loss->lossFuncTP,0x00,sizeof(double)*num);

	loss->num = num;

	return loss;
}

void
PR_DeleteLossFuncBoost(PR_LOSSFUNCBOOST* loss)
{
	delete [] loss->lossFunc;
	delete [] loss->lossFuncFP;
	delete [] loss->lossFuncTP;
}

PR_LOSSFUNCBOOST*
PR_CalculateLossOfBoostEnsemble(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;
	PR_ENSEMBLENODE* wModel;
	PR_DISCRETEADABOOST_NODEPARAM* wmParam;

	double* lossFunc = new double [ensemble->numNode];
	memset(lossFunc,0x00,sizeof(double)*ensemble->numNode);

	double* lossFunc0 = new double [ensemble->numNode];
	memset(lossFunc0,0x00,sizeof(double)*ensemble->numNode);
	double* lossFunc1 = new double [ensemble->numNode];
	memset(lossFunc1,0x00,sizeof(double)*ensemble->numNode);
//	printf("1");

	unsigned int ii;
	int idxFeat;

	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;

		while(candData!=NULL)
		{
			double output = 0.0;
			wModel = ensemble->firstNode;

			for(ii=0; ii<ensemble->numNode; ii++)
			{
				wmParam = (PR_DISCRETEADABOOST_NODEPARAM*)wModel->data;
				idxFeat = PR_ExtractFeatNumberInFeatTagTable(caseList->featTagTable, wmParam->featureTag);
			//	output += candData->featValues[idxFeat] * wmParam->nodeWeight;

				if(wmParam->mode==-1)
				{
					if(candData->featValues[idxFeat]<=wmParam->threshold)	output += wmParam->nodeWeight;
					else													output += -wmParam->nodeWeight;
				}
				else
				{
					if(candData->featValues[idxFeat]>=wmParam->threshold)	output += wmParam->nodeWeight;
					else													output += -wmParam->nodeWeight;
				}

				if(candData->classLabel)
				{
					lossFunc[ii] += exp(-1.0*output);
					lossFunc1[ii] += exp(-1.0*output);
				}
				else
				{
					lossFunc[ii] += exp(output);
					lossFunc0[ii] += exp(output);
				}

				wModel = wModel->next;
			}

			candData = candData->nodeNextCand;
		}

		caseData = caseData->nodeNextCase;
	}
//	printf("2");


	PR_CountNumCandsForCaseList(caseList);

	for(ii=0; ii<ensemble->numNode; ii++)
	{
		lossFunc[ii] /= (double)caseList->totalNumCand;
		lossFunc0[ii] /= (double)caseList->totalNumCand;
		lossFunc1[ii] /= (double)caseList->totalNumCand;
	}
//	printf("3");


	PR_LOSSFUNCBOOST* loss = new PR_LOSSFUNCBOOST;
	loss->num = ensemble->numNode;
	loss->numFP = caseList->totalNumCandOfClass[0];
	loss->numTP = caseList->totalNumCand-caseList->totalNumCandOfClass[0];
	loss->lossFunc = lossFunc;
	loss->lossFuncFP = lossFunc0;
	loss->lossFuncTP = lossFunc1;
//	printf("4");

	return loss;
}


int
PR_StandardizeEnsembleOutputOfDiscereteAdaBoostForCase(
	PR_CASEDATA* caseData, PR_ENSEMBLE* ensemble, double min, double max)
{
	if(caseData==NULL || ensemble==NULL)	return PR_NG;

	if(min>=max)	return PR_NG;

	unsigned int ii;
	double sumWeightModel=0.0;

	PR_ENSEMBLENODE *eNode = ensemble->firstNode;
	for(ii=0; ii<ensemble->numNode; ii++)
	{
		sumWeightModel += ((PR_DISCRETEADABOOST_NODEPARAM*)eNode->data)->nodeWeight;
		eNode = eNode->next;
	}

	PR_CANDDATA* candData = caseData->childNodeFirstCand;
	while(candData!=NULL)
	{
		candData->likelihood[0] /= sumWeightModel;
		candData->likelihood[1] /= sumWeightModel;
		if(min!=-1.0 || max!=1.0)
		{
			candData->likelihood[0] = (candData->likelihood[0]+1.0)*0.5*(max-min)+min;
			candData->likelihood[1] = (candData->likelihood[1]+1.0)*0.5*(max-min)+min;
		}

		candData = candData->nodeNextCand;
	}

	return PR_GOOD;
}

int
PR_StandardizeEnsembleOutputOfDiscereteAdaBoostForList(
	PR_CASELIST* caseList, PR_ENSEMBLE* ensemble, double min, double max)
{
	if(caseList==NULL || ensemble==NULL)	return PR_NG;
	
	if(min>=max)	return PR_NG;

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		int r = PR_StandardizeEnsembleOutputOfDiscereteAdaBoostForCase(caseData, ensemble, min, max);
		if(r==PR_NG)	return PR_NG;
		caseData = caseData->nodeNextCase;
	}

	return PR_GOOD;
}


int
PR_CalcLhodMarginDiscAdaBoostFromCase(PR_CASEDATA* caseData, PR_ENSEMBLE* ensemble)
{
	if(caseData==NULL || ensemble==NULL)	return PR_NG;

	unsigned int ii;
	double sumWeightModel=0.0;

	PR_ENSEMBLENODE *eNode = ensemble->firstNode;
	for(ii=0; ii<ensemble->numNode; ii++)
	{
		sumWeightModel += ((PR_DISCRETEADABOOST_NODEPARAM*)eNode->data)->nodeWeight;
		eNode = eNode->next;
	}

	PR_CANDDATA* candData = caseData->childNodeFirstCand;
	while(candData!=NULL)
	{
		candData->likelihood[0] /= sumWeightModel;
		candData->likelihood[1] /= sumWeightModel;
		candData = candData->nodeNextCand;
	}

	return PR_GOOD;
}


int PR_DisplayInConsoleWindowEnsembleDetail(PR_ENSEMBLE* ensemble)
{
	PR_ENSEMBLENODE* node = ensemble->firstNode;
	PR_DISCRETEADABOOST_NODEPARAM* nodedata;

	for(unsigned int ii=0; ii<ensemble->numNode; ii++)
	{
		nodedata = (PR_DISCRETEADABOOST_NODEPARAM*)node->data;
		fprintf(stderr,"#%d: [%04x.%04x] (%d) th=%f w=%f e=%f\n",
						ii+1,
						nodedata->featureTag->group,nodedata->featureTag->element,
						nodedata->mode,nodedata->threshold,nodedata->nodeWeight,nodedata->trainError);
		node = node->next;
	}

	return 0;
}


int
PR_CalcLhodMarginDiscAdaBoostFromList(PR_CASELIST* caseList, PR_ENSEMBLE* ensemble)
{
	if(caseList==NULL || ensemble==NULL)	return PR_NG;

	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		int r = PR_CalcLhodMarginDiscAdaBoostFromCase(caseData, ensemble);
		if(r==PR_NG)	return PR_NG;
		caseData = caseData->nodeNextCase;
	}

	return PR_GOOD;
}

int
PR_IncludeCostInDataWeight(PR_CASELIST* caseList)
{
	PR_CASEDATA* caseData = caseList->childNodeFirstCase;
	PR_CANDDATA* candData;
	PR_BOOSTOPTION* bOption;
	unsigned int cnt=0, cntCase=0;

	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			bOption = (PR_BOOSTOPTION*)(candData->optionParam);
			if(bOption->initialCost!=1.0)		bOption->dataWeight *= bOption->initialCost;
			if(bOption->onProcCost!=1.0)		bOption->dataWeight *= bOption->onProcCost;
			cnt++;
			candData=candData->nodeNextCand;
		}
		cntCase++;
		caseData=caseData->nodeNextCase;
	}
//	printf("%dcands(from%dcases),",cnt,cntCase);

	return PR_GOOD;
}

double
PR_NormalizeDataWeight(PR_CASELIST* caseList)
{
	PR_CASEDATA* caseData;
	PR_CANDDATA* candData;
	PR_BOOSTOPTION* bOption;
	double sumWeight=0.0;
	unsigned int cnt=0, cntCase=0;

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			bOption = (PR_BOOSTOPTION*)(candData->optionParam);
			sumWeight += bOption->dataWeight;
			cnt++;
			candData=candData->nodeNextCand;
		}
		cntCase++;
		caseData=caseData->nodeNextCase;
	}
//	printf("%dcands(from%dcases),sumW%f,",cnt,cntCase,sumWeight);

	caseData = caseList->childNodeFirstCase;
	while(caseData!=NULL)
	{
		candData = caseData->childNodeFirstCand;
		while(candData!=NULL)
		{
			bOption = (PR_BOOSTOPTION*)(candData->optionParam);
			bOption->dataWeight /= sumWeight;
			candData=candData->nodeNextCand;
		}
		caseData=caseData->nodeNextCase;
	}

	return sumWeight;
}


