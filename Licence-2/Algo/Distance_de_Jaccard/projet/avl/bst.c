//  bst.c : partie implantation d'un module polymorphe pour la spécification
//   ABINR du TDA ABinR(T).

#include "bst.h"
#include "stdio.h"


//=== Type cbst ================================================================

//--- Définition cbst ----------------------------------------------------------

typedef struct cbst cbst;

struct cbst {
  cbst *next[2];
  const void *ref;
  int height;
};

//--- Raccourcis cbst ----------------------------------------------------------

#define EMPTY()     nullptr
#define IS_EMPTY(p) ((p) == nullptr)
#define LEFT(p)     ((p)->next[0])
#define RIGHT(p)    ((p)->next[1])
#define REF(p)      ((p)->ref)
#define HEIGHT(p) ((p)->height)
#define NEXT(p, d)  ((p)->next[(d) > 0])

//--- Divers -------------------------------------------------------------------

static size_t add__size_t(size_t x1, size_t x2) {
  return x1 + x2;
}

static int max__int(int x1, int x2) {
  return x1 > x2 ? x1 : x2;
}

static int min__int(int x1, int x2) {
  return x1 < x2 ? x1 : x2;
}

//--- Fonctions cbst -----------------------------------------------------------

//  DEFUN_CBST__MEASURE : définit la fonction récursive de nom « cbst__ ## fun »
//    et de paramètre un pointeur d'arbre binaire, qui renvoie zéro si l'arbre
//    est vide et « 1 + oper(r0, r1) » sinon, où r0 et r1 sont les valeurs
//    renvoyées par les appels récursifs de la fonction avec les pointeurs des
//    sous-arbres gauche et droit de l'arbre comme paramètres.
#define DEFUN_CBST__MEASURE(fun, oper, type)                                         \
  static type cbst__ ## fun(const cbst * p) {                                \
    return IS_EMPTY(p)                                                         \
      ? 0                                                                      \
      : 1 + oper(cbst__ ## fun(LEFT(p)), cbst__ ## fun(RIGHT(p)));             \
  }

//  cbst__size, cbst__height, cbst__distance : définitions.

DEFUN_CBST__MEASURE(size, add__size_t, size_t)
DEFUN_CBST__MEASURE(distance, min__int, int)

static int cbst__height(const cbst *p){
  return IS_EMPTY(p) ? 0 : (p->height);
}

static void cbst__update_height(cbst *p) {
  p->height = 1 + max__int(cbst__height(LEFT(p)),cbst__height(RIGHT(p)));
}

static int cbst__balance(const cbst *p) {
  return IS_EMPTY(p) ? 0 : cbst__height(LEFT(p)) - cbst__height(RIGHT(p));
}

static void cbst__rotation_left(cbst **pp) {
  cbst *p = RIGHT(*pp);
  RIGHT(*pp) = LEFT(p);
  LEFT(p) = *pp;
  *pp = p;
  cbst__update_height(LEFT(*pp));
  cbst__update_height(*pp);
}

static void cbst__rotation_right(cbst **pp) {
  cbst *p = LEFT(*pp);
  LEFT(*pp) = RIGHT(p);
  RIGHT(p) = *pp;
  *pp = p;
  cbst__update_height(RIGHT(*pp));
  cbst__update_height(*pp);
}

static void cbst__rotation_left_right(cbst **pp) {
  cbst__rotation_left(&LEFT(*pp));
  cbst__rotation_right(pp);
}

static void cbst__rotation_right_left(cbst **pp) {
  cbst__rotation_right(&RIGHT(*pp));
  cbst__rotation_left(pp);
}

