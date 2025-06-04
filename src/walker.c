#include <stdio.h>

#include "../include/ast.h"
#include "../include/walker.h"

const char *enum_to_string(enum_string_map_t *map, int value) {
  for (int i = 0; map[i].name != NULL; i++) {
    if (map[i].value == value) {
      return map[i].name;
    }
  }
  return "UNKNOWN_ENUM";
}

void walk_expr(expr_t *expr) {
  if (!expr)
    return;

  switch (expr->kind) {
  case EXPR_INTEGER_LITERAL:
    printf("Integer: %d\n", expr->integer_literal);
    break;
  case EXPR_CHAR_LITERAL:
    printf("Char: %c\n", expr->char_literal);
    break;
  case EXPR_ID:
    printf("ID: %s\n", expr->id);
    break;
  case EXPR_ADD:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" + ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_SUB:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" - ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_MUL:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" * ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_DIV:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" / ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_EQ:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" == ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case EXPR:
    walk_expr(expr->left);
  default:
    /* printf("Expr tipo %s não tratado\n", enum_to_string(expr_map,
     * expr->kind)); */
    break;
  }
}

void walk_cmd(cmd_t *cmd) {
  if (!cmd)
    return;

  switch (cmd->kind) {
  case STMT_LEIA:
    printf("Leia variável: %s\n", cmd->id);
    break;
  case STMT_ESC:
    printf("Escreva expressão:\n");
    walk_expr(cmd->expr);
    break;
  case STMT_IF:
    printf("If:\nCondição:\n");
    walk_expr(cmd->expr);
    printf("Bloco:\n");
    walk_cmd(cmd->if_cmd.body);
    break;
  case STMT_IF_ELSE:
    printf("If-Else:\nCondição:\n");
    walk_expr(cmd->expr);
    printf("Bloco then:\n");
    walk_cmd(cmd->if_cmd.body);
    printf("Bloco else:\n");
    walk_cmd(cmd->if_cmd.else_body);
    break;
  case STMT_WHILE:
    printf("While:\nCondição:\n");
    walk_expr(cmd->expr);
    printf("Corpo:\n");
    walk_cmd(cmd->while_cmd.body);
    break;
  case STMT_RETURN:
    printf("Return expressão:\n");
    walk_expr(cmd->expr);
    break;
  case STMT_BLOCK:
    printf("Bloco de comandos:\n");
    walk_block(cmd->blk);
    break;
  case STMT_STR:
    printf("Escreva String:\n");
    printf("%s\n", cmd->id);
    break;
  default:
    /* printf("Comando tipo %s não tratado\n", */
    /* enum_to_string(stmt_map, cmd->kind)); */
    break;
  }
}

void walk_block(block_t *block) {
  if (!block)
    return;

  printf("Variáveis locais:\n");
  decl_varlist_t *var = block->var_list;
  while (var) {
    printf("- %s\n", var->id);
    var = var->next;
  }

  printf("Comandos:\n");
  cmd_list_t *cmds = block->cmd_list;
  while (cmds) {
    walk_cmd(cmds->cmd);
    cmds = cmds->next;
  }
}

void print_params(param_listcount_t *params) {

  if (params == NULL)
    return;

  param_listcount_t *paramlist = params;

  printf("Parâmetros: ");
  printf("%s\n", paramlist->id);

  print_params(params->next);
}

void print_decl_list(decl_var_t *declv) {
  if (declv == NULL)
    return;

  printf("%s, ", declv->id);

  print_decl_list(declv->next);
}

void walk_program(program_t *program) {
  if (!program)
    return;

  printf("=== Programa ===\n");

  decl_funcvar_t *decl = program->funcvar;
  while (decl) {
    if (decl->decl_var) {
      printf("Decl var global: %s, ", decl->id);
      print_decl_list(decl->decl_var);
      printf("\n");
    }

    if (decl->decl_func == NULL && decl->decl_var == NULL) {
      printf("Decl var global: %s \n", decl->id);
    }

    if (decl->decl_func) {
      printf("Decl função: %s\n", decl->id);
      print_params(decl->decl_func->params->param_count);
      walk_block(decl->decl_func->block);
    }

    decl = decl->next;
  }

  if (program->prog) {
    printf("\nBloco principal do programa:\n");
    walk_block(program->prog->block);
  }
}
