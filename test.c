// test.c

#include <stdlib.h>

int main(void) {
  ($(int a, int b) -> int {
    return a + b;
  })(2, 3);

  ($(int a, int b) -> int {
    return a - b;
  })(5, 3);
  return 0;
}
