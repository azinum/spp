// test.c

#include <stdlib.h>
#include <stdio.h>

int main(void) {
  int result = ($(int a, int b) -> int {
    return a + b + $() -> int {
      return 1;
    }();
  })(5, 3);

  printf("Result: %i\n", result);
  return 0;
}