static int cbst__balancing(cbst **pp) {
  cbst__update_height(*pp);
  if (cbst__balance(*pp) == -2) {
    if (cbst__balance(RIGHT(*pp)) > 0) {
      #ifdef VERBOSE_ROTA
        printf("right-left \n");
      #endif
      cbst__rotation_right_left(pp);
    }
    else {
      #ifdef VERBOSE_ROTA
        printf("left \n");
      #endif
      cbst__rotation_left(pp);
    }
    return -1;
  }
  if (cbst__balance(*pp) == 2){
    if (cbst__balance(LEFT(*pp)) < 0) {
      #ifdef VERBOSE_ROTA
        printf("left-right \n");
      #endif
      cbst__rotation_left_right(pp);
    }
    else {
      #ifdef VERBOSE_ROTA
        printf("right \n");
      #endif
      cbst__rotation_right(pp);
    }
    return 1;
  }
  return 0;
}

//  cbst__dispose : libère les ressources allouées à l'arbre binaire pointé par
//    p.
static void cbst__dispose(cbst *p) {
  if (!IS_EMPTY(p)) {
    cbst__dispose(LEFT(p));
    cbst__dispose(RIGHT(p));
    free(p);
  }
}

//  cbst__add_endofpath : tente d'allouer les ressources nécessaires à l'ajout
//  d'un nouvel élément à l'arbre pointé par *pp. Renvoie nullptr en cas d'échec, ajoute la
//  référence à l'arbre en fin de chemin et renvoie la référence ajoutée sinon.

static void *cbst__add_endofpath(cbst **pp, const void *ref,
  int (*compar)(const void *, const void *)) {
    if (*pp == nullptr) {
      struct cbst *p = malloc(sizeof (*p));
      if (p == nullptr) {
        return nullptr;
      }
      p->ref = ref;
      p->height = 1;
      LEFT(p) = nullptr;
      RIGHT(p) = nullptr;
      *pp = p;
      return (void*) ref;
    }
    int c = compar (ref, (*pp)->ref);
    if (c == 0) {
      return (void*) ref;
    }
    void *q = cbst__add_endofpath(&(NEXT(*pp,c)),ref, compar);
    cbst__balancing(pp);
    return q;
  }

//  cbst__remove_max : libère le noeud ayant la valeur maximale dans l'arbre pointé
//  par *pp
static void *cbst__remove_max(cbst **pp){
  if (IS_EMPTY(RIGHT(*pp))) {
    const void *pref = REF(*pp);
    cbst *p = *pp;
    *pp = LEFT(p);
    free(p);
    cbst__balancing(pp);
    return (void*) pref;
  }
  return cbst__remove_max(&(RIGHT(*pp)));
}

//  cbst__remove_root : libère la racine de l'arbre pointé par *pp
static void cbst__remove_root(cbst **pp) {
  if (IS_EMPTY(LEFT(*pp))) {
    cbst *q = *pp;
    *pp = RIGHT(*pp);
    free(q);
  }
  else {
    *pp = cbst__remove_max(pp);
  }
}

//  cbst__remove_climbup_left : Retire la référence pointée par ref de l'arbre pointé
//  par *pp en utilisant la méthode de la remontée par la gauche dans le sens de la
//  fonction de comparaison pointée par compar. Renvoie la référence supprimé de l'arbre.
static void *cbst__remove_climbup_left(cbst **pp, const void *ref,
  int (*compar)(const void *, const void *)) {
    int c = compar(ref, REF(*pp));
    if (c == 0) {
      const void *aref = REF(*pp);
      cbst__remove_root(pp);
      return (void*) aref;
    }
    const void *res = cbst__remove_climbup_left(&(NEXT(*pp,c)), ref, compar);
    cbst__balancing(pp);
    return (void*) res;
}

//  cbst__search : cherche la référence ref dans l'arbre pointé par p.
//  Renvoie nullptr en cas d'échec, la référence trouvée sinon.
static void *cbst__search(const cbst *p, const void *ref,
  int (*compar)(const void *, const void *)) {
    if (p == nullptr) {
      return nullptr;
    }
    int c = compar (ref, p->ref);
    if (c == 0) {
      return (void*) (p->ref);
    }
    return (void*) cbst__search(NEXT(p,c),ref,compar);
  }


//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static size_t cbst__number(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *), size_t number);
//    static size_t cbst__rank(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *), size_t rank);

#define REPR__TAB 4

