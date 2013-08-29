/*************************************
 * Author: JBanier
 * Date: 21 Feb 2007
 * Purpose: link list manipulation
 *************************************/

#ifndef __LIST_UTIL
#define __LIST_UTIL

typedef struct {
	void* prev;
	void* next;
	void*	item;
} listItem;

#endif

