

#include "prEvaluation.h"
#include "prEvaluation.private.h"


PR_ROCDATA* PR_NewRocData(unsigned long num)
{
	PR_ROCDATA* data = new PR_ROCDATA;
	data->areaUnderRoc = 0.0;
	data->axisFPR = new float [num];
	data->axisTPR = new float [num];
	data->value = new float [num];
	data->notes = new char* [num];
	for(unsigned long n=0; n<num; n++)	data->notes[n]=NULL;
	data->num = num;
	data->numTP = data->numFP = 0;
	return data;
}

void PR_DeleteRocData(PR_ROCDATA* data)
{
	delete [] data->axisFPR;
	delete [] data->axisTPR;
	delete [] data->value;
	for(unsigned long n=0; n<data->num; n++)	
		if(data->notes[n]!=NULL)	delete [] data->notes[n];
	delete [] data->notes;
	delete data;
}

/*
typedef struct _PR_Classification_Evaluation_One_Threashold
{
	double tpr, fpr, error;
	float threshold;
}
PR_CLASSIFICATIONPERFORMANCE;
*/

PR_CLASSIFICATIONPERFORMANCE* PR_GetThresholdWithMinErrorFromRocData(PR_ROCDATA* rocData)
{
	PR_CLASSIFICATIONPERFORMANCE* evData = new PR_CLASSIFICATIONPERFORMANCE;
	evData->error = 2.0f;

	for(unsigned long n=0; n<rocData->num; n++)
	{
		float dmyError = (rocData->axisFPR[n] + 1.0f-rocData->axisTPR[n])/2.0f;
		if(evData->error>dmyError)
		{
			evData->error = dmyError;
			evData->fpr = rocData->axisFPR[n];
			evData->tpr = rocData->axisTPR[n];
			if(n)	evData->threshold = (rocData->value[n]+rocData->value[n-1])/2.0f;
			else	evData->threshold = rocData->value[n]-(rocData->value[n]+rocData->value[n+1])/5.0f;
		}
	}

//	fprintf(stderr,"error=%.3e(fpr%.3e,tpr%.3e) <-> thd%.3e\n",evData->error,evData->fpr,evData->tpr,evData->threshold);
	return evData;
}

PR_CLASSIFICATIONPERFORMANCE* PR_GetClassificationPerformanceOfOneThresholdFromRocData(PR_ROCDATA* rocData, float threshold)
{
	PR_CLASSIFICATIONPERFORMANCE* evData = new PR_CLASSIFICATIONPERFORMANCE;
	evData->error = 2.0f;

	fprintf(stderr,"thd%.3e; ",threshold);

	for(unsigned long n=1; n<rocData->num; n++)
	{
		if(rocData->value[n]<threshold)
		{
			evData->error = (rocData->axisFPR[n-1] + 1.0f-rocData->axisTPR[n-1])/2.0f;
			evData->fpr = rocData->axisFPR[n-1];
			evData->tpr = rocData->axisTPR[n-1];
			evData->threshold = threshold;
			fprintf(stderr,"err%.3e(fpr%.3e,tpr%.3e)\n",evData->error,evData->fpr,evData->tpr);
			return evData;
		}
	}

	fprintf(stderr,"not found..\n");
	return NULL;
}
/**/

int 
PR_WriteRocTxt(PR_ROCDATA* data, char* filename, unsigned long maxNumLine)
{
//	printf("inWrite: ");

	unsigned long n,cnt=0;

	char* boo = new char [data->num];
	memset(boo,0x00,sizeof(char)*data->num);
//	printf("num%d,",data->num);

	for(n=0; n<data->num; n++)
	{
		if(n<=2 || n>=data->num-3)
		{
			boo[n]=1;
		}
		else if(data->axisTPR[n]>data->axisTPR[n-1])
		{
			boo[n-1]=boo[n]=1;
		}
	}
//	printf("a");

	for(n=0; n<data->num; n++)
	{
		if(boo[n])	cnt++;
	}
//	printf("cnt%d<->max%d,",cnt,maxNumLine);

	unsigned long interval=(unsigned long)((float)data->num/(float)maxNumLine+0.5f);
	if(interval==0)		interval=1;
//	cnt=0;
	
//	printf("make_\"%s\",",filename);
//	fprintf(stderr,"line,%d,az%e,",data->num,data->areaUnderRoc);

	FILE* fp = fopen(filename,"w");
	if(fp==NULL)	printf("era!!!!!");

	fprintf(fp,"numTP=%d,numFP=%d\n",data->numTP,data->numFP);
	fprintf(fp,"line,%d\n",data->num);
	fprintf(fp,"underROC,%e\n\n",data->areaUnderRoc);
//	printf("d");

	fprintf(fp,"num,likelihood,ratioFP,numFP,ratioTP,note\n");
	for(n=0; n<data->num; n++)
	if(boo[n] || (n+1)%interval==0)
	{
//		if(n==0 || n==data->num-1 || (n+1)%interval==0)
			fprintf(fp,"%d,%e,%e,%e,%e,%s\n",
						n,data->value[n],
						data->axisFPR[n],(float)data->numFP*data->axisFPR[n],
						data->axisTPR[n],data->notes[n]);
//		if(n>0)	cnt++;
	}
//	printf("e");

	fclose(fp);
	delete [] boo;
//	printf("ok!\n");

	return PR_EOT;
}


