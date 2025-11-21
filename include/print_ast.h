#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "ast.h"

/* Ponto de entrada principal para imprimir a árvore completa */
void print_program(program_t *prog);

/* Funções auxiliares (caso queira debugar partes específicas) */
void print_expr(expr_t *expr);
void print_block(block_t *block);

#endif
