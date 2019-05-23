#include <stdlib.h>
#include <stdio.h>
#include "FreeList.h"
#include "ListNode.h"
#include "Info.h"

// constructor for a FreeList
// takes the size of the FreeList
// returns a pointer to a FreeList
FreeListP newFreeList(size_t maxSize)
{
	// create a new FreeList
	FreeListP newFreeList = malloc(sizeof(FreeList));
	// specify the amount of free space starting from position 0
	InfoP info = newInfo(0, maxSize);
	// create a ListNode with this new Info
	ListNodeP listNode = newListNode(NULL, info);
	// this node will be the head of the FreeList
	newFreeList->head = listNode;
	return newFreeList;
}

// destructor for a FreeList
// takes the FreeList to free
// also frees all ListNodes in the FreeList
void destroyFreeList(FreeListP this)
{
	// check if head is not null
	if (this->head == NULL) {
		free(this);
		return;
	}
	// create a temp vars to point to head and next
	ListNodeP temp = this->head;
	ListNodeP next = this->head->next;
	// loop through the entire list
	while (temp != NULL) {
		// remove the ListNode
		destroyFreeListNode(temp);
		// move along the list
		temp = next;
		if (next != NULL) {
			next = next->next;
		}
	}
	// free the FreeList
	free(this);
}



// allocates space in the given free list based on the first free spot found
// takes a FreeList and the size of the allocation
// returns the position of the allocation
size_t allocateFirstFitFreeList(FreeListP this, size_t allocSize)
{
	// We are inserting at the head - item is smaller than memory chunk
	if (allocSize < this->head->info->size) {
		// decrease the size of the head by the allocSize
		this->head->info->size -= allocSize;
		// store the starting pos for the item
		size_t position = this->head->info->pos;
		// move up the starting pos of head
		this->head->info->pos += allocSize;		
		return position;
	}
	// We are inserting at the head - item is same size as memory chunk;
	else if (allocSize == this->head->info->size) {
		// head->next will be our new head
		ListNodeP temp = this->head;
		this->head = this->head->next;
		// return its pos - don't forget to free the old head
		size_t position = temp->info->pos;
		destroyFreeListNode(temp);
		return position;
	}
	// We are inserting in the middle
	else if (this->head != NULL) {		
		// Keep track of the previous and current node
		ListNodeP prev = this->head;
		ListNodeP curr = this->head->next;
		// loop till curr==NULL checking if memory chunk is big enough
		while (curr != NULL) {
			// Item is smaller than memory chunk
			if (allocSize < curr->info->size) {				
				// decrease the size of the memory chunk by allocSize
				curr->info->size -= allocSize;
				// store the starting pos for the item
				size_t position = curr->info->pos;
				// move up the starting pos of this chunk
				curr->info->pos += allocSize;
				return position;
			}
			// Item is same size as memory chunk
			else if (allocSize == curr->info->size) {				
				// make the next of prev point to next of curr
				prev->next = curr->next;
				// store the starting pos for the item
				size_t position = curr->info->pos;
				// free curr as we have skipped over it
				destroyFreeListNode(curr);
				return position;
			}
			// otherwise we want to move along
			prev = curr;
			curr = curr->next;
		}
	}
	// there is not enough space !! 
	return -1;
}

// Allocates space in the FreeList using Best Fit allocation
// takes a FreeList and the size of the allocation
// returns the position of allocation
size_t allocateBestFitFreeList(FreeListP this, size_t allocSize)
{
	// this points to the start of the list
	ListNodeP temp = this->head;
	// this will be used to point to the "best" slot
	ListNodeP best = NULL;
	// loop through the whole list
	while (temp != NULL) {
		// if the item will fit into this memory chunk
		if (allocSize <= temp->info->size) {
			// if we haven't yet found a 'best' spot 
			if (best == NULL) {
				best = temp;
			}
			// otherwise check to see if this spot is 'better' than current best
			else {
				if (temp->info->size < best->info->size) {
					best = temp;
				}
			}
		}
		// otherwise we want to move along
		temp = temp->next;
	}
	// check if our item will fit in the list
	if (best != NULL) {
		// check if allocSize is smaller than memory chunk
		if (allocSize < best->info->size) {			
			// store the starting position
			size_t position = best->info->pos;
			// update the position and size
			best->info->pos += allocSize;
			best->info->size -= allocSize;
			return position;
		}
		// if allocSize is the same size as the memory chunk
		else {
			// check if we are inserting at head
			if (best->info->pos == this->head->info->pos) {
				// head->next will be our next head
				temp = this->head;
				this->head = this->head->next;
				// free old head and return pos
				size_t position = temp->info->pos;
				destroyFreeListNode(temp);
				return position;
			}				
			// find the prev 
			temp = this->head;
			while (temp->next != NULL) {
				// if temp is the prev item to best, break
				if (temp->next->info->pos == best->info->pos) {
					break;
				}
				// move along one
				temp = temp->next;
			}
			// link up temp to best->next
			temp->next = best->next;
			size_t position = best->info->pos;
			destroyFreeListNode(best);
			// if we are removing head then set head to null
			if (this->head->info == NULL) {
				this->head = NULL;
			}
			return position;
		}
	}
	// otherwise there is no space for the item so return -1
	return -1;
}

