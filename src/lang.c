#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vec.h"
#include "token.h"
#include "parse.h"
#include "midi.h"
#include "writer.h"


void read_file(char *src, const char *filename) {
  FILE *fp = fopen(filename, "r");
  char line[256];
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
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Source required\n");
    exit(0);
  }
  const char *filename = argv[1];
  printf("filename: %s\n", filename);

  // read from file
  char src[4096];
  read_file(src, filename);

  // tokenize
  Vec *tokens = Vec_new(10);
  tokenize(tokens, src);

  printf("debug 1");
  // parse
  SMF *smf;
  SMF_init(smf);
  parse(smf, tokens);

  // Save
  Writer writer;
  Writer_open(&writer, "output/a.mid");
  Writer_write_smf(&writer, smf);
  Writer_close(&writer);

  // free
  SMF_release(smf);
  while (tokens->len > 0) {
    Token_free(Vec_top(tokens));
    Vec_pop(tokens);
  }
  Vec_free(tokens);
  return 0;
}