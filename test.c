#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const char* string = "This is a string";
  char this_is_a_char = 'C';
  if (this_is_a_char) {
    this_is_a_char = 'A';
  }
  return 0;
}
