%define parse.error verbose
%locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"
#include "../include/symbol_table.h"
#include "../include/print_ast.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern char *yytext;
extern int yylineno;

void yyerror(const char *s);

program_t *root;

%}

%union {
  int number;
  char c;
  int line;
  char *id;
  program_t *program;
  decl_funcvar_t *declfv;
  decl_prog_t *declp;
  decl_var_t *declvar;
  decl_func_t *declf;
  param_list_t *params;
  param_listcount_t *paramsl;
  block_t *blk;
  decl_varlist_t *declvl;
  cmd_list_t *cmdl;
  cmd_t *cmd;
  expr_t *expr;
  expr_list_t *exprl;
  types_t type;
}


/* declarações de símbolos terminais */
%token <id> IDENTIFIER STRING
%token <id> NUMBER
%token PLUS MINUS COMMA SEMICOLON PROGRAMA LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE
%token RETORNE LEIA ESCREVA ENQUANTO SENAO ENTAO EXECUTE 
%token E OU SE EQUAL EQUAL_EQUAL BANG BANG_EQUAL
%token LESS LESS_EQUAL GREATER GREATER_EQUAL
%token STAR SLASH INT CAR NOVALINHA 
%token <number> INTCONST 
%token <c> CARCONST

/* declarações de símbolos não-terminal inicial */
%start Programa
%type <declfv> DeclFuncVar 
%type <declp> DeclProg
%type <declvar> DeclVar 
%type <declf> DeclFunc 
%type <params> ListaParametros
%type <paramsl> ListaParametrosCont
%type <blk> Bloco 
%type <declvl> ListaDeclVar 
%type <type> Tipo 
%type <cmdl> ListaComando 
%type <cmd> Comando
%type <expr> Expr OrExpr AndExpr EqExpr DesigExpr AddExpr MulExpr UnExpr PrimExpr 
%type <exprl> ListExpr

/* regras gramaticais */
%%
Programa:
        DeclFuncVar DeclProg { root = ast_program($1, $2, yylineno); }
        ;

DeclFuncVar:
           Tipo IDENTIFIER DeclVar SEMICOLON DeclFuncVar { $$ = ast_decl_funcvar($1, $2, $3, NULL, $5, yylineno); }
           | Tipo IDENTIFIER DeclFunc DeclFuncVar { $$ = ast_decl_funcvar($1, $2, NULL, $3, $4, yylineno); }
           | /* vazio */ { $$ = NULL; }
           ;

DeclProg:
        PROGRAMA Bloco { $$ = ast_decl_prog($2, yylineno); }
        ;

DeclVar:
       COMMA IDENTIFIER DeclVar { $$ = ast_decl_var($2, $3, yylineno); }
       | /* vazio */ { $$ = NULL; }
       ;

DeclFunc:
        LEFT_PAREN ListaParametros RIGHT_PAREN Bloco { $$ = ast_decl_func($2, $4, yylineno); }
        ;

ListaParametros:
               /* vazio */ { $$ = NULL;}
               | ListaParametrosCont { $$ = ast_param_list($1, yylineno); }
               ;

ListaParametrosCont:
                   Tipo IDENTIFIER { $$ = ast_param_listcount($1, $2, NULL, yylineno); }
                   | Tipo IDENTIFIER COMMA ListaParametrosCont { $$ = ast_param_listcount($1, $2, $4, yylineno); }
                   ;

Bloco:
     LEFT_BRACE ListaDeclVar ListaComando RIGHT_BRACE { $$ = ast_block($2, $3, yylineno); }
     ;

ListaDeclVar:
            /* vazio */ { $$ = NULL; }
            | Tipo IDENTIFIER DeclVar SEMICOLON ListaDeclVar { $$ = ast_decl_varlist($1, $2, $3, $5, yylineno); }
            ;

Tipo:
    INT { $$ = T_INT; }
    | CAR { $$ = T_CAR; }
    ;

ListaComando:
            Comando { $$ = ast_cmd_list($1, NULL, yylineno); }
            | Comando ListaComando {  $$ = ast_cmd_list($1, $2, yylineno); } 
            ;

