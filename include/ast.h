// ast.h

#ifndef _AST_H
#define _AST_H

#include "common.h"
#include "token.h"

enum Status_codes {
  ERR = -1,
  NO_ERR = 0,
};

typedef struct Token Value;

typedef struct Node* Ast;

Ast ast_create();

i32 ast_is_empty(const Ast ast);

i32 ast_add_node(Ast* ast, Value value);

i32 ast_add_node_at(Ast* ast, i32 index, Value value);

Ast ast_get_node_at(Ast* ast, i32 index);

Ast ast_get_last(Ast* ast);

i32 ast_remove_node_at(Ast* ast, i32 index);

i32 ast_child_count(const Ast* ast);

i32 ast_child_count_total(const Ast* ast);

Ast* ast_get_node(Ast* ast, int index);

Value* ast_get_node_value(Ast* ast, i32 index);

Value* ast_get_value(Ast* ast);

void ast_print(const Ast ast);

void ast_free(Ast* ast);

#endif
