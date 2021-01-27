// spp.c

#include "spp.h"
#include "memory.h"
#include "ast.h"
#include "parser.h"
#include "util.h"
#include "generator.h"

#define MAX_PATH_SIZE 512
#define OUTPUT_PATH "output"

i32 spp_start(i32 argc, char** argv) {
  char* filename = "test.c";

  if (argc > 1) {
    filename = argv[1];
  }
  else {
    fprintf(stderr, "[Error]: Please select a file to parse\n");
    return -1;
  }

  char* input = read_file(filename);
  if (!input) {
    fprintf(stderr, "[Error]: '%s' No such file\n", filename);
    return ERR;
  }
  i32 input_size = strlen(input);

  Ast ast = ast_create();

  if (parser_parse(input, filename, &ast) == NO_ERR) {
    char out_filename[MAX_PATH_SIZE] = {0};
    snprintf(out_filename, MAX_PATH_SIZE, "%s/%s", OUTPUT_PATH, filename);
    FILE* file = fopen(out_filename, "w");
    Generator g;
    generator_init(&g, file, input, input_size);
    generate_from_ast(&g, &ast);
    fclose(file);
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

