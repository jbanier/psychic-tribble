/*******************************************************
 * Author: JBanier
 * Date: 22 Feb 2007
 * Purpose: implement the syscalls
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "listUtil.h"
#include "operation.h"
#include "parser.h"
#include "rep_syscall.h"

int	ga_fileDescriptors[1024];
char*	gb_BigBuffer;
unsigned int gi_bigBufferSize = 0;

void replay_error(const char* a_message, int syserror, void* operation){
	closeOperation *t = (closeOperation*) operation;
	//printf("[ERROR] line:%d \"%s\" -- (%s)\n", (*t).lineNumber, a_message, strerror(syserror));
	printf("[ERROR] line:%d \"%s\" -- (%d)\n", (*t).lineNumber, a_message, syserror);
}


void replaySyscall(void* a_operation) {
	closeOperation *t = (closeOperation*) a_operation;

	if (strcmp((*t).operation, "open") == 0) {
		openOperation *op = (openOperation*) t;

		int fd = open((*op).fileName, (*op).oflags, 0600);

		if (fd == -1) {
			replay_error("open failed", errno, t);
		}
		else {
			ga_fileDescriptors[ (*op).targetFD ] = fd;
		}
		return;
	}

	if (ga_fileDescriptors[ (*t).targetFD ] == 0) {
		replay_error("Bad file descriptor,open failed ?", errno, t);
		return;
	}

	if (strcmp((*t).operation, "close") == 0) {
		closeOperation	*op = (closeOperation*) t;

		if (close( ga_fileDescriptors[ (*op).targetFD ] ) == -1) {
			replay_error("close failed", errno, t);
		}
		ga_fileDescriptors[ (*op).targetFD ] = 0;
		return;
	}
	else if (strcmp((*t).operation, "read") == 0) {
		readOperation *op = (readOperation*) t;

		if ((*op).bufferSize > gi_bigBufferSize){
			free(gb_BigBuffer);
			gi_bigBufferSize = (*op).bufferSize;
			gb_BigBuffer = (char*) malloc( gi_bigBufferSize );
			memset(gb_BigBuffer, 'R', gi_bigBufferSize);
		}

		if (! read( ga_fileDescriptors[ (*op).targetFD ], gb_BigBuffer, (*op).bufferSize)){
			replay_error("read failed", errno, t);
		}
		return;
	}
	else if (strcmp((*t).operation, "write") == 0) {
		writeOperation *op = (writeOperation*) t;

		if ((*op).bufferSize > gi_bigBufferSize){
			free(gb_BigBuffer);
			gi_bigBufferSize = (*op).bufferSize;
			gb_BigBuffer = (char*) malloc( gi_bigBufferSize );
			memset(gb_BigBuffer, 'W', gi_bigBufferSize);
		}

		if (! write( ga_fileDescriptors[ (*op).targetFD ], gb_BigBuffer, (*op).bufferSize)){
			replay_error("write failed", errno, t);
		}

		return;
	}
	else if (strcmp((*t).operation, "lseek") == 0) {
		lseekOperation *op = (lseekOperation*) t;

		if (lseek( ga_fileDescriptors[ (*op).targetFD ], (*op).offset, (*op).whence) == -1) {
			replay_error("lseek failed", errno, t);
		}
		return;
	}
	else {
		printf("[BUG] -- unknown syscall %s\n", (*t).operation);
		return;
	}
}

