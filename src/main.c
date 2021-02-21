#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// #include "person.h"
#include "midi.h"
#include "writer.h"

int main() {
  // FILE *fp = NULL;
  // PERSON p;
  // memset(p.name, 0xff, sizeof(p.name));
  // snprintf(p.name, sizeof(p.name), "Hanako Yamda");
  // p.age = 20;
  // if ((fp = fopen("foo.dat", "wb")) == NULL) {
  //   perror(NULL);
  //   return 1;
  // }
  // fwrite(&p, sizeof(PERSON), 1, fp);
  // fclose(fp);

  // SMF Data
  SMF smf;
  SMF_init(&smf);


  // Save
  Writer writer;
  Writer_open(&writer, "output/sample.mid");
  Writer_close(&writer);

  SMF_release(&smf);
  return 0;
}
