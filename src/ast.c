#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"

expr_t *ast_expr_literal_int(int value, int line) {
  expr_t *node = (expr_t *)malloc(sizeof(expr_t));
  if (node) {
    node->kind = EXPR_LIT_INT;
    node->line = line;
    node->data.i_val = value;
    node->next = NULL;
  }
  return node;
}

expr_t *ast_expr_literal_string(char *value, int line) {
  expr_t *node = (expr_t *)malloc(sizeof(expr_t));
  if (node) {
    node->kind = EXPR_LIT_STRING;
    node->line = line;
    node->data.s_val = value ? strdup(value) : NULL;
    node->next = NULL;
  }
  return node;
}

expr_t *ast_expr_binary(expr_e kind, expr_t *left, expr_t *right, int line) {
  expr_t *node = (expr_t *)malloc(sizeof(expr_t));
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
  expr_t *node = (expr_t *)malloc(sizeof(expr_t));
  if (node) {
    node->kind = EXPR_CALL;
    node->line = line;
    node->data.call.id = id ? strdup(id) : NULL;
    node->data.call.args = args;
    node->next = NULL;
  }
  return node;
}

cmd_t *ast_cmd_assign(char *id, expr_t *expr, int line) {
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

cmd_t *ast_cmd_leia(char *id, int line) {
  cmd_t *node = (cmd_t *)malloc(sizeof(cmd_t));
  if (node) {
    node->kind = CMD_LEIA;
    node->line = line;
    node->data.io_expr.expr->data.s_val = id;
    node->next = NULL;
  }
  return node;
}

cmd_t *ast_cmd_escreva(expr_t *expr, int line) {
  cmd_t *node = (cmd_t *)malloc(sizeof(cmd_t));
  if (node) {
    node->kind = CMD_ESCREVA;
    node->line = line;
    node->data.io_expr.expr = expr;
    node->next = NULL;
  }
  return node;
}

var_decl_t *ast_decl_var(types_t type, char *id, int line) {
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

param_t *ast_param(types_t type, char *id, int line) {
  param_t *node = (param_t *)malloc(sizeof(param_t));
  if (node) {
    node->type = type;
    node->line = line;
    node->id = id ? strdup(id) : NULL;
    node->next = NULL;
  }
  return node;
}

func_decl_t *ast_decl_func(types_t type, char *id, param_t *params,
                           block_t *blk, int line) {

  func_decl_t *node = (func_decl_t *)malloc(sizeof(func_decl_t));
  if (node) {
    node->ret_type = type;
    node->line = line;
    node->id = id ? strdup(id) : NULL;
    node->block = blk;
    node->params = params;
  }
  return node;
}

global_decl_t *ast_global_var(var_decl_t *var, int line) {
  global_decl_t *node = (global_decl_t *)malloc(sizeof(global_decl_t));
  if (node) {
    node->decl.var = var;
    node->line = line;
    node->next = NULL;
  }
  return node;
}

global_decl_t *ast_global_func(func_decl_t *func, int line) {
  global_decl_t *node = (global_decl_t *)malloc(sizeof(global_decl_t));
  if (node) {
    node->decl.func = func;
    node->line = line;
    node->next = NULL;
  }
  return node;
}

program_t *ast_program(global_decl_t *globals, block_t *main_block, int line) {

  program_t *p = (program_t *)malloc(sizeof(program_t));

  if (p) {
    p->globais = globals;
    p->main_block = main_block;
    p->line = line;
  }

  return p;
}

void ast_append_cmd(cmd_t *head, cmd_t *new_node) {
  if (!head)
    return;
  cmd_t *current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_node;
}
