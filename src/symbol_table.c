#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/symbol_table.h"

/* Compara dois símbolos pelo ID (Lexema) */
int symbol_compare(const void *s1, const void *s2, void *udata) {
  const symbol_t *sym1 = (const symbol_t *)s1;
  const symbol_t *sym2 = (const symbol_t *)s2;
  return strcmp(sym1->id, sym2->id);
}

/* Gera o Hash baseado no ID do símbolo */
uint64_t symbol_hash(const void *item, uint64_t seed0, uint64_t seed1) {
  const symbol_t *sym = (const symbol_t *)item;
  return hashmap_sip(sym->id, strlen(sym->id), seed0, seed1);
}

/* Libera a memória de um único símbolo */
void symbol_free(void *item) {
  symbol_t *s = (symbol_t *)item;
  if (!s)
    return;

  if (s->id)
    free(s->id);

  if (s->kind == SYM_FUNC && s->info.func.params) {
    list_destroy(s->info.func.params);
  }

}

env_t env_new(void) {
  env_t env = (env_t)malloc(sizeof(list_t));
  list_init(env, (void *)hashmap_free);
  return env;
}

void env_free(env_t env) {
  if (env) {
    list_destroy(env);
    free(env);
  }
}

void env_push_scope(env_t env) {
  struct hashmap *scope = hashmap_new(sizeof(symbol_t), 0, 0, 0, symbol_hash,
                                      symbol_compare, symbol_free, NULL);

  list_insert_begin(env, scope);
}

void env_pop_scope(env_t env) { list_remove_begin(env); }

symbol_t *symbol_new_var(char *id, symbol_kind_t kind, types_t type, int offset,
                         int line) {
  symbol_t *s = (symbol_t *)malloc(sizeof(symbol_t));
  if (s) {
    s->id = id ? strdup(id) : NULL;
    s->kind = kind;
    s->data_type = type;
    s->line = line;

    s->info.var.offset = offset;
    s->info.var.scope_level = 0;
  }
  return s;
}

symbol_t *symbol_new_func(char *id, types_t return_type, list_t *params,
                          int line) {
  symbol_t *s = (symbol_t *)malloc(sizeof(symbol_t));
  if (s) {
    s->id = id ? strdup(id) : NULL;
    s->kind = SYM_FUNC;
    s->data_type = return_type;
    s->line = line;

    s->info.func.params = params;
    s->info.func.num_params = params ? params->size : 0;
  }
  return s;
}

int symbol_add(env_t env, symbol_t *s) {
  node_t *head = list_head(env);
  if (!head)
    return 0; // Erro: sem escopo

  struct hashmap *scope = (struct hashmap *)list_data(head);

  if (hashmap_get(scope, s) != NULL) {
    return 0;
  }

  hashmap_set(scope, s);
  return 1;
}

symbol_t *symbol_search(env_t env, const char *key) {
  symbol_t search_key;
  search_key.id = (char *)key;

  node_t *current_node = list_head(env);
  while (current_node != NULL) {
    struct hashmap *scope = (struct hashmap *)list_data(current_node);
    symbol_t *found = (symbol_t *)hashmap_get(scope, &search_key);
    if (found) {
      return found;
    }
    current_node = current_node->next;
  }
  return NULL;
}

symbol_t *symbol_search_current(env_t env, const char *key) {
  node_t *head = list_head(env);
  if (!head)
    return NULL;

  symbol_t search_key;
  search_key.id = (char *)key;

  struct hashmap *scope = (struct hashmap *)list_data(head);
  return (symbol_t *)hashmap_get(scope, &search_key);
}

void symbol_print(symbol_t *s) {
  if (!s)
    return;

  const char *kind_str = (s->kind == SYM_FUNC)    ? "FUNC"
                         : (s->kind == SYM_PARAM) ? "PARAM"
                                                  : "VAR";

  printf("Symbol: %s | Kind: %s | Type: %d | Line: %d", s->id, kind_str,
         s->data_type, s->line);

  if (s->kind == SYM_VAR || s->kind == SYM_PARAM) {
    printf(" | Offset: %d", s->info.var.offset);
  } else {
    printf(" | Params: %d", s->info.func.num_params);
  }
  printf("\n");
}
