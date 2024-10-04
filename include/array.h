#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct {
    size_t length;
    size_t capacity;
} ArrayHeader;

#define ARRAY_INITIAL_CAPACITY 16
#define array(T) arr_create(sizeof(T), ARRAY_INITIAL_CAPACITY)
#define arr_header(a) ((ArrayHeader*)(a) - 1)
#define arr_length(a) (arr_header(a)->length)
#define arr_capacity(a) (arr_header(a)->capacity)
#define arr_append(a, v) ( \
    (a) = arr_ensureCapacity(a, 1, sizeof(v)), \
    (a)[arr_header(a)->length] = (v), \
    &(a)[arr_header(a)->length++])

void *arr_create(size_t itemSize, size_t capacity);
void arr_delete(void *a);
void *arr_ensureCapacity(void *a, size_t itemCount, size_t itemSize);

#endif // ARRAY_H