PR_ROCDATA*
PR_CalculateTwoClassROC(
	unsigned long num, float* data, unsigned char* label, double* weight, char** memo)
{
	unsigned long ii, jj;
	char booW=1;
//	printf("\n");


	// weight memory malloc
	if(weight==NULL)
	{
		booW=0;
		weight = new double [num];
		for(ii=0; ii<num; ii++)		weight[ii]=1.0;
	}
//	printf("set,");
	

	// weight normalization
	double wTP=0.0, wFP=0.0;
	for(ii=0; ii<num; ii++)
	{
		if(label[ii])	wTP+=weight[ii];
		else			wFP+=weight[ii];
	}
	for(ii=0; ii<num; ii++)
	{
		if(label[ii])	weight[ii]/=wTP;
		else			weight[ii]/=wFP;
	}
//	printf("nor,");
	

	// calc ROC under area
	unsigned long* rank = PR_QuickSortDescendingWithRank(data, num);
	double underROC=0.0, currentTPR=0.0, currentFPR=0.0;
//	printf("sort(n%d),",num);

	unsigned long cnt=1;
	for(jj=1; jj<num; jj++)
		if(data[jj]!=data[jj-1])	cnt++;
	PR_ROCDATA* rocData = PR_NewRocData(cnt);
//	printf("new(cnt%d)\n",cnt);

	unsigned long interval=cnt/20+1;
	ii=cnt=0;
	while(ii<num)
	{
		float targetValue=data[ii];
		double addTPR=0.0, addFPR=0.0;
		jj=0;
		char* str = new char [2048];
		str[0]='\0';

		while(1)
		{
			unsigned long dmyRank=rank[ii+jj];
			if(label[dmyRank])	addTPR+=weight[dmyRank];
			else				addFPR+=weight[dmyRank];
			
			if(memo!=NULL)
			if(memo[dmyRank]!=NULL)
			{
				int a,len1=(int)strlen(str),len2=(int)strlen(memo[dmyRank]);
				for(a=0; a<len1; a++)
					if(strncmp(&(str[a]),memo[dmyRank],(size_t)len2)==0)	break;

		//		printf("%s<-%s(%d/%d)",str,memo[dmyRank],a,len1);

			//	if(strpbrk(str,memo[dmyRank])==NULL)
				if(len1==0 || a>=len1)
				{
					strcat(str,memo[dmyRank]);
					int len = (int)strlen(str);
					str[len]=' ';
					str[len+1]='\0';
		//			printf("=%s,",str);
				}
		//		else
		//			printf("=x,");
			}

			if(ii+jj+1>=num)						break;
			else if(targetValue!=data[ii+jj+1])		break;
			jj++;
		}
	//	if(strlen(str)>2)		printf("\n");

		underROC+=addFPR*(addTPR*0.5+currentTPR);
		currentFPR+=addFPR;
		currentTPR+=addTPR;
		
		int boo=0;
		rocData->value[cnt]=targetValue;
		rocData->axisFPR[cnt]=(float)currentFPR;
		rocData->axisTPR[cnt]=(float)currentTPR;
	//	rocData->axisFPR[cnt]+=(float)currentFPR;
	//	rocData->axisTPR[cnt]+=(float)currentTPR;

		if(strlen(str)>1)
		{
			boo=1;
			rocData->notes[cnt] = new char [strlen(str)+1];
			strcpy(rocData->notes[cnt],str);
		}

	//	if(boo || rocData->notes[cnt]!=NULL || cnt%interval==0)
	//	{
	//		printf("[%05d] (%d+%d) value%.3f fp%.3e tp%.3e az%.3e len%d ",
	//				cnt,ii,jj,targetValue,currentFPR,currentTPR,underROC,strlen(str));
	//		if(rocData->notes[cnt]!=NULL)	printf("*");
	//								//		printf("%s",rocData->notes[cnt]);
	//		printf("\n");
	//	}
		
		delete [] str;
		ii+=jj+1;
		cnt++;
	}

	rocData->areaUnderRoc=underROC;
//	printf("areaUnderRoc=%e\n",rocData->areaUnderRoc);
//	getchar();

	if(!booW)	delete [] weight;
	delete [] rank;

	return rocData;
}



