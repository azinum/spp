// generator.c

#include <stdarg.h>

#include "common.h"
#include "ast.h"
#include "token.h"
#include "generator.h"

#define LAMBDA_NAME_SIZE 64
static const char* lambda_identifier = "_spp_lambda";

static void write_out(FILE* file, char* fmt, ...);
static i32 generate_lambda(Generator* g, char* source, Ast* args, Ast* ret, Ast* body);
static i32 generate(Generator* g, char* source, Ast* ast);

void write_out(FILE* file, char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(file, fmt, args);
  va_end(args);
}

i32 generate_lambda(Generator* g, char* source, Ast* args, Ast* ret, Ast* body) {
  struct Token* token = NULL;

  for (i32 i = 0; i < ast_child_count(ret); i++) {
    token = ast_get_node_value(ret, i);
    if (token) {
      write_out(g->file, "%.*s ", token->length, token->string);
    }
  }

  struct Token* return_node = ast_get_value(ret);
  assert(return_node);
  char lambda_name[LAMBDA_NAME_SIZE] = {0};
  snprintf(lambda_name, LAMBDA_NAME_SIZE, "%s%i", lambda_identifier, return_node->id);
  write_out(g->file, "%s", lambda_name);

  write_out(g->file, "(");
  for (i32 i = 0; i < ast_child_count(args); i++) {
    token = ast_get_node_value(args, i);
    if (token) {
      write_out(g->file, "%.*s ", token->length, token->string);
    }
  }

  struct Token* first_in_body = ast_get_node_value(body, 0);
  write_out(g->file, ") ");
  write_out(g->file, "{\n");
  if (first_in_body) {
    if (first_in_body->string) {
      generate(g, first_in_body->string, body);
    }
  }
  write_out(g->file, "}\n");
  return 0;
}

i32 generate(Generator* g, char* source, Ast* ast) {
  struct Token* token = ast_get_node_value(ast, 0);
  if (!token) {
    return 0;
  }

  i32 child_count = ast_child_count(ast);
  for (i32 i = 0; i < child_count; i++) {
    token = ast_get_node_value(ast, i);
    if (token) {

      switch (token->type) {
        // NOTE(lucas): This is a little confusing naming, but essentially this branch is where all the lambdas are located
        case T_LAMBDA_BODY: {
          Ast lambda_branch = ast_get_node_at(ast, i);
          if (ast_child_count(&lambda_branch) > 0) {
            generate(g, source, &lambda_branch);
          }
          break;
        }

        // Here is where lambda expressions begin in the ast:
        // { arglist, return type, body }
        case T_ARGLIST: {
          Ast arglist_branch =  ast_get_node_at(ast, i + 0);
          Ast return_branch =   ast_get_node_at(ast, i + 1);
          Ast body_branch =     ast_get_node_at(ast, i + 2);
          i += 2;
          // We need to know what size the lambda was here, so that we can update the source pointer to begin at the end of this lambda expression.
          generate_lambda(g, source, &arglist_branch, &return_branch, &body_branch);
          break;
        }

        case T_LAMBDA: {
          char* at = token->string;
          i32 size = (at - source);
          write_out(g->file, "%.*s", size, source);
          char lambda_name[LAMBDA_NAME_SIZE] = {0};
          snprintf(lambda_name, LAMBDA_NAME_SIZE, "%s%i", lambda_identifier, token->id);
          write_out(g->file, "%s", lambda_name);
          source += size + token->block_size;
          break;
        }

        default:
          break;
      }
    }
  }
  if (token) {
    char* at = token->string;
    if (at) {
      i32 size_to_write = token->length + (at - source);
      write_out(g->file, "%.*s", size_to_write, source);
    }
  }
  return NO_ERR;
}

i32 generator_init(Generator* g, FILE* file, char* source, i32 source_size) {
  assert(file);
  assert(source);
  g->file = file;
  g->source = source;
  g->source_size = source_size;
  return 0;
}

i32 generate_from_ast(Generator* g, Ast* ast) {
  generate(g, g->source, ast);
  // ast_print(*ast);
  return NO_ERR;
}
