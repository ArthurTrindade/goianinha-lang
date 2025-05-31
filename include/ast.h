#ifndef AST_H
#define AST_H

#include "../include/symbol_table.h"
#include "../include/token.h"

typedef enum {
  STMT_DECL,
  STMT_EXPR,
  STMT_IF,
  STMT_IF_ELSE,
  STMT_WHILE,
  STMT_PRINT,
  STMT_RETURN,
  STMT_BLOCK
} stmt_e;

typedef enum {
  EXPR_OR,
  EXPR_AND,
  EXPR_EQ,
  EXPR_NEQ,
  EXPR_INQ,
  EXPR_LT,
  EXPR_LEQ,
  EXPR_GT,
  EXPR_GEQ,
  EXPR_ADD,
  EXPR_SUB,
  EXPR_MUL,
  EXPR_DIV,
  EXPR_NOT,
  /* TODO: - PrimExpr */
  EXPR_ID,
  EXPR_INTEGER_LITERAL,
  EXPR_STRING_LITERAL,
} expr_e;

typedef enum {
  TYPE_INTEGER,
  TYPE_STRING,
  TYPE_FUNCTION,
} type_e;

typedef struct program {
  struct decl_funcvar *funcvar;
  struct decl_prog *prog;
} program_t;

typedef struct decl_funcvar {
  type_e type;
  char *id;
  struct decl_var *decl_var;
  struct decl_func *decl_func;
  struct decl_funcvar *next;
} decl_funcvar_t;

typedef struct decl_prog {
  struct block *block;
} decl_prog_t;

typedef struct decl_var {
  char *id;
  struct decl_var *next;
} decl_var_t;

typedef struct decl_func {
  struct param_list *params;
  struct block *block;
} decl_func_t;

typedef struct param_list {
  struct param_listcount *param_count;
} param_list_t;

typedef struct param_listcount {
  type_e type;
  char *id;
  struct param_listcount *next;
} param_listcount_t;

typedef struct block {
  struct decl_varlist *var_list;
  struct cmd_list *cmd_list;
} block_t;

typedef struct decl_varlist {
  type_e type;
  char *id;
  struct decl_var *var;
  struct decl_varlist *next;
} decl_varlist_t;

typedef struct cmd_list {
  struct cmd *cmd;
  struct cmd_list *next;
} cmd_list_t;

typedef struct cmd {
  stmt_e kind;
  char *id;
  char *str;
  struct block *block;
  struct cmd *body;
  struct cmd *else_body;
} cmd_t;

typedef struct expr {
  expr_e kind;
  const char *id;
  int integer_literal;
  char char_literal;
  struct expr *left;
  struct expr *right;
  struct expr_list *expr_list;
} expr_t;

typedef struct expr_list {
  struct expr *expr;
  struct expr_list *next;
} expr_list_t;

#endif
