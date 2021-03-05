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
  printf("<Token type: %s(%u), start: %s, len: %lu>\n", TokenTypeName[t->ty], t->ty, str, t->len);
  free(str);
}

Token *consume_reserved(char *p, char* name, TokenType ty) {
  Token *token = NULL;
  size_t len = strlen(name);
  if (strncmp(name, p, len) == 0) {
    token = Token_new(ty, p, len);
    // printf("match: %lu %s\n", token->len, name);
  }
  return token;
}

size_t skip_range(char *p, char min, char max) {
  size_t n = 0;
  for(; min <= p[n] && p[n] <= max; n++) {}
  return n;
}

Token *tokenize_reserved(char *p) {
  size_t len = skip_range(p, 'a', 'z');
  if (len == 0) return NULL;
  Token *t = NULL;
  if (!t) t = consume_reserved(p, "phrase", TokenType_PHRASE);
  if (!t) t = consume_reserved(p, "write", TokenType_WRITE);
  return t;
}

Token *tokenize_quoted(char *p) {
  if (p[0] != '"') return NULL;
  // skip left quote
  p++;
  size_t len = 0;
  Token *token = NULL;
  // max length: 256
  for (; len<256; len++) {
    char ch = p[len];
    if ( !BETWEEN('a', ch ,'z')
      && !BETWEEN('A', ch ,'Z')
      && !BETWEEN('0', ch ,'9')
      && !strchr("| +-._$#()[]{}", ch)
    ) {
      if (ch == '"') {
        token = Token_new(TokenType_QUOTED, p, len);
      }
      break;
    }
  }
  return token;
}
Token *tokenize_eol(char *p) {
  if (p[0] == '\n') {
    return Token_new(TokenType_EOL, p, 1);
  } else if (p[0] == '\r' && p[1] == '\n') {
    return Token_new(TokenType_EOL, p, 2);
  }
  return NULL;
}

void tokenize(Vec *list, char *src) {
  char ch;
  size_t cur = 0;
  while (1) {
    char *p = src + cur;
    Token *t = NULL;
    ch = *p;
    if (ch == 0) {
      break;
    }
    if (!t) t = tokenize_reserved(p);
    if (!t) t = tokenize_quoted(p);
    if (!t) t = tokenize_eol(p);
    if (t != NULL) {
      Vec_push(list, t);
      // Token_print(t);
      cur += t->len;
      continue;
    }
    cur++;
  }
  for (size_t i=0; i<list->len; i++) {
    Token *t = (Token *)list->items[i];
    if (t == NULL) {
      printf("list done\n");
      break;
    }
    Token_print(t);
  }
  printf("tokenize done. %lu\n", cur);
}
