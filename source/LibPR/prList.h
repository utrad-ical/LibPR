
#ifndef PR_LIST_H

/* to prList.h */


typedef struct _PR_NODE
{
	struct _PR_NODE*	next;
	struct _PR_NODE*	prev;
	void*	rootList;

	void*	data;			
}
PR_NODE;

typedef struct _PR_LIST
{
	void*	data;
	
	PR_NODE*	first;
	PR_NODE*	last;

	unsigned int	numNode;
}
PR_LIST;


PR_LIST* PR_NewList();
PR_NODE* PR_NewListNode();

PR_LIST* PR_NewListWithNode(unsigned int numNode);

PR_NODE* PR_NewNodeAddToListTop(PR_LIST* list);
PR_NODE* PR_NewNodeAddToListEnd(PR_LIST* list);

int PR_InsertNodeToListTop(PR_NODE* node, PR_LIST* list);
int PR_InsertNodeToListEnd(PR_NODE* node, PR_LIST* list);
int PR_InsertNodeToList(PR_NODE* node, PR_LIST* list, PR_NODE* insertNextTo);

int PR_ClipOutNode(PR_NODE* node);
//int PR_DeleteNodeInList(PR_NODE* node, PR_LIST* list);

int PR_DeleteNode(PR_NODE* node);
int PR_DeleteList(PR_LIST* list);

int PR_CheckWhetherNodeIsMemberOfList(PR_NODE* srcNode, PR_LIST* list);

/* end prList.h */

#define PR_LIST_H
#endif //PR_LIST_H
