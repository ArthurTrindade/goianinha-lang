/*
 * token_types.h
 * Definição dos tipos de tokens para o analisador léxico
 */

#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

/*
 * Esta enumeração define todos os tipos de tokens reconhecidos
 * pelo analisador léxico. Os valores são usados tanto pelo scanner
 * quanto pelo parser.
 */
typedef enum token_ {

  /* Identificadores e literais */
  IDENTIFIER = 258, /* Identificador (nome de variável/função) */
  NUMBER,           /* Literal numérico */
  STRING,           /* Literal string (entre aspas) */

  /* Operadores */
  PLUS,          /* + */
  MINUS,         /* - */
  STAR,          /* * */
  SLASH,         /* / */
  EQUAL,         /* = */
  EQUAL_EQUAL,   /* == */
  BANG,          /* ! */
  BANG_EQUAL,    /* != */
  LESS,          /* < */
  LESS_EQUAL,    /* <= */
  GREATER,       /* > */
  GREATER_EQUAL, /* >= */

  /* Delimitadores */
  LEFT_PAREN,  /* ( */
  RIGHT_PAREN, /* ) */
  LEFT_BRACE,  /* { */
  RIGHT_BRACE, /* } */
  SEMICOLON,   /* ; */
  COMMA,       /* , */

  /* Palavras-chave */
  SE,        /* se */
  ENTAO,     /* entao */
  SENAO,     /* senao */
  ENQUANTO,  /* enquanto */
  RETORNE,   /* retorne */
  CAR,       /* car */
  INT,       /* int */
  LEIA,      /* leia */
  ESCREVA,   /* escreva */
  NOVALINHA, /* novalinha */
  EXECUTE,   /* execute */
  PROGRAMA,  /* programa */

} token_t;

#endif /* TOKEN_TYPES_H */
