#ifndef LISTNODE_H_
#define LISTNODE_H_
#include "Info.h"

typedef struct ListNode *ListNodeP;

typedef struct ListNode {
	InfoP info;
	ListNodeP next;
}ListNode;

ListNodeP newListNode(ListNodeP next, InfoP info);
void destroyFreeListNode(ListNodeP listNode);

#endif
