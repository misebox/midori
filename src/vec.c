#include <stdio.h>
#include <stdlib.h>
#include "vec.h"
#include "assert.h"


Vec *Vec_new(size_t size) {
  Vec *v = (Vec *)malloc(sizeof(Vec));
  v->items = (void **)malloc(sizeof(void *) * size);
  v->size = size;
  v->len = 0;
  return v;
}

void Vec_push(Vec *v, void *item) {
  if (v->len == v->size) {
    v->size *= 2;
    v->items = (void **)realloc(v, v->size);
  }
  printf("Vec_push len: %d, size: %d\n", v->len, v->size);
  assert(v->len < v->size);
  v->items[v->len] = item;
  v->len++;
}

void Vec_pop(Vec *v) {
  assert(v->len > 0);
  v->len--;
}

void *Vec_top(Vec *v) {
  assert(v->len > 0);
  return v->items[v->len-1];
}

void Vec_free(Vec *v) {
  free(v->items);
  free(v);
}
