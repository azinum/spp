// spp.c

#include "spp.h"
#include "memory.h"
#include "ast.h"
#include "parser.h"
#include "util.h"
#include "generator.h"

i32 spp_start(i32 argc, char** argv) {
  char* filename = "stdin";

  if (argc > 1) {
    filename = argv[1];
  }
  else {
    fprintf(stderr, "[Error]: Please select a file that you want to parse\n");
    return -1;
  }

  char* input = read_file(filename);
  if (!input) {
    fprintf(stderr, "[Error]: '%s' No such file\n", filename);
    return ERR;
  }
  Ast ast = ast_create();

  if (parser_parse(input, filename, &ast) == NO_ERR) {
    generate_from_ast(stdout, &ast);
  }

  ast_free(&ast);
  free(input);
  input = NULL;

  assert(memory_total() == 0);
  if (memory_total() != 0) {
    fprintf(stderr, "Memory leak!\n");
    memory_print_info();
    return ERR;
  }
  return NO_ERR;
}

