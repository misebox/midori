#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vec.h"
#include "token.h"


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Source required\n");
    exit(0);
  }
  const char *filename = argv[1];
  printf("filename: %s\n", filename);

  // read from file
  FILE *fp = fopen(filename, "r");
  char line[256];
  char src[4096];
  memset(src, 0, 4096);
  int no = 1;
  size_t cur = 0;
  while (1) {
    if (fgets(line, 255, fp) == NULL) {
      src[cur] = 0; 
      printf("Reading done\n");
      break;
    }
    size_t len = strlen(line);
    memcpy(src + cur, line, len);
    cur += len;
    no += 1;
  };
  fclose(fp);

  // tokenize
  Vec *tokens = Vec_new(100);
  tokenize(tokens, src);
  for (size_t i=0; i<tokens->len; i++) {
    Token *t = (Token *)tokens->items[i];
    if (t == NULL) {
      printf("list done\n");
      break;
    }
    Token_print(t);
  }
  while (tokens->len > 0) {
    Token_free(Vec_top(tokens));
    Vec_pop(tokens);
  }
  Vec_free(tokens);
  return 0;
}