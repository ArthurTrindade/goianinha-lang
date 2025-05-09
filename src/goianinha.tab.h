/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_GOIANINHA_TAB_H_INCLUDED
# define YY_YY_SRC_GOIANINHA_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    STRING = 259,                  /* STRING  */
    NUMBER = 260,                  /* NUMBER  */
    PLUS = 261,                    /* PLUS  */
    MINUS = 262,                   /* MINUS  */
    COMMA = 263,                   /* COMMA  */
    SEMICOLON = 264,               /* SEMICOLON  */
    PROGRAMA = 265,                /* PROGRAMA  */
    LEFT_PAREN = 266,              /* LEFT_PAREN  */
    RIGHT_PAREN = 267,             /* RIGHT_PAREN  */
    LEFT_BRACE = 268,              /* LEFT_BRACE  */
    RIGHT_BRACE = 269,             /* RIGHT_BRACE  */
    RETORNE = 270,                 /* RETORNE  */
    LEIA = 271,                    /* LEIA  */
    ESCREVA = 272,                 /* ESCREVA  */
    ENQUANTO = 273,                /* ENQUANTO  */
    SENAO = 274,                   /* SENAO  */
    ENTAO = 275,                   /* ENTAO  */
    EXECUTE = 276,                 /* EXECUTE  */
    E = 277,                       /* E  */
    OU = 278,                      /* OU  */
    SE = 279,                      /* SE  */
    EQUAL = 280,                   /* EQUAL  */
    EQUAL_EQUAL = 281,             /* EQUAL_EQUAL  */
    BANG = 282,                    /* BANG  */
    BANG_EQUAL = 283,              /* BANG_EQUAL  */
    LESS = 284,                    /* LESS  */
    LESS_EQUAL = 285,              /* LESS_EQUAL  */
    GREATER = 286,                 /* GREATER  */
    GREATER_EQUAL = 287,           /* GREATER_EQUAL  */
    STAR = 288,                    /* STAR  */
    SLASH = 289,                   /* SLASH  */
    INT = 290,                     /* INT  */
    CAR = 291,                     /* CAR  */
    NOVALINHA = 292,               /* NOVALINHA  */
    INTCONST = 293,                /* INTCONST  */
    CARCONST = 294                 /* CARCONST  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_GOIANINHA_TAB_H_INCLUDED  */
