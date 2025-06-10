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
        DeclFuncVar DeclProg { root = ast_program($1, $2); }
        ;

DeclFuncVar:
           Tipo IDENTIFIER DeclVar SEMICOLON DeclFuncVar { $$ = ast_decl_funcvar($1, $2, $3, NULL, $5); }
           | Tipo IDENTIFIER DeclFunc DeclFuncVar { $$ = ast_decl_funcvar($1, $2, NULL, $3, $4); }
           | /* vazio */ { $$ = NULL; }
           ;

DeclProg:
        PROGRAMA Bloco { $$ = ast_decl_prog($2); }
        ;

DeclVar:
       COMMA IDENTIFIER DeclVar { $$ = ast_decl_var($2, $3); }
       | /* vazio */ { $$ = NULL; }
       ;

DeclFunc:
        LEFT_PAREN ListaParametros RIGHT_PAREN Bloco { $$ = ast_decl_func($2, $4); }
        ;

ListaParametros:
               /* vazio */ { $$ = NULL;}
               | ListaParametrosCont { $$ = ast_param_list($1); }
               ;

ListaParametrosCont:
                   Tipo IDENTIFIER { $$ = ast_param_listcount($1, $2, NULL); }
                   | Tipo IDENTIFIER COMMA ListaParametrosCont { $$ = ast_param_listcount($1, $2, $4); }
                   ;

Bloco:
     LEFT_BRACE ListaDeclVar ListaComando RIGHT_BRACE { $$ = ast_block($2, $3); }
     ;

ListaDeclVar:
            /* vazio */ { $$ = NULL; }
            | Tipo IDENTIFIER DeclVar SEMICOLON ListaDeclVar { $$ = ast_decl_varlist($1, $2, $3, $5); }
            ;

Tipo:
    INT { $$ = T_INT; }
    | CAR { $$ = T_CAR; }
    ;

ListaComando:
            Comando { $$ = ast_cmd_list($1, NULL); }
            | Comando ListaComando {  $$ = ast_cmd_list($1, $2); } 
            ;

Comando:
       SEMICOLON 
       | Expr SEMICOLON { $$ = ast_cmd_expr($1); }
       | RETORNE Expr SEMICOLON { $$ = ast_cmd_expr($2); } 
       | LEIA IDENTIFIER SEMICOLON { $$ = ast_cmd_leia($2); }
       | ESCREVA Expr SEMICOLON    { $$ = ast_cmd_escreva($2); }
       | ESCREVA STRING SEMICOLON  { $$ = ast_cmd_leia($2); }
       | NOVALINHA SEMICOLON    
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando { $$ = ast_cmd_if($3, $6); }
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando SENAO Comando { $$ = ast_cmd_if_else($3, $6, $8); }
       | ENQUANTO LEFT_PAREN Expr RIGHT_PAREN EXECUTE Comando { $$ = ast_cmd_while($3, $6); }
       | Bloco { $$ = ast_cmd_block($1); }
       ;

Expr:
    OrExpr  { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
    | IDENTIFIER EQUAL Expr  { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, $3, NULL, NULL); }
    ;

OrExpr:
      OrExpr OU AndExpr  { $$ = ast_expr(T_OR, NULL, 0, 0, $1, $3, NULL); }
      | AndExpr  { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
      ;

AndExpr:
       AndExpr E EqExpr { $$ = ast_expr(T_AND, NULL, 0, 0, $1, $3, NULL); }
       | EqExpr  { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
       ;

EqExpr:
      EqExpr EQUAL_EQUAL DesigExpr { $$ = ast_expr(T_EQUAL, NULL, 0, 0, $1, $3, NULL); }
      | EqExpr BANG_EQUAL DesigExpr { $$ = ast_expr(T_BANG_EQUAL, NULL, 0, 0, $1, $3, NULL); }
      | DesigExpr { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
      ;

DesigExpr: 
         DesigExpr LESS AddExpr { $$ = ast_expr(T_LESS, NULL, 0, 0, $1, $3, NULL); }
         | DesigExpr GREATER AddExpr { $$ = ast_expr(T_GREATER, NULL, 0, 0, $1, $3, NULL); }
         | DesigExpr GREATER_EQUAL AddExpr { $$ = ast_expr(T_GREATER_EQUAL, NULL, 0, 0, $1, $3, NULL); }
         | DesigExpr LESS_EQUAL AddExpr { $$ = ast_expr(T_LESS_EQUAL, NULL, 0, 0, $1, $3, NULL); }
         | AddExpr { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
         ;

AddExpr:
       AddExpr PLUS MulExpr { $$ = ast_expr(T_PLUS, NULL, 0, 0, $1, $3, NULL); }
       | AddExpr MINUS MulExpr { $$ = ast_expr(T_MINUS, NULL, 0, 0, $1, $3, NULL); }
       | MulExpr { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
       ;

MulExpr:
       MulExpr STAR UnExpr { $$ = ast_expr(T_STAR, NULL, 0, 0, $1, $3, NULL); }
       | MulExpr SLASH UnExpr { $$ = ast_expr(T_SLASH, NULL, 0, 0, $1, $3, NULL); }
       | UnExpr { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
       ;

UnExpr:
      MINUS PrimExpr { $$ = ast_expr(T_MINUS, NULL, 0, 0, $2, NULL, NULL); }
      | BANG PrimExpr { $$ = ast_expr(T_BANG, NULL, 0, 0, $2, NULL, NULL); }
      | PrimExpr { $$ = ast_expr(T_EXPR, NULL, 0, 0, $1, NULL, NULL); }
      ;

PrimExpr:
        IDENTIFIER LEFT_PAREN ListExpr RIGHT_PAREN { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, NULL, NULL, $3); }
        | IDENTIFIER LEFT_PAREN RIGHT_PAREN { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, NULL, NULL, NULL); }
        | IDENTIFIER { $$ = ast_expr(T_IDENTIFIER, $1, 0, 0, NULL, NULL, NULL); }
        | CARCONST   { $$ = ast_expr(T_CAR, NULL, 0, $1, NULL, NULL, NULL); }
        | INTCONST   { $$ = ast_expr(T_NUMBER, NULL, $1, 0, NULL, NULL, NULL); }
        | LEFT_PAREN Expr RIGHT_PAREN { $$ = ast_expr(T_EXPR, NULL, 0, 0, $2, NULL, NULL); }
        ;

ListExpr:
        Expr { $$ = ast_expr_list($1, NULL); }
        | ListExpr COMMA Expr { $$ = ast_expr_list($3, $1); }
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

