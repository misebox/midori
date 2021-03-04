#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


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

typedef struct _Vec {
  void **items;
  size_t len;
} Vec;

Vec *Vec_new(size_t size) {
  Vec *v = malloc(sizeof(Vec));
  v->items = malloc(sizeof(void *) * size);
  return v;
}
void Vec_free(Vec *v) {
  free(v);
}

void Token_print(Token *t) {
  printf("<Token type: %u, start: %p, len: %lu>", t->ty, t->start, t->len);
}

void tokenize(Vec *list, char *src) {
  char ch;
  size_t cur = 0;
  list->items[0] = NULL;
  while (1) {
    ch = src[cur];
    if (ch != EOF) {
      break;
    }
    printf("[%ld]%c ", cur, ch);
    cur++;
  }
  
  printf("tokenize done.");
}
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
  for (size_t i=0; i<500; i++) {
    Token *t = (Token *)tokens->items[i];
    if (t == NULL) {
      printf("done\n");
      break;
    }
    Token_print(t);
  }
  return 0;
}