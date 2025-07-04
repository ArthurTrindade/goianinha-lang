#ifndef AST_H
#define AST_H

#include "../include/symbol_table.h"
#include "../include/types.h"

typedef struct program {
  int line;
  struct decl_funcvar *funcvar;
  struct decl_prog *prog;
} program_t;

typedef struct decl_funcvar {
  types_t type;
  int line;
  char *id;
  struct decl_var *decl_var;
  struct decl_func *decl_func;
  struct decl_funcvar *next;
} decl_funcvar_t;

typedef struct decl_prog {
  int line;
  struct block *block;
} decl_prog_t;

typedef struct decl_var {
  int line;
  char *id;
  struct decl_var *next;
} decl_var_t;

typedef struct decl_func {
  int line;
  struct param_list *params;
  struct block *block;
} decl_func_t;

typedef struct param_list {
  int line;
  struct param_listcount *param_count;
} param_list_t;

typedef struct param_listcount {
  int line;
  types_t type;
  char *id;
  struct param_listcount *next;
} param_listcount_t;

typedef struct block {
  int line;
  struct decl_varlist *var_list;
  struct cmd_list *cmd_list;
} block_t;

typedef struct decl_varlist {
  int line;
  types_t type;
  char *id;
  struct decl_var *var;
  struct decl_varlist *next;
} decl_varlist_t;

typedef struct cmd_list {
  int line;
  struct cmd *cmd;
  struct cmd_list *next;
} cmd_list_t;

typedef struct cmd {
  cmd_e kind;
  int line;
  char *id;
  struct expr *expr;
  struct block *blk;
  struct cmd *body;
  struct cmd *else_body;
} cmd_t;

typedef struct expr {
  int line;
  expr_e kind;
  char *id;
  int integer_literal;
  char char_literal;
  struct expr *left;
  struct expr *right;
  struct expr_list *expr_list;
} expr_t;

typedef struct expr_list {
  int line;
  struct expr *expr;
  struct expr_list *next;
} expr_list_t;

program_t *ast_program(decl_funcvar_t *funcvar, decl_prog_t *decl_prog, int line);

decl_funcvar_t *ast_decl_funcvar(types_t type, char *id, decl_var_t *decl_var,
                                 decl_func_t *decl_func, decl_funcvar_t *next,
                                 int line);

decl_prog_t *ast_decl_prog(block_t *blk, int line);

decl_var_t *ast_decl_var(char *id, decl_var_t *next, int line);

decl_func_t *ast_decl_func(param_list_t *params, block_t *blk, int line);

param_list_t *ast_param_list(param_listcount_t *plc, int line);

param_listcount_t *ast_param_listcount(types_t t, char *id,
                                       param_listcount_t *next, int line);

block_t *ast_block(decl_varlist_t *dvl, cmd_list_t *cmdl, int line);

decl_varlist_t *ast_decl_varlist(types_t t, char *id, decl_var_t *var,
                                 decl_varlist_t *next, int line);

cmd_list_t *ast_cmd_list(cmd_t *cmd, cmd_list_t *next, int line);

expr_t *ast_expr(expr_e e, char *id, int const_int, const char const_char,
                 expr_t *l, expr_t *r, expr_list_t *elist, int line);

expr_list_t *ast_expr_list(expr_t *expr, expr_list_t *next, int line);

cmd_t *ast_cmd_expr(expr_t *expr, int line);

cmd_t *ast_cmd_block(block_t *blk, int line);

cmd_t *ast_cmd_while(expr_t *expr, cmd_t *body, int line);

cmd_t *ast_cmd_if(expr_t *expr, cmd_t *body, int line);

cmd_t *ast_cmd_if_else(expr_t *expr, cmd_t *body, cmd_t *else_body, int line);

cmd_t *ast_cmd_leia(char *id, int line);

cmd_t *ast_cmd_escreva(expr_t *expr, int line);

cmd_t *ast_cmd_string(char *id, int line);

cmd_t *ast_cmd_ret(expr_t *expr, int line);

#endif