PR_ROCDATA*
PR_CalculateSimpleTwoClassROC(unsigned long num, float* data, unsigned char* label)
{
	unsigned long ii, jj;
	unsigned long cntFP=0, cntTP=0;
	
	// weight normalization
	for(ii=0; ii<num; ii++)
	{
		if(label[ii])	cntTP++;//+=weight[ii];
		else			cntFP++;//wFP+=weight[ii];
	}
	double wTP=1.0/(double)cntTP, wFP=1.0/(double)cntFP;
//	printf("nor,");
	

	// calc ROC under area
	unsigned long* rank = PR_QuickSortDescendingWithRank(data, num);
	double underROC=0.0, currentTPR=0.0, currentFPR=0.0;
//	printf("sort(n%d),",num);

	unsigned long cnt=1;
	for(jj=1; jj<num; jj++)
		if(data[jj]!=data[jj-1])	cnt++;
	PR_ROCDATA* rocData = PR_NewRocData(cnt);
//	printf("new(cnt%d)\n",cnt);

	unsigned long interval=cnt/20+1;
	ii=cnt=0;
	while(ii<num)
	{
		float targetValue=data[ii];
		double addTPR=0.0, addFPR=0.0;
		jj=0;

		while(1)
		{
			unsigned int dmyRank=rank[ii+jj];
			if(label[dmyRank])	addTPR+=wTP;//weight[dmyRank];
			else				addFPR+=wFP;//weight[dmyRank];

			if(ii+jj+1>=num)						break;
			else if(targetValue!=data[ii+jj+1])		break;
			jj++;
		}
	//	if(strlen(str)>2)		printf("\n");

		underROC+=addFPR*(addTPR*0.5+currentTPR);
		currentFPR+=addFPR;
		currentTPR+=addTPR;
		
		rocData->value[cnt]=targetValue;
		rocData->axisFPR[cnt]=(float)currentFPR;
		rocData->axisTPR[cnt]=(float)currentTPR;

	//	if(rocData->notes[cnt]!=NULL || cnt%interval==0)
	//	{
	//		printf("[%05d] (%d+%d) value%.3f fp%.3e tp%.3e az%.3e\n",
	//				cnt,ii,jj,targetValue,currentFPR,currentTPR,underROC);
	//	}
		
		ii+=jj+1;
		cnt++;
	}

	rocData->areaUnderRoc = underROC;
	rocData->numFP = cntFP;
	rocData->numTP = cntTP;

	delete [] rank;

	return rocData;
}

