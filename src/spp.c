// spp.c

#include "spp.h"
#include "memory.h"
#include "ast.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "util.h"

#define TAB_SIZE 2

static void print_tabs(FILE* file, i32 level);

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

  Lexer lexer;
  lexer_init(
    &lexer,
    input,
    filename
  );

  i32 level = 0;
  i32 is_running = 1;
  FILE* file = stdout;

  while (is_running) {
    struct Token token = next_token(&lexer);
    switch (token.type) {
      case T_EOF:
        is_running = 0;
        break;

      case T_NEWLINE:
        fprintf(file, "\n");
        print_tabs(file, level);
        break;

      case T_BLOCKBEGIN:
        ++level;
        token_print(file, token);
        print_tabs(file, level);
        break;

      case T_BLOCKEND:
        --level;
        token_print(file, token);
        print_tabs(file, level);
        break;

      default:
        token_print(file, token);
        fprintf(file, " ");
        break;
    }
  }

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

void print_tabs(FILE* file, i32 level) {
  for (i32 i = 0; i < level * TAB_SIZE; ++i) {
    fprintf(file, " ");
  }
}
