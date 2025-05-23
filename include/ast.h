#ifndef AST_H
#define AST_H

#include "../include/token.h"

#include "../include/symbol_table.h"

typedef struct astnode_ {
  symbol_t *symbol;
  struct astnode_ *left, *right;
} astnode_t;

typedef astnode_t *ast_t;

ast_t ast_set(symbol_t *s, ast_t l, ast_t r);

void ast_travel(ast_t root);

#endif
