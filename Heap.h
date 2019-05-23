#ifndef HEAP_H_
#define HEAP_H_
#include "AllocList.h"
#include "FreeList.h"

typedef struct Heap *HeapP;

typedef struct Heap {
	FreeListP freeList;
	AllocListP allocList;
	void* heapBuffer;
	size_t usedSize;
	size_t freeSize;
	size_t allocMode; // 0 first fit, 1 best fit, 2 worst fit
}Heap;

HeapP newHeap(size_t heapSize, size_t allocMode);
void destroyHeap(HeapP this);
void* allocateHeap(HeapP this, size_t allocSize);
size_t deallocateHeap(HeapP this, void* p);
void heapStats(HeapP this);

#endif
