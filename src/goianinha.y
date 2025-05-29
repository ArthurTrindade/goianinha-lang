%{
#include <stdio.h>
#include <stdlib.h>

#include "../include/ast.h"
#include "../include/symbol_table.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern char *yytext;
extern int yylineno;

void yyerror(const char *s);

ast_t root;
%}

%union {
  int line;
  char *lexeme;
}

/* declarações de símbolos terminais */
%token IDENTIFIER STRING
%token NUMBER
%token PLUS MINUS COMMA SEMICOLON PROGRAMA LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE
%token RETORNE LEIA ESCREVA ENQUANTO SENAO ENTAO EXECUTE 
%token E OU SE EQUAL EQUAL_EQUAL BANG BANG_EQUAL
%token LESS LESS_EQUAL GREATER GREATER_EQUAL
%token STAR SLASH INT CAR NOVALINHA INTCONST CARCONST

/* declarações de símbolos não-terminal inicial */
%start Programa
%type DeclFuncVar DeclProg DeclVar DeclFunc ListaParametros ListaParametrosCont
%type Bloco ListaDeclVar Tipo ListaComando Comando Expr OrExpr AndExpr EqExpr DesigExpr
%type AddExpr MulExpr UnExpr PrimExpr ListExpr

/* regras gramaticais */
%%
Programa:
        DeclFuncVar DeclProg 
        ;

DeclFuncVar:
           Tipo IDENTIFIER DeclVar SEMICOLON DeclFuncVar 
           | Tipo IDENTIFIER DeclFunc DeclFuncVar 
           | /* vazio */
           ;

DeclProg:
        PROGRAMA Bloco
        ;

DeclVar:
       COMMA IDENTIFIER DeclVar 
       | /* vazio */
       ;

DeclFunc:
        LEFT_PAREN ListaParametros RIGHT_PAREN Bloco
        ;

ListaParametros:
               /* vazio */ 
               | ListaParametrosCont
               ;

ListaParametrosCont:
                   Tipo IDENTIFIER 
                   | Tipo IDENTIFIER COMMA ListaParametrosCont
                   ;

Bloco:
     LEFT_BRACE ListaDeclVar ListaComando RIGHT_BRACE
     ;

ListaDeclVar:
            /* vazio */ 
            | Tipo IDENTIFIER DeclVar SEMICOLON ListaDeclVar
            ;

Tipo:
    INT 
    | CAR
    ;

ListaComando:
            Comando 
            | Comando ListaComando
            ;

Comando:
       SEMICOLON 
       | Expr SEMICOLON
       | RETORNE Expr SEMICOLON
       | LEIA IDENTIFIER SEMICOLON
       | ESCREVA Expr SEMICOLON
       | ESCREVA STRING SEMICOLON
       | NOVALINHA SEMICOLON
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando SENAO Comando
       | ENQUANTO LEFT_PAREN Expr RIGHT_PAREN EXECUTE Comando
       | Bloco
       ;

Expr:
    OrExpr
    | IDENTIFIER EQUAL Expr
    ;

OrExpr:
      OrExpr OU AndExpr
      | AndExpr
      ;

AndExpr:
       AndExpr E EqExpr
       | EqExpr
       ;

EqExpr:
      EqExpr EQUAL_EQUAL DesigExpr
      | EqExpr BANG_EQUAL DesigExpr
      | DesigExpr
      ;

DesigExpr: 
         DesigExpr LESS AddExpr
         | DesigExpr GREATER AddExpr
         | DesigExpr GREATER_EQUAL AddExpr
         | DesigExpr LESS_EQUAL AddExpr
         | AddExpr
         ;

AddExpr:
       AddExpr PLUS MulExpr
       | AddExpr MINUS MulExpr
       | MulExpr
       ;

MulExpr:
       MulExpr STAR UnExpr
       | MulExpr SLASH UnExpr
       | UnExpr
       ;

UnExpr:
      MINUS PrimExpr
      | BANG PrimExpr
      | PrimExpr
      ;

PrimExpr:
        IDENTIFIER LEFT_PAREN ListExpr RIGHT_PAREN 
        | IDENTIFIER LEFT_PAREN RIGHT_PAREN
        | IDENTIFIER
        | CARCONST
        | INTCONST
        | LEFT_PAREN Expr RIGHT_PAREN 
        ;

ListExpr:
        Expr
        | ListExpr COMMA Expr
        ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe: %s, na linha '%d'\n", s, yylineno);
}

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
    
    printf("Teste primeiro lexema\n");
    // printf("%s\n", root->symbol->lexeme);
    ast_travel(root);

    return 0;
}

