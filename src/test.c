#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"
#include "../include/walker.h"

int main() {

  program_t *program = malloc(sizeof(program_t));
  program->funcvar = NULL;

  decl_prog_t *decl_prog = malloc(sizeof(decl_prog_t));
  program->prog = decl_prog;

  block_t *block = malloc(sizeof(block_t));
  decl_prog->block = block;

  decl_varlist_t *var_list1 = malloc(sizeof(decl_var_t));
  var_list1->id = strdup("x");
  var_list1->next = NULL;
  var_list1->type = TYPE_INTEGER;

  decl_varlist_t *var_list2 = malloc(sizeof(decl_varlist_t));
  var_list2->id = strdup("y");
  var_list2->type = TYPE_INTEGER;
  var_list2->next = NULL;
  var_list2->var = NULL;

  var_list1->next = var_list2;

  block->var_list = var_list1;

  expr_t *expr = malloc(sizeof(expr_t));
  expr->kind = EXPR_ADD;
  expr->id = NULL;
  expr->expr_list = NULL;
  expr->left = NULL;
  expr->right = NULL;
  expr->char_literal = 0;
  expr->integer_literal = 0;

  expr_t *expr1 = malloc(sizeof(expr_t));
  expr1->kind = EXPR_ID;
  expr1->id = strdup("x");
  expr1->expr_list = NULL;
  expr1->left = NULL;
  expr1->right = NULL;
  expr1->char_literal = 0;
  expr1->integer_literal = 0;

  expr_t *expr2 = malloc(sizeof(expr_t));
  expr2->kind = EXPR_ID;
  expr2->id = strdup("y");
  expr2->expr_list = NULL;
  expr2->left = NULL;
  expr2->right = NULL;
  expr2->char_literal = 0;
  expr2->integer_literal = 0;

  expr->left = expr1;
  expr->right = expr2;

  cmd_t *leia_cmd = malloc(sizeof(cmd_t));
  leia_cmd->kind = STMT_ESC;
  leia_cmd->id = strdup("x");
  leia_cmd->expr = expr;
  leia_cmd->block = NULL;
  leia_cmd->body = NULL;
  leia_cmd->else_body = NULL;

  cmd_list_t *cmd_list = malloc(sizeof(cmd_list_t));
  cmd_list->cmd = leia_cmd;
  cmd_list->next = NULL;

  block->cmd_list = cmd_list;

  walk_program(program);

  return 0;
}
