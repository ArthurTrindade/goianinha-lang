/*
 * token_types.h
 * Definição dos tipos de tokens para o analisador léxico
 */

#ifndef TYPES_H
#define TYPES_H

/*
 * Esta enumeração define todos os tipos de tokens reconhecidos
 * pelo analisador léxico. Os valores são usados tanto pelo scanner
 * quanto pelo parser.
 */
typedef enum types_ {

  /* Identificadores e literais */
  T_IDENTIFIER = 258, /* Identificador (nome de variável/função) */
  T_NUMBER,           /* Literal numérico */
  T_STRING,           /* Literal string (entre aspas) */

  /* Tipos */
  T_VAR,
  T_FUNCTION,
  T_PARAM,

  /* Operadores */
  T_PLUS,          /* + */
  T_MINUS,         /* - */
  T_STAR,          /* * */
  T_SLASH,         /* / */
  T_EQUAL,         /* = */
  T_EQUAL_EQUAL,   /* == */
  T_BANG,          /* ! */
  T_BANG_EQUAL,    /* != */
  T_LESS,          /* < */
  T_LESS_EQUAL,    /* <= */
  T_GREATER,       /* > */
  T_GREATER_EQUAL, /* >= */

  /* Delimitadores */
  T_LEFT_PAREN,  /* ( */
  T_RIGHT_PAREN, /* ) */
  T_LEFT_BRACE,  /* { */
  T_RIGHT_BRACE, /* } */
  T_SEMICOLON,   /* ; */
  T_COMMA,       /* , */

  /* Palavras-chave */
  T_SE,        /* se */
  T_ENTAO,     /* entao */
  T_SENAO,     /* senao */
  T_SE_ENTAO,
  T_ENQUANTO,  /* enquanto */
  T_RETORNE,   /* retorne */
  T_CAR,       /* car */
  T_INT,       /* int */
  T_LEIA,      /* leia */
  T_ESCREVA,   /* escreva */
  T_NOVALINHA, /* novalinha */
  T_EXECUTE,   /* execute */
  T_PROGRAMA,  /* programa */

  T_EXPR,
  T_BLOCK,
  T_AND,
  T_OR

} types_t;

#endif /* TYPES_H */
