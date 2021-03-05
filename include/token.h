#pragma once

#include <stdlib.h>
#include "vec.h"


#define BETWEEN(a, ch, b) (a <= ch && ch <= b)

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

#define STR(var) #var

const char *TokenTypeName[] = {
  STR(TokenType_NONE),
  STR(TokenType_PHRASE),
  STR(TokenType_WRITE),
  STR(TokenType_QUOTED),
  STR(TokenType_VARIABLE),
  STR(TokenType_WHITESPACE),
  STR(TokenType_ASSIGNMENT),
  STR(TokenType_EOL),
};

typedef struct _Token {
  TokenType ty;
  char *start;
  size_t len;
} Token;

Token *Token_new(TokenType ty, char *start, size_t len);
void Token_free(Token *t);
void Token_print(Token *t);
void tokenize(Vec *list, char *src);
