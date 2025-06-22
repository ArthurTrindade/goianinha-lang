
#include "../include/semantic.h"

env_t semantic_init() {
  env_t env = env_new();
  if (env == NULL) {
    fprintf(stderr, "Erro fatal: Falha ao alocar ambiente de escopos.\n");
    exit(1);
  }
  scope_t h = symboltable_new();
  if (h == NULL) {
    fprintf(stderr, "Erro fatal: Falha ao alocar tabela de símbolos global.\n");
    env_free(env);
    exit(1);
  }
  env_add(env, h);
  return env;
}

void semantic_end(env_t current_env) { env_free(current_env); }

void report_semantic_error(int line, const char *message) {
  fprintf(stderr, "Erro semântico na linha %d: %s\n", line, message);
}

scope_t get_current_scope(env_t env) {
  if (env == NULL || env->head == NULL) {
    fprintf(
        stderr,
        "Erro interno: A pilha de escopos está vazia ou não inicializada.\n");
    return NULL;
  }

  return env->head->data;
}

symbol_t *check_redefinition(scope_t scope, char *id, int line) {
  if (!id) {
    return NULL;
  }
  symbol_t aux = {.lexeme = id};
  symbol_t *s = symboltable_get(scope, &aux.lexeme);
  if (s) {
    return s;
  }

  return NULL;
}

void add_var_to_scope(scope_t scope, char *id, types_t type, int line) {
  if (!id) {
    return;
  }
  symbol_t *new_sym = symbol_var(id, type, -1, line);
  if (!new_sym) {
    report_semantic_error(line, "Falha ao alocar novo símbolo.");
    return;
  }
  symboltable_set(scope, new_sym);
}

int are_types_compatible(types_t expected, types_t found) {
  if (expected == found)
    return 1;
  return 0;
}

void semantic_program(program_t *node) {
  env_t current_env = semantic_init();

  scope_t global_scope = get_current_scope(current_env);

  if (!global_scope) {
    fprintf(stderr, "Erro crítivo: Escopo global não disponível.\n");
    semantic_end(current_env);
    return;
  }

  decl_funcvar_t *current_funcvar = node->funcvar;
  while (current_funcvar) {
    if (current_funcvar->decl_var) {
      symbol_t *s = check_redefinition(global_scope, current_funcvar->id,
                                       current_funcvar->line);

      if (s) {
        report_semantic_error(current_funcvar->decl_var->line,
                              "Variável redefinida.\n");
      } else {
        add_var_to_scope(global_scope, current_funcvar->id,
                         current_funcvar->type, current_funcvar->line);
      }
      decl_var_t *current_var = current_funcvar->decl_var;
      while (current_var) {

        symbol_t *s = check_redefinition(global_scope, current_var->id,
                                         current_var->line);
        if (s) {
          report_semantic_error(current_var->line,
                                "Variável global redefinida.\n");
        } else {
          add_var_to_scope(global_scope, current_var->id, current_funcvar->type,
                           current_var->line);
        }
        current_var = current_var->next;
      }

    } else if (current_funcvar->decl_func) {
      symbol_t *s = check_redefinition(global_scope, current_funcvar->id,
                                       current_funcvar->line);
      if (s) {
        report_semantic_error(current_funcvar->decl_func->params->line,
                              "Função redefinida\n");
      } else {
        list_symbol_t fun_params = list_symbol_new();
        param_listcount_t *param_node =
            current_funcvar->decl_func->params->param_count;

        while (param_node) {
          symbol_t *param_sym = symbol_param(param_node->id, param_node->type,
                                             -1, param_node->line);

          list_symbol_add(fun_params, param_sym);
          param_node = param_node->next;
        }

        symbol_t *new_func = symbol_function(
            current_funcvar->id, current_funcvar->type, list_size(fun_params),
            fun_params, current_funcvar->decl_func->params->line);

        symboltable_set(global_scope, new_func);

        semantic_function(current_env, current_funcvar->decl_func,
                          current_funcvar->type);
      }
    }

    current_funcvar = current_funcvar->next;
  }

  if (node->prog && node->prog->block) {
    semantic_block(current_env, node->prog->block);
  }

  semantic_end(current_env);

  //   if (semantic_errors > 0) {
  //     fprintf(stderr, "Compilação falhou devido a %d erro(s)
  //                     semântico(s)
  //                         .\n ",
  //                     semantic_errors);
  //   } else {
  //     fprintf(stdout, "Análise semântica concluída com sucesso.\n");
  //   }
}

