#include "../include/semantic.h"
#include "../include/symbol_table.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>

int semantic_errors = 0;

/* Função para reportar erros */
void report_semantic_error(int line, const char *msg) {
  fprintf(stderr, "Erro Semântico (Linha %d): %s\n", line, msg);
  semantic_errors++;
}

env_t semantic_init() { return env_new(); }

void semantic_end(env_t env) { env_free(env); }

/* Verificação compatibilidade de tipos */
int check_type_compatibility(types_t dest, types_t src) {
  if (dest == src)
    return 1;
  if (dest == T_INT && src == T_CAR)
    return 1;
  if (dest == T_CAR && src == T_INT)
    return 1;
  return 0;
}

void register_var(env_t env, char *id, types_t type, int line) {
  // Verifica se já existe no escopo atual
  if (symbol_search_current(env, id)) {
    char msg[256];
    sprintf(msg, "Variavel '%s' já declarada neste escopo.", id);
    report_semantic_error(line, msg);
    return;
  }

  // se não existir adiciona no escopo
  symbol_t *s = symbol_new_var(id, SYM_VAR, type, 0, line);
  symbol_add(env, s);

  free(s);
}

list_t *extract_params_from_ast(param_t *ast_params) {
  list_t *l = NULL;
  list_init(l, symbol_free);
  param_t *p = ast_params;
  while (p != NULL) {
    symbol_t *s = symbol_new_var(p->id, SYM_PARAM, p->type, 0, p->line);
    list_insert_end(l, s);
    p = p->next;
  }
  return l;
}

types_t check_expr(env_t env, expr_t *node) {
  if (!node)
    return T_VOID;

  switch (node->kind) {
  case EXPR_LIT_INT:
    return T_INT;

  case EXPR_LIT_STRING: {
    symbol_t *s = symbol_search(env, node->data.s_val);
    if (s != NULL)
      return s->data_type;
    return T_STRING;
  }

  case EXPR_CALL: {
    symbol_t *s = symbol_search(env, node->data.call.id);
    if (s == NULL) {
      char msg[256];
      sprintf(msg, "Função '%s' não declarada", node->data.call.id);
      return T_ERROR;
    }

    if (s->kind != SYM_FUNC) {
      report_semantic_error(node->line,
                            "Tentativa de chamar variável como função.");
      return T_ERROR;
    }

    node_t *def_node = list_head(s->info.func.params);
    expr_t *call_node = node->data.call.args;
    int arg_indx = 1;

    while (def_node != NULL && call_node != NULL) {
      symbol_t *param_def = (symbol_t *)list_data(def_node);
      types_t expected = param_def->data_type;
      types_t passed = check_expr(env, call_node);

      if (!check_type_compatibility(expected, passed)) {
        char msg[256];
        sprintf(msg, "Tipo incompativel no argumentp %d da chamada de '%s'",
                arg_indx, s->id);
        report_semantic_error(node->line, msg);
        return T_ERROR;
      }
      def_node = def_node->next;
      call_node = call_node->next;
      arg_indx++;
    }
    if (def_node != NULL) {
      report_semantic_error(node->line,
                            "Menos argumentos do que o necessário.");
    } else if (call_node != NULL) {
      report_semantic_error(node->line, "Mais argumentos do que o necessário.");
    }

    return s->data_type;
  }
  case EXPR_ADD:
  case EXPR_SUB:
  case EXPR_MUL:
  case EXPR_DIV: {
    types_t t1 = check_expr(env, node->data.bin_op.left);
    types_t t2 = check_expr(env, node->data.bin_op.right);
    if (t1 == T_ERROR || t2 == T_ERROR)
      return T_ERROR;

    if (!check_type_compatibility(T_INT, t1) ||
        !check_type_compatibility(T_INT, t2)) {
      report_semantic_error(node->line,
                            "Operação aritmetica requer tipos númericos.");
      return T_ERROR;
    }
    return T_INT;
  }

  case EXPR_EQUAL:
  case EXPR_DIFF:
  case EXPR_LESS:
  case EXPR_GREATER:
  case EXPR_LESS_EQUAL:
  case EXPR_GREATER_EQUAL: {
    types_t t1 = check_expr(env, node->data.bin_op.left);
    types_t t2 = check_expr(env, node->data.bin_op.right);

    if (!check_type_compatibility(t1, t2) &&
        !check_type_compatibility(t2, t1)) {
      report_semantic_error(node->line,
                            "Comparação entre tipos incompativeis.");
      return T_ERROR;
    }
    return T_INT;
  }

  default:
    return T_INT;
  }
}

