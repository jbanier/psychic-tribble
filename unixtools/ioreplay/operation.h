/*******************************************
 * Author: JBanier
 * Date: 20 Feb 2007
 * Purpose: replay io according to an input file
 *  generated using truss.
 *******************************************/

#ifndef __OPERATION
#define __OPERATION

typedef struct {
	unsigned int lineNumber;
	unsigned int targetFD; 
	const char* operation;

	const char* fileName;
	int   oflags;
	const char* mode;
} openOperation;

typedef struct {
	unsigned int lineNumber;
	unsigned int targetFD; 
	const char* operation;
} closeOperation;

typedef struct {
	unsigned int lineNumber;
	unsigned int targetFD; 
	const char* operation;

	unsigned int bufferSize;
} writeOperation;

typedef struct {
	unsigned int lineNumber;
	unsigned int targetFD; 
	const char* operation;

	unsigned int bufferSize;
} readOperation;

typedef struct {
	unsigned int lineNumber;
	unsigned int targetFD; 
	const char* operation;

	unsigned int offset;
	int whence;
} lseekOperation;

#endif

