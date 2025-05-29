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
} stmt_t;

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
} expr_t;

typedef enum {
  TYPE_INTEGER,
  TYPE_STRING,
  TYPE_FUNCTION,
} type_t;

struct program {
  struct decl_funcvar *funcvar;
  struct decl_prog *prog;
};

struct decl_funcvar {
  type_t type;
  char *id;
  struct decl_var *decl_var;
  struct decl_func *decl_func;
  struct decl_funcvar *next;
};

struct decl_prog {
  struct stmt *block;
};

struct decl_var {
  char *name;
  struct decl_var *next;
};

struct decl_func {
  struct param_list *params;
  struct block *block;
};

struct param_list {
  struct param_listcount *param_count;
};

struct param_listcount {
  type_t type;
  char *id;
  struct param_listcount *next;
};

struct block {
  struct decl_varlist *var_list;
  struct stmt_list *stmt_list;
};

struct decl_varlist {
  type_t type;
  char *id;
  struct decl_var *var;
  struct decl_varlist *next;
};

struct stmt_list {
  struct stmt *stmt;
  struct stmt_list *next;
};

struct stmt {
  stmt_t kind;
  char *id;
  char *str;
  struct block *block;
  struct stmt *body;
  struct stmt *else_body;
};

struct expr {
  expr_t kind;
  char *id;
  struct expr_or *expr_or;
  struct expr *expr;
};

struct expr_or {
  expr_t kind;
  struct expr_or *expr_or;
  struct expr_and *expr_and;
};

struct expr_and {
  expr_t kind;
  struct expr_and *expr_and;
  struct expr_eq *expr_eq;
};

struct expr_eq {
  expr_t kind;
  struct expr_eq *expr_eq;
  struct expr_ineq *expr_ineq;
};

struct expr_ineq {
  expr_t kind;
  struct expr_ineq *expr_ineq;
  struct expr_add *expr_add;
};

struct expr_add {
  expr_t kind;
  struct expr_add *expr_add;
  struct expr_mul *expr_mult;
};

struct expr_mult {
  expr_t kind;
  struct expr_mult *expr_mult;
  struct expr_un *expr_un;
};

struct expr_un {
  expr_t kind;
  struct expr_prim;
};

struct expr_prim {
  char *id;
  struct expr *expr;
  struct expr_list *expr_list;
};

struct expr_list {
  struct expr *expr;
  struct expr_list *next;
};

// struct expr {
//   expr_t kind;
//   struct expr_list *expr_list;
//   const char *id;
//   struct expr *left;
//   struct expr *right;
//   int integer_literal;
//   const char char_literal;
// };

#endif
