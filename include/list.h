#ifndef list_H
#define list_H

#include <stdio.h>

typedef struct node_t {
  void *data;
  struct node_t *prev;
  struct node_t *next;
} node_t;

typedef struct list_t {
  int size;
  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  node_t *head;
  node_t *tail;
} list_t;

/**
 * Inicializa uma lista duplamente ligada especificada de lista
 * o arg destroy providencia uma maneira de liberar dinamicamente a memoria
 * quando list_destroy for chamada
 * return NONE
 */
void list_init(list_t *list, void (*destroy)(void *data));

/**
 * Destroi a lista duplamente ligada, usando a função destry

 */
void list_destroy(list_t *list);

/*
 *
 * return 0 se a element foi inserido com sucesso, ou -1 caso contrário
 * */
int list_insert_next(list_t *list, node_t *element, const void *data);

/*
 * return 0 se o elemento foi inserido com sucesso, ou -1 caso contrário
 * */
int dist_insert_prev(list_t *list, node_t *element, const void *data);

/**
 * return 0 se o elemento foi removido com sucesso, ou -1 caso contrário
 */
int list_remove(list_t *list, node_t *element);

int list_insert_begin(list_t *list, void *data);

int list_insert_end(list_t *list, void *data);

int list_remove_begin(list_t *list);

int list_remove_last(list_t *list);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(element) ((element)->prev == NULL ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)
#define list_prev(element) ((element)->prev)

#endif // !list_H
