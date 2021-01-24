// generator.c

#include <stdarg.h>

#include "common.h"
#include "ast.h"
#include "token.h"
#include "generator.h"

#define LAMBDA_NAME_SIZE 64
static const char* lambda_identifier = "_spp_lambda";

static void write_out(FILE* file, char* fmt, ...);
static i32 generate(FILE* file, Ast* ast);

void write_out(FILE* file, char* fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(file, fmt, args);
  va_end(args);
}

// TODO(lucas): Prettify code generation!
i32 generate(FILE* file, Ast* ast) {
  struct Token* token = NULL;
  i32 child_count = ast_child_count(ast);
  for (i32 i = 0; i < child_count; i++) {
    token = ast_get_node_value(ast, i);
    if (token) {
      switch (token->type) {
        // NOTE(lucas): This is a little confusing naming, but essentially this branch is where all the lambdas are located
        case T_LAMBDA_BODY: {
          Ast lambda_branch = ast_get_node_at(ast, i);
          if (ast_child_count(&lambda_branch) > 0) {
            generate(file, &lambda_branch);
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

          generate(file, &return_branch);

          Value* return_node = ast_get_value(&return_branch);  // This is where we store the id of the lambda
          assert(return_node);
          char lambda_name[LAMBDA_NAME_SIZE] = {0};
          snprintf(lambda_name, LAMBDA_NAME_SIZE, "%s%i", lambda_identifier, return_node->id);
          write_out(file, "%s", lambda_name);

          write_out(file, "(");
          generate(file, &arglist_branch);
          write_out(file, ") ");

          write_out(file, "{\n");
          generate(file, &body_branch);
          write_out(file, "\n}\n");
          break;
        }

        case T_LAMBDA: {
          char lambda_name[LAMBDA_NAME_SIZE] = {0};
          snprintf(lambda_name, LAMBDA_NAME_SIZE, "%s%i", lambda_identifier, token->id);
          write_out(file, "%s", lambda_name);
          break;
        }
        case T_MACRO:
          write_out(file, "%.*s\n", token->length, token->string);
          break;

        default:
          write_out(file, "%.*s ", token->length, token->string);
          break;
      }
    }
  }
  return NO_ERR;
}

i32 generate_from_ast(FILE* file, Ast* ast) {
  generate(file, ast);
  return NO_ERR;
}
