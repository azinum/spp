// spp.c

#include "spp.h"
#include "memory.h"
#include "ast.h"
#include "parser.h"
#include "util.h"
#include "generator.h"

#define MAX_PATH_SIZE 512
#define OUTPUT_PATH_DEFAULT "output"

i32 spp_start(i32 argc, char** argv) {
  i32 status = NO_ERR;
  char* filename = "test.c";
  char* output_path = OUTPUT_PATH_DEFAULT;

  if (argc > 2) {
    filename = argv[1];
    output_path = argv[2];
  }
  else {
    fprintf(stderr, "Usage: ./spp <source file> <output directory>\n");
    status = ERR;
    goto done;
  }

  char* input = read_file(filename);
  if (!input) {
    fprintf(stderr, "[Error]: '%s' No such file\n", filename);
    status = ERR;
    goto done;
  }
  i32 input_size = strlen(input);

  Ast ast = ast_create();

  if (parser_parse(input, filename, &ast) == NO_ERR) {
    char out_filename[MAX_PATH_SIZE] = {0};
    snprintf(out_filename, MAX_PATH_SIZE, "%s/%s", output_path, filename);
    FILE* file = fopen(out_filename, "w");
    Generator g;
    generator_init(&g, file, input, input_size);
    generate_from_ast(&g, &ast);
    fclose(file);
  }
  else {
    status = ERR;  
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

done:
  if (status != NO_ERR) {
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}

