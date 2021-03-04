#pragma once

#include <stdlib.h>
#include "vec.h"


typedef enum ETokenType {
  TOKENTYPE_RESERVED,
  TOKENTYPE_STRING_LITERAL,
  TOKENTYPE_VARIABLE,
  TOKENTYPE_WHITESPACE,
  TOKENTYPE_ASSIGNMENT,
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
