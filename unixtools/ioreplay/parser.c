/*******************************************************
 * Author: JBanier
 * Date: 21 Feb 2007
 * Purpose: parse the data file build a linked list of operations
 *******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "listUtil.h"
#include "operation.h"
#include "parser.h"
#include "rep_syscall.h"

/*=======================GLOBALS*/
FILE	*g_fdDataFile;

/*=======================IMPL*/
int startParsing(const char* a_fileName){
	g_fdDataFile = fopen(a_fileName, "rb");
	if (g_fdDataFile == NULL) {
		printf("[ERROR] Unable to open %s err=%d\n",
			a_fileName,
			errno);
		return -1;
	}
}

listItem* buildOperationList(){
	unsigned int i_lineNumber = 1;
	void	*currentOperation;
	listItem *currListOperation = (listItem*) malloc(sizeof(listItem));
	listItem *startOfList = currListOperation;
	char s[1024];

	memset(currListOperation, 0, sizeof(listItem));
	memset(s, 0, 1024);

	while(fgets(s, 1024, g_fdDataFile) != NULL) {
		char	*s_operation, *s_argument;
		int i = 0;
		char *last, *tok[6];
		unsigned int i_fileDescriptor = 0;

		s[ strlen(s) - 1 ] = '\0';

		tok[0] = strtok(s, ",");
		while (tok[++i] = strtok(NULL, ","));

		s_operation = tok[0];
		i_fileDescriptor = atoi( tok[1] );
		s_argument = tok[2];

		currentOperation = buildOperation(s_operation,
					i_fileDescriptor,
					s_argument,
					i_lineNumber);

		if (currentOperation != NULL) {
			listItem	*t = (listItem*) malloc(sizeof(listItem));
			(*t).item = NULL;
			(*t).next = NULL;

			(*currListOperation).item = currentOperation;

			(*currListOperation).next = t;
			(*t).prev = currListOperation;
			currListOperation = t;
		}
		else {
			printf("[WARNING] Unrecognized operation line %d [%s]\n", i_lineNumber, (s == NULL ? "null" : s) );
		}

		i_lineNumber ++;
		memset(s, 0, 1024);
	}
	fclose(g_fdDataFile);
	return startOfList;
}

void* buildOperation(const char* a_operation, const unsigned int a_fileDescriptor, const char* a_argument, unsigned int a_lineNumber) {
	if (strcmp(a_operation, "open") == 0){
		char* whiteSpace;

		openOperation *op = (openOperation*) malloc(sizeof(openOperation));

		(*op).operation = strdup(a_operation);
		(*op).fileName = strdup(a_argument);
		(*op).oflags = 0;
		whiteSpace = index((*op).fileName, ' ');
		if (whiteSpace != NULL) {
      int j = 0;
			char* tok[16];
			unsigned int i = 0;
			*whiteSpace = '\0';
			whiteSpace ++;

			tok[0] = strtok(whiteSpace, "|");
			while (tok[++i] = strtok(NULL, "|"));

			for (j = 0; j < i; j++){
				if (strcmp(S_RDONLY, tok[j]) == 0) {
					(*op).oflags |= O_RDONLY;
				}
				else if (strcmp(S_WRONLY, tok[j]) == 0) {
					(*op).oflags |= O_WRONLY;
				}
				else if (strcmp(S_RDWR, tok[j]) == 0) {
					(*op).oflags |= O_RDWR;
				}
				else if (strcmp(S_APPEND, tok[j]) == 0) {
					(*op).oflags |= O_APPEND;
				}
				else if (strcmp(S_CREAT, tok[j]) == 0) {
					(*op).oflags |= O_CREAT;
				}
				else if (strcmp(S_EXCL, tok[j]) == 0) {
					(*op).oflags |= O_EXCL;
				}
				else if (strcmp(S_NDELAY, tok[j]) == 0) {
					(*op).oflags |= O_NDELAY;
				}
				else if (strcmp(S_NOCTTY, tok[j]) == 0) {
					(*op).oflags |= O_NOCTTY;
				}
				else if (strcmp(S_NONBLOCK, tok[j]) == 0) {
					(*op).oflags |= O_NONBLOCK;
				}
				else if (strcmp(S_TRUNC, tok[j]) == 0) {
					(*op).oflags |= O_TRUNC;
				}
#ifndef __sun
				else if (strcmp(S_DSYNC, tok[j]) == 0) {
					(*op).oflags |= O_DSYNC;
				}
#endif
				else if (strcmp(S_SYNC, tok[j]) == 0) {
					(*op).oflags |= O_SYNC;
				}
				else {
					printf("[WARNING] Ignored open option line %d [%s]\n", a_lineNumber, (tok[j] == NULL ? "null" : tok[j]) );
				}
			}
		}


		(*op).targetFD = a_fileDescriptor;
		(*op).lineNumber = a_lineNumber;
		return op;
	}
	else if (strcmp(a_operation, "close") == 0) {
		closeOperation *op = (closeOperation*) malloc(sizeof(closeOperation));

		(*op).operation = strdup(a_operation);
		(*op).targetFD = a_fileDescriptor;
		(*op).lineNumber = a_lineNumber;
		return op;
	}
	else if (strcmp(a_operation, "read") == 0) {
		readOperation *op = (readOperation*) malloc(sizeof(readOperation));

		(*op).operation = strdup(a_operation);
		(*op).targetFD = a_fileDescriptor;
		(*op).lineNumber = a_lineNumber;
		(*op).bufferSize = atoi(a_argument);
		return op;
	}
	else if (strcmp(a_operation, "write") == 0) {
		writeOperation *op = (writeOperation*) malloc(sizeof(writeOperation));

		(*op).operation = strdup(a_operation);
		(*op).targetFD = a_fileDescriptor;
		(*op).lineNumber = a_lineNumber;
		(*op).bufferSize = atoi(a_argument);
		return op;
	}
	else if (strcmp(a_operation, "lseek") == 0) {
		char whence[10];
		lseekOperation *op = (lseekOperation*) malloc(sizeof(lseekOperation));

		(*op).operation = strdup(a_operation);
		(*op).targetFD = a_fileDescriptor;
		(*op).lineNumber = a_lineNumber;
		sscanf(a_argument, "%d%s", &(*op).offset, &whence);

		if (strcmp(whence, S_SEEK_SET) == 0){
			(*op).whence = SEEK_SET;
		}
		else if (strcmp(whence, S_SEEK_CUR) == 0) {
			(*op).whence = SEEK_CUR;
		}
		else if (strcmp(whence, S_SEEK_END) == 0) {
			(*op).whence = SEEK_END;
		}
		return op;
	}
	return NULL;
}


#ifdef __UT
/*=============================UNIT-TEST*/
int main(int argc, char** argv) {
	startParsing("ut.dat");
	listItem *p = buildOperationList();
	while((*p).next != NULL) {
		openOperation *t = (openOperation*) (*p).item;
		printf(">%s", (*t).operation);
		if (strcmp((*t).operation, "open") == 0) {
			printf("[%s]", (*t).fileName);
		}
		printf("\n");
		p = (*p).next;
	}
}
#endif