void semantic_function(env_t current_env, decl_func_t *node,
                       types_t return_type) {

  env_add(current_env, symboltable_new());
  scope_t func_scope = get_current_scope(current_env);

  param_listcount_t *param_node = node->params->param_count;
  while (param_node) {
    if (check_redefinition(func_scope, param_node->id, param_node->line)) {
      report_semantic_error(param_node->line, "Parametro redefinido.\n");
    } else {
      add_var_to_scope(func_scope, param_node->id, param_node->type,
                       param_node->line);
    }
    param_node = param_node->next;
  }

  semantic_block(current_env, node->block);

  env_delete(current_env);
}

void semantic_block(env_t current_env, block_t *node) {

  env_add(current_env, symboltable_new());
  decl_varlist_t *current_varlist = node->var_list;
  scope_t top = get_current_scope(current_env);

  while (current_varlist) {

    if (check_redefinition(top, current_varlist->id, current_varlist->line)) {
      report_semantic_error(current_varlist->var->line,
                            "Variável local redefinida.");
    } else {
      add_var_to_scope(top, current_varlist->id, current_varlist->type,
                       current_varlist->var->line);
    }

    decl_var_t *current_var = current_varlist->var;
    while (current_var) {

      if (check_redefinition(top, current_var->id, current_var->line)) {
        report_semantic_error(current_var->line, "Variável redefinida");
      } else {
        add_var_to_scope(top, current_var->id, current_varlist->type,
                         current_var->line);
      }
      current_var = current_var->next;
    }
    current_varlist = current_varlist->next;
  }

  cmd_list_t *current_cmd_list = node->cmd_list;
  while (current_cmd_list) {
    semantic_cmd(current_env, current_cmd_list->cmd);
    current_cmd_list = current_cmd_list->next;
  }

  env_delete(current_env);
}

void semantic_cmd(env_t current_env, cmd_t *node) {

  if (node == NULL)
    return;
  switch (node->kind) {
  case CMD_EXPR:
    semantic_expr(current_env, node->expr);
    break;
  case CMD_BLOCK:
    semantic_block(current_env, node->blk);
    break;
  case CMD_WHILE:
  case CMD_IF:
  case CMD_IF_ELSE: {
    types_t cond_type = semantic_expr(current_env, node->expr);
    if (cond_type != T_INT) {
      printf("A condição incorreta\n");
    }
    semantic_cmd(current_env, node->body);
    if (node->else_body && node->kind == CMD_IF_ELSE) {
      semantic_cmd(current_env, node->else_body);
    }
    break;
  }
  case CMD_LEIA: {
    symbol_t *sym = symbol_search(current_env, node->id);
    if (sym == NULL) {
      report_semantic_error(node->line, "Variável não declarada.");
    } else if (sym->symbol_type != T_VAR && sym->symbol_type != T_PARAM) {
      report_semantic_error(node->line, "Não é um parâmetro ou variável.");
    }
    break;
  }
  case CMD_ESCREVA: {
    semantic_expr(current_env, node->expr);
    break;
  }
  case CMD_RETORNE: {
  }

  default:
    break;
  }
}

