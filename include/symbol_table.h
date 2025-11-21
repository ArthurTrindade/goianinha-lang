#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>
#include <stdio.h>
#include "hashmap.h"
#include "list.h"
#include "types.h"


typedef enum {
    SYM_VAR,    /* Variável Local ou Global */
    SYM_PARAM,  /* Parâmetro de Função */
    SYM_FUNC    /* Função */
} symbol_kind_t;

/* Estrutura do Símbolo */
typedef struct symbol {
    char *id;             /* Nome (lexema) */
    symbol_kind_t kind;   /* VAR, PARAM ou FUNC */
    types_t data_type;    /* T_INT, T_CAR, T_VOID (Retorno ou Tipo da var) */
    int line;             /* Linha da declaração (para erros) */

    union {
        /* Dados exclusivos para Variáveis e Parâmetros */
        struct {
            int offset;       /* Posição na pilha (endereço relativo) */
            int scope_level;  /* Nível do escopo (0=global, 1=main, etc) */
        } var;

        /* Dados exclusivos para Funções */
        struct {
            list_t *params; /* Lista de parâmetros (tipos e nomes) */
            int num_params;      /* Contagem rápida de params */
        } func;
    } info;

} symbol_t;


typedef list_t *env_t;       /* A Pilha de Escopos (Stack of Hashmaps) */
typedef struct hashmap *scope_t;  /* Um Escopo individual (Hashmap) */


/* Cria um novo ambiente (Pilha vazia) */
env_t env_new(void);

/* Destrói o ambiente inteiro e todas as tabelas dentro */
void env_free(env_t env);

/* Entra num novo escopo (Push na pilha) */
void env_push_scope(env_t env);

/* Sai do escopo atual (Pop na pilha) */
void env_pop_scope(env_t env);


/* Cria um símbolo de VARIÁVEL ou PARÂMETRO.
   - kind: SYM_VAR ou SYM_PARAM
   - type: T_INT, T_CAR...
   - offset: Posição de memória (calculado pelo gerador de código)
*/
symbol_t *symbol_new_var(char *id, symbol_kind_t kind, types_t type, int offset, int line);

/* Cria um símbolo de FUNÇÃO.
   - return_type: Tipo de retorno
   - params: Lista de parâmetros (pode ser NULL se vazia)
*/
symbol_t *symbol_new_func(char *id, types_t return_type, list_t *params, int line);

/* Libera a memória de um símbolo único */
void symbol_free(void *s);

/* Imprime debug do símbolo */
void symbol_print(symbol_t *s);

/* Adiciona um símbolo ao escopo ATUAL (topo da pilha).
   Retorna 1 se sucesso, 0 se já existe (erro semântico de redeclaração).
*/
int symbol_add(env_t env, symbol_t *s);

/* Busca um símbolo em TODOS os escopos, do topo para a base.
   Retorna o símbolo ou NULL se não achar.
*/
symbol_t *symbol_search(env_t env, const char *key);

/* Busca um símbolo APENAS no escopo atual.
   Útil para verificar redeclaração de variáveis no mesmo bloco.
*/
symbol_t *symbol_search_current(env_t env, const char *key);

#endif
