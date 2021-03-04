#pragma once
#include <stdlib.h>

typedef struct _Vec {
  void **items;
  size_t len;
  size_t size;
} Vec;

Vec *Vec_new(size_t size);
void Vec_push(Vec *v, void *item);
void Vec_pop(Vec *v);
void *Vec_top(Vec *v);
void Vec_free(Vec *v);