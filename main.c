#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <person.h>

int main() {
  FILE *fp = NULL;
  PERSON p;

  memset(p.name, 0xff, sizeof(p.name));
  snprintf(p.name, sizeof(p.name), "Hanako Yamda");
  p.age = 20;

  if ((fp = fopen("foo.dat", "wb")) == NULL) {
    perror(NULL);
    return 1;
  }

  fwrite(&p, sizeof(PERSON), 1, fp);

  fclose(fp);
  return 0;
}
