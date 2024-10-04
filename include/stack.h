#ifndef STACK_H
#define STACK_H

#include "utils.h"
#include <stdbool.h>

typedef struct stack_s Stack;
typedef Position StackNode;

Stack *st_create(size_t capacity);
void st_delete(Stack *s);

void st_insert(Stack *s, StackNode sn);
StackNode st_pop(Stack *s);
bool st_empty(Stack *s);

#endif // STACK_H