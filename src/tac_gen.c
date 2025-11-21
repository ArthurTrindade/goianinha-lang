#include <stdlib.h>
#include <stdio.h>
#include "../include/tac_gen.h"

/* Retorna o nome da variável onde o resultado ficou --- */
char *get_last_res(tac_t *head) {
    if (!head) return NULL;
    tac_t *aux = head;
    while(aux->next != NULL) {
        aux = aux->next;
    }
    return aux->res;
}

/* Converte Tipo AST para Tipo TAC --- */
tac_op_type get_tac_op(expr_e kind) {
    switch (kind) {
        case EXPR_ADD: return TAC_ADD;
        case EXPR_SUB: return TAC_SUB;
        case EXPR_MUL: return TAC_MUL;
        case EXPR_DIV: return TAC_DIV;
        default: return TAC_SYMBOL;
    }
}

tac_t* gen_expr(expr_t *node);
tac_t* gen_cmd(cmd_t *node);
tac_t* gen_block(block_t *node);