void check_cmd(env_t env, cmd_t *node, types_t return_expected) {
  if (!node)
    return;
  switch (node->kind) {
  case CMD_ASSIGN: {
    symbol_t *s = symbol_search(env, node->data.assign.id);
    if (s == NULL) {
      char msg[256];
      sprintf(msg, "Variavel '%s' nao declarada. (atribuição)",
              node->data.assign.id);
      report_semantic_error(node->line, msg);
      return;
    }

    if (s->kind == SYM_FUNC) {
      report_semantic_error(node->line,
                            "Atribuição invalida: nome refere-se a uma função");
      return;
    }

    types_t expr_val = check_expr(env, node->data.assign.expr);

    if (!check_type_compatibility(s->data_type, expr_val)) {
      report_semantic_error(node->line, "Tipos incompativeis na atribuição.");
    }
    break;
  }

  case CMD_IF:
  case CMD_WHILE: {
    types_t cond = check_expr(env, node->data.control.cond);
    if (cond != T_INT && T_CAR) {
      report_semantic_error(node->line,
                            "A condição deve ser um valor lógico/numerico.");
    }
    check_cmd(env, node->data.control.body, return_expected);
    if (node->kind == CMD_IF && node->data.control.else_body) {
      check_cmd(env, node->data.control.else_body, return_expected);
    }
    break;
  }

  case CMD_RETURN: {
    types_t ret = check_expr(env, node->data.io_expr.expr);
    if (!check_type_compatibility(return_expected, ret)) {
      char msg[256];
      sprintf(msg, "Tipo de retorno incorreto. Esperado: %d, Encontrado: %d",
              return_expected, ret);
      report_semantic_error(node->line, msg);
    }
    break;
  }

  case CMD_LEIA: {
    symbol_t *s = symbol_search(env, node->data.io_read.id);
    if (!s) {
      char msg[256];
      sprintf(msg, "Variavel '%s' não declarada. (leia)",
              node->data.io_read.id);
      report_semantic_error(node->line, msg);
    }
    break;
  }

  case CMD_ESCREVA:
  case CMD_EXPR: {
    check_expr(env, node->data.io_expr.expr);
    break;
  }

  case CMD_BLOCK:
    check_block(env, node->data.block, return_expected);
    break;
  }
}

void check_var_decl(env_t env, var_decl_t *node) {
  while (node) {
    register_var(env, node->id, node->type, node->line);
    node = node->next;
  }
}

void check_block(env_t env, block_t *node, types_t return_expected) {
  if (!node)
    return;

  env_push_scope(env);

  var_decl_t *v = node->vars;
  while (v) {
    register_var(env, v->id, v->type, v->line);
    v = v->next;
  }

  cmd_t *c = node->cmds;
  while (c) {
    check_cmd(env, c, return_expected);
    c = c->next;
  }

  env_pop_scope(env);
}
void semantic_program(program_t *root) {
  if (!root)
    return;

  env_t env = semantic_init();
  env_push_scope(env); // Escopo Global

  global_decl_t *g = root->globals;

  while (g != NULL) {
    if (g->kind == G_VAR) {
      var_decl_t *v = g->decl.var;
      while (v != NULL) {
        register_var(env, v->id, v->type, v->line);
        v = v->next;
      }
    } else if (g->kind == G_FUNC) {
      func_decl_t *f = g->decl.func;
      if (symbol_search_current(env, f->id)) {
        report_semantic_error(f->line, "Função redeclarada.");
      } else {
        list_t *params_list = extract_params_from_ast(f->params);
        symbol_t *s = symbol_new_func(f->id, f->ret_type, params_list, f->line);
        symbol_add(env, s);
        free(s);
      }
    }
    g = g->next;
  }

  g = root->globals;
  while (g != NULL) {
    if (g->kind == G_FUNC) {
      func_decl_t *f = g->decl.func;
      env_push_scope(env);

      param_t *p = f->params;
      while (p != NULL) {
        register_var(env, p->id, p->type, p->line);
        p = p->next;
      }

      check_block(env, f->block, f->ret_type);

      env_pop_scope(env);
    }
    g = g->next;
  }

  if (root->main_block) {
    check_block(env, root->main_block, T_INT);
  }

  env_pop_scope(env);

  semantic_end(env);

  if (semantic_errors > 0) {

    fprintf(stderr,
            "\nERRO: %d erros semanticos encontrados. Compilacao abortada.\n",
            semantic_errors);
    exit(1);
  }
}
