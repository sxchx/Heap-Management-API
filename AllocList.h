#ifndef ALLOCLIST_H_
#define ALLOCLIST_H_
#include "ListNode.h"
#include "Info.h"

typedef struct AllocList *AllocListP;

typedef struct AllocList {
	ListNodeP head;
}AllocList;

AllocListP newAllocList();
void destroyAllocList(AllocListP this);
size_t addAllocList(AllocListP this, size_t allocPos, size_t allocSize);
size_t removeAllocList(AllocListP this, size_t allocPos);
size_t countAllocChunks(AllocListP this);

#endif
