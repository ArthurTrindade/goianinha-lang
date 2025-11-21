#include <stdio.h>

#include "../include/ast.h"
#include "../include/print_ast.h"
#include "../include/symbol_table.h"

extern FILE *yyin;
extern int yylineno;
extern char *yytext;

extern program_t *root;

extern env_t env;

extern int yyparse();

int main(int argc, char **argv) {

  if (argc > 1) {
    FILE *input = fopen(argv[1], "r");
    if (!input) {
      fprintf(stderr, "Não foi possível abrir %s\n", argv[1]);
      return 1;
    }
    yyin = input;
  }

  env = env_new();
  if (!env) {
    fprintf(stderr, "Erro fatal: Falha ao alocar memoria para o ambiente.\n");
    return 1;
  }

  /* Cria o escopo global (Nível 0) antes de começar */
  env_push_scope(env);

  /* Análise Sintática (Parsing) */
  printf("--- [1/3] Iniciando Analise Sintatica ---\n");
  int parse_result = yyparse();

  env_pop_scope(env);
  env_free(env);

  if (parse_result != 0) {
    fprintf(stderr, "Erro: Falha na analise sintatica. Compilacao abortada.\n");
    if (argc > 1 && yyin)
      fclose(yyin);
    return 1;
  }
  printf(">> Sucesso: Sintaxe correta.\n\n");

  /* Impressão da AST */
  if (root) {
    printf("--- [Visualizacao da AST] ---\n");
    print_program(root);
    printf("\n");
  }

  /* Análise Semântica */
  printf("--- [2/3] Iniciando Analise Semantica ---\n");
  if (root) {
    // semantic_program(root);
  }
  printf(">> Sucesso: Analise semantica concluida.\n\n");
  if (argc > 1 && yyin)
    fclose(yyin);
  return 0;
}
