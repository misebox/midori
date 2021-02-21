#pragma once

#include <stdio.h>

typedef struct _Writer {
  FILE *fp;
  const char *filename;
} Writer;

void Writer_init(Writer *w, const char *filename);
void Writer_release(Writer *w);