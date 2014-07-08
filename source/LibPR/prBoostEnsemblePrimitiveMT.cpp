
#pragma warning(disable: 4996)


#include <pthread.h>
#include "../LibPR.h"

#include "prBoostEnsemblePrimitive.h"
#include "prBoostEnsemblePrimitive.private.h"



typedef struct _PRIMITIVES_ENSEMBLE_CLASSIFICATION_MT_
{
	PR_ENSEMBLE* ensemble;
	PR_CANDDATA* top;
	PR_CANDDATA* end;
}
PRIMITIVES_ENSEMBLE_CLASSIFICATION_MT;


static void* threaded_EnsembleClassificationForCase(void* argument)
{
	PRIMITIVES_ENSEMBLE_CLASSIFICATION_MT* primitives = (PRIMITIVES_ENSEMBLE_CLASSIFICATION_MT*)argument;
	PR_CANDDATA* cand = primitives->top;

	while(1)
	{
		PR_CalcOutputBoostEnsembleForCand(primitives->ensemble, cand);
		if(cand==primitives->end)	break;
		cand = cand->nodeNextCand;
	}

	return NULL;
}

int PR_CalcOutputBoostEnsembleForCaseMT(PR_ENSEMBLE* ensemble, PR_CASEDATA* casedata, int nThread)
{

	if(casedata==NULL || ensemble==NULL)	return PR_AVEND;
	

	int* nSum = new int [nThread];
	double divided = (double)casedata->numCand/(double)nThread;

	for(int i=0; i<nThread-1; i++)
	{
		nSum[i] = (int)(divided*(double)(i+1)+0.5);
//		fprintf(stderr,"i%d=%d, ",i,nSum[i]);
	}
	nSum[nThread-1] = casedata->numCand-1;
//	fprintf(stderr,"i%d=%d\n",nThread-1,nSum[nThread-1]);


	PRIMITIVES_ENSEMBLE_CLASSIFICATION_MT** primitives = new PRIMITIVES_ENSEMBLE_CLASSIFICATION_MT* [nThread];

	for(int i=0; i<nThread; i++)
	{
		primitives[i] = new PRIMITIVES_ENSEMBLE_CLASSIFICATION_MT;
		primitives[i]->ensemble = ensemble;
		primitives[i]->top = primitives[i]->end = NULL;
	}

	primitives[0]->top = casedata->childNodeFirstCand;

	int i=0, count=0;
	PR_CANDDATA* current = casedata->childNodeFirstCand;
	
	while(current!=NULL)
	{
		if(count==nSum[i])
		{
			primitives[i]->end = current;
			primitives[i+1]->top = current->nodeNextCand;
			i++;
		}

		if(i==nThread-1)	break;
		
		current = current->nodeNextCand;
		count++;
	}

	primitives[nThread-1]->end = casedata->childNodeLastCand;


	pthread_t* threads = new pthread_t [nThread];

	for(i=0; i<nThread; i++)
	{
//		fprintf(stderr,"i[%d}--%d~%d, ",i,primitives[i]->top,primitives[i]->end);
		pthread_create(&threads[i], NULL, threaded_EnsembleClassificationForCase, (void *)primitives[i]);
	}
	
	for(i=0; i<nThread; i++)
		pthread_join(threads[i],NULL);
//	fprintf(stderr," ok!\n");


	delete [] nSum;
	for(int i=0; i<nThread; i++)	delete primitives[i];
	delete [] primitives;
	delete [] threads;


	return PR_EOT;
}


