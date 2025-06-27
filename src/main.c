#include <stdio.h>

#include "../include/ast.h"
// #include "../include/print_ast.h"
#include "../include/semantic.h"


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

  semantic_program(root);

  return 0;
}