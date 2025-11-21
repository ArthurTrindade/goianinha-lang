
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"
#include "../include/symbol_table.h"
#include "../include/semantic.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern char *yytext;
extern int yylineno;

void yyerror(const char *s);

env_t *env;
program_t *root;

%}

%union {
  int i_val;
  char *s_val;
  program_t *prog;
  global_decl_t *global;
  func_decl_t *func;
  var_decl_t *var_decl;
  param_t *param;
  block_t *block;
  cmd_t *cmd;
  expr_t *expr;
  types_t type;
}

/* declarações de símbolos terminais */
%token <s_val> IDENTIFIER STRING
%token <i_val> INTCONST CARCONST

%token PLUS MINUS COMMA SEMICOLON PROGRAMA LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE
%token RETORNE LEIA ESCREVA ENQUANTO SENAO ENTAO EXECUTE
%token E OU SE EQUAL EQUAL_EQUAL BANG BANG_EQUAL
%token LESS LESS_EQUAL GREATER GREATER_EQUAL
%token STAR SLASH INT CAR NOVALINHA

/* declarações de símbolos não-terminal inicial */
%start Programa
%type <global> DeclFuncVar
%type <block> DeclProg Bloco
%type <var_decl> DeclVar ListaDeclVar
%type <func> DeclFunc
%type <param> ListaParametros ListaParametrosCont
%type <type> Tipo
%type <cmd> ListaComando Comando
%type <expr> Expr OrExpr AndExpr EqExpr DesigExpr AddExpr MulExpr UnExpr PrimExpr ListExpr

/* regras gramaticais */
%%
Programa:
        DeclFuncVar DeclProg { root = ast_program($1, $2, yylineno); }
        ;

DeclFuncVar:
           DeclFuncVar Tipo IDENTIFIER DeclVar SEMICOLON {
                var_decl_t *v = ast_decl_var($2, $3, yylineno);
                v->next = $4;

                global_decl_t *g = ast_global_var(v, yylineno);

                if ($1 != NULL) {
                    global_decl_t *aux = $1;
                    while (aux->next != NULL) {
                        aux = aux->next;
                    }
                    aux->next = g;
                    $$ = $1;
                } else {
                    $$ = g;
                }
            }
           | DeclFuncVar Tipo IDENTIFIER DeclFunc {
                $4->ret_type = $2;
                $4->id = $3;

                global_decl_t *g = ast_global_func($4, yylineno);

                if ($1 != NULL) {
                    global_decl_t *aux = $1;
                    while (aux->next != NULL) {
                        aux = aux->next;
                    }
                    aux->next = g;
                    $$ = $1;
                } else {
                    $$ = g;
                }

            }
           | /* vazio */ { $$ = NULL; }
           ;

DeclProg:
        PROGRAMA Bloco { $$ = $2; }
        ;

DeclVar:
       COMMA IDENTIFIER DeclVar {
            $$ = ast_decl_var(T_INT, $2, yylineno);
            $$->next = $3;

        }
       | /* vazio */ { $$ = NULL; }
       ;

DeclFunc:
        LEFT_PAREN ListaParametros RIGHT_PAREN Bloco {
             $$ = ast_decl_func(T_INT, NULL, $2, $4, yylineno);
        }
        ;

ListaParametros:
               /* vazio */ { $$ = NULL;}
               | ListaParametrosCont { $$ = $1; }
               ;

ListaParametrosCont:
                   Tipo IDENTIFIER {
                        $$ = ast_param($1, $2, yylineno);
                    }
                   | ListaParametrosCont COMMA Tipo IDENTIFIER {
                        param_t *p = ast_param($3, $4, yylineno);
                        param_t *aux = $1;
                        while (aux->next != NULL) {
                            aux = aux->next;
                        }
                        aux->next = p;
                        $$ = $1;
                    }
                   ;

Bloco:
     LEFT_BRACE ListaDeclVar ListaComando RIGHT_BRACE {
        $$ = ast_block($2, $3, yylineno);
    }
     ;

ListaDeclVar:
            /* vazio */ { $$ = NULL; }
            | ListaDeclVar Tipo IDENTIFIER DeclVar SEMICOLON {
                var_decl_t *v = ast_decl_var($2, $3, yylineno);
                var_decl_t *rest = $4;
                while (rest != NULL) {
                    rest->type = $2;
                    rest = rest->next;
                }
                v->next = $4;

                if ($1 != NULL) {
                    var_decl_t *aux = $1;
                    while (aux->next != NULL) {
                        aux = aux->next;
                    }
                    aux->next = v;
                    $$ = $1;
                } else {
                    $$ = v;
                }
            }
            ;

Tipo:
    INT { $$ = T_INT; }
    | CAR { $$ = T_CAR; }
    ;

ListaComando:
            Comando { $$ = $1; }
            | ListaComando Comando {
                ast_append_cmd($1, $2);
                $$ = $1;
             }
            ;

