#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/ast.h"
#include "../include/symbol_table.h"
#include "types.h"

void semantic_program(program_t *root);
void semantic_end(env_t env);
int check_type_compatibility(types_t dest, types_t src);
types_t chech_expr(env_t env, expr_t *node);
void check_cmd(env_t env, cmd_t *node, types_t return_expected);
void check_block(env_t env, block_t *node, types_t return_expected);
void check_var_decl(env_t env, var_decl_t *node);
void register_var(env_t env, char *id, types_t type, int line);

#endif
