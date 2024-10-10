#include <stdlib.h>

#include "stack.h"
#include "msgassert.h"

#define MIN_CAPACITY 2

struct stack_s {
    size_t capacity;
    size_t size;
    StackNode *data;
};

Stack *st_create(size_t capacity) {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    msgassert(s != NULL, "Memory allocation failed for stack: { capacity: %d }\n", capacity);

    if (capacity < MIN_CAPACITY) capacity = MIN_CAPACITY;

    s->capacity = capacity;
    s->size = 0;
    s->data = (StackNode*)malloc(capacity * sizeof(StackNode));
    msgassert(s->data, "Memory allocation failed for stack: { capacity: %d }\n", capacity);

    return s;
};

void st_delete(Stack *s) {
    free(s->data);
    free(s);
};

void st_insert(Stack *s, StackNode sn) {
    if (s->size >= s->capacity) {
        s->capacity *= 2;
        s->data = (StackNode*)realloc(s->data, s->capacity * sizeof(StackNode));
        msgassert(s->data != NULL, "Memory reallocation failed for min heap: { capacity: %d }\n", s->capacity);
    }

    s->data[s->size] = sn;
    s->size += 1;
};

StackNode st_pop(Stack *s) {
    msgassert(s->size > 0, "Empty stack\n");
    
    s->size--;
    StackNode sn = s->data[s->size];

    return sn;
};

bool st_empty(Stack *s) {
    return s->size == 0;
};