// Allocates space in the FreeList using Worst Fit allocation
// takes a FreeList and the size of the allocation
// returns the position of allocation
size_t allocateWorstFitFreeList(FreeListP this, size_t allocSize)
{
	// this points to the start of the list
	ListNodeP temp = this->head;
	// this will be used to point to the "worst" slot
	ListNodeP worst = NULL;
	// loop through the whole list
	while (temp != NULL) {
		// if the item will fit into this memory chunk
		if (allocSize <= temp->info->size) {
			// if we haven't yet found a 'worst' spot
			if (worst == NULL) {
				worst = temp;
			}
			// otherwise check to see if this spot is 'worse' than current worst
			else {
				if (temp->info->size > worst->info->size) {
					worst = temp;
				}
			}
		}
		// otherwise we want to move along
		temp = temp->next;
	}
	// check if our item will fit in the list
	if (worst != NULL) {
		// check if allocsize is smaller than the memory chunk
		if (allocSize < worst->info->size) {
			//store the starting position
			size_t position = worst->info->pos;
			// update the position and size
			worst->info->pos += allocSize;
			worst->info->size -= allocSize;
			return position;
		}
		// if allocSize is the same size as the memory chunk
		else {
			// check if we are inserting at head
			if (worst->info->pos == this->head->info->pos) {
				// head->next will be our next head
				temp = this->head;
				this->head = this->head->next;
				// free old head and return pos
				size_t position = temp->info->pos;
				destroyFreeListNode(temp);
				return position;
			}
			// inserting in the middle				
			// find the prev
			temp = this->head;
			while (temp->next != NULL) {
				// if temp is the previous item to worst, break
				if (temp->next->info->pos == worst->info->pos) {
					break;
				}
				// move along one
				temp = temp->next;
			}
			// link up temp to worst->next
			temp->next = worst->next;
			size_t position = worst->info->pos;
			destroyFreeListNode(worst);
			//if we are removing the head then set head to null
			if (this->head->info == NULL) {
				this->head = NULL;
			}
			return position;
		}
	}
	// otherwise there is no space for the item so return -1
	return -1;

}

// There has been deallocation from AllocList so we need to free space in FreeList
// takes a FreeList, position and size of deallocation and returns an int
int deallocateFreeList(FreeListP this, size_t allocPos, size_t allocSize)
{
	// if there is no head - ie there is nothing free
	if (this->head == NULL) {
		// create a new info/listnode 
		InfoP insertInfo = newInfo(allocPos, allocSize);
		ListNodeP insert = newListNode(NULL, insertInfo);
		// make it head
		this->head = insert;
		return 0;
	}
	// if the deallocation comes straight before head
	else if ((allocPos + allocSize) == this->head->info->pos) {
		// then this will be merging with head
		this->head->info->size += allocSize;
		this->head->info->pos = allocPos;
		// what do i return ahhh
		return 0;
	}
	// if the deallocation occurs somewhere else before head
	else if ((allocPos + allocSize) < this->head->info->pos) {
		// this will become the new head, its next will be current head
		InfoP insertInfo = newInfo(allocPos, allocSize);
		ListNodeP insert = newListNode(this->head, insertInfo);
		this->head = insert;
		return 0;
	}
	// if the deallocation occurs somewhere in the middle
	else if (this->head != NULL) {
		// create pointer for head and head.next
		ListNodeP prev = this->head;
		ListNodeP curr = this->head->next;
		// if there is only a head and curr is empty
		if (curr == NULL) {
			// if deallocating right after head, merge with head
			if (allocPos == (prev->info->pos + prev->info->size)) {
				prev->info->size += allocSize;
				return 0;
			} 
			// otherwise its somehere in the middle
			else {
				InfoP insertInfo = newInfo(allocPos, allocSize);
				ListNodeP insert = newListNode(NULL, insertInfo);
				prev->next = insert;
				return 0;
			}
		}
		// loop through the FreeList
		while (curr != NULL) {
			// if it doesn't touch prev or curr
			if (((allocPos + allocSize) < curr->info->pos) && (allocPos > (prev->info->pos + prev->info->size))) {
				// create a new ListNode
				InfoP insertInfo = newInfo(allocPos, allocSize);
				ListNodeP insert = newListNode(curr, insertInfo);
				// make prev point to it
				prev->next = insert;
				return 0;
			}
			// if it only touches prev
			else if ((allocPos == (prev->info->pos + prev->info->size)) && ((allocSize + allocPos) != curr->info->pos)) {
				// then just increase the size of prev
				prev->info->size += allocSize;
				return 0;
			}
			// if it only touches curr
			else if (((allocPos + allocSize) == curr->info->pos) && (allocPos != (prev->info->pos + prev->info->size))) {
				// then increase the size of current and move back its starting point
				curr->info->size += allocSize;
				curr->info->pos -= allocSize;
				return 0;
			}
			// if it touches both curr and prev
			else if ((allocPos == (prev->info->pos + prev->info->size)) && ((allocPos + allocSize) == curr->info->pos)) {
				// increase the size of prev to cover our new item and curr
				prev->info->size += (allocSize + curr->info->size);
				// make prev.next point to whatever curr.next points to
				prev->next = curr->next;
				// free curr
				destroyFreeListNode(curr);
				return 0;
			}
			// move along one
			prev = curr;
			curr = curr->next;
		}
	}
	// error deallocation return -1
	return -1;
}

// adds up how much free space in the list
// takes a FreeList
// returns amount of free space
size_t totalFreeSpace(FreeListP this)
{
	// create a temp var to point to head
	ListNodeP temp = this->head;
	// var to store total space
	size_t space = 0; 
	// loop through the entire list
	while (temp != NULL) {
		space += temp->info->size;
		temp = temp->next;
	}
	return space;
}

// counts up the number of free holes
// takes a FreeList
// returns number of free holes
size_t countFreeHoles(FreeListP this)
{
	// create a temp var to point to head
	ListNodeP temp = this->head;
	// var to store counts
	size_t count = 0;
	// loop through the entire list
	while (temp != NULL) {
		count++;
		temp = temp->next;
	}
	return count;
}
