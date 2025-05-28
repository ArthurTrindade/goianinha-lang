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
  EXPR_ADD,
  EXPR_SUB,
  EXPR_MUL,
  EXPR_DIV,
  EXPR_NAME,
  EXPR_INTEGER_LITERAL,
  EXPR_STRING_LITERAL
} expr_t;

typedef enum {
  TYPE_INTEGER,
  TYPE_STRING,
  TYPE_FUNCTION,
} type_t;

struct decl {
  char *name;
  struct type *type;
  struct expr *value;
  struct stmt *code;
  // struct decl *next;
};

struct decl_list {
  struct decl *decl;
  struct decl_list *decl_next;
};

struct stmt {
  stmt_t kind;
  struct decl *decl;
  struct expr *init_expr;
  struct expr *expr;
  struct expr *next_expr;
  struct stmt *body;
  struct stmt *else_body;
  struct stmt *next;
};

struct stmt_list {
  struct stmt *stmt;
  struct stmt_list *stmt_next;
};

struct expr {
  expr_t kind;
  struct expr *left;
  struct expr *right;

  const char *name;
  int integer_value;
  const char *string_literal;
};

struct type {
  type_t kind;
  struct type *subtype;
  struct param_list *params;
};

struct param_list {
  char *name;
  struct type *type;
  struct param_list *next;
};

#endif
