// main.c
// spp - simple preprocessor
// Usage:
// ./spp <source file> <output directory>

#include "spp.h"

int main(int argc, char** argv) {
  return spp_start(argc, argv);
}
