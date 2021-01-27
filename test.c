// test.c

#include <stdlib.h>
#include <stdio.h>

typedef int (*cb)(int, int);

void do_thing(cb callback);

int main(void) {

  do_thing($(int a, int b) -> int {
    return a + b;
  });

  do_thing($(int a, int b) -> int {
    return a - b;
  });

  return 0;
}

void do_thing(cb callback) {
  int result = callback(5, 3);
  printf("Result: %i\n", result);
}
