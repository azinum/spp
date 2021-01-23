// test.c

#include <stdio.h>
#include <stdlib.h>

#define VERY_COOL_MACRO(a, b, c) { \
  if (a) { \
    printf("%i\n", a + b); \
  } \
}

int main(int argc, char** argv) {
  if (argc > 2) {
    printf("OK\n");
  }
  else {
    printf("AHA!\n");
  }
  return 0;
}
