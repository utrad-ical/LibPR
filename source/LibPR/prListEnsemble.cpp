
#pragma warning(disable: 4996)

#include "prListEnsemble.h"
#include "prListEnsemble.private.h"

#include "../LibPR.h"

//#include "./prErrorMessage.h"


PR_ENSEMBLE* PR_NewEnsemble()
{
	PR_ENSEMBLE* ensemble = new PR_ENSEMBLE;
	if(ensemble==NULL)	return NULL;

	ensemble->codeEnsembleType = 0;
	ensemble->data = NULL;
	ensemble->firstNode = NULL;
	ensemble->lastNode = NULL;
	ensemble->numNode = 0;

	return ensemble;
}

PR_ENSEMBLENODE* PR_NewEnsembleNode()
{
	PR_ENSEMBLENODE* node = new PR_ENSEMBLENODE;
	if(node==NULL)	return NULL;

	node->data = NULL;
	node->next = NULL;
	node->prev = NULL;
	node->rootEnsemble = NULL;

	return node;
}

int PR_DeleteEnsemble(PR_ENSEMBLE* ensemble)
{
	if(ensemble==NULL)	return PR_EOT;

	if(ensemble->data!=NULL)
		return PR_ErrorMessageInt("remain ensemble->data (@PR_DeleteEnsemble)");

	int resReturn = PR_EOT;
	PR_ENSEMBLENODE* currentNode = ensemble->firstNode;
	PR_ENSEMBLENODE* nextNode = currentNode->next;

	while(currentNode!=NULL)
	{
		int resCheck = PR_DeleteEnsembleNode(currentNode);
		if(resCheck==PR_AVEND)
			return PR_ErrorMessageInt("cannot delete ENSEMBLENODE (@PR_DeleteEnsemble)");

		if(resReturn>resCheck)	resReturn=resCheck;

		currentNode=ensemble->firstNode;
	}

	return resReturn;
}

int PR_DeleteEnsembleNode(PR_ENSEMBLENODE* node)
{
	if(node==NULL)	return PR_EOT;
	
	if(node->data!=NULL)
		return PR_ErrorMessageInt("remain ensembleNode->data (@PR_DeleteEnsembleNode)");

	PR_ENSEMBLE* rootEnsemble = (PR_ENSEMBLE*)node->rootEnsemble;
	if(rootEnsemble!=NULL && PR_CheckIfNodeIsMemberOfEnsemble(rootEnsemble, node)==PR_AVEND)
		return PR_ErrorMessageInt("inNode is not member of root (@PR_DeleteEnsembleNode)");

	PR_ENSEMBLENODE* prevNode = node->prev;
	PR_ENSEMBLENODE* nextNode = node->next;

//	printf("[-1]%d-[0]%d-[+1]%d (first%d~last%d)\n",
//			prevNode,node,nextNode,rootEnsemble->firstNode,rootEnsemble->lastNode);

	if(prevNode!=NULL)	prevNode->next = nextNode;	//	printf("c");
	if(nextNode!=NULL)	nextNode->prev = prevNode;	//	printf("d");

	if(rootEnsemble!=NULL && PR_CheckIfNodeIsMemberOfEnsemble(rootEnsemble, node)!=PR_AVEND)
		rootEnsemble->numNode--;
	
	if(rootEnsemble->firstNode==node)	rootEnsemble->firstNode = nextNode;	//	printf("a");
	if(rootEnsemble->lastNode==node)	rootEnsemble->lastNode = prevNode;	//	printf("b");

//	printf("e");
	delete node;
//	printf("deleteNode..");

	return PR_EOT;
}

int PR_CheckIfNodeIsMemberOfEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* node)
{
	if(ensemble==NULL || node==NULL)
		return PR_ErrorMessageInt("inputs are not exist (@PR_CheckIfNodeIsMemberOfEnsemble)");

	int chechRes = PR_EOT;
	PR_ENSEMBLE* root = (PR_ENSEMBLE*)node->rootEnsemble;
	PR_ENSEMBLENODE* current=ensemble->firstNode;
	PR_ENSEMBLENODE* next=current->next;

	while(current!=NULL)
	{
		if(current==node)	break;
		current = next;
		next = current->next;
	}

	if(current==NULL)	chechRes = PR_AVEND;

	if(chechRes!=PR_AVEND && node->rootEnsemble!=ensemble)
	{
		chechRes = PR_EOT;
		node->rootEnsemble = ensemble;
	}

	return chechRes;
}

PR_ENSEMBLE* PR_NewEnsembleWithNode(unsigned int numNode)
{
	PR_ENSEMBLE* ensemble = PR_NewEnsemble();
	if(ensemble==NULL)	return ensemble;

	for(unsigned int ii=0; ii<numNode; ii++)
	{
		PR_ENSEMBLENODE* current = PR_NewNodeAddToEnsembleLast(ensemble);
		if(current==NULL)
		{
			PR_DeleteEnsemble(ensemble);
			return NULL;
		}
	}

	return ensemble;
}

int PR_InsertNodeToEnsembleFirst(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* insert)
{
	if(ensemble==NULL || insert==NULL)
		return PR_ErrorMessageInt("inputs are not exist (@PR_InsertNodeToEnsembleFirst)");

	ensemble->numNode++;
	
	insert->prev = NULL;
	insert->next = ensemble->firstNode;
	
	if(ensemble->firstNode!=NULL)	ensemble->firstNode->prev = insert;
	if(ensemble->lastNode==NULL)	ensemble->lastNode = insert;
	ensemble->firstNode = insert;

	insert->rootEnsemble = ensemble;

	return PR_EOT;
}

