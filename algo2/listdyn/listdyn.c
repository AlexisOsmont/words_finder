//  Partie implantation du module listdyn

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "listdyn.h"

#define FUN_SUCCESS 0
#define FUN_FAILURE 1

typedef struct cell cell;

struct cell {
  void *value;
  cell *next;
};

struct listdyn {
  cell *head;
  cell *tail;
};

listdyn *listdyn_empty() {
  listdyn *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  p->head = NULL;
  p->tail = NULL;
  return p;
}

bool listdyn_is_empty(const listdyn *s) {
  return s->head == NULL;
}

int listdyn_insert_tail(listdyn *s, void *ptr) {
  if (ptr == NULL) {
    return FUN_FAILURE;
  }
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

int listdyn_insert_head(listdyn *s, void *xptr) {
  if (xptr == NULL) {
    return FUN_FAILURE;
  }
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

size_t listdyn_length(const listdyn *s) {
  size_t t = 0;
  cell *c = s->head;
  while (c != NULL) {
    ++t;
    c = c->next;
  }
  return t;
}

void *listdyn_index_value(const listdyn *s, size_t n) {
  if (listdyn_length(s) < n) {
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

long int listdyn_index_number(const listdyn *s, size_t n) {
  if (listdyn_length(s) < n) {
    return (long int)s->tail->value;
  }
  cell *p = s->head;
  size_t j = 0;
  while (p != NULL && j < n) {
    p = p->next;
    ++j;
  }
  return (long int)p->value;
}

int listdyn_comparison(listdyn *s1, listdyn *s2) {
  cell *p1 = s1->head;
  cell *p2 = s2->head;
  while (p1 != NULL && p2 != NULL && strcmp(p1->value, p2->value) == 0) {
    p1 = p1->next;
    p2 = p2->next;
  }
  if (p1 != NULL && p2 != NULL) {
    return strcmp(p1->value, p2->value) > 0 ? 1 : -1;
  }
  if (p1 != NULL && p2 == NULL) {
    return 1;
  }
  if (p1 == NULL && p2 != NULL) {
    return -1;
  }
  return 0;
}

size_t listdyn_hashfun(const listdyn *s) {
  cell *p = s->head;
  size_t h = 0;
  while (p != NULL) {
    char *c = (char *) p->value;
    for (unsigned char *t = (unsigned char *) c; *t != '\0'; ++t) {
      h = 37 * h + *t;
    }
    p = p->next;
  }
  return h;
}

listdyn *listdyn_key(void *xptr, size_t order) {
  if (xptr == NULL) {
    return NULL;
  }
  listdyn *s = listdyn_empty((int (*)(const void *, const void *))strcmp);
  if (s == NULL) {
    return NULL;
  }
  while (order != 0) {
    if (listdyn_insert_tail(s, xptr) != 0) {
      listdyn_dispose(&s);
      return NULL;
    }
    order -= 1;
  }
  return s;
}

listdyn *listdyn_replace(const listdyn *s, void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  if (listdyn_is_empty(s)) {
    return NULL;
  }
  listdyn *p = listdyn_empty((int (*)(const void *, const void *))strcmp);
  if (p == NULL) {
    return NULL;
  }
  cell **pp = &(p->head);
  cell *q = s->head->next;
  while (q != NULL) {
    *pp = malloc(sizeof **pp);
    if (*pp == NULL) {
      listdyn_dispose(&p);
      return NULL;
    }
    (*pp)->value = q->value;
    p->tail = *pp;
    (*pp)->next = NULL;
    if (listdyn_length(p) == 1) {
      p->head = *pp;
    }
    pp = (&(*pp)->next);
    q = q->next;
  }
  if (listdyn_insert_tail(p, xptr) != 0) {
    listdyn_dispose(&p);
    return NULL;
  }
  return p;
}

#define ESPACE 0

int listdyn_fput(const listdyn *s, FILE *stream) {
  cell *p = s->head;
  if (fputc('\t', stream) == EOF) {
    return -1;
  }
  while (p != NULL) {
   if (fputc('\t', stream) == EOF) {
    return -1;
  }
    if (fprintf(stream, "%s", (char *) p->value) < 0) {
      return FUN_FAILURE;
    }
    p = p->next;
  }
  if (fputc('\n', stream) == EOF) {
    return -1;
  }
  return FUN_SUCCESS;
}

listdyn *reverse(listdyn *src) {
  listdyn *dest = listdyn_empty((int (*)(void *, void *))strcmp);
  if (dest == NULL) {
    return NULL;
  }
  cell *p = src->head;
  while (p != NULL) {
    if (listdyn_insert_head(dest, p->value) != 0) {
      listdyn_dispose(&dest);
      return NULL;
    }
    p = p->next;
  }
  return dest;
}

void listdyn_dispose(listdyn **sptr) {
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

static cell **ls__search(const listdyn *s, void *xptr) {
  cell * const *pp = &(s->head);
  while (*pp != NULL && strcmp((*pp)->value, xptr) != 0) {
    pp = &((*pp)->next);
  }
  return (cell **) pp;
}

void *listdyn_search(const listdyn *s, void *xptr) {
  cell *p = *ls__search(s, xptr);
  return p == NULL ? NULL : p->value;
}

void *lisdyn_remove(listdyn *s, void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  cell **pp = ls__search(s, xptr);
  if (*pp == NULL) {
    return NULL;
  }
  void * yptr = (*pp)->value;
  cell *t = *pp;
  *pp = t->next;
  //free((char *)t->value);
  free(t);
  return yptr;
}
