#ifndef FREELIST_H_
#define FREELIST_H_
#include "ListNode.h"
#include "Info.h"

typedef struct FreeList *FreeListP;

typedef struct FreeList {
	ListNodeP head;
}FreeList;

FreeListP newFreeList(size_t maxSize);
void destroyFreeList(FreeListP this);
size_t allocateFirstFitFreeList(FreeListP this, size_t allocSize);
size_t allocateBestFitFreeList(FreeListP this, size_t allocSize);
size_t allocateWorstFitFreeList(FreeListP this, size_t allocSize);
int deallocateFreeList(FreeListP this, size_t allocPos, size_t allocSize);
size_t countFreeHoles(FreeListP this);
size_t totalFreeSpace(FreeListP this);

#endif
