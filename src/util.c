#include <stdio.h>
#include <stdlib.h>

void exit_failure(char *message) {
  fputs(message, stderr);
  exit(EXIT_FAILURE);
}