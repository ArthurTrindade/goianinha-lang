#ifndef AST_WALKER_H
#define AST_WALKER_H

#include "../include/ast.h"

typedef struct {
  int value;
  const char *name;
} enum_string_map_t;

// static enum_string_map_t stmt_map[] = {
//     {STMT_DECL, "STMT_DECL"},       {STMT_LEIA, "STMT_LEIA"},
//     {STMT_ESC, "STMT_ESC"},         {STMT_STR, "STMT_STR"},
//     {STMT_EXPR, "STMT_EXPR"},       {STMT_IF, "STMT_IF"},
//     {STMT_IF_ELSE, "STMT_IF_ELSE"}, {STMT_WHILE, "STMT_WHILE"},
//     {STMT_PRINT, "STMT_PRINT"},     {STMT_RETURN, "STMT_RETURN"},
//     {STMT_BLOCK, "STMT_BLOCK"},     {-1, NULL}};
//
// static enum_string_map_t expr_map[] = {
//     {EXPR_OR, "EXPR_OR"},
//     {EXPR_AND, "EXPR_AND"},
//     {EXPR_EQ, "EXPR_EQ"},
//     {EXPR_NEQ, "EXPR_NEQ"},
//     {EXPR_INQ, "EXPR_INQ"},
//     {EXPR_LT, "EXPR_LT"},
//     {EXPR_LEQ, "EXPR_LEQ"},
//     {EXPR_GT, "EXPR_GT"},
//     {EXPR_GEQ, "EXPR_GEQ"},
//     {EXPR_ADD, "EXPR_ADD"},
//     {EXPR_SUB, "EXPR_SUB"},
//     {EXPR_MUL, "EXPR_MUL"},
//     {EXPR_DIV, "EXPR_DIV"},
//     {EXPR_NOT, "EXPR_NOT"},
//     {EXPR_BANG, "EXPR_BANG"},
//     {EXPR_ID, "EXPR_ID"},
//     {EXPR, "EXPR"},
//     {EXPR_INTEGER_LITERAL, "EXPR_INTEGER_LITERAL"},
//     {EXPR_CHAR_LITERAL, "EXPR_CHAR_LITERAL"},
//     {-1, NULL}};
//
// static enum_string_map_t type_map[] = {{TYPE_INT, "TYPE_INTEGER"},
//                                        {TYPE_CHAR, "TYPE_CHAR"},
//                                        {TYPE_FUNCTION, "TYPE_FUNCTION"},
//                                        {-1, NULL}};

const char *enum_to_string(enum_string_map_t *map, int value);

void walk_program(program_t *program);

void walk_block(block_t *block);

void walk_cmd(cmd_t *cmd);

void walk_expr(expr_t *expr);

#endif
