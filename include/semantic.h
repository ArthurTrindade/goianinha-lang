#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/symbol_table.h"
#include "../include/ast.h"

env_t semantic_init();

void semantic_end(env_t current_env);

void semantic_program(program_t *node);

void semantic_function(env_t current_env, decl_func_t *node, types_t return_type);

void semantic_block(env_t current_env, block_t *node);

void semantic_cmd(env_t current_env, cmd_t *node);

types_t semantic_expr(env_t current_env, expr_t *node);

#endif