#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vec.h"
#include "token.h"


Token *Token_new(TokenType ty, char *start, size_t len) {
  Token *t = malloc(sizeof(Token));
  t->ty = ty;
  t->start = start;
  t->len = len;
  return t;
}
void Token_free(Token *t) {
  free(t);
}

void Token_print(Token *t) {
  char *str = malloc(sizeof(char) * (t->len + 1));
  strncpy(str, t->start, t->len);
  str[t->len] = 0;
  printf("<Token type: %u, start: %s, len: %lu>", t->ty, str, t->len);
  free(str);
}

void tokenize(Vec *list, char *src) {
  char ch;
  size_t cur = 0;
  while (1) {
    ch = src[cur];
    if (ch != EOF) {
      break;
    }
    printf("[%ld]%c ", cur, ch);
    cur++;
  }
  Vec_push(list, Token_new(TOKENTYPE_RESERVED, src, 10));

  printf("tokenize done.");
}