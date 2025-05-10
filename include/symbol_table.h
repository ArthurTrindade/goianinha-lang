#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "hashmap.h"
#include <stdio.h>

#include "list.h"
#include "token.h"

typedef list_t *env_t;
typedef struct hashmap *scope_t;

typedef struct symbol_ {
  char *lexeme;
  token_t type;
} symbol_t;

env_t env_new();
void env_free(env_t e);
int env_add(env_t e, scope_t h);
int env_delete(env_t e);

int symbol_compare(const void *s1, const void *s2, void *udata);
uint64_t symbol_hash(const void *item, uint64_t seed0, uint64_t seed1);
scope_t symboltable_new();
void symboltable_set(scope_t h, symbol_t *s);
symbol_t *symboltable_get(scope_t h, const void *key);
symbol_t *symbol_search(env_t env, char *lexeme);
void symbol_print(symbol_t *s);

#endif