int PR_InsertNodeToEnsembleLast(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* insert)
{
	if(ensemble==NULL || insert==NULL)
		return PR_ErrorMessageInt("inputs are not exist (@PR_InsertNodeToEnsembleLast)");

	ensemble->numNode++;
	
	insert->prev = ensemble->lastNode;
	insert->next = NULL;
	
	if(ensemble->lastNode!=NULL)	ensemble->lastNode->next = insert;
	if(ensemble->firstNode==NULL)	ensemble->firstNode = insert;
	ensemble->lastNode = insert;

	insert->rootEnsemble = ensemble;

	return PR_EOT;
}

int PR_InsertNodeToEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* insert, PR_ENSEMBLENODE* nextTo)
{
	if(ensemble==NULL || insert==NULL)
		return PR_ErrorMessageInt("inputs are not exist (@PR_InsertNodeToEnsemble)");

	if(nextTo==NULL)
		return PR_InsertNodeToEnsembleLast(ensemble, insert);

	if(PR_CheckIfNodeIsMemberOfEnsemble(ensemble, nextTo)==PR_AVEND)
		return PR_ErrorMessageInt("nextTo is not member of ensemble (@PR_InsertNodeToEnsemble)");

	ensemble->numNode++;
	
	insert->prev = nextTo;
	insert->next = nextTo->next;

	if(nextTo->next!=NULL)	nextTo->next->prev = insert;
	nextTo->next = insert;

	if(ensemble->lastNode==nextTo)	ensemble->lastNode = insert;

	insert->rootEnsemble = ensemble;

	return PR_EOT;
}

PR_ENSEMBLENODE* PR_NewNodeAddToEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* nextTo)
{
	if(ensemble==NULL)
		return (PR_ENSEMBLENODE*)PR_ErrorMessageNULL("inputs is not exist (@PR_NewNodeAddToEnsemble)");

	if(PR_CheckIfNodeIsMemberOfEnsemble(ensemble, nextTo)==PR_AVEND)
		return (PR_ENSEMBLENODE*)PR_ErrorMessageNULL("nextTo is not member of ensemble (@PR_NewNodeAddToEnsemble)");

	PR_ENSEMBLENODE* addNode = PR_NewEnsembleNode();
	if(addNode==NULL)	return NULL;

	PR_InsertNodeToEnsemble(ensemble, addNode, nextTo);

	return addNode;
}

PR_ENSEMBLENODE* PR_NewNodeAddToEnsembleFirst(PR_ENSEMBLE* ensemble)
{
	if(ensemble==NULL)
		return (PR_ENSEMBLENODE*)PR_ErrorMessageNULL("nextTo is not member of ensemble (@PR_NewNodeAddToEnsemble)");

	PR_ENSEMBLENODE* addNode = PR_NewEnsembleNode();
	if(addNode==NULL)	return NULL;

	PR_InsertNodeToEnsembleFirst(ensemble, addNode);

	return addNode;
}

PR_ENSEMBLENODE* PR_NewNodeAddToEnsembleLast(PR_ENSEMBLE* ensemble)
{
	if(ensemble==NULL)
		return (PR_ENSEMBLENODE*)PR_ErrorMessageNULL("nextTo is not member of ensemble (@PR_NewNodeAddToEnsemble)");

	PR_ENSEMBLENODE* addNode = PR_NewEnsembleNode();
	if(addNode==NULL)	return NULL;

	PR_InsertNodeToEnsembleLast(ensemble, addNode);

	return addNode;
}

int PR_ClipOutEnsembleNodeFromEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* pullOut)
{
	if(ensemble==NULL || pullOut==NULL)
		return PR_ErrorMessageInt("inputs are not exist (@PR_ClipOutEnsembleNodeFromEnsemble)");

	if(PR_CheckIfNodeIsMemberOfEnsemble(ensemble, pullOut)==PR_AVEND)
		return PR_ErrorMessageInt("pullOut is not member of ensemble (@PR_ClipOutEnsembleNodeFromEnsemble)");

	PR_ENSEMBLENODE* prev = pullOut->prev;
	PR_ENSEMBLENODE* next = pullOut->next;

	if(prev!=NULL)	prev->next = next;
	if(next!=NULL)	next->prev = prev;

	if(ensemble->firstNode==pullOut)	ensemble->firstNode = next;
	if(ensemble->lastNode==pullOut)		ensemble->lastNode = prev;

	ensemble->numNode--;
	
	return PR_EOT;
}

int PR_CheckNumEnsembleNode(PR_ENSEMBLE* ensemble)
{
//	printf("1");

	if(ensemble==NULL)
		return PR_ErrorMessageInt("ensemble is not exist (@PR_CheckNumEnsembleNode)");

//	printf("2");

	unsigned int numModel=0;
	PR_ENSEMBLENODE* current = ensemble->firstNode;
	PR_ENSEMBLENODE* next;// = current->next;

//	printf("3");

	while(current!=NULL)
	{
//		printf("num=%d,",numModel);
		if(current->data==NULL)		break;
		next = current->next;
		current = next;
		numModel++;
	}
//	printf("num=%d,",numModel);

	if(numModel==ensemble->numNode)		return PR_EOT;
	else
	{
		ensemble->numNode = numModel;
		return PR_EOT;
	}
}

