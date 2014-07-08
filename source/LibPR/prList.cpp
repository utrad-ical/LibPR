

#include "./prList.h"
#include "./prList.private.h"



PR_LIST* PR_NewList()
{
	PR_LIST* list = new PR_LIST;
	if(list==NULL)	return NULL;

	list->numNode = 0;
	list->last = list->first = NULL;
	list->data = NULL;

	return list;
}

PR_NODE* PR_NewListNode()
{
	PR_NODE* listNode = new PR_NODE;
	if(listNode==NULL)	return NULL;

	listNode->next = listNode->prev = NULL;
	listNode->data = NULL;
	return listNode;
}

PR_LIST* PR_NewListWithNode(unsigned int numNode)
{
	PR_LIST* list = PR_NewList();
	if(list==NULL)	return NULL;

	PR_NODE* dmyNode;

	for(unsigned int ii=0; ii<numNode; ii++)
	{
		dmyNode = PR_NewNodeAddToListTop(list);
		
		if(dmyNode==NULL)
		{
			fprintf(stderr,"cannot create node (num=%d/%d)\n",ii,numNode);
			PR_DeleteList(list);
			break;
		}
	}
	return list;
}

PR_NODE* PR_NewNodeAddToListTop(PR_LIST* list)
{
	PR_NODE* addNode = new PR_NODE;
	
	if(list==NULL || addNode==NULL)
	{
		fprintf(stderr,"Not add \"ListNode\" to top of list\n");
		return NULL;
	}

	if(list->first == NULL)
		list->last = list->first = addNode;
	else
	{
		addNode->next = list->first;
		addNode->prev = NULL;

		list->first->prev = addNode;
		list->first = addNode;
	}

	list->numNode++;

	return addNode;
}

PR_NODE* PR_NewListNodeAddToListEnd(PR_LIST* list)
{
	PR_NODE* addNode = new PR_NODE;
	
	if(list==NULL || addNode==NULL)
	{
		fprintf(stderr,"Not add \"ListNode\" to tail of list\n");
		return NULL;
	}

	if(list->last == NULL)
		list->last = list->first = addNode;
	else
	{
		addNode->next = NULL;
		addNode->prev = list->last;

		list->last->next = addNode;
		list->last = addNode;
	}

	list->numNode++;

	return addNode;
}

int PR_InsertNodeToListTop(PR_NODE* addNode, PR_LIST* list)
{
	if(list==NULL || addNode==NULL)
	{
		fprintf(stderr,"Not add node to top of list\n");
		return PR_NG;
	}

	if(list->first == NULL)
		list->last = list->first = addNode;
	else
	{
		addNode->next = list->first;
		addNode->prev = NULL;

		list->first->prev = addNode;
		list->first = addNode;
	}
	list->numNode++;

	return PR_GOOD;
}

int PR_InsertNodeToListEnd(PR_NODE* addNode, PR_LIST* list)
{
	if(list==NULL || addNode==NULL)
	{
		fprintf(stderr,"Not add node to tail of list\n");
		return PR_NG;
	}

	if(list->last == NULL)
		list->last = list->first = addNode;
	else
	{
		addNode->next = NULL;
		addNode->prev = list->last;

		list->last->next = addNode;
		list->last = addNode;
	}
	list->numNode++;

	return PR_GOOD;
}

int PR_InsertNodeToList(PR_NODE* addNode, PR_LIST* list, PR_NODE* insertNextTo)
{
	if(list==NULL || addNode==NULL)
	{
		fprintf(stderr,"Cannot insert node to list\n");
		return PR_NG;
	}

	if(PR_CheckWhetherNodeIsMemberOfList(insertNextTo, list)==PR_NG)
	{
		fprintf(stderr,"Cannot find out landmark node for inserting\n");
		return PR_NG;
	}

	if(insertNextTo==NULL || insertNextTo==list->last)
		return PR_InsertNodeToListTop(addNode,list);

	if(insertNextTo->next!=NULL)	insertNextTo->next->prev = addNode;
	addNode->next = insertNextTo->next;

	if(insertNextTo->prev!=NULL)	insertNextTo->prev->next = addNode;
	addNode->prev = insertNextTo->prev;

	list->numNode++;

	return PR_GOOD;
}

int PR_ClipOutListNode(PR_NODE* node)
{
	if(node==NULL)		return PR_NG;

	if(node->rootList!=NULL)
	{
		PR_LIST* list = (PR_LIST*)node->rootList;

		if(PR_CheckWhetherNodeIsMemberOfList(node, list)==PR_NG)	return PR_NG;

		if(list->first==node)	list->first = node->next;
		if(list->last==node)	list->last = node->prev;

		list->numNode--;
	}

	if(node->next!=NULL)	node->next->prev = node->prev;
	if(node->prev!=NULL)	node->prev->next = node->next;

	return PR_GOOD;
}

int PR_DeleteListNode(PR_NODE* node)
{
	if(node==NULL)			return PR_NG;
	if(node->data!=NULL)	return PR_NG;

	if(PR_ClipOutListNode(node) == PR_NG)	return PR_NG;

	delete node;

	return PR_GOOD;
}

int PR_DeleteList(PR_LIST* list)
{
	if(list==NULL)			return PR_NG;
	if(list->data!=NULL)	return PR_NG;
	
	PR_NODE* node = list->first;

	while(node!=NULL)
	{
		if(PR_DeleteListNode(node)==PR_NG)	return PR_NG;
		node = list->first;
	}

	delete list;

	return PR_GOOD;
}

int PR_CheckWhetherNodeIsMemberOfList(PR_NODE* srcNode, PR_LIST* list)
{
	if(list==NULL)	return PR_NG;

	PR_NODE* desNode = list->first;
	PR_NODE* dmyNode;
	int boo = PR_NG;

	while(desNode!=NULL)
	{
		if(desNode == srcNode)
		{
			boo=PR_GOOD;
			break;
		}
		else
		{
			dmyNode = desNode;
			desNode = dmyNode->next;
		}
	}

	return boo;
}

