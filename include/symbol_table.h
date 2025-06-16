#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "hashmap.h"
#include <stdio.h>

#include "list.h"
#include "types.h"

typedef list_t *env_t;
typedef struct hashmap *scope_t;
typedef list_t *list_symbol_t;

typedef struct symbol_ {
  char *lexeme;
  types_t symbol_type;
  types_t data_type;
  int pos;
  int line;
  list_symbol_t list_symbol;
} symbol_t;

/*
 * Cria a pilha para os escopo do programa
 * */
env_t env_new();

/*
 * Libera a memória para a pillha de escopo
 * */
void env_free(env_t e);

/*
 * Adiciona um escopo na pilha
 * */
int env_add(env_t e, scope_t h);

/*
 * Deleta o escopo da pilha
 * */
int env_delete(env_t e);

/*
 * Compara dois simbolos
 * */
int symbol_compare(const void *s1, const void *s2, void *udata);

/*
 * Função de hash do símbolo
 * */
uint64_t symbol_hash(const void *item, uint64_t seed0, uint64_t seed1);

/*
 * Cria uma nova tabela de simbolo
 * returna o ponteiro para a tabela
 * */
scope_t symboltable_new();

void symbol_free(symbol_t *s);

/*
 * Adiciona o símbolo s na tabela de símbolos h
 * */
void symboltable_set(scope_t h, symbol_t *s);

/*
 * Procura na tabela o símbolo com a chave key
 * retorna o ponteiro para o símbolo
 * */
symbol_t *symboltable_get(scope_t h, const void *key);

/*
 * Procura o símbolo com a chave lexeme na pilha de
 * tabela de símbolos
 * */
symbol_t *symbol_search(env_t env, char *lexeme);

symbol_t *symbol_var(char *l, types_t dt, int pos, int line);

symbol_t *symbol_param(char *l, types_t dt, int pos, int line);

symbol_t *symbol_function(char *l, types_t return_type, int num_param, list_symbol_t list_symbol, int line);

/*
 * Imprime as informações do símbolo na saída padrão
 * */
void symbol_print(symbol_t *s);

#endif
