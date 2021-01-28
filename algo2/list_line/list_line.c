//  Partie implantation du module list_line

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "list_line.h"

#define FUN_SUCCESS 0
#define FUN_FAILURE 1

typedef struct cell cell;

struct cell {
  long int value;
  cell *next;
};

struct list_line {
  cell *head;
  cell *tail;
};

list_line *list_line_empty() {
  list_line *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  p->head = NULL;
  p->tail = NULL;
  return p;
}

bool list_line_is_empty(const list_line *s) {
  return s->head == NULL;
}

int list_line_insert_tail(list_line *s, long int ptr) {
  cell *c = malloc(sizeof *c);
  if (c == NULL) {
    return FUN_FAILURE;
  }
  c->value = ptr;
  c->next = NULL;
  if (s->head == NULL) {
    s->head = c;
  } else {
    s->tail->next = c;
  }
  s->tail = c;
  return FUN_SUCCESS;
}

int list_line_insert_head(list_line *s, long int xptr) {
  cell *p = malloc(sizeof *p);
  if (p == NULL) {
    return FUN_FAILURE;
  }
  p->value = xptr;
  p->next = s->head;
  s->head = p;
  if (s->tail == NULL) {
    s->tail = p;
  }
  return FUN_SUCCESS;
}

size_t list_line_length(const list_line *s) {
  size_t t = 0;
  cell *c = s->head;
  while (c != NULL) {
    ++t;
    c = c->next;
  }
  return t;
}

long int list_line_index_number(const list_line *s, size_t n) {
  if (list_line_length(s) < n) {
    return s->tail->value;
  }
  cell *p = s->head;
  size_t j = 0;
  while (p != NULL && j < n) {
    p = p->next;
    ++j;
  }
  return p->value;
}

void list_line_dispose(list_line **sptr) {
  if (*sptr == NULL) {
    return;
  }
  cell *c = (*sptr)->head;
  while (c != NULL) {
    cell *t = c;
    c = c->next;
    free(t);
  }
  free(*sptr);
  *sptr = NULL;
}

static cell **ls__search(const list_line*s, long int xptr) {
  cell * const *pp = &(s->head);
  while (*pp != NULL && ((*pp)->value != xptr)) {
    pp = &((*pp)->next);
  }
  return (cell **) pp;
}

long int list_line_search(const list_line *s, long int xptr) {
  cell *p = *ls__search(s, xptr);
  return p == NULL ? FUN_FAILURE : p->value;
}
