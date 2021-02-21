#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "midi.h"
#include "writer.h"

int main() {
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
