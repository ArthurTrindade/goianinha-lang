#include "../include/ast.h"
#include <stdio.h>
#include <stdlib.h>

ast_t ast_set(symbol_t *s, ast_t l, ast_t r) {

  ast_t a = (ast_t)malloc(sizeof(astnode_t));

  if (a) {
    a->symbol = s;
    a->left = l;
    a->right = r;
  }

  return a;
}

void ast_travel(ast_t root) {
  printf("Chegou na ast travel\n");
  if (root) {
    /*TODO*/
    printf("%s\n", root->symbol->lexeme);
    ast_travel(root->left);
    ast_travel(root->right);
  }
}
