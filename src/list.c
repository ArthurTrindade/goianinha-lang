#include "../include/list.h"
#include <stdlib.h>
#include <string.h>

void list_init(list_t *list, void (*destroy)(void *)) {
  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;
  return;
}

void list_destroy(list_t *list) {
  /* void *data; */
  node_t *aux = list_head(list);

  while (list_size(list) > 0) {
    if (list_remove(list, aux) == 0) {
      aux = aux->next;
    }
  }

  memset(list, 0, sizeof(list_t));

  return;
}

int list_insert_next(list_t *list, node_t *element, const void *data) {
  node_t *new_element;

  if (element == NULL && list_size(list) != 0)
    return -1;

  if ((new_element = (node_t *)malloc(sizeof(node_t))) == NULL)
    return -1;

  new_element->data = (void *)data;

  if (list_size(list) == 0) {
    list->head = new_element;
    list->head->prev = NULL;
    list->head->next = NULL;
    list->tail = NULL;
  } else {
    new_element->next = element->next;
    new_element->prev = element;

    if (element->next == NULL) {
      list->tail = new_element;
    } else {
      element->next->prev = new_element;
    }

    element->next = new_element;
  }

  list->size++;

  return 0;
}

int list_insert_prev(list_t *list, node_t *element, const void *data) {
  node_t *new_element;

  if (element == NULL && list_size(list) != 0)
    return -1;

  if ((new_element = (node_t *)malloc(sizeof(node_t))) == NULL)
    return -1;

  new_element->data = (void *)data;

  if (list_size(list) == 0) {
    list->head = new_element;
    list->head->prev = NULL;
    list->head->next = NULL;
    list->tail = NULL;
  } else {
    new_element->next = element;
    new_element->prev = element->prev;

    if (element->prev == NULL) {
      list->head = new_element;
    } else {
      element->prev->next = new_element;
    }

    element->prev = new_element;
  }

  list->size++;

  return 0;
}

int list_remove(list_t *list, node_t *element) {

  if (element == NULL || list_size(list) == 0)
    return -1;

  void *data = element->data;

  if (element == list->head) {
    list->head = element->next;
    if (list->head == NULL)
      list->tail = NULL;
    else
      element->next->prev = NULL;
  } else {
    element->prev->next = element->next;

    if (element->next == NULL)
      list->tail = element->prev;
    else
      element->next->prev = element->prev;
  }

  list->destroy(data);

  free(element);

  list->size--;

  return 0;
}

int list_insert_begin(list_t *list, void *data) {
  return list_insert_prev(list, list->head, data);
}

int list_insert_end(list_t *list, void *data) {
  return list_insert_next(list, list->tail, data);
}

int list_remove_begin(list_t *list) { return list_remove(list, list->head); }

int list_remove_last(list_t *list) { return list_remove(list, list->tail); }