//  cbst__repr_graphic : affiche la représentation de l'arbre pointé par p,
//  affiche également pour chaque noeud sa hauteur level.
static void cbst__repr_graphic(const cbst *p,
  void (*put)(const void *ref), size_t level){
  if IS_EMPTY(p) {
    printf("\n");
    printf("%*s%s", REPR__TAB * (int)level, "", "|");
  }
  else {
    cbst__repr_graphic(RIGHT(p), put, level + 1);
    printf("\n");
    printf("%*s", REPR__TAB * (int) level, "");
    put(p->ref);
    printf("%s%d", " h=",cbst__height(p));
    printf("%s%d", " b=",cbst__balance(p));
    cbst__repr_graphic(LEFT(p), put, level + 1);
  }
}

static int infix_apply_context(cbst *p, int dir, void *context,
  int (*fun)(void *, const void *),
  int (*fun_pre)(void *),
  int (*fun_post)(void *)) {
  if (IS_EMPTY(p)) {
    return 0;
  }
  int r = 0;
  if (fun_pre != nullptr && (r = fun_pre(context)) != 0) {
    return r;
  }
  cbst *first = dir <= 0 ? LEFT(p) : RIGHT(p);
  cbst *second = dir <= 0 ? RIGHT(p) : LEFT(p);
  if ((r = infix_apply_context(first, dir, context, fun, fun_pre, fun_post)) != 0) {
    return r;
  }
  if ((r = fun(context, p->ref)) != 0) {
    return r;
  }
  if ((r = infix_apply_context(second, dir, context, fun, fun_pre, fun_post)) != 0) {
    return r;
  }
  if (fun_post != nullptr && (r = fun_post(context)) != 0) {
    return r;  
  }
  return r;
}

//=== Type bst =================================================================

//--- Définition bst -----------------------------------------------------------

struct bst {
  int (*compar)(const void *, const void *);
  cbst *root;
};

//--- Fonctions bst ------------------------------------------------------------

int bst_dft_infix_apply_context(bst *t, int dir,
  void *context, int (*fun)(void *context, const void *ref),
  int (*fun_pre)(void *context), int (*fun_post)(void *context)) {
    if (IS_EMPTY(t) || fun == nullptr) {
      return -1;
    }
    return infix_apply_context(t->root, dir, context, fun, fun_pre, fun_post);
  }

bst *bst_empty(int (*compar)(const void *, const void *)) {
  bst *t = malloc(sizeof *t);
  if (t == nullptr) {
    return nullptr;
  }
  t->compar = compar;
  t->root = EMPTY();
  return t;
}

void bst_dispose(bst **tptr) {
  if (*tptr == nullptr) {
    return;
  }
  cbst__dispose((*tptr)->root);
  free(*tptr);
  *tptr = nullptr;
}

void *bst_add_endofpath(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__add_endofpath(&(t->root), ref, t->compar);
}

//  bst_remove_climbup_left : renvoie un pointeur nul si ref vaut un pointeur
//    nul. Recherche sinon dans l'arbre binaire de recherche associé à t la
//    référence d'un objet égal à celui de référence ref au sens de la fonction
//    de comparaison. Si la recherche est négative, renvoie un pointeur nul.
//    Retire sinon la référence trouvée selon la méthode du retrait par remontée
//    gauche et renvoie la référence trouvée.
void *bst_remove_climbup_left(bst *t, const void *ref) {
  if (ref == nullptr || cbst__search(t->root,ref,t->compar) == nullptr) {
    return nullptr;
  }
  return cbst__remove_climbup_left(&(t->root),ref, t->compar);
}

void *bst_search(bst *t, const void *ref){
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__search(t->root,ref,t->compar);
}

size_t bst_size(bst *t) {
  return cbst__size(t->root);
}

size_t bst_height(bst *t) {
  return (size_t) cbst__height(t->root);
}

size_t bst_distance(bst *t) {
  return (size_t) cbst__distance(t->root);
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bst_number
//    bst_rank

void bst_repr_graphic(bst *t, void (*put)(const void *ref)) {
  cbst__repr_graphic(t->root, put, 0);
  printf("\n");
}
