#include "Info.h"
#include <stdlib.h>
#include <stdio.h>
#include "ListNode.h"
#include "Info.h"

// constructor for a ListNode
// takes a pointer to the next ListNode, and a pointer to an Info
// returns a pointer to ListNode
ListNodeP newListNode(ListNodeP next, InfoP info) 
{
	ListNodeP newNodeP = malloc(sizeof(ListNode));
	newNodeP->info = info;
	newNodeP->next = next;	
	return newNodeP;
}

// destructor for a ListNode
// takes the ListNode to free
// frees its info and the ListNode itself
void destroyFreeListNode(ListNodeP listNode)
{
	free(listNode->info);
	free(listNode);
} 
