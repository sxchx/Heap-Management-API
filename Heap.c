#include <stdlib.h>
#include <stdio.h>
#include "Heap.h"
#include "FreeList.h"
#include "AllocList.h"

// Constructor for a new Heap
// Takes the size of the heap and the mode of allocation
// Returns a pointer to the heap
HeapP newHeap(size_t heapSize, size_t allocMode)
{
	// create a new Heap
	HeapP heap = malloc(sizeof(Heap));
	// set its allocation mode - 0 first fit, 1 best fit, 2 worst fit
	heap->allocMode = allocMode;
	// create a FreeList and an AllocList
	FreeListP freeList = newFreeList(heapSize);
	AllocListP allocList = newAllocList();
	heap->freeList = freeList;
	heap->allocList = allocList;
	// set the size of the heap
	heap->usedSize = 0;
	heap->freeSize = heapSize;
	// initialize the heap buffer
	heap->heapBuffer = malloc(heapSize);
	return heap;
};

// Deconstructor for a Heap
// Takes a Heap to free
// Also frees the AllocList and FreeList in the heap
void destroyHeap(HeapP this) 
{
	// Call the deconstructor for the FreeList and AllocList
	destroyFreeList(this->freeList);
	destroyAllocList(this->allocList);
	// Free the heapBuffer
	free(this->heapBuffer);
	// Free the Heap
	free(this);
}

// Allocate an item to the heap
// Takes a heap and the size of the item to allocate
void* allocateHeap(HeapP this, size_t allocSize)
{
	// call the apprpriate FreeList allocation function
	size_t position;
	size_t size;
	if (this->allocMode == 0) {
		// First Fit
		position = allocateFirstFitFreeList(this->freeList, allocSize);
	}
	else if (this->allocMode == 1) {
		// Best Fit
		position = allocateBestFitFreeList(this->freeList, allocSize);
	}
	else if (this->allocMode == 2) {
		// Worst Fit
		position = allocateWorstFitFreeList(this->freeList, allocSize);
	}
	else {
		// invalid allocMode
		printf("Invalid allocation mode: %zu - 0 First Fit, 1 Best Fit, 2 Worst Fit\n", this->allocMode);
		exit(1);
	}
	// call the addAllocList to add in an entry
	if (position != -1) {
		size = addAllocList(this->allocList, position, allocSize);
		// update the heap used size and free size
		this->usedSize += size;
		this->freeSize -= size;
	}
	else {
		printf("Allocation unsuccesful - No free space on Heap\n");
		return NULL;
	}
	// return a pointer in the newly allocated part of the heap
	void * addr = this->heapBuffer + position; 
	return addr;
}

// Deallocate an item from the Heap
// Takes a Heap and the address in the heap to remove item from 
size_t deallocateHeap(HeapP this, void* p)
{
	// find the memory chunk pos by offsetting by heap
	size_t position = p - this->heapBuffer;
	// use the position to find its size
	size_t size = removeAllocList(this->allocList, position);
	// call the deallocate FreeList function
	if (size != -1) {
		int deallocation = deallocateFreeList(this->freeList, position, size);
		if (deallocation == -1) {
			return -1;
		}
		// update the heap used size and free size
		this->usedSize -= size;
		this->freeSize += size;
	}
	else {
		return -1;
	}
	return 0;
}

// Prints the statistics of the Heap
// takes a Heao
void heapStats(HeapP this)
{
	// Ratio of the heap used
	printf("\nRatio of heap - Used : Free - %zu : %zu\n", this->usedSize, this->freeSize);
	// Number of free holes
	size_t freeHoles = countFreeHoles(this->freeList);
	printf("Number of Free Holes: %zu\n", freeHoles);
	// Average size of free holes 
	size_t freeSpace = totalFreeSpace(this->freeList);
	size_t average = freeSpace / freeHoles;
	printf("Average Size of Free Holes: %zu\n", average);
	// Number of allocated chunks
	size_t allocChunks = countAllocChunks(this->allocList);
	printf("Number of Allocated Chunks: %zu\n\n", allocChunks);
}