Comando:
       SEMICOLON 
       | Expr SEMICOLON { $$ = ast_cmd_expr($1, yylineno); }
       | RETORNE Expr SEMICOLON { $$ = ast_cmd_expr($2, yylineno); } 
       | LEIA IDENTIFIER SEMICOLON { $$ = ast_cmd_leia($2, yylineno); }
       | ESCREVA Expr SEMICOLON    { $$ = ast_cmd_escreva($2, yylineno); }
       | ESCREVA STRING SEMICOLON  { $$ = ast_cmd_leia($2, yylineno); }
       | NOVALINHA SEMICOLON    
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando { $$ = ast_cmd_if($3, $6, yylineno); }
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando SENAO Comando { $$ = ast_cmd_if_else($3, $6, $8, yylineno); }
       | ENQUANTO LEFT_PAREN Expr RIGHT_PAREN EXECUTE Comando { $$ = ast_cmd_while($3, $6, yylineno); }
       | Bloco { $$ = ast_cmd_block($1, yylineno); }
       ;

Expr:
    OrExpr  { $$ = $1; }
    | IDENTIFIER EQUAL Expr  { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, $3, NULL, NULL, yylineno); }
    ;

OrExpr:
      OrExpr OU AndExpr  { $$ = ast_expr(T_OR, NULL, 0, 0, $1, $3, NULL, yylineno); }
      | AndExpr  { $$ = $1; }
      ;

AndExpr:
       AndExpr E EqExpr { $$ = ast_expr(T_AND, NULL, 0, 0, $1, $3, NULL, yylineno); }
       | EqExpr  { $$ = $1; }
       ;

EqExpr:
      EqExpr EQUAL_EQUAL DesigExpr { $$ = ast_expr(T_EQUAL, NULL, 0, 0, $1, $3, NULL, yylineno); }
      | EqExpr BANG_EQUAL DesigExpr { $$ = ast_expr(T_BANG_EQUAL, NULL, 0, 0, $1, $3, NULL, yylineno); }
      | DesigExpr { $$ = $1; }
      ;

DesigExpr: 
         DesigExpr LESS AddExpr { $$ = ast_expr(T_LESS, NULL, 0, 0, $1, $3, NULL, yylineno); }
         | DesigExpr GREATER AddExpr { $$ = ast_expr(T_GREATER, NULL, 0, 0, $1, $3, NULL, yylineno); }
         | DesigExpr GREATER_EQUAL AddExpr { $$ = ast_expr(T_GREATER_EQUAL, NULL, 0, 0, $1, $3, NULL, yylineno); }
         | DesigExpr LESS_EQUAL AddExpr { $$ = ast_expr(T_LESS_EQUAL, NULL, 0, 0, $1, $3, NULL, yylineno); }
         | AddExpr { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL, yylineno); }
         ;

AddExpr:
       AddExpr PLUS MulExpr { $$ = ast_expr(T_PLUS, NULL, 0, 0, $1, $3, NULL, yylineno); }
       | AddExpr MINUS MulExpr { $$ = ast_expr(T_MINUS, NULL, 0, 0, $1, $3, NULL, yylineno); }
       | MulExpr { $$ = $1; }
       ;

MulExpr:
       MulExpr STAR UnExpr { $$ = ast_expr(T_STAR, NULL, 0, 0, $1, $3, NULL, yylineno); }
       | MulExpr SLASH UnExpr { $$ = ast_expr(T_SLASH, NULL, 0, 0, $1, $3, NULL, yylineno); }
       | UnExpr { $$ = $1; }
       ;

UnExpr:
      MINUS PrimExpr { $$ = ast_expr(T_MINUS, NULL, 0, 0, $2, NULL, NULL, yylineno); }
      | BANG PrimExpr { $$ = ast_expr(T_BANG, NULL, 0, 0, $2, NULL, NULL, yylineno); }
      | PrimExpr { $$ = $1; }
      ;

PrimExpr:
        IDENTIFIER LEFT_PAREN ListExpr RIGHT_PAREN { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, NULL, NULL, $3, yylineno); }
        | IDENTIFIER LEFT_PAREN RIGHT_PAREN { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, NULL, NULL, NULL, yylineno); }
        | IDENTIFIER { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, NULL, NULL, NULL, yylineno); }
        | CARCONST   { $$ = ast_expr(T_CAR, NULL, 0, $1, NULL, NULL, NULL, yylineno); }
        | INTCONST   { $$ = ast_expr(T_NUMBER, NULL, $1, 0, NULL, NULL, NULL, yylineno); }
        | LEFT_PAREN Expr RIGHT_PAREN { $$ = ast_expr(T_EXPR, NULL, 0, 0, $2, NULL, NULL, yylineno); }
        ;

ListExpr:
        Expr { $$ = ast_expr_list($1, NULL, yylineno); }
        | ListExpr COMMA Expr { $$ = ast_expr_list($3, $1, yylineno); }
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
    printf("\n");

    walk_program(root);

    return 0;
}