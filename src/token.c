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

Token *consume_reserved(char *start, size_t len) {
  const char *names[] = {
    "phrase",
    "bbb",
    "ccc",
    NULL,
  };
  size_t n = strlen(names);
  Token *token = NULL;
  for (char *name = names[0]; name != NULL; name++) {
    if (strncmp(name, start, len) == 0) {
      token = Token_new(TokenType_RESERVED, start, len);
      printf("match: %u %s\n", token->len, name);
      break;
    }
  }
  return token;
}

char *skip_range(char *start, char min, char max) {
  int i = 0;
  for(; min <= start[i] && start[i] <= max; i++) {
    printf("test");
    continue;
  }
  return i;
}

void tokenize(Vec *list, char *src) {
  char ch;
  size_t start = 0;
  size_t cur = 0;
  while (1) {
    Token *t = NULL;
    ch = src[cur];
    if (ch == 0) {
      break;
    }
    if ('a' <= ch && ch <= 'z') {
      int len = skip_range(src + cur, 'a', 'z');
      printf("len: %d\n", len);
      //t = consume_reserved(src+cur, len);
      if (t != NULL) {
        Vec_push(list, t);
        cur += t->len;
        continue;
      }
    }
    printf("%c", ch);
    cur++;
  }
  printf("tokenize done. %lu", cur);
}