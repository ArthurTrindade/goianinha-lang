#ifndef AST_H
#define AST_H

#include <stdint.h>

#include "../include/symbol_table.h"
#include "../include/types.h"

typedef struct expr expr_t;
typedef struct cmd cmd_t;
typedef struct var_decl var_decl_t;
typedef struct param param_t;
typedef struct block block_t;
typedef struct func_decl func_decl_t;
typedef struct global_decl global_decl_t;
typedef struct program program_t;

/* Estrutura de Expressões */
struct expr {
  int line;
  expr_e kind;
  union {
    int i_val;          // Inteiros Literais
    char *s_val;        // String Literais e Identificadores
    struct {
      struct expr *left;
      struct expr *right;
    } bin_op;
    struct {            // Chamada de função
      char *id;
      struct expr *args; // Lista encadeada de argumentos
    } call;
    struct {            // Operações unárias (NOT, Minus)
        struct expr *arg;
    } uni_op;
    struct {            // Atribução
        char *id;
        expr_t *expr;
    } assign;
  } data;

  struct expr *next; // Lista de exprs (argumentos)
};

/* Estrutura de Comandos */
struct cmd {
  int line;
  cmd_e kind;
  union {
    struct { // Atribuição: id = expr
      char *id;
      struct expr *expr;
    } assign;

    struct { // If/Else/While
      struct expr *cond;
      struct cmd *body;
      struct cmd *else_body;
    } control;

    struct { // Escreva/Retorne/Expr (Comandos que usam expressões)
      struct expr *expr;
    } io_expr;

    struct { // Leia (Comando que usa ID)
        char *id;
    } io_read;

    struct block *block; // Para blocos aninhados
  } data;

  struct cmd *next;
};

/* Declaração de Variáveis */
struct var_decl {
  int line;
  types_t type;
  char *id;
  struct var_decl *next;
};

/* Parâmetros */
struct param {
  int line;
  types_t type;
  char *id;
  struct param *next;
};

/* Blocos */
struct block {
  int line;
  struct var_decl *vars;
  struct cmd *cmds;
};

/* Declaração de Função */
struct func_decl {
  int line;
  char *id;
  types_t ret_type;
  struct param *params;
  struct block *block;
};

/* Declarações Globais */
typedef enum {
    G_VAR,
    G_FUNC,
} global_kind_e;

struct global_decl {
  int line;
  global_kind_e kind;
  union {
    struct var_decl *var;
    struct func_decl *func;
  } decl;
  struct global_decl *next;
};

/* Programa (Raiz) */
struct program {
  int line;
  struct global_decl *globals;
  struct block *main_block;
};

/* Expressões */
expr_t *ast_expr_literal_int(int value, int line);
expr_t *ast_expr_literal_string(char *value, int line);
expr_t *ast_expr_binary(expr_e kind, expr_t *left, expr_t *right, int line);
expr_t *ast_expr_call(char *id, expr_t *args, int line);
expr_t *ast_expr_assign(char *id, expr_t *expr, int line);

/* Comandos */
cmd_t *ast_cmd_assign(char *id, expr_t *expr, int line);
cmd_t *ast_cmd_if(expr_t *cond, cmd_t *body, cmd_t *else_body, int line);
cmd_t *ast_cmd_while(expr_t *cond, cmd_t *body, int line);
cmd_t *ast_cmd_ret(expr_t *expr, int line);
cmd_t *ast_cmd_leia(char *id, int line);
cmd_t *ast_cmd_escreva(expr_t *expr, int line);
cmd_t *ast_cmd_block(block_t *blk, int line);
cmd_t *ast_cmd_expr(expr_t *expr, int line);

/* Estruturais */
var_decl_t *ast_decl_var(types_t type, char *id, int line);
param_t *ast_param(types_t type, char *id, int line);
block_t *ast_block(var_decl_t *vars, cmd_t *cmds, int line);
func_decl_t *ast_decl_func(types_t type, char *id, param_t *params, block_t *blk, int line);

/* Globais */
global_decl_t *ast_global_var(var_decl_t *var, int line);
global_decl_t *ast_global_func(func_decl_t *func, int line);
program_t *ast_program(global_decl_t *globals, block_t *main_block, int line);

/* Auxiliar */
void ast_append_cmd(cmd_t *head, cmd_t *new_node);
void* safe_alloc(size_t size);

#endif