types_t semantic_expr(env_t current_env, expr_t *node) {
  if (node == NULL)
    return T_UNKNOWN;

  switch (node->kind) {
  case EXPR_ID: {
    symbol_t *sym = symbol_search(current_env, node->id);
    if (sym == NULL) {
      report_semantic_error(node->line, "Identificador não encontrado");
      return T_UNKNOWN;
    }

    if (sym->symbol_type == T_VAR || sym->symbol_type == T_PARAM) {
      return sym->data_type;
    } else if (sym->symbol_type == T_FUNCTION && node->expr_list != NULL) {
      int args_count = 0;
      expr_list_t *current_arg = node->expr_list;
      while (current_arg) {
        args_count++;
        current_arg = current_arg->next;
      }

      if (args_count != list_size(sym->params)) {
        report_semantic_error(node->line, "Números de parametros incorreto.");
      }

      // comparar tipos dos argumentos
      current_arg = node->expr_list;
      node_t *param_node = list_head(sym->params);
      for (int i = 0; i < args_count; i++) {
        types_t type = semantic_expr(current_env, current_arg->expr);
        symbol_t *s = list_data(param_node);
        if (type != s->data_type) {
          // report error - Tipo do argumento difrente do parametro
        }
        current_arg = current_arg->next;
        param_node = list_next(param_node);
      }
      return sym->data_type;
    } else if (sym->symbol_type == T_FUNCTION && node->expr_list == NULL) {
      // report error Uso de função sem chamada?
      return T_UNKNOWN;
    }
    return T_UNKNOWN;
  }
  case EXPR_INT:
    return T_INT;
  case EXPR_CHAR:
    return T_CAR;

  case EXPR_ADD:
  case EXPR_SUB:
  case EXPR_MUL:
  case EXPR_DIV: {
    types_t left_type  = semantic_expr(current_env, node->left);
    types_t right_type = semantic_expr(current_env, node->right);
    if (left_type == T_UNKNOWN || right_type == T_UNKNOWN) {
      return T_UNKNOWN;
    }

    if (left_type == T_INT && right_type == T_INT) {
      return T_INT;
    }

    report_semantic_error(node->line, "Tipos incompátiveis");
    return T_UNKNOWN;
  }
  case EXPR_EQUAL:
  case EXPR_DIFF:
  case EXPR_LESS:
  case EXPR_LESS_EQUAL:
  case EXPR_GREATER:
  case EXPR_GREATER_EQUAL: {
    types_t left_type = semantic_expr(current_env, node->left);
    types_t right_type = semantic_expr(current_env, node->right);
    if (left_type == T_UNKNOWN || right_type == T_UNKNOWN) {
      return T_UNKNOWN;
    }

    if (left_type == T_INT && right_type == T_INT) {
      return T_INT;
    }

    report_semantic_error(node->line, "Tipos inconpatíveis");

    return T_UNKNOWN;
  }

  case EXPR_OR:
  case EXPR_AND: {
    types_t left_type  = semantic_expr(current_env, node->left);
    types_t right_type = semantic_expr(current_env, node->right);
    if (left_type == T_UNKNOWN || right_type == T_UNKNOWN) {
      return T_UNKNOWN;
    }

    if (left_type == T_INT && right_type == T_INT) {
      return T_INT;
    }
    report_semantic_error(node->line, "Tipos incompatíveis");
    return T_UNKNOWN;
  }

  case EXPR_NOT:
  case EXPR_MINUS: {
    types_t operand_type = semantic_expr(current_env, node->left);

    if (operand_type == T_UNKNOWN) {
        return T_UNKNOWN;
    }

    if (node->kind == EXPR_NOT) {
        if (operand_type == T_INT) {
            return T_INT;
        }
        report_semantic_error(node->line, "Tipo inconpatível");
        return T_UNKNOWN;
    } else if (node->kind == EXPR_MINUS) {
        if (operand_type == T_INT) {
            return operand_type;
        }

        report_semantic_error(node->line, "Tipo incompatível");
        return T_UNKNOWN;
    }
  }


  default:
    return T_UNKNOWN;
  }
}