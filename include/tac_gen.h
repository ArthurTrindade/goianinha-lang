#ifndef TAC_GEN_H
#define TAC_GEN_H

#include "ast.h"
#include "tac.h"

/* Função principal: Recebe a AST e retorna a lista de instruções TAC */
tac_t* generate_code(program_t *program);

#endif
