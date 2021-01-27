// ast.c

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "memory.h"
#include "ast.h"

struct Node {
  struct Node** children;
  i32 child_count;
  Value value;
};

static i32 is_empty(const Ast ast);
static struct Node* create_node(Value value);
static i32 print_tree(const Ast ast, i32 level);

i32 is_empty(const Ast ast) {
  return ast == NULL;
}

struct Node* create_node(Value value) {
  struct Node* node = m_malloc(sizeof(struct Node));
  if (!node) {
    printf("Failed to allocate new AST node\n");
    return NULL;
  }

  node->value = value;
  node->child_count = 0;
  node->children = NULL;

  return node;
}

i32 print_tree(const Ast ast, i32 level) {
  if (is_empty(ast))
    return NO_ERR;
  for (i32 i = 0; i < level - 1; i++) {
    printf("  ");
  }
  if (ast->value.length > 0) {
    printf("%.*s\n", ast->value.length, ast->value.string);
  }
  else {
    printf("[%i]\n", ast->value.type);
  }

  for (u32 i = 0; i < ast->child_count; i++) {
    print_tree(ast->children[i], level + 1);
  }

  return NO_ERR;
}

Ast ast_create() {
  return NULL;
}

i32 ast_is_empty(const Ast ast) {
  return is_empty(ast);
}

i32 ast_add_node(Ast* ast, Value value) {
  struct Node* new_node = create_node(value);
  if (!new_node)
    return ERR;

  if (is_empty(*ast)) {
    *ast = create_node((Value) {});
    if (!(*ast))
      return ERR;
  }
  if (!(*ast)->children) {
    (*ast)->children = m_malloc(sizeof(struct Node*));
  }
  else {
    struct Node** tmp = m_realloc((*ast)->children, (sizeof(struct Node*) * (*ast)->child_count), (sizeof(struct Node*)) * ((*ast)->child_count + 1));
    if (!tmp)
      return ERR;
    (*ast)->children = tmp;
  }
  (*ast)->children[(*ast)->child_count++] = new_node;
  return NO_ERR;
}

i32 ast_add_node_at(Ast* ast, i32 index, Value value) {
  assert(!is_empty(*ast));
  assert(index < (*ast)->child_count);
  return ast_add_node(&(*ast)->children[index], value);
}

Ast ast_get_node_at(Ast* ast, i32 index) {
  assert(!is_empty(*ast));
  if (index < 0 || index > (*ast)->child_count)
    return NULL;
  return (*ast)->children[index];
}

Ast ast_get_last(Ast* ast) {
  assert(!is_empty(*ast));
  i32 child_count = ast_child_count(ast);
  if (child_count == 0)
    return NULL;
  return (*ast)->children[child_count - 1];
}

i32 ast_remove_node_at(Ast* ast, i32 index) {
  assert(!is_empty(*ast));
  i32 child_count = ast_child_count(ast);
  assert(index < child_count);
  Ast node_to_remove = ast_get_node_at(ast, index);
  ast_free(&node_to_remove);	// NOTE(lucas): Removes the subtree(s) from this node - if there are any
  (*ast)->children[index] = NULL;
  for (i32 i = index; i < child_count - 1; i++) {
    (*ast)->children[i] = (*ast)->children[i + 1];
  }
  (*ast)->child_count--;
  return NO_ERR;
}

i32 ast_child_count(const Ast* ast) {
  assert(ast != NULL);
  if (!(*ast))
    return 0;
  return (*ast)->child_count;
}

i32 ast_child_count_total(const Ast* ast) {
  assert(ast != NULL);
  if (!(*ast)) {
    return 0;
  }
  if (!(*ast)->child_count) {
    return 1;
  }
  i32 count = 0;
  for (i32 i = 0; i < (*ast)->child_count; i++) {
    count += ast_child_count_total(&(*ast)->children[i]);
  }
  return count;
}

Ast* ast_get_node(Ast* ast, i32 index) {
  assert(ast != NULL);
  if (index >= 0 && index < ast_child_count(ast)) {
    return &(*ast)->children[index];
  }
  return NULL;
}

Value* ast_get_node_value(Ast* ast, i32 index) {
  assert(ast != NULL);
  if (index >= 0 && index < ast_child_count(ast)) {
    struct Node* node = (*ast)->children[index];
    if (!node)
      return NULL;
    return &node->value;
  }
  return NULL;
}

Value* ast_get_last_node_value(Ast* ast) {
  i32 child_count = ast_child_count(ast);
  return ast_get_node_value(ast, child_count - 1);
}

Value* ast_get_value(Ast* ast) {
  assert(ast);
  if (is_empty(*ast))
    return NULL;
  return &((*ast)->value);
}

void ast_print(const Ast ast) {
  if (is_empty(ast)) return;
  print_tree(ast, 0);
  printf("\n");
}

void ast_free(Ast* ast) {
  assert(ast != NULL);
  if (is_empty(*ast)) return;
  for (u32 i = 0; i < (*ast)->child_count; i++) {
    ast_free(&(*ast)->children[i]);
  }

  if ((*ast)->children) {
    m_free((*ast)->children, sizeof(struct Node*) * (*ast)->child_count);
    (*ast)->children = NULL;
    (*ast)->child_count = 0;
  }
  if (*ast) {
    m_free(*ast, sizeof(struct Node));
    *ast = NULL;
  }
  assert(is_empty(*ast));
}
