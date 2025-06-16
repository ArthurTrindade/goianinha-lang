%option noyywrap
%option yylineno
%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/ast.h"
#include "goianinha.tab.h"

struct keyword_map {
    const char* keyword;
    int token;
};

static struct keyword_map keywords[] = {
    {"se", SE},
    {"entao", ENTAO},
    {"senao", SENAO},
    {"enquanto", ENQUANTO},
    {"retorne", RETORNE},
    {"car", CAR},
    {"int", INT},
    {"leia", LEIA},
    {"escreva", ESCREVA},
    {"novalinha", NOVALINHA},
    {"execute", EXECUTE},
    {"programa", PROGRAMA},
    {NULL, 0} /* End marker */
};

int check_keyword(const char* id) {
    struct keyword_map* k;
    for (k = keywords; k->keyword != NULL; k++) {
        if (strcmp(id, k->keyword) == 0) {
            return k->token;
        }
    }
    return IDENTIFIER;
}
%}

ID      [a-zA-Z_][a-zA-Z0-9_]*
DIGIT   [0-9]+

%%
[ \t\n]+ { /* Ignorar espaços e quebras de linha */ }

"/*"([^*]|\*+[^/])*"*"+"/"     { /* Ignorar comentário válido */ }

\"([^\\\n"]|\\.)*\" { 
                        yylval.id = (char*)malloc(sizeof(yytext)+1);
                        if (yylval.id) {
                        strcpy(yylval.id, yytext);
                            return check_keyword(yytext);
                        }
                        return STRING; 
                    }

{ID}     { 
            yylval.id = (char*)malloc(sizeof(yytext)+1);
            if (yylval.id) {
                strcpy(yylval.id, yytext);
                return check_keyword(yytext);
            }

            printf("Erro de memória\n");
            exit(1);
          }

{DIGIT}  {
            yylval.id = (char*)malloc(sizeof(yytext)+1);
            if (yylval.id) {
                strcpy(yylval.id, yytext);
                return INTCONST;
            }
            printf("Erro de memória\n");
            exit(1);
         }

\'[^']\' { return CARCONST; }
"+"      { return PLUS; }
"-"      { return MINUS; }
"="      { return EQUAL; }
"*"      { return STAR; }
"/"      { return SLASH; }
"("      { return LEFT_PAREN; }
")"      { return RIGHT_PAREN; }
"{"      { return LEFT_BRACE; }
"}"      { return RIGHT_BRACE; }
";"      { return SEMICOLON; }
","      { return COMMA; }
"=="     { return EQUAL_EQUAL; }
"!="     { return BANG_EQUAL; }
"<"      { return LESS; }
">"      { return GREATER; }
">="     { return GREATER_EQUAL; }
"<="     { return LESS_EQUAL; }
"!"      { return BANG; }

\"[^\"]*\"     {
    fprintf(stderr, "Erro: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA %d\n", yylineno);
    exit(EXIT_FAILURE);
}

"/*"([^*]|\*+[^/])*(\*+)? {
    fprintf(stderr, "Erro: COMENTÁRIO NÃO TERMINADO na linha %d\n", yylineno);
    exit(EXIT_FAILURE);
}

.        { 
    fprintf(stderr, "ERRO: CARACTERE INVALIDO '%s' LINHA: %d\n", yytext, yylineno); 
    exit(EXIT_FAILURE);
}


%%