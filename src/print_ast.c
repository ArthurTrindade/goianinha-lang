#include <stdio.h>

#include "../include/ast.h"
#include "../include/print_ast.h"

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
  case T_NUMBER:
    printf("Integer: %d\n", expr->integer_literal);
    break;
  case T_CAR:
    printf("Char: %c\n", expr->char_literal);
    break;
  case T_IDENTIFIER:
    printf("ID: %s\n", expr->id);
    walk_expr(expr->left);
    break;
  case T_PLUS:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" + ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case T_MINUS:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" - ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case T_STAR:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" * ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case T_SLASH:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" / ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case T_EQUAL_EQUAL:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" == ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case T_EQUAL:
    printf("Expr: (\n");
    printf("%s\n", expr->id);
    printf(" = ");
    walk_expr(expr->left);
    printf(")\n");
    break;

  case T_GREATER:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" > ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case T_GREATER_EQUAL:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" >= ");
    walk_expr(expr->right);
    printf(")\n");
    break;

  case T_LESS:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" == ");
    walk_expr(expr->right);
    printf(")\n");
    break;

  case T_LESS_EQUAL:
    printf("Expr: (\n");
    walk_expr(expr->left);
    printf(" == ");
    walk_expr(expr->right);
    printf(")\n");
    break;
  case T_EXPR:
    walk_expr(expr->left);
    break;
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
  case T_LEIA:
    printf("linha: %d\n", cmd->line);
    printf("Leia variável: %s\n", cmd->id);
    break;
  case T_ESCREVA:
    printf("linha: %d\n", cmd->line);
    printf("Escreva expressão:\n");
    walk_expr(cmd->expr);
    break;
  case T_SE:
    printf("linha: %d\n", cmd->line);
    printf("If:\nCondição:\n");
    walk_expr(cmd->expr);
    printf("Bloco:\n");
    walk_cmd(cmd->body);
    break;
  case T_SE_ENTAO:
    printf("linha: %d\n", cmd->line);
    printf("If-Else:\nCondição:\n");
    walk_expr(cmd->expr);
    printf("Bloco then:\n");
    walk_cmd(cmd->body);
    printf("Bloco else:\n");
    walk_cmd(cmd->else_body);
    break;
  case T_ENQUANTO:
    printf("linha: %d\n", cmd->line);
    printf("While:\nCondição:\n");
    walk_expr(cmd->expr);
    printf("Corpo:\n");
    walk_cmd(cmd->body);
    break;
  case T_RETORNE:
    printf("linha: %d\n", cmd->line);
    printf("Return expressão:\n");
    walk_expr(cmd->expr);
    break;
  case T_BLOCK:
    printf("linha: %d\n", cmd->line);
    printf("Bloco de comandos:\n");
    walk_block(cmd->blk);
    break;
  case T_STRING:
    printf("linha: %d\n", cmd->line);
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
  decl_varlist_t *var_list = block->var_list;
  while (var_list) {

    if (var_list->var) {
      printf("- %s, ", var_list->id);
      print_decl_list(var_list->var);
      printf("Linha: %d\n", var_list->var->line);
    }

    if (var_list->var == NULL) {
      printf("- %s\n", var_list->id);
    }

    var_list = var_list->next;
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

  printf("%s, ", paramlist->id);

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
      printf("Decl vars global: %s, ", decl->id);
      print_decl_list(decl->decl_var);
      printf("Linha: %d\n", decl->decl_var->line);
      printf("\n");
    }

    if (decl->decl_func == NULL && decl->decl_var == NULL) {
      printf("Decl var global: %s, Linha: %d\n", decl->id, decl->line);
    }

    if (decl->decl_func) {
      printf("Decl função: %s\n", decl->id);
      printf("Parâmetros: ");
      print_params(decl->decl_func->params->param_count);
      printf("Linha: %d\n\n", decl->decl_func->params->line);
      walk_block(decl->decl_func->block);
    }

    decl = decl->next;
  }

  if (program->prog) {
    printf("Bloco principal do programa:\n");
    walk_block(program->prog->block);
  }
}
