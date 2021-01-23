// spp.c

#include "spp.h"
#include "memory.h"
#include "ast.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"

void spp_start(i32 argc, char** argv) {
  Lexer lexer;
  lexer_init(
    &lexer,
    "int main(int argc, char** argv) { return 0xff << 0xa; }",
    "stdin"
  );

  for (;;) {
    struct Token token = next_token(&lexer);
    
    if (token.type == T_EOF) {
      break;
    }
    else {
      printf("%.*s\n", token.length, token.string);
    }
  }

  assert(memory_total() == 0);
  if (memory_total() != 0) {
    fprintf(stderr, "Memory leak!\n");
    memory_print_info();
  }
}
