// spp.c

#include "spp.h"
#include "memory.h"
#include "ast.h"
#include "parser.h"
#include "util.h"

i32 spp_start(i32 argc, char** argv) {
  char* filename = "stdin";

  if (argc > 1) {
    filename = argv[1];
  }
  else {
    fprintf(stderr, "[ERROR]: Please select a file that you want to parse\n");
    return -1;
  }

  char* input = read_file(filename);

  Ast ast = ast_create();

  parser_parse(input, filename, &ast);
  ast_print(ast);
  ast_free(&ast);

  free(input);
  input = NULL;

  assert(memory_total() == 0);
  if (memory_total() != 0) {
    fprintf(stderr, "Memory leak!\n");
    memory_print_info();
    return -1;
  }
  return 0;
}

