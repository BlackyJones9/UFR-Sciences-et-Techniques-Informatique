//  Partie implantation du module holdall.

#include "holdall.h"

//  struct holdall, holdall : implantation par liste dynamique simplement
//    chainée.

//  Si la macroconstante HOLDALL_PUT_TAIL est définie et que sa macro-évaluation
//    donne un entier non nul, l'insertion dans la liste a lieu en queue. Dans
//    le cas contraire, elle a lieu en tête.

typedef struct choldall choldall;

struct choldall {
  void *ref;
  choldall *next;
};

struct holdall {
  choldall *head;
#if defined HOLDALL_PUT_TAIL && HOLDALL_PUT_TAIL != 0
  choldall **tailptr;
#endif
  size_t count;
};

choldall *fusion(choldall *a, choldall *b, int (*compar)(const void *,
    const void *));

choldall *triFusion(choldall *holdc, int (*compar)(const void *, const void *));

choldall *triFusion_context(choldall *holdc, const void *context, int (*compar)(
    const void *, const void *, const void *));

choldall *fusion_context(choldall *a, choldall *b, const void *context,
    int (*compar)(
    const void *, const void *,
    const void *));

void split(choldall *head, choldall **first, choldall **second);

holdall *holdall_empty(void) {
  holdall *ha = malloc(sizeof *ha);
  if (ha == NULL) {
    return NULL;
  }
  ha->head = NULL;
#if defined HOLDALL_PUT_TAIL && HOLDALL_PUT_TAIL != 0
  ha->tailptr = &ha->head;
#endif
  ha->count = 0;
  return ha;
}

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

int holdall_put(holdall *ha, void *ref) {
  choldall *p = malloc(sizeof *p);
  if (p == NULL) {
    return -1;
  }
  p->ref = ref;
#if defined HOLDALL_PUT_TAIL && HOLDALL_PUT_TAIL != 0
  p->next = NULL;
  *ha->tailptr = p;
  ha->tailptr = &p->next;
#else
  p->next = ha->head;
  ha->head = p;
#endif
  ha->count += 1;
  return 0;
}

size_t holdall_count(holdall *ha) {
  return ha->count;
}

int holdall_apply(holdall *ha,
    int (*fun)(void *)) {
  for (const choldall *p = ha->head; p != NULL; p = p->next) {
    int r = fun(p->ref);
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context(holdall *ha,
    void *context, void *(*fun1)(void *context, void *ptr),
    int (*fun2)(void *ptr, void *resultfun1)) {
  for (const choldall *p = ha->head; p != NULL; p = p->next) {
    int r = fun2(p->ref, fun1(context, p->ref));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context2(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ptr),
    void *context2, int (*fun2)(void *context2, void *ptr, void *resultfun1)) {
  for (const choldall *p = ha->head; p != NULL; p = p->next) {
    int r = fun2(context2, p->ref, fun1(context1, p->ref));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

holdall *holdall_sort(holdall *hold,
    int (*compar)(const void *, const void *)) {
  hold->head = triFusion(hold->head, compar);
  return hold;
}

choldall *triFusion(choldall *holdc,
    int (*compar)(const void *, const void *)) {
  choldall *a;
  choldall *b;
  if (holdc == NULL || holdc->next == NULL) {
    return holdc;
  }
  split(holdc, &a, &b);
  a = triFusion(a, compar);
  b = triFusion(b, compar);
  // Fusionner les deux moitiés triées
  return fusion(a, b, compar); // Déréférencer a et b
}

void split(choldall *head, choldall **first, choldall **second) {
  choldall *fast;
  choldall *slow;
  slow = head;
  fast = head->next;
  while (fast != NULL) {
    fast = fast->next;
    if (fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }
  *first = head;
  *second = slow->next;
  slow->next = NULL;
}

choldall *fusion(choldall *a, choldall *b, int (*compar)(const void *,
    const void *)) {
  if (a == NULL) {
    return b;
  }
  if (b == NULL) {
    return a;
  }
  choldall *result = NULL;
  choldall *current = NULL;
  if (compar(a->ref, b->ref) <= 0) {
    result = a;
    a = a->next;
  } else {
    result = b;
    b = b->next;
  }
  current = result;
  while (a != NULL && b != NULL) {
    if (compar(a->ref, b->ref) <= 0) {
      current->next = a;
      a = a->next;
    } else {
      current->next = b;
      b = b->next;
    }
    current = current->next;
  }
  if (a != NULL) {
    current->next = a;
  }
  if (b != NULL) {
    current->next = b;
  }
  return result;
}

holdall *holdall_sort_context(holdall *hold, const void *context, int (*compar)(
    const void *, const void *, const void *)) {
  hold->head = triFusion_context(hold->head, context, compar);
  return hold;
}

choldall *triFusion_context(choldall *holdc, const void *context, int (*compar)(
    const void *, const void *, const void *)) {
  choldall *a;
  choldall *b;
  if (holdc == NULL || holdc->next == NULL) {
    return holdc;
  }
  split(holdc, &a, &b);
  a = triFusion_context(a, context, compar);
  b = triFusion_context(b, context, compar);
  //Fusionner les deux moitiés triées
  return fusion_context(a, b, context, compar); // Déréférencer a et b
}

choldall *fusion_context(choldall *a, choldall *b, const void *context,
    int (*compar)(
    const void *, const void *,
    const void *)) {
  if (a == NULL) {
    return b;
  }
  if (b == NULL) {
    return a;
  }
  choldall *result = NULL;
  choldall *current = NULL;
  if (compar(a->ref, b->ref, context) <= 0) {
    result = a;
    a = a->next;
  } else {
    result = b;
    b = b->next;
  }
  current = result;
  while (a != NULL && b != NULL) {
    if (compar(a->ref, b->ref, context) <= 0) {
      current->next = a;
      a = a->next;
    } else {
      current->next = b;
      b = b->next;
    }
    current = current->next;
  }
  if (a != NULL) {
    current->next = a;
  }
  if (b != NULL) {
    current->next = b;
  }
  return result;
}

#if defined HOLDALL_WANT_EXT && HOLDALL_WANT_EXT != 0

/*
 *  IMPLANTATION DE L'EXTENSION OPTIONNELLE
 */

#endif