Comando:
       SEMICOLON { $$ = NULL; }
       | Expr SEMICOLON { $$ = ast_cmd_assign(NULL, $1, yylineno); }
       | RETORNE Expr SEMICOLON { $$ = ast_cmd_ret($2, yylineno); }
       | LEIA IDENTIFIER SEMICOLON { $$ = ast_cmd_leia($2, yylineno); }
       | ESCREVA Expr SEMICOLON    { $$ = ast_cmd_escreva($2, yylineno); }
       | ESCREVA STRING SEMICOLON  {
            expr_t *s = ast_expr_literal_string($2, yylineno);
            $$ = ast_cmd_escreva(s, yylineno);
        }
       | NOVALINHA SEMICOLON {
            expr_t *nl = ast_expr_literal_string("\n", yylineno);
            $$ = ast_cmd_escreva(nl, yylineno);
       }
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando { $$ = ast_cmd_if($3, $6, NULL, yylineno); }
       | SE LEFT_PAREN Expr RIGHT_PAREN ENTAO Comando SENAO Comando { $$ = ast_cmd_if($3, $6, $8, yylineno); }
       | ENQUANTO LEFT_PAREN Expr RIGHT_PAREN EXECUTE Comando { $$ = ast_cmd_while($3, $6, yylineno); }
       | Bloco { $$ = ast_cmd_block($1, yylineno); }
       ;

Expr:
    OrExpr  { $$ = $1; }
    | IDENTIFIER EQUAL Expr  { $$ = ast_expr_assign($1, $3, yylineno); }
    ;

OrExpr:
      OrExpr OU AndExpr  { $$ = ast_expr_binary(EXPR_OR, $1, $3, yylineno); }
      | AndExpr  { $$ = $1; }
      ;

AndExpr:
       AndExpr E EqExpr { $$ = ast_expr_binary(EXPR_AND, $1, $3, yylineno); }
       | EqExpr  { $$ = $1; }
       ;

EqExpr:
      EqExpr EQUAL_EQUAL DesigExpr { $$ = ast_expr_binary(EXPR_EQUAL, $1, $3, yylineno); }
      | EqExpr BANG_EQUAL DesigExpr { $$ = ast_expr_binary(EXPR_DIFF, $1, $3, yylineno); }
      | DesigExpr { $$ = $1; }
      ;

DesigExpr:
         DesigExpr LESS AddExpr {  $$ = ast_expr_binary(EXPR_LESS, $1, $3, yylineno); }
         | DesigExpr GREATER AddExpr { $$ = ast_expr_binary(EXPR_GREATER, $1, $3, yylineno); }
         | DesigExpr GREATER_EQUAL AddExpr { $$ = ast_expr_binary(EXPR_GREATER_EQUAL, $1, $3, yylineno); }
         | DesigExpr LESS_EQUAL AddExpr { $$ = ast_expr_binary(EXPR_LESS_EQUAL, $1, $3, yylineno); }
         | AddExpr { $$ = $1; }
         ;

AddExpr:
       AddExpr PLUS MulExpr { $$ = ast_expr_binary(EXPR_ADD, $1, $3, yylineno); }
       | AddExpr MINUS MulExpr { $$ = ast_expr_binary(EXPR_SUB, $1, $3, yylineno); }
       | MulExpr { $$ = $1; }
       ;

MulExpr:
       MulExpr STAR UnExpr { $$ = ast_expr_binary(EXPR_MUL, $1, $3, yylineno); }
       | MulExpr SLASH UnExpr { $$ = ast_expr_binary(EXPR_DIV, $1, $3, yylineno); }
       | UnExpr { $$ = $1; }
       ;

UnExpr:
      MINUS PrimExpr {
             $$ = ast_expr_binary(EXPR_SUB, ast_expr_literal_int(0, yylineno), $2, yylineno);
        }
      | BANG PrimExpr {
            $$ = ast_expr_call("!", $2, yylineno);
       }
      | PrimExpr { $$ = $1; }
      ;

PrimExpr:
        IDENTIFIER LEFT_PAREN ListExpr RIGHT_PAREN { $$ = ast_expr_call($1, $3, yylineno); }
        | IDENTIFIER LEFT_PAREN RIGHT_PAREN { $$ = ast_expr_call($1, NULL, yylineno); }
        | IDENTIFIER { $$ = ast_expr_literal_string($1, yylineno); }
        | CARCONST   { $$ = ast_expr_literal_int($1, yylineno); }
        | INTCONST   { $$ = ast_expr_literal_int($1, yylineno); }
        | LEFT_PAREN Expr RIGHT_PAREN { $$ = $2; }
        ;

ListExpr:
        Expr { $$ = $1; }
        | ListExpr COMMA Expr {
            expr_t *aux = $1;
            while (aux->next != NULL) {
                aux = aux->next;
            }
            aux->next = $3;
            $$ = $1;
         }
        ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe: %s, na linha '%d'\n", s, yylineno);
}
