#ifndef AST_WALKER_H
#define AST_WALKER_H

#include "../include/ast.h"

typedef struct {
  int value;
  const char *name;
} enum_string_map_t;

const char *enum_to_string(enum_string_map_t *map, int value);

void print_program(program_t *program);

void print_block(block_t *block);

void print_cmd(cmd_t *cmd);

void print_expr(expr_t *expr);

void print_decl_list(decl_var_t *declv);

void print_params(param_listcount_t *params);

#endif
