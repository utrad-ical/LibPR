

#include "prIsomap.h"
#include "prIsomap.private.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prSortTools.h"


typedef struct _K_Neighbors
{
	int k;
	double* dists;
	unsigned long* indexes;
}
PR_KNEIGHBORS;

PR_KNEIGHBORS* PR_MeasureDistanceOfKNeighbors(PR_CANDDATA** candidates, unsigned long index, int k)
{
	PR_CASELIST* caselist = (PR_CASELIST*)((PR_CASEDATA*)candidates[0]->rootCaseData)->rootCaseList;
	unsigned int numFeat = caselist->featTagTable->numFeat;
	unsigned long numCand = caselist->totalNumCand;

	PR_KNEIGHBORS* out = new PR_KNEIGHBORS;
	out->dists = new double [k];
	out->indexes = new unsigned long [k];
	out->k = k;

	float* feat1 = candidates[index]->featValues;
	float* feat2;
	unsigned long j, count=0;
	double sub, maxdist, dmydist;
	int iswap;

	for(j=0; j<numCand ;j++)
	{
		if(count==(unsigned long)k)		break;

		if(j!=index)
		{
			count++;
			out->dists[count] = 0.0;
			out->indexes[count] = j;
			feat2 = candidates[j]->featValues;

			for(unsigned int i=0; i<numFeat; i++)
			{
				sub = (double)feat1[i]-(double)feat2[i];
				out->dists[j] += sub*sub;
			}

			if(count==0)						maxdist = out->dists[0];
			else if(maxdist<out->dists[count])	maxdist = out->dists[count];
		}
	}

	for(j; j<numCand; j++)
	if(j!=index)
	{
		feat2 = candidates[j]->featValues;
		dmydist = 0.0;

		for(unsigned int i=0; i<numFeat; i++)
		{
			sub = (double)feat1[i]-(double)feat2[i];
			dmydist += sub*sub;
			if(dmydist>=maxdist)	break;
		}

		if(dmydist<maxdist)
		{
			for(iswap=0; iswap<k; iswap++)
			{
				if(out->dists[iswap]==maxdist)	break;
			}

			out->dists[iswap] = dmydist;
			out->indexes[iswap] = j;

			maxdist = out->dists[0];
			for(iswap=1; iswap<k; iswap++)
			{
				if(maxdist<out->dists[iswap])	maxdist = out->dists[iswap];
			}
		}
	}

	return out;
}


double** GetEuclideanDistanceMatrix(PR_CANDDATA** candidates)
{
	PR_CASELIST* caselist = (PR_CASELIST*)((PR_CASEDATA*)candidates[0]->rootCaseData)->rootCaseList;
	unsigned int numFeat = caselist->featTagTable->numFeat;
	unsigned long numCand = caselist->totalNumCand;

	float* feat1;
	float* feat2;
	unsigned long c1, c2;
	unsigned int i;

	double** edMatrix = new double* [numCand];
	edMatrix[0] = new double [numCand*numCand];
	edMatrix[0][0] = 0.0;

	for(c1=1; c1<numCand-1; c1++)
	{
		edMatrix[c1] = edMatrix[0]+c1*numCand;
		edMatrix[c1][c1] = 0.0;
	}

	for(c1=0; c1<numCand-1; c1++)
	{
		feat1 = candidates[c1]->featValues;

		for(c2=c1+1; c2<numCand-1; c2++)
		{
			feat2 = candidates[c2]->featValues;
			double dmydist = 0.0;

			for(i=0; i<numFeat; i++)
			{
				double sub = (double)feat1[i]-(double)feat2[i];
				dmydist += sub*sub;
			}

			edMatrix[c1][c2] = edMatrix[c2][c1] = dmydist;
		}
	}

	return edMatrix;
}


double** PR_GenerateNeighborGraph(PR_CASELIST* caselist, int k)
{
	unsigned long numCand = caselist->totalNumCand;
	PR_CANDDATA** candidates = new PR_CANDDATA* [numCand];

	
	PR_CASEDATA* casedata = caselist->childNodeFirstCase;
	PR_CANDDATA* canddata;
	unsigned long count=0;

	while(casedata!=NULL)
	{
		canddata = casedata->childNodeFirstCand;
		while(canddata!=NULL)
		{
			candidates[count++] = canddata;
		}
		casedata = casedata->nodeNextCase;
	}


	double** edMap = GetEuclideanDistanceMatrix(candidates);


	double* dummy = new double [numCand];

	for(unsigned long j=0; j<numCand; j++)
	{
		memcpy(dummy,edMap[j],sizeof(double)*numCand);
		PR_QuickSortAscending64(dummy,numCand);
		
		for(unsigned long i=0; i<numCand; i++)
		{
			if(edMap[j][i]>dummy[k])	edMap[j][i]=-1.0;
		}
	}

	delete [] dummy;





	return edMap;
}


