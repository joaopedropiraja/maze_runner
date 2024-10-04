#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "msgassert.h"

void *arr_create(size_t itemSize, size_t capacity) {
    size_t size = itemSize * capacity + sizeof(ArrayHeader);
    
    ArrayHeader *h = (ArrayHeader*)malloc(size);
    msgassert(h != NULL, "Memory allocation failed for array: { itemSize: %u, capacity: %u }\n", itemSize, capacity);

    h->capacity = capacity;
    h->length = 0;

    return h + 1;
};

void arr_delete(void *a) {
    ArrayHeader *h = arr_header(a);
    free(h);
};

void *arr_ensureCapacity(void *a, size_t itemCount, size_t itemSize) {
    ArrayHeader *h = arr_header(a);
    size_t desiredCapacity = h->length + itemCount;

    if (h->capacity >= desiredCapacity) return h + 1;
    
    size_t newCapacity = h->capacity * 2;
    while (newCapacity < desiredCapacity) 
        newCapacity *= 2;
    
    size_t newSize = sizeof(ArrayHeader) + newCapacity * itemSize;
    ArrayHeader *newH = (ArrayHeader*)malloc(newSize);
    msgassert(newH != NULL, "Memory reallocation failed for array: { length: %u, capacity: %u }\n", h->length, h->capacity);

    size_t oldSize = sizeof(*h) + h->length * itemSize;
    memcpy(newH, h, oldSize);
    free(h);

    newH->capacity = newCapacity;
    
    return newH + 1;
};