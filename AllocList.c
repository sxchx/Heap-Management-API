#include <stdlib.h>
#include <stdio.h>
#include "AllocList.h"
#include "ListNode.h"
#include "Info.h"

// Constructor for AllocList
// returns a pointer to an AllocList
AllocListP newAllocList()
{
	// create a new AllocList
	AllocListP newAllocList = malloc(sizeof(AllocList));
	newAllocList->head = NULL;
	return newAllocList;
}

// Destructor for AllocList
// Takes an AllocList to free
// Frees all the ListNodes associated with this AllocList
void destroyAllocList(AllocListP this)
{
	// free everything in the AllocList
	// create a temp var to point to head
	if (this->head == NULL) {
		free(this);
		return;
	}
	ListNodeP temp = this->head;
	ListNodeP next = this->head->next;
	// loop through the entire list
	while (temp != NULL) {
		destroyFreeListNode(temp);
		temp = next;
		if (next != NULL) {
			next = next->next;
		}
	}
	// free the AllocList
	free(this);
}

// Adds an item to the AllocList
// Takes an AllocList, the position and size of the object to allocate
// Returns the allocated size
size_t addAllocList(AllocListP this, size_t allocPos, size_t allocSize)
{
	// since the entries do not need to be in any particular order, insert at the head
	// create a new info to store the pos and size
	InfoP insertInfo = newInfo(allocPos, allocSize);
	// create a new ListNode to store the info, set next to the current head
	ListNodeP newNode = newListNode(this->head, insertInfo);
	// make it the head
	this->head = newNode;
	return allocSize;
}

// Remove an item from the AllocList
// Takes an AllocList and the position to remove item from 
// Returns the size of the remove item
size_t removeAllocList(AllocListP this, size_t allocPos)
{
	// check if we are removing the head
	if (this->head->info->pos == allocPos) {
		//head->next will be our new head
		ListNodeP temp = this->head;
		this->head = this->head->next;
		size_t size = temp->info->size;
		// free temp
		destroyFreeListNode(temp);
		return size;
	}
	else if (this->head != NULL) {
		// loop through the list until we find the item to remove
		ListNodeP prev = this->head;
		ListNodeP curr = this->head->next;
		while (curr != NULL) {
			// check if this is the item to remove
			if (curr->info->pos == allocPos) {
				// make the next of prev point to the next of curr
				prev->next = curr->next;
				// store the size of item
				size_t size = curr->info->size;
				// free curr as we have skipped over it 
				destroyFreeListNode(curr);
				return size;
			}
			// move along one
			prev = curr;
			curr = curr->next;
		}
	}
	// could not find item at specified pos to remove
	printf("Could not find item to remove\n");
	return -1;
}

// Counts the number of allocated chunks in the AllocList
// Takes an AllocList
// Returns the number of allocated chunks
size_t countAllocChunks(AllocListP this)
{
	// create a temp var to point to head
	ListNodeP temp = this->head;
	// var to store counts
	size_t count = 0;
	// loop through the list 
	while (temp != NULL) {
		count++;
		temp = temp->next;
	}
	return count;
}
