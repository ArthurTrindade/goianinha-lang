#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/symbol_table.h"
#include "../include/types.h"

env_t env_new() {
  env_t e = (env_t)malloc(sizeof(list_t));
  list_init(e, (void *)hashmap_free);
  return e;
}

void env_free(env_t e) { list_destroy(e); }

int env_add(env_t e, scope_t h) { return list_insert_begin(e, h); }

int env_delete(env_t e) { return list_remove_begin(e); }

scope_t symboltable_new() {
  return hashmap_new(sizeof(symbol_t), 0, 0, 0, symbol_hash, symbol_compare,
                     NULL, NULL);
}

void symboltable_set(scope_t h, symbol_t *s) { hashmap_set(h, s); }

symbol_t *symboltable_get(scope_t h, const void *key) {
  return (symbol_t *)hashmap_get(h, key);
}

symbol_t *symbol_new(char *lexeme, types_t token) {
  symbol_t *s = (symbol_t *)malloc(sizeof(symbol_t));
  if (s != NULL) {
    s->lexeme = lexeme;
    s->symbol_type = token;
  }
  return s;
}

void symbol_free(symbol_t *s) { free(s); }

int symbol_compare(const void *s1, const void *s2, void *udata) {
  symbol_t *us1 = (symbol_t *)s1;
  symbol_t *us2 = (symbol_t *)s2;
  return strcmp(us1->lexeme, us2->lexeme);
}

uint64_t symbol_hash(const void *item, uint64_t seed0, uint64_t seed1) {
  const symbol_t *symbol = item;
  return hashmap_sip(symbol->lexeme, strlen(symbol->lexeme), seed0, seed1);
}

symbol_t *symbol_search(env_t env, char *lexeme) {
  symbol_t *s = NULL;
  symbol_t s1 = {.lexeme = lexeme};
  node_t *aux;

  for (aux = list_head(env); aux != NULL; aux = aux->next) {
    s = symboltable_get(list_data(aux), &s1.lexeme);
    if (s != NULL)
      return s;
  }
  return s;
}

symbol_t *symbol_var(char *l, types_t dt, int pos, int line) {
  symbol_t *s = (symbol_t *)malloc(sizeof(symbol_t));

  if (s) {
    s->lexeme = l;
    s->symbol_type = T_VAR;
    s->data_type = dt;
    s->pos = pos;
    s->func = NULL;
    s->line = line;
  }

  return s;
}

symbol_t *symbol_param(char *l, types_t dt, int pos, scope_t func, int line) {
  symbol_t *s = (symbol_t *)malloc(sizeof(symbol_t));

  if (s) {
    s->lexeme = l;
    s->symbol_type = T_PARAM;
    s->data_type = dt;
    s->pos = pos;
    s->func = func;
    s->line = line;
  }

  return s;
}

symbol_t *symbol_function(char *l, types_t return_type, int num_param,
                          int line) {
  symbol_t *s = (symbol_t *)malloc(sizeof(symbol_t));

  if (s) {
    s->lexeme = l;
    s->symbol_type = T_PARAM;
    s->data_type = return_type;
    s->pos = num_param;
    s->func = NULL;
    s->line = line;
  }

  return s;
}

void symbol_print(symbol_t *s) {
  if (s == NULL) {
    printf("Símbolo nulo\n");
    return;
  }
  printf("Lexeme: %s, Type: %d\n", s->lexeme, s->symbol_type);
}

/**/
/* int main() { */
/**/
/*   env_t env = env_new(); */
/**/
/*   scope_t scope1 = symboltable_new(); */
/*   scope_t scope2 = symboltable_new(); */
/*   env_add(env, scope1); */
/*   env_add(env, scope2); */
/**/
/*   symbol_t s1 = {.lexeme = "y", .symbol_type = IDENTIFIER}; */
/*   symbol_t s2 = {.lexeme = "z", .symbol_type = IDENTIFIER}; */
/*   symbol_t s3 = {.lexeme = "soma", .symbol_type = NUMBER}; */
/*   symbol_t s4 = {.lexeme = "a", .symbol_type = STRING}; */
/*   symboltable_set(scope1, */
/*                   &(symbol_t){.lexeme = "x", .symbol_type = IDENTIFIER}); */
/*   symboltable_set(scope1, &s1); */
/*   symboltable_set(scope1, &s2); */
/**/
/*   symboltable_set(scope2, &s2); */
/*   symboltable_set(scope2, &s3); */
/*   symboltable_set(scope2, &s4); */
/**/
/*   symbol_t *symbol = NULL; */
/*   symbol_t *symbol2 = NULL; */
/**/
/*   printf("Scope 1\n"); */
/*   symbol2 = symbol_search(env, "z"); */
/*   symbol_print(symbol2); */
/**/
/*   printf("Scope 2\n"); */
/*   symbol2 = symbol_search(env, "a"); */
/*   symbol_print(symbol2); */
/**/
/*   symbol2 = symbol_search(env, "x"); */
/*   symbol_print(symbol2); */
/**/
/*   symbol2 = symbol_search(env, "soma"); */
/*   symbol_print(symbol2); */
/**/
/*   env_free(env); */
/* } */
