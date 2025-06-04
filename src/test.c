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
  var_list1->type = TYPE_INT;

  decl_varlist_t *var_list2 = malloc(sizeof(decl_varlist_t));
  var_list2->id = strdup("y");
  var_list2->type = TYPE_INT;
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

  cmd_t *leia_cmd = ast_cmd_leia("x");

  expr_t expr_if1 = {.kind = EXPR_ID, .id = strdup("n")};
  expr_t expr_if2 = {.kind = EXPR_INTEGER_LITERAL, .integer_literal = 0};
  expr_t expr_if = {.kind = EXPR_EQ,
                    .id = NULL,
                    .integer_literal = 0,
                    .char_literal = 0,
                    .left = &expr_if1,
                    .right = &expr_if2};

  cmd_t *if_cmd = ast_cmd_if(&expr_if, NULL);

  cmd_list_t *cmd_list = malloc(sizeof(cmd_list_t));
  cmd_list->cmd = leia_cmd;
  cmd_list->next = NULL;

  cmd_list->cmd = if_cmd;

  block->cmd_list = cmd_list;

  walk_program(program);

  return 0;
}
