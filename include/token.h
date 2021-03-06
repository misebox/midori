#pragma once

#include <stdlib.h>
#include "vec.h"


typedef enum ETokenType {
  TokenType_NONE,
  TokenType_PHRASE,
  TokenType_WRITE,
  TokenType_QUOTED,
  TokenType_VARIABLE,
  TokenType_WHITESPACE,
  TokenType_ASSIGNMENT,
  TokenType_EOL,
} TokenType;

typedef struct _Token {
  TokenType ty;
  char *start;
  size_t len;
} Token;

Token *Token_new(TokenType ty, char *start, size_t len);
void Token_free(Token *t);
void Token_print(Token *t);
void tokenize(Vec *list, char *src);
