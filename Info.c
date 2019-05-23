#include <stdlib.h>
#include "Info.h"

// constructor for new Info object
// takes a position and a size 
// returns a pointer to an Info
InfoP newInfo(size_t pos, size_t size) 
{
	InfoP newInfoP = malloc(sizeof(Info));
	newInfoP->pos = pos;
	newInfoP->size = size;
	return newInfoP;
}
