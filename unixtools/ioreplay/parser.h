/*******************************************************
 * Author JBanier
 * Date 22 Feb 2007
 * Purpose:  parse the data file build a linked list of operations
 ********************************************************/
#ifndef __PARSER
#define __PARSER
int startParsing(const char*);
listItem *buildOperationList();
void* buildOperation(const char* , const unsigned int , const char* , unsigned int );
#endif
