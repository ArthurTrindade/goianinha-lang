/*
 * types.h
 * Definições de enums para os tipos de nós da AST e tipos de dados da
 * linguagem.
 */

#ifndef TYPES_H
#define TYPES_H

/* * 1. Tipos de Comandos (Statements)
 * Define o que um nó de comando está fazendo.
 */
typedef enum {
  CMD_ASSIGN,  /* Atribuição (x = 10) */
  CMD_IF,      /* Se */
  CMD_WHILE,   /* Enquanto */
  CMD_RETURN,  /* Retorne */
  CMD_BLOCK,   /* Bloco de código { ... } */
  CMD_LEIA,    /* Comando de entrada */
  CMD_ESCREVA, /* Comando de saída */
  CMD_EXPR,    /* Expressão usada como comando (ex: chamada de função) */
  CMD_EMPTY    /* ; (comando vazio) */
} cmd_e;

/* * 2. Tipos de Expressões
 * Define a operação ou o valor contido no nó.
 */
typedef enum {
  /* Literais e Terminais */
  EXPR_LIT_INT,    /* 10, 20, -5 */
  EXPR_LIT_STRING, /* "texto", ou identificador de variável 'x' */
  EXPR_CALL,       /* Chamada de função: soma(1, 2) */

  /* Operações Aritméticas */
  EXPR_ADD, /* + */
  EXPR_SUB, /* - */
  EXPR_MUL, /* * */
  EXPR_DIV, /* / */

  /* Operações Lógicas e Relacionais */
  EXPR_AND,          /* E */
  EXPR_OR,           /* OU */
  EXPR_NOT,          /* ! */
  EXPR_EQUAL,        /* == */
  EXPR_DIFF,         /* != */
  EXPR_LESS,         /* < */
  EXPR_GREATER,      /* > */
  EXPR_LESS_EQUAL,   /* <= */
  EXPR_GREATER_EQUAL /* >= */
} expr_e;

/* * 3. Tipos de Dados da Linguagem (Data Types)
 * Usado para definir o tipo de variáveis e retornos de função.
 * Removemos todos os TOKENS (T_PLUS, T_WHILE, etc), pois o Bison cuida deles.
 */
typedef enum {
  T_INT,    /* Tipo inteiro */
  T_CAR,    /* Tipo caractere */
  T_STRING, /* Tipo string (se sua linguagem suportar como tipo) */
  T_VOID,   /* Retorno vazio (se houver) */
  T_ERROR   /* Usado para verificação de tipos (Type Checking) */
} types_t;

#endif /* TYPES_H */
