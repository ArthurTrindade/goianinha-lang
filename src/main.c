#include <stdio.h>

#include "../include/ast.h"
#include "../include/print_ast.h"
#include "../include/semantic.h"
#include "../include/symbol_table.h"

#include "goianinha.tab.h"

extern FILE *yyin;
extern int yylineno;
extern char *yytext;
extern program_t *root;

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

  program_t *p = root;

  env_t e = env_new();

  semantic_program(root);
//   scope_t scope1 = symboltable_new();
//   env_add(e, scope1);

//   symbol_t *sym = symbol_var("x", T_INT, 1, 1);
//   symboltable_set(scope1, sym);

//   symbol_t s1 = {.lexeme = "x"};

//   symbol_t *s = symboltable_get(list_data(list_head(e)), &s1.lexeme);

//   symbol_print(s);

  env_free(e);

  return 0;
}