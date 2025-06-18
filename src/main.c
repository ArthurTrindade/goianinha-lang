#include <stdio.h>

#include "../include/ast.h"
#include "../include/print_ast.h"

#include "goianinha.tab.h"

extern FILE *yyin;
extern int yylineno;
extern char *yytext;
extern program_t *root;

env_t current_env;
types_t current_function_return_type;
int semantic_errors = 0;

void semantic_init() {
  current_env = env_new();
  env_add(current_env, symboltable_new());
}

void semantic_end() { env_free(current_env); }

void semantic_program(program_t *node) {
  semantic_init();

  // Para declarações de funções e variaveis globais
  decl_funcvar_t *current_funcvar = node->funcvar;
  while (current_funcvar != NULL) {
    if (current_funcvar->decl_var != NULL) {
      if (symboltable_get(list_data(list_head(current_env)),
                          current_funcvar->id)) {
        // semantic_error(current_funcvar, "Variável global redefinida");
      } else {
        symbol_t *new_var_sym =
            symbol_var(current_funcvar->id, current_funcvar->type, -1,
                       current_funcvar->line);
        symboltable_set(list_data(list_head(current_env)), new_var_sym);
      }
    } else if (current_funcvar->decl_func != NULL) {
      if (symboltable_get(list_data(list_head(current_env)),
                          current_funcvar->id) != NULL) {
        // semantic_error(current_funcvar->line, "Função Redefinida.");
      } else {
        list_symbol_t func_params = list_symbol_new();
        param_listcount_t *param_node =
            current_funcvar->decl_func->params->param_count;
        while (param_node != NULL) {
          symbol_t *param_sym = symbol_param(param_node->id, param_node->type,
                                             -1, param_node->line);
          list_symbol_add(func_params, param_sym);
          param_node = param_node->next;
        }

        symbol_t *new_func_sym = symbol_function(
            current_funcvar->id, current_funcvar->type, list_size(func_params),
            func_params, current_funcvar->line);

        symboltable_set(list_data(list_head(current_env)), new_func_sym);

        // semantic_function(current_funcvar->decl_func, current_funcvar->type);
      }
    }
    current_funcvar = current_funcvar->next;
  }

  if (node->prog && node->prog->block) {
    // current_function_return_type
    // semantic_block(node->prog->block);
  }

  semantic_end();

  if (semantic_errors > 0) {
    fprintf(stderr, "Compilação falhou devido a %d erro(s) semântico(s).\n",
            semantic_errors);
  } else {
    fprintf(stdout, "Análise semântica concluída com sucesso.\n");
  }
}

void semantic_function(decl_func_t *node, types_t return_type) {
  env_add(current_env, symboltable_new());

  current_function_return_type = return_type;

  param_listcount_t *param_node = node->params->param_count;

  while (param_node != NULL) {
    if (symboltable_get(list_data(list_head(current_env)), param_node->id) !=
        NULL) {
      // report erros - Parametro redefinido
    } else {
      symbol_t *new_param_sym =
          symbol_param(param_node->id, param_node->type, -1, param_node->line);
      symboltable_set(list_data(list_head(current_env)), new_param_sym);
    }
    param_node = param_node->next;
  }

  // semantic_block(node->block);

  env_delete(current_env);
}

void semantic_block(block_t *node) {
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

void semantic_cmd(cmd_t *node) {
  if (node == NULL)
    return;
  switch (node->kind) {
  case CMD_EXPR:
    // semantic_expr(node->expr);
    break;
  case CMD_BLOCK:
    semantic_block(node->blk);
    break;
  case CMD_WHILE:
  case CMD_IF:
  case CMD_IF_ELSE: {
    // types_t conde_type = semantic_expr(node->expr);
    // if (cond_type != BOOL_TYPE && cond_type != INT_TYPE) {
    //     // report error A condição do if/while deve ser booleana
    // }
    semantic_cmd(node->body);
    if (node->else_body) {
      semantic_cmd(node->else_body);
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
    // semantic_expr(node->expr);
    break;
  }
  case CMD_RETURN: {
  }

  default:
    break;
  }
}

types_t semantic_expr(expr_t *node) {
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
        types_t type = semantic_expr(current_arg->expr);
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
  case EXPR_INTEGER_LITERAL:
    return T_INT;
  case EXPR_CHAR_LITERAL:
    return T_CAR;

  default:
    return T_UNKNOWN;
  }
}

int main(int argc, char **argv) {

  if (argc > 1) {
    FILE *input = fopen(argv[1], "r");
    if (!input) {
      fprintf(stderr, "Não foi possível abrir %s\n", argv[1]);
      return 1;
    }
    yyin = input;
  }

  printf("Teste código correto: ");
  int res = yyparse();
  printf("%d\n", res);
  printf("\n");

  print_program(root);

  return 0;
}