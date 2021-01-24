// test.c

#include <stdlib.h>
#include <stdio.h>

typedef int (*callback)(int, int);

void hello(const char* string, callback cb) {
  printf("%s: %i\n", string, cb(5, 6));
}

int main() {
  hello("Hello there", $(int a, int b) -> int {
    return a + b;
  });
  return 0;
}

