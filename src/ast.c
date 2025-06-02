#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"

program_t *ast_program(decl_funcvar_t *funcvar, decl_prog_t *decl_prog) {

  program_t *p = (program_t *)malloc(sizeof(program_t));

  if (p) {
    p->prog = decl_prog;
    p->funcvar = funcvar;
  }

  return p;
}

decl_funcvar_t *ast_decl_funcvar(type_e type, char *id, decl_var_t *decl_var,
                                 decl_func_t *decl_func, decl_funcvar_t *next) {

  decl_funcvar_t *df = (decl_funcvar_t *)malloc(sizeof(decl_funcvar_t));

  if (df) {
    df->type = type;
    df->id = id ? strdup(id) : NULL;
    df->decl_var = decl_var;
    df->decl_func = decl_func;
    df->next = next;
  }

  return df;
}

decl_prog_t *ast_decl_prog(block_t *blk) {
  decl_prog_t *prog = (decl_prog_t *)malloc(sizeof(decl_prog_t));

  if (prog) {
    prog->block = blk;
  }

  return prog;
}

decl_var_t *ast_decl_var(char *id, decl_var_t *next) {
  decl_var_t *dv = (decl_var_t *)malloc(sizeof(decl_var_t));

  if (dv) {
    dv->id = id ? strdup(id) : NULL;
    dv->next = next;
  }

  return dv;
}

decl_func_t *ast_decl_func(param_list_t *params, block_t *blk) {

  decl_func_t *df = (decl_func_t *)malloc(sizeof(decl_func_t));

  if (df) {
    df->params = params;
    df->block = blk;
  }

  return df;
}

param_list_t *ast_param_list(param_listcount_t *plc) {
  param_list_t *pl = (param_list_t *)malloc(sizeof(param_list_t));

  if (pl) {
    pl->param_count = plc;
  }

  return pl;
}

param_listcount_t *ast_param_listcount(type_e t, char *id,
                                       param_listcount_t *next) {

  param_listcount_t *plc =
      (param_listcount_t *)malloc(sizeof(param_listcount_t));

  if (plc) {
    plc->id = id ? strdup(id) : NULL;
    plc->type = t;
    plc->next = next;
  }

  return plc;
}

block_t *ast_block(decl_varlist_t *dvl, cmd_list_t *cmdl) {
  block_t *blk = (block_t *)malloc(sizeof(block_t));
  if (blk) {
    blk->cmd_list = cmdl;
    blk->var_list = dvl;
  }

  return blk;
}

decl_varlist_t *ast_decl_varlist(type_e t, char *id, decl_var_t *var,
                                 decl_varlist_t *next) {
  decl_varlist_t *dvl = (decl_varlist_t *)malloc(sizeof(decl_varlist_t));

  if (dvl) {
    dvl->type = t;
    dvl->id = id ? strdup(id) : NULL;
    dvl->var = var;
    dvl->next = next;
  }
  return dvl;
}

cmd_list_t *ast_cmd_list(cmd_t *cmd, cmd_list_t *next) {
  cmd_list_t *cmdl = (cmd_list_t *)malloc(sizeof(cmd_list_t));

  if (cmdl) {
    cmdl->cmd = cmd;
    cmdl->next = next;
  }

  return cmdl;
}

cmd_t *ast_cmd(stmt_e ctype, char *id, char *str, expr_t *expr, block_t *blk,
               cmd_t *body, cmd_t *else_body) {

  cmd_t *cmd = (cmd_t *)malloc(sizeof(cmd_t));

  if (cmd) {
    cmd->kind = ctype;
    cmd->id = id ? strdup(id) : NULL;
    cmd->str = str;
    cmd->expr = expr;
    cmd->block = blk;
    cmd->body = body;
    cmd->else_body = else_body;
  }

  return cmd;
}

expr_t *ast_expr(expr_e e, char *id, int const_int, char const_char, expr_t *l,
                 expr_t *r, expr_list_t *elist) {

  expr_t *expr = (expr_t *)malloc(sizeof(expr_t));

  if (expr) {
    expr->kind = e;
    expr->id = id ? strdup(id) : NULL;
    expr->integer_literal = const_int;
    expr->char_literal = const_char;
    expr->left = l;
    expr->right = r;
    expr->expr_list = elist;
  }

  return expr;
}

expr_list_t *ast_expr_list(expr_t *expr, expr_list_t *next) {
  expr_list_t *exprl = (expr_list_t *)malloc(sizeof(expr_list_t));

  if (exprl) {
    exprl->expr = expr;
    exprl->next = next;
  }

  return exprl;
}
