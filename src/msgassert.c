#include "msgassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void _assert (const char* snippet, const char* file, int line, const char* message, ...) {
  printf("assert failed %s:%d %s\n", file, line, snippet);
 
  if (*message) {
    va_list arg;
    va_start(arg, message);
    char* data = va_arg(arg, char*);
    vprintf(data, arg);
  }
  printf("\n");

  exit(EXIT_FAILURE);
}