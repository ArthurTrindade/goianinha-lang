
#include "../include/semantic.h"

env_t semantic_init() {
  env_t env = env_new();
  scope_t h = symboltable_new();
  env_add(env, h);
  return env;
}

void semantic_end(env_t current_env) { env_free(current_env); }

void semantic_program(program_t *node) {
  env_t current_env = semantic_init();

  // Para declarações de funções e variaveis globais
  decl_funcvar_t *current_funcvar = node->funcvar;
  scope_t top = current_env->head->data;

  while (current_funcvar != NULL) {
    if (current_funcvar->decl_var) {
      symbol_t aux = {.lexeme = current_funcvar->id};
      symbol_t *s = symboltable_get(top, &aux.lexeme);
      if (s) {
        printf("Variável global já declarada\n");
      } else {
        symbol_t *new_var_sym =
            symbol_var(current_funcvar->id, current_funcvar->type, -1,
                       current_funcvar->line);
        symboltable_set(top, new_var_sym);
      }
    } else if (current_funcvar->decl_func) {
      symbol_t aux = {.lexeme = current_funcvar->id};
      symbol_t *s = symboltable_get(top, &aux.lexeme);
      if (s) {
        printf("Função da definida\n");
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

        symboltable_set(top, new_func);
        // TODO: semantic_function
        semantic_function(current_env, current_funcvar->decl_func,
          current_funcvar->type);
      }
    }

    current_funcvar = current_funcvar->next;
  }

  if (node->prog && node->prog->block) {
    // current_function_return_type
    // TODO: Semantic Block
    // semantic_block(current_env, node->prog->block);
    printf("semantic block\n");
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

  //   current_function_return_type = return_type;
  scope_t top = list_data(list_head(current_env));
  param_listcount_t *param_node = node->params->param_count;

  while (param_node) {
    symbol_t aux = {.lexeme = param_node->id};
    symbol_t *s = symboltable_get(top, &aux.lexeme);
    if (s) {
      printf("Parametro redefinido\n");
    } else {
      symbol_t *new_param_sym = symbol_param(param_node->id, param_node->type, -1, param_node->line);
      symboltable_set(top, new_param_sym);
    }
    param_node = param_node->next;
  }

//   TODO: semantic_block(current_env, node->block);

  env_delete(current_env);
}

void semantic_block(env_t current_env, block_t *node) {
  env_add(current_env, symboltable_new());

  decl_varlist_t *current_varlist = node->var_list;
  while (current_varlist != NULL) {
    if (symboltable_get(list_data(list_head(current_env)),
                        current_varlist->id) != NULL) {
      // report error
    } else {
      symbol_t *new_var_sym =
          symbol_var(current_varlist->id, current_varlist->type, -1,
                     current_varlist->line);
      symboltable_set(list_data(list_head(current_env)), new_var_sym);
    }

    current_varlist = current_varlist->next;
  }

  cmd_list_t *current_cmd_list = node->cmd_list;
  while (current_cmd_list != NULL) {
    // semantic_cmd(current_cmd_list->cmd);
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
    // types_t conde_type = semantic_expr(node->expr);
    // if (cond_type != BOOL_TYPE && cond_type != INT_TYPE) {
    //     // report error A condição do if/while deve ser booleana
    // }
    semantic_cmd(current_env, node->body);
    if (node->else_body) {
      semantic_cmd(current_env, node->else_body);
    }
    break;
  }
  case CMD_LEIA: {
    symbol_t *sym = symbol_search(current_env, node->id);
    if (sym == NULL) {
      // report error - Variavel não declarada
    } else if (sym->symbol_type != T_VAR && sym->symbol_type != T_PARAM) {
      // report error - não é uma variavel ou parametro
    }
    break;
  }
  case CMD_ESCREVA: {
    semantic_expr(current_env, node->expr);
    break;
  }
  case CMD_RETURN: {
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
      // report error - Identificador não encontrado
      return T_UNKNOWN;
    }

    if (sym->symbol_type == T_VAR || sym->symbol_type == T_PARAM) {
      return sym->data_type;
    } else if (sym->symbol_type == T_FUNCTION && node->expr_list != NULL) {
      int args_count = 0;
      expr_list_t *current_arg = node->expr_list;
      while (current_arg != NULL) {
        args_count++;
        current_arg = current_arg->next;
      }

      if (args_count != list_size(sym->params)) {
        // report error - Numero incorreto de parametros
        return sym->data_type;
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

  default:
    return T_UNKNOWN;
  }
}