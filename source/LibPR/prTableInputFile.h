////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibPR : library for classification
//
//			prInputFileTable.h : manage datas for training any classifiers (large size data)
//			main coder: M. Nemoto
//
//			update history
//			(previous version = GetFileNameTable)
//			2008.12.12	start coding
//			2009.11.20	new create "PR_INPUTFILELIST" group
//			(recent version)
//			2009.12.10	join LIB_PR group
//			2009.12.10	"PR_TABLEINPUTFILE" series
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef GET_INPUT_FILE_TABLE_H

#include <vector>
#include <string>
//#include "windows.h"

#include "./prFeatTag.h"


typedef struct _PR_INPUT_FILE
{
	unsigned int idCase;
	char* name;
	void* specific;
}
PR_INPUTFILE;

typedef struct _PR_TABLE_INPUT_FILE
{
	unsigned int num;
	PR_INPUTFILE** info;
}
PR_TABLEINPUTFILE;


PR_INPUTFILE* PR_NewInputFile();
void PR_DeleteInputFile(PR_INPUTFILE* info);

PR_TABLEINPUTFILE* PR_NewTableInputFile(unsigned int num);
void PR_DeleteTableInputFile(PR_TABLEINPUTFILE* tabula);

PR_TABLEINPUTFILE* PR_NewReadTableInputFile(char* listname);




#define GET_INPUT_FILE_TABLE_H
#endif

