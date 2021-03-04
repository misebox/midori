#include <stdlib.h>
#include "vec.h"
#include "assert.h"


Vec *Vec_new(size_t size) {
  Vec *v = malloc(sizeof(Vec));
  v->items = malloc(sizeof(void *) * size);
  v->size = size;
  v->len = 0;
  return v;
}
void Vec_push(Vec *v, void *item) {
  if (v->len == v->size) {
    v->size *= 2;
    v->items = realloc(v, v->size);
  }
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
