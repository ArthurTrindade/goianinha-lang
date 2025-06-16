#include <stdio.h>
#include <stdlib.h>

#include "goianinha.tab.h"

extern FILE *yyin;
extern int yylineno;
extern char *yytext;
extern int yylex(void);


int main(int argc, char *argv[]) {
  FILE *input = NULL;
  FILE *input2 = NULL;
  int token;

  printf("Analisador Léxico - Versão 1.0\n");
  printf("===============================\n");

  if (argc > 1) {
    input = fopen(argv[1], "r");
    input2 = fopen(argv[1], "r");
    if (!input) {
      fprintf(stderr, "Erro ao abrir arquivo '%s'\n", argv[1]);
      return 1;
    }
    yyin = input;
    printf("Analisando arquivo: %s\n", argv[1]);
  } else {
    printf("Lendo da entrada padrão. Pressione Ctrl+D para finalizar.\n");
  }

  printf("\nIniciando análise léxica...\n\n");

  /* Ler tokens até o fim do arquivo */
  while ((token = yylex()) != 0) {

    printf("Encontrado o lexema %-15s pertencente ao token de código %-3d "
           "linha %-3d\n",
           yytext, token, yylineno);
  }

  printf("\nAnálise léxica concluída com sucesso.\n");

  printf("\nIniciando Análise Sintática\n");
  yyin = input2;
  yylineno = 0;
  int res = yyparse();
  printf("Resultado análise sintática: %d\n", res);

  if (input) {
    fclose(input);
    fclose(input2);
  }

  return 0;
}
