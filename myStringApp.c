#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "Heap.h"
	
// This application maintains an array of 10000 char* pointers to string allocated on the heap
// It reads in strings from stdin
int main(int argc, char *argv[])
{
	// check we have the correct number of args passed
	if (argc != 5) {
		printf("Usage: ./project -h <heapSize> -a [first|best|worst]\n");
		exit(1);
	}

	int ch;
	size_t heapSize;
	char* allocMode;
	// get our arguments
	while ((ch = getopt (argc, argv, "h:a:")) != -1) {
		switch (ch) {
			case 'h':
				heapSize = atoi (optarg);
				break;
			case 'a':
				allocMode = optarg;
				break;
			case '?':
				printf("Usage: ./project -h <heapSize> -a [first|best|worst]\n");
				exit(1);
		}
	}
	size_t allocVal;
	// array of 10000 char* pointers to strings
	char* ptrArray[10000];
	int i;
	// set everything in the array to NULL
	for (i = 0; i < 10000; i++) {
		ptrArray[i] = NULL;
	}
	// set the allocMode of the heap
	if (strcmp(allocMode, "first") == 0) {
		allocVal = 0;
	}
	else if (strcmp(allocMode, "best") == 0) {
		allocVal = 1;
	}
	else if (strcmp(allocMode, "worst") == 0) {
		allocVal = 2;
	}
	else {
		printf("Allocation type should be first, best or worst\n");
		return -1;
	}
	// make a heap
	HeapP heap = newHeap(heapSize, allocVal);
	// addition/deletion flag
	int addition = 1;
	printf("\n");
	while(1) {
		// get the input from stdin using getline
		char *line = NULL;
		size_t length = 0;
		if ( getline(&line, &length, stdin) == -1)
		{
			// end of file
			free(line);
			heapStats(heap);
			destroyHeap(heap);
			return 0;
		}
		// remove the newline character
		line[strcspn(line, "\n")] = 0;
		// parse the line
		char* string = line + 2;
		if (line[0] == '+') {
			addition = 1;
		}
		else if (line[0] == '-') {
			addition = 0;
		}
		else {
			printf("Input Error\n");
			continue;
		}
		// if we are adding
		if (addition) {
			// find a free spot in the array
			for (i = 0; i < 10000; i++) {
				if (ptrArray[i] == NULL) {
					break;
				}
			}
			if (i == 10000) {
				printf("No space in array\n");
				free(line);
				heapStats(heap);
				destroyHeap(heap);
				exit(1);
			}
			// get the size of the string + 1 for the \0 character
			size_t size = strlen(string) + 1;
			// add the null termination character to the end of the string
			string[size-1] = '\0';
			// allocate the string
			ptrArray[i] = (char *)allocateHeap(heap, size);
			// check if we have run out of space on the heap
			if (ptrArray[i] == NULL) {
				free(line);
				heapStats(heap);
				destroyHeap(heap);
				exit(1);
			}
			// store the string on the heap 
			strcpy(ptrArray[i], string);
			printf("Allocate: %s\nAddress: %p\n", ptrArray[i], ptrArray[i]);
		}
		// if we are removing
		else {
			int delIndex;  
			// find string in array
			for (delIndex = 0; delIndex < 10000; delIndex++) {
				if (ptrArray[delIndex] != NULL) {
					if (strcmp(string, ptrArray[delIndex]) == 0) {
						// deallocate from heap
						size_t rv = deallocateHeap(heap, ptrArray[delIndex]);
						if (rv == -1) {
							printf("Deallocation Error\n");
							free(line);
							heapStats(heap);
							destroyHeap(heap);
							exit(1);
						}
						printf("Deallocate: %s\nAddress: %p\n", ptrArray[delIndex], ptrArray[delIndex]);
						// remove the pointer
						ptrArray[delIndex] = NULL;
						break;
					}
				}
			}
		}
		free(line);
	}
	heapStats(heap);
	destroyHeap(heap);
	return 0;
}
