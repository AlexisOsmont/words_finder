//  Partie implantation du module holdall

#include <stdlib.h>
#include "holdall.h"

//  struct holdall, holdall : implantation par liste dynamique simplement
//    chainée. L'insertion a lieu en queue si la macroconstante
//    HOLDALL_INSERT_TAIL est définie, en tête sinon

typedef struct choldall choldall;

struct choldall {
  void *value;
  choldall *next;
};

struct holdall {
  choldall *head;
#ifdef HOLDALL_INSERT_TAIL
  choldall *tail;
#endif
  size_t count;
};

holdall *holdall_empty(void) {
  holdall *ha = malloc(sizeof *ha);
  if (ha == NULL) {
    return NULL;
  }
  ha->head = NULL;
#ifdef HOLDALL_INSERT_TAIL
  ha->tail = NULL;
#endif
  ha->count = 0;
  return ha;
}

int holdall_put(holdall *ha, void *ptr) {
  choldall *p = malloc(sizeof *p);
  if (p == NULL) {
    return -1;
  }
  p->value = ptr;
#ifdef HOLDALL_INSERT_TAIL
  p->next = NULL;
  if (ha->tail == NULL) {
    ha->head = p;
  } else {
    ha->tail->next = p;
  }
  ha->tail = p;
#else
  p->next = ha->head;
  ha->head = p;
#endif
  ha->count += 1;
  return 0;
}

size_t holdall_count(const holdall *ha) {
  return ha->count;
}

int holdall_apply(holdall *ha, int (*fun)(void *)) {
  choldall *p = ha->head;
  while (p != NULL) {
    int r = fun(p->value);
    if (r != 0) {
      return r;
    }
    p = p->next;
  }
  return 0;
}

#ifdef HOLDALL_APPLY_CONTEXT_EXTENDED

int holdall_apply_context(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ptr),
    void *context2, int (*fun2)(void *context2, void *ptr, void *resultfun1)) {
  choldall *p = ha->head;
  while (p != NULL) {
    int r = fun2(context2, p->value, fun1(context1, p->value));
    if (r != 0) {
      return r;
    }
    p = p->next;
  }
  return 0;
}

#else

int holdall_apply_context(holdall *ha,
    int (*fun)(void *ptr, void *resultfuncontext),
    void *(*funcontext)(void *context, void *ptr), void *context) {
  choldall *p = ha->head;
  while (p != NULL) {
    int r = fun(p->value, funcontext(context, p->value));
    if (r != 0) {
      return r;
    }
    p = p->next;
  }
  return 0;
}

#endif

void holdall_dispose(holdall **haptr) {
  if (*haptr == NULL) {
    return;
  }
  choldall *p = (*haptr)->head;
  while (p != NULL) {
    choldall *t = p;
    p = p->next;
    free(t);
  }
  free(*haptr);
  *haptr = NULL;
}

void holdall_sort(holdall *ha, int (*compar)(const void *, const void *)) {
  if (ha->head == NULL) {
    return;
  }
  void *t;
  for (choldall *p = ha->head; p->next != NULL; p = p->next) {
    for (choldall *q = p->next; q != NULL; q = q->next) {
      if (compar(p->value, q->value) > 0) {
        t = p->value;
        p->value = q->value;
        q->value = t;
      }
    }
  }
}

