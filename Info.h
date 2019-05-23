#ifndef INFO_H_
#define INFO_H_
#include <stdlib.h>

typedef struct Info *InfoP;

typedef struct Info {
	size_t pos;
	size_t size;
}Info;

InfoP newInfo(size_t pos, size_t size);

#endif
