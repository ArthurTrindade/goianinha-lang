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

void print_expr(expr_t *expr) {
  if (!expr)
    return;

  switch (expr->kind) {
  case EXPR_INT:
    printf("Integer: %d\n", expr->integer_literal);
    break;
  case EXPR_CHAR:
    printf("Char: %c\n", *expr->char_literal);
    break;
  case EXPR_ID:
    printf("ID: %s\n", expr->id);
    print_expr(expr->left);
    break;
  case EXPR_ADD:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" + ");
    print_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_SUB:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" - ");
    print_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_MUL:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" * ");
    print_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_DIV:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" / ");
    print_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_EQUAL:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" == ");
    print_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_ASSIGN:
    printf("Expr: (\n");
    printf("%s\n", expr->id);
    printf(" = ");
    print_expr(expr->left);
    printf(")\n");
    break;

  case EXPR_GREATER:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" > ");
    print_expr(expr->right);
    printf(")\n");
    break;
  case EXPR_GREATER_EQUAL:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" >= ");
    print_expr(expr->right);
    printf(")\n");
    break;

  case EXPR_LESS:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" == ");
    print_expr(expr->right);
    printf(")\n");
    break;

  case EXPR_LESS_EQUAL:
    printf("Expr: (\n");
    print_expr(expr->left);
    printf(" == ");
    print_expr(expr->right);
    printf(")\n");
    break;
  default:
    /* printf("Expr tipo %s não tratado\n", enum_to_string(expr_map,
     * expr->kind)); */
    break;
  }
}

void print_cmd(cmd_t *cmd) {
  if (!cmd)
    return;
  switch (cmd->kind) {
  case CMD_EXPR:
    printf("linha: %d\n", cmd->line);
    print_expr(cmd->expr);
    break;
  case CMD_LEIA:
    printf("linha: %d\n", cmd->line);
    printf("Leia variável: %s\n", cmd->id);
    break;
  case CMD_ESCREVA:
    printf("linha: %d\n", cmd->line);
    printf("Escreva expressão:\n");
    print_expr(cmd->expr);
    break;
  case CMD_IF:
    printf("linha: %d\n", cmd->line);
    printf("If:\nCondição:\n");
    print_expr(cmd->expr);
    printf("Bloco:\n");
    print_cmd(cmd->body);
    break;
  case CMD_IF_ELSE:
    printf("linha: %d\n", cmd->line);
    printf("If-Else:\nCondição:\n");
    print_expr(cmd->expr);
    printf("Bloco then:\n");
    print_cmd(cmd->body);
    printf("Bloco else:\n");
    print_cmd(cmd->else_body);
    break;
  case CMD_WHILE:
    printf("linha: %d\n", cmd->line);
    printf("While:\nCondição:\n");
    print_expr(cmd->expr);
    printf("Corpo:\n");
    print_cmd(cmd->body);
    break;
  case CMD_RETORNE:
    printf("linha: %d\n", cmd->line);
    printf("Return expressão:\n");
    print_expr(cmd->expr);
    break;
  case CMD_BLOCK:
    printf("linha: %d\n", cmd->line);
    printf("Bloco de comandos:\n");
    print_block(cmd->blk);
    break;
  case CMD_STRING:
    printf("linha: %d\n", cmd->line);
    printf("Escreva String: %s\n", cmd->id);
  default:
    /* printf("Comando tipo %s não tratado\n", */
    /* enum_to_string(stmt_map, cmd->kind)); */
    break;
  }
}

void print_block(block_t *block) {
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
    print_cmd(cmds->cmd);
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

void print_program(program_t *program) {
  if (!program)
    return;

  printf("=== Programa ===\n");

  decl_funcvar_t *decl = program->funcvar;
  while (decl) {

    if (decl->decl_var == NULL && decl->decl_func == NULL) {
      printf("Decl var global: %s, Linha: %d\n", decl->id, decl->line);
    }

    if (decl->decl_var) {
      printf("Decl vars global: %s, ", decl->id);
      print_decl_list(decl->decl_var);
      printf("Linha: %d\n", decl->decl_var->line);
      printf("\n");
    }

    if (decl->decl_func) {
      printf("Decl função: %s\n", decl->id);
      printf("Parâmetros: ");
      print_params(decl->decl_func->params->param_count);
      printf("Linha: %d\n\n", decl->decl_func->params->line);
      print_block(decl->decl_func->block);
    }

    decl = decl->next;
  }

  if (program->prog) {
    printf("Bloco principal do programa:\n");
    print_block(program->prog->block);
  }
}