PR_ROCDATA*
PR_CalculateSimpleTwoClassRocByDistance(unsigned long num, float* data, unsigned char* label)
{
	unsigned long ii, jj;
	unsigned long cntFP=0, cntTP=0;
	
	// weight normalization
	for(ii=0; ii<num; ii++)
	{
		if(label[ii])	cntTP++;//+=weight[ii];
		else			cntFP++;//wFP+=weight[ii];
	}
	double wTP=1.0/(double)cntTP, wFP=1.0/(double)cntFP;
//	printf("nor,");
	

	// calc ROC under area
	unsigned long* rank = PR_QuickSortAscendingWithRank(data, (unsigned long)num);
	double underROC=0.0, currentTPR=0.0, currentFPR=0.0;
//	printf("sort(n%d),",num);

	unsigned long cnt=1;
	for(jj=1; jj<num; jj++)
		if(data[jj]!=data[jj-1])	cnt++;
	PR_ROCDATA* rocData = PR_NewRocData(cnt);
//	printf("new(cnt%d)\n",cnt);

	unsigned long interval=cnt/20+1;
	ii=cnt=0;
	while(ii<num)
	{
		float targetValue=data[ii];
		double addTPR=0.0, addFPR=0.0;
		jj=0;

		while(1)
		{
			unsigned long dmyRank=rank[ii+jj];
			if(label[dmyRank])	addTPR+=wTP;//weight[dmyRank];
			else				addFPR+=wFP;//weight[dmyRank];

			if(ii+jj+1>=num)						break;
			else if(targetValue!=data[ii+jj+1])		break;
			jj++;
		}
	//	if(strlen(str)>2)		printf("\n");

		underROC+=addFPR*(addTPR*0.5+currentTPR);
		currentFPR+=addFPR;
		currentTPR+=addTPR;
		
		rocData->value[cnt]=targetValue;
		rocData->axisFPR[cnt]=(float)currentFPR;
		rocData->axisTPR[cnt]=(float)currentTPR;

	//	if(rocData->notes[cnt]!=NULL || cnt%interval==0)
	//	{
	//		printf("[%05d] (%d+%d) value%.3f fp%.3e tp%.3e az%.3e\n",
	//				cnt,ii,jj,targetValue,currentFPR,currentTPR,underROC);
	//	}
		
		ii+=jj+1;
		cnt++;
	}

	rocData->areaUnderRoc = underROC;
	rocData->numFP = cntFP;
	rocData->numTP = cntTP;

	delete [] rank;

	return rocData;
}


PR_CLASSIFICATIONPERFORMANCE* PR_NewClassificationPerformance()
{
	PR_CLASSIFICATIONPERFORMANCE* data = new PR_CLASSIFICATIONPERFORMANCE;
	return data;
}

void PR_DeleteClassificationPerformance(PR_CLASSIFICATIONPERFORMANCE* data)
{
	delete data;
}

PR_CLASSIFICATIONPERFORMANCE* PR_GetClassificationPerformanceOfOneTPR(PR_ROCDATA* rocData, float tpr)
{
	int booDebug=0;

	PR_CLASSIFICATIONPERFORMANCE* data = PR_NewClassificationPerformance();
	float accidentError = 1.0f/(float)rocData->num;
	if(booDebug)	printf("accidentError%e\n",accidentError);

	unsigned int i;
	for(i=0; i<rocData->num; i++)
	{
		if(rocData->axisTPR[i]>=tpr-accidentError)
		{
			data->error = 0.5f*(rocData->axisFPR[i]+1.0f-rocData->axisTPR[i]);
			data->fpr = rocData->axisFPR[i];
			if(i<rocData->num-1)	data->threshold = (rocData->value[i]+rocData->value[i+1])/2.0f;
			else					data->threshold = rocData->value[i];
			data->tpr = rocData->axisTPR[i];
			if(booDebug)	printf("> error%.3f, tpr%.3f, fpr%.3f, thd%.3f\n",data->error,data->tpr,data->fpr,data->threshold);
			break;
		}
	}
//	if(booDebug)	printf("i%d\n",i);

	return data;
}

PR_CLASSIFICATIONPERFORMANCE* PR_GetClassificationPerformanceOfMaxTPR(PR_ROCDATA* rocData)
{
	int booDebug=0;

	PR_CLASSIFICATIONPERFORMANCE* data = PR_NewClassificationPerformance();
//	float accidentError = 1.0f/(float)rocData->num;
//	if(booDebug)	printf("accidentError%e\n",accidentError);

	data->tpr=-1.0f;

	unsigned int i;
	for(i=0; i<rocData->num; i++)
	{
		if(data->tpr<rocData->axisTPR[i] && rocData->value[i]>-10.0f)
		{
			data->error = 0.5f*(rocData->axisFPR[i]+1.0f-rocData->axisTPR[i]);
			data->fpr = rocData->axisFPR[i];
			if(i<rocData->num-1)	data->threshold = (rocData->value[i]+rocData->value[i+1])/2.0f;
			else					data->threshold = rocData->value[i];
			data->tpr = rocData->axisTPR[i];
			if(booDebug)	printf("> error%.3f, tpr%.3f, fpr%.3f, thd%.3f\n",data->error,data->tpr,data->fpr,data->threshold);
		}
	}
//	if(booDebug)	printf("i%d\n",i);

	return data;
}

