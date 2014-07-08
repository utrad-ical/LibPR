
#ifndef PR_LIST_OF_ENSEMBLE_H


#define	PR_ENSEMBLETYPE_DISCRETEADABOOST		1
#define	PR_ENSEMBLETYPE_DISCRETEADABOOSTMULTI	2
#define	PR_ENSEMBLETYPE_REALADABOOST			3


typedef struct _PR_ENSEMBLENODE
{
	void*	data;
	struct _PR_ENSEMBLENODE*	next;
	struct _PR_ENSEMBLENODE*	prev;
	void*	rootEnsemble;
}
PR_ENSEMBLENODE;

typedef struct _PR_ENSEMBLE
{
	unsigned char	codeEnsembleType;
	void*			data;

	PR_ENSEMBLENODE*	firstNode;
	PR_ENSEMBLENODE*	lastNode;
	unsigned int	numNode;
}
PR_ENSEMBLE;


PR_ENSEMBLE* PR_NewEnsemble();
PR_ENSEMBLENODE* PR_NewEnsembleNode();

int PR_DeleteEnsemble(PR_ENSEMBLE* ensemble);
int PR_DeleteEnsembleNode(PR_ENSEMBLENODE* node);

int PR_CheckIfNodeIsMemberOfEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* node);

PR_ENSEMBLE* PR_NewEnsembleWithNode(unsigned int numNode);

int PR_InsertNodeToEnsembleFirst(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* insert);
int PR_InsertNodeToEnsembleLast(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* insert);
int PR_InsertNodeToEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* insert, PR_ENSEMBLENODE* nextTo);

PR_ENSEMBLENODE* PR_NewNodeAddToEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* nextTo);
PR_ENSEMBLENODE* PR_NewNodeAddToEnsembleFirst(PR_ENSEMBLE* ensemble);
PR_ENSEMBLENODE* PR_NewNodeAddToEnsembleLast(PR_ENSEMBLE* ensemble);

int PR_ClipOutEnsembleNodeFromEnsemble(PR_ENSEMBLE* ensemble, PR_ENSEMBLENODE* pullOut);

int PR_CheckNumEnsembleNode(PR_ENSEMBLE* ensemble);

/* end prList.h */

#define PR_LIST_OF_ENSEMBLE_H
#endif //PR_LIST_H

