#include "array.h"

#include <stdlib.h>
#include <string.h>

void *arr_create(size_t itemSize, size_t capacity) {
    void *ptr = 0;
    size_t size = itemSize * capacity + sizeof(ArrayHeader);
    ArrayHeader *h = (ArrayHeader*)malloc(size);

    if (h) {
        h->capacity = capacity;
        h->length = 0;
        ptr = h + 1;
    }

    return ptr;
};
void *arr_ensureCapacity(void *a, size_t itemCount, size_t itemSize) {
    ArrayHeader *h = arr_header(a);
    size_t desiredCapacity = h->length + itemCount;

    if (h->capacity >= desiredCapacity) {
        h += 1;
        return h;
    }

    size_t newCapacity = h->capacity * 2;
    while (newCapacity < desiredCapacity) 
        newCapacity *= 2;
    
    size_t newSize = sizeof(ArrayHeader) + newCapacity * itemSize;
    ArrayHeader *newH = (ArrayHeader*)malloc(newSize);

    if (newH) {
        size_t oldSize = sizeof(*h) + h->length * itemSize;
        memcpy(newH, h, oldSize);

        free(h);

        newH->capacity = newCapacity;
        h = newH + 1;
    } else {
        h = 0;
    }
    
    return h;
};