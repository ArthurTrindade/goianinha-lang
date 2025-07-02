#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/ast.h"
#include "../include/symbol_table.h"

env_t semantic_init();

void semantic_end(env_t current_env);

void semantic_program(program_t *node);

void semantic_function(env_t current_env, decl_func_t *node,
                       types_t return_type);

void semantic_block(env_t current_env, block_t *node, list_symbol_t params,
                    types_t return_function);

void semantic_cmd(env_t current_env, cmd_t *node, types_t return_function);

types_t semantic_expr(env_t current_env, expr_t *node);

void report_semantic_error(int line, const char *message);

scope_t get_current_scope(env_t env);

symbol_t *check_redefinition(scope_t scope, char *id, int line);

void add_var_to_scope(scope_t scope, char *id, types_t type, int line);

#endif