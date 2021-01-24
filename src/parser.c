// parser.c

#include "common.h"
#include "ast.h"
#include "util.h"
#include "lexer.h"
#include "token.h"
#include "parser.h"

typedef struct Parser {
  Lexer* l;
  Ast* ast;
  i32 status;
} Parser;

#define TAB_SIZE 2

static void parser_init(Parser* parser, Lexer* lexer, Ast* ast);
static void print_tabs(FILE* file, i32 level);
static i32 end(Parser* p);
static i32 statement(Parser* p);
static i32 statements(Parser* p);

void parser_init(Parser* p, Lexer* l, Ast* ast) {
  p->l = l;
  p->ast = ast;
  p->status = 0;
}

void print_tabs(FILE* file, i32 level) {
  for (i32 i = 0; i < level * TAB_SIZE; ++i) {
    fprintf(file, " ");
  }
}

i32 end(Parser* p) {
  struct Token token = get_token(p->l);
  return token.type == T_EOF || token.type == T_BLOCKEND;
}

i32 statement(Parser* p) {
  struct Token token = next_token(p->l);
  switch (token.type) {
    case T_NEWLINE:
      break;

    default:
      ast_add_node(p->ast, token);
      break;
  }
  return 0;
}

i32 statements(Parser* p) {
  while (!end(p)) {
    p->status = statement(p);
  }
  return p->status;
}

i32 parser_parse(char* input, char* filename, Ast* ast) {
  Lexer lexer;
  lexer_init(&lexer, input, filename);

  Parser parser;
  parser_init(&parser, &lexer, ast);
  next_token(parser.l);
  statements(&parser);
/*
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
  */

  return 0;
}
