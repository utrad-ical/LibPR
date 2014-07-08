#pragma warning(disable: 4996)

#include "./prTableInputFile.h"
#include "./prTableInputFile.private.h"



PR_INPUTFILE* PR_NewInputFile()
{
	PR_INPUTFILE* info = new PR_INPUTFILE;
	info->idCase = 0;
	info->name = new char [256];
	info->specific = NULL;
	return info;
}

void PR_DeleteInputFile(PR_INPUTFILE* info)
{
	delete [] info->name;
	if(info->specific!=NULL)	delete info->specific;
	delete info;
}



PR_TABLEINPUTFILE* PR_NewTableInputFile(unsigned int num)
{
	PR_TABLEINPUTFILE* tabula = new PR_TABLEINPUTFILE;
	tabula->info = new PR_INPUTFILE* [num];
	for(unsigned int i=0; i<num; i++)
	{
		tabula->info[i] = PR_NewInputFile();
	}
	tabula->num = num;
	return tabula;
}

void PR_DeleteTableInputFile(PR_TABLEINPUTFILE* tabula)
{
	for(unsigned int i=0; i<tabula->num; i++)
	{
		PR_DeleteInputFile(tabula->info[i]);
	}
	delete [] tabula->info;
	delete tabula;
}

PR_TABLEINPUTFILE* PR_NewReadTableInputFile(char* listname)
{
	int bStdOut=0;
	fprintf(stderr,"text \"%s\"..\n",listname);

	FILE* fpi=fopen(listname,"r");
	if(fpi==NULL)
	{
		fprintf(stderr,"unfound text \"%s\"..",listname);
		return NULL;
	}

	unsigned int num;
	char dmystr[256];
	fscanf(fpi,"%lu %s\n",&num,dmystr);

	if(bStdOut)
		printf("num%lu\n",num);
	
	PR_TABLEINPUTFILE* tabula = PR_NewTableInputFile(num);

	for(unsigned int i=0; i<num; i++)
	{
		int a=fscanf(fpi,"%s %lu\n",tabula->info[i]->name,&(tabula->info[i]->idCase));
		if(a==EOF)
		{
			fprintf(stderr,"wrong file num%lu <-> EOF%lu..",num,i);
			PR_DeleteTableInputFile(tabula);
			return NULL;
		}

		if(bStdOut)
			printf("[#%d] id%04u: %s \n",i,tabula->info[i]->idCase,tabula->info[i]->name);
	}

	return tabula;
}


