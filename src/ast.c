#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"

expr_t *ast_expr_literal_int(int value, int line) {
    expr_t *node = (expr_t*) malloc(sizeof(expr_t));
    if (node) {
        node->kind = EXPR_LIT_INT;
        node->line = line;
        node->data.i_val = value;
        node->next = NULL;
    }
    return node;
}

expr_t *ast_expr_literal_string(char *value, int line) {
    expr_t *node = (expr_t*) malloc(sizeof(expr_t));
    if (node) {
        node->kind = EXPR_LIT_STRING;
        node->line = line;
        node->data.s_val = value ? strdup(value) : NULL;
        node->next = NULL;
    }
    return node;
}

expr_t *ast_expr_binary(expr_e kind, expr_t *left, expr_t *right, int line) {
    expr_t *node = (expr_t*) malloc(sizeof(expr_t));
    if (node) {
        node->kind = kind;
        node->line = line;
        node->data.bin_op.left = left;
        node->data.bin_op.right = right;
        node->next = NULL;
    }
    return node;
}

expr_t *ast_expr_call(char *id, expr_t *args, int line) {
    expr_t *node = (expr_t*) malloc(sizeof(expr_t));
    if (node) {
        node->kind = EXPR_CALL;
        node->line = line;
        node->data.call.id = id ? strdup(id) : NULL;
        node->data.call.args = args;
        node->next = NULL;
    }
    return node;
}

cmd_t *ast_cmd_assign(char *id, expr_t* expr, int line) {
  cmd_t *node = (cmd_t *)malloc(sizeof(cmd_t));
  if (node) {
    node->kind = CMD_ASSIGN;
    node->line = line;
    node->data.assing.id = id ? strdup(id) : NULL;
    node->next = NULL;
  }

  return node;
}

cmd_t *ast_cmd_if(expr_t *cond, cmd_t *body, cmd_t *else_body, int line) {
  cmd_t *node = (cmd_t *)malloc(sizeof(cmd_t));
  if (node) {
    node->kind = CMD_IF;
    node->line = line;
    node->data.control.cond = cond;
    node->data.control.body = body;
    node->data.control.else_body = else_body;
    node->next = NULL;
  }
  return node;
}

cmd_t *ast_cmd_while(expr_t *cond, cmd_t *body, int line) {
  cmd_t *node = (cmd_t *)malloc(sizeof(cmd_t));
  if (node) {
    node->kind = CMD_WHILE;
    node->line = line;
    node->data.control.cond = cond;
    node->data.control.body = body;
    node->next = NULL;
  }
  return node;
}

cmd_t *ast_cmd_ret(expr_t *expr, int line) {
  cmd_t *node = (cmd_t *)malloc(sizeof(cmd_t));
  if (node) {
    node->kind = CMD_RETURN;
    node->line = line;
    node->data.io_expr.expr = expr;
    node->next = NULL;
  }
  return node;
}

var_decl_t *ast_decl_var(types_t type, char *id,  int line) {
  var_decl_t *dv = (var_decl_t *)malloc(sizeof(var_decl_t));

  if (dv) {
      dv->type = type;
      dv->id = id ? strdup(id) : NULL;
      dv->line = line;
      dv->next = NULL;
  }

  return dv;
}

block_t *ast_block(var_decl_t *vars, cmd_t *cmds, int line) {
  block_t *blk = (block_t *)malloc(sizeof(block_t));
  if (blk) {
    blk->vars = vars;
    blk->cmds = cmds;
    blk->line = line;
  }
  return blk;
}


program_t *ast_program(global_decl_t *globals, block_t *main_block,
                       int line) {

  program_t *p = (program_t *)malloc(sizeof(program_t));

  if (p) {
    p->globais = globals;
    p->main_block = main_block;
    p->line = line;
  }

  return p;
}
