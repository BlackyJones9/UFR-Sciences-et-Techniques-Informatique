//  Programme principal de l'executable jdis, alloue les ressources nécessaires
//    aux calculs et affichages de la distance de Jaccard des textes donné par
//    l'utilisateur selon ses contraintes établies et vérifiés selon les
//    fonction du module op.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "hashtable.h"
#include "op.h"
#include "holdall.h"
#include "bst.h"

//--- DEFINITION STRUCTURE -----------------------------------------------------

typedef struct cref cref;
struct cref {
  const char *ref;
  bool *aref;
};

typedef struct dist dist;
struct dist {
  float *uni;
  float *inter;
  size_t nmemb;
};

//--- MACRO --------------------------------------------------------------------

#define LEN_TAB_PONCT 18
#define STD_NAME "\"\""

//------------------------------------------------------------------------------

int rfree(void *ptr);
int cref_free(cref *ptr);
cref *cref_empty();

int end_word(opts *optr, context *lfiles, size_t pos, size_t cur, char *w,
    hashtable *ht, holdall *has, holdall *hasbl,
    bst *bt, cref *cref_w);
bool is_separator(opts **optr, int c);
void next_word(opts **optr, FILE *file);

char *add_word_hashtable(hashtable *ht, holdall *has, holdall *hasbl,
    context *lfiles, char *w, size_t pos);
size_t str_hashfun(const char *s);

cref *add_word_bst(bst *bt, context *lfiles, const char *w,
    size_t pos);
int compar(const cref *x, const cref *y);

int intermediary_repr_graph(context *lfiles, void *ref, cref *ptr);
int intermediary_dist_calc(dist *dt, void *ref, cref *ptr);
int intermediary_cref_free(void *, cref *ptr);

dist *dist_init(context *lfiles);
void dist_dispose(dist **pp);
int dist_calc(dist *dt, cref *ptr);

int repr_graphic(context *lfiles, cref *ptr);
int repr_simple(dist *dt, context *lfiles);

//------------------------------------------------------------------------------

int main(int argc, char **argv) {
  //Lecture en cas d'appel à l'aide
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i],
        OPT_LONG LONG_HELP) == 0
        || strcmp(argv[i], OPT_SHORT SHORT_HELP) == 0) {
      print_help();
      return EXIT_SUCCESS;
    }
  }
  //Set locale
  setlocale(LC_COLLATE, "fr_FR.UTF-8");
  opts *optr = opts_empty();
  context *lfiles = context_empty();
  if (optr == nullptr || lfiles == nullptr) {
    goto error_capacity;
  }
  dist *dt = nullptr;
  hashtable *ht = nullptr;
  holdall *hasbl = nullptr;
  holdall *has = nullptr;
  bst *bt = nullptr;
  cref *cref_w = nullptr;
  //Lecture des options et fichiers sur l'entrée standard
  int c = stdin_read_set(argc, argv, &optr, &lfiles);
  if (c == EXIT_FAILURE) {
    goto error;
  }
  //Allocation des structures
  if (!opts_isAvl(&optr)) {
    ht = hashtable_empty((int (*)(const void *, const void *))strcoll,
        (size_t (*)(const void *))str_hashfun, 1.0);
    hasbl = holdall_empty();
    has = holdall_empty();
    if (ht == nullptr || hasbl == nullptr || has == nullptr) {
      goto error_capacity;
    }
  } else {
    bt = bst_empty((int (*)(const void *, const void *))compar);
    cref_w = cref_empty();
    if (bt == nullptr || cref_w == nullptr) {
      goto error_capacity;
    }
  }
  //Erreur si nombre de fichiers < 2
  if (context_pos(&lfiles) < 2) {
    goto error_operand;
  }
  //Initialisation de la structure de calcule de Distance de Jacar
  dt = dist_init(lfiles);
  if (dt == nullptr) {
    goto error_capacity;
  }
  //Lecture fichier par fichier
  size_t pos = 0;
  size_t i = 0;
  size_t imax = opts_length_max(&optr);
  size_t max = INIT_LENGTH_WORD;
  char *w = malloc(INIT_LENGTH_WORD);
  if (w == nullptr) {
    goto error_capacity;
  }
  while (pos < context_pos(&lfiles)) {
    FILE *file;
    if (is_std_file(context_aref(&lfiles)[pos])) {
      file = stdin;
      printf("--- starts reading for #%zu FILE\n", pos + 1);
    } else {
      file = fopen(context_aref(&lfiles)[pos], "r");
    }
    if (file == nullptr) {
      goto error_read;
    }
    size_t cur = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
      if (!is_separator(&optr, c)) {
        //Ajout caractère
        if (cur == max) {
          //Realloc max * MUL
          char *p = realloc(w, max * MUL);
          if (p == nullptr) {
            free(w);
            w = nullptr;
            goto error_capacity;
          }
          w = p;
          max *= MUL;
        }
        w[cur] = (char) c;
        cur += 1;
        i += 1;
        if (i == imax) {
          if (end_word(optr, lfiles, pos, cur, w,
              ht, has, hasbl, bt, cref_w) == -1) {
            goto error_capacity;
          }
          if (cur == max) {
            //Realloc max * MUL
            char *p = realloc(w, max * MUL);
            if (p == nullptr) {
              free(w);
              w = nullptr;
              return -1;
            }
            w = p;
            max *= MUL;
          }
          ungetc(c, file);
          next_word(&optr, file);
          cur = 0;
          i = 0;
        }
      } else {
        if (cur == max) {
          //Realloc max * MUL
          char *p = realloc(w, max * MUL);
          if (p == nullptr) {
            free(w);
            w = nullptr;
            return -1;
          }
          w = p;
          max *= MUL;
        }
        if (end_word(optr, lfiles, pos, cur, w,
            ht, has, hasbl, bt, cref_w) == -1) {
          goto error_capacity;
        }
        ungetc(c, file);
        next_word(&optr, file);
        cur = 0;
        i = 0;
      }
    }
    //Gestion des erreurs de fin de lecture
    if (file != stdin) {
      if (!feof(file)) {
        goto error_read;
      }
      if (fclose(file) != 0) {
        goto error_close;
      }
    } else {
      printf("--- ends reading for #%zu FILE\n", pos + 1);
      rewind(file);
    }
    pos += 1;
  }
  free(w);
  free(cref_w);
  //Affichage Hashtable
  if (!opts_isAvl(&optr)) {
    if (opts_isGraph(&optr)) {
	  print_files(lfiles);
      if (holdall_apply_context2(has, ht,
          (void *(*)(void *, void *))hashtable_search,
          lfiles,
          (int (*)(void *, void *, void *))intermediary_repr_graph) != 0) {
        goto error_write;
      }
    } else {
      if (holdall_apply_context2(has, ht,
          (void *(*)(void *, void *))hashtable_search,
          dt, (int (*)(void *, void *, void *))intermediary_dist_calc) != 0) {
        goto error_write;
      }
      if (repr_simple(dt, lfiles) != 0) {
        goto error_write;
      }
    }
    //Affichage AVL
  } else {
    if (opts_isGraph(&optr)) {
	  print_files(lfiles);
      if (bst_dft_infix_apply_context(bt, 0, lfiles,
          (int (*)(void *, const void *))repr_graphic,
          nullptr, nullptr) != 0) {
        goto error_write;
      }
    } else {
      if (bst_dft_infix_apply_context(bt, 0, dt,
          (int (*)(void *, const void *))dist_calc,
          nullptr, nullptr) != 0) {
        goto error_write;
      }
      if (repr_simple(dt, lfiles) != 0) {
        goto error_write;
      }
    }
  }
  int r = EXIT_SUCCESS;
  goto dispose;
  //Gestion des erreurs
error_read:
  fprintf(stderr, "*** Error: A read error occurs\n");
  goto error;
error_write:
  printf("*** Something went wrong while writing\n");
  goto error;
error_close:
  printf("*** Error: Something went wrong while closing the file\n");
  goto error;
error_operand:
  printf("*** Error: Missing argument\n");
  goto error;
error_capacity:
  printf("*** Error: Not enough memories\n");
  goto error;
error:
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  if (!opts_isAvl(&optr)) {
    holdall_dispose(&has);
    if (hasbl != nullptr) {
      holdall_apply(hasbl, (int (*)(void *))cref_free);
    }
    holdall_dispose(&hasbl);
    hashtable_dispose(&ht);
  } else {
    bst_dft_infix_apply_context(bt, 0, nullptr, (int (*)(void *,
        const void *))intermediary_cref_free,
        nullptr, nullptr);
    bst_dispose(&bt);
  }
  opts_dispose(&optr);
  context_dispose(&lfiles);
  dist_dispose(&dt);
  return r;
}

//--- FONCTION REF && CREF -----------------------------------------------------

// rfree : Libère les ressources alloués par ptr puis renvoie 0.
// Temps : Constant.
// Espace : Constant.
int rfree(void *ptr) {
  free(ptr);
  return 0;
}

// cref_empty : Alloue les ressources nécessaires à la gestion d'un élément de
// type cref, renvoie nullptr en cas d'erreur d'allocation.
// Renvoie un pointeur vers l'élément alloué sinon.
// Temps : Constant.
// Espace : Constant.
cref *cref_empty() {
  cref *p = malloc(sizeof *p);
  if (p == nullptr) {
    return nullptr;
  }
  p->aref = nullptr;
  p->ref = nullptr;
  return p;
}

// cref_free : Libère les ressources alloués à la gestion du cref pointé par ptr
// puis renvoie 0.
// Temps : Constant.
// Espace : Constant.
int cref_free(cref *ptr) {
  if (ptr == nullptr) {
    return 0;
  }
  free((char *) ptr->ref);
  free(ptr->aref);
  free(ptr);
  return 0;
}

//--- FONCTIONS WORD ----------------------------------------------------------

// end_word : Finis la lecture du mot contenu dans le texte présent dans la
// structure de context lfiles. La chaîne pointé par w est ajouté à la
// partie hachage (table ht et holdall ht et has) ou la partie AVL (arbre bt)
// avec cur le numéro du texte actuellement lu et cref_w le cref correspondant à
// w. Renvoie -1 en cas d'erreur d'allocation, renvoie 0 sinon.
// Temps : Constant.
// Espace : Constant.
int end_word(opts *optr, context *lfiles, size_t pos, size_t cur, char *w,
    hashtable *ht, holdall *has, holdall *hasbl,
    bst *bt, cref *cref_w) {
  w[cur] = '\0';
  if (!opts_isAvl(&optr)) {
    cref *p = hashtable_search(ht, w);
    if (p != nullptr) {
      p->aref[pos] = true;
      hashtable_add(ht, w, p);
    } else {
      if (add_word_hashtable(ht, has, hasbl, lfiles, w, pos) == nullptr) {
        return -1;
      }
    }
  } else {
    cref_w->ref = w;
    cref *ref = bst_search(bt, cref_w);
    if (ref != nullptr) {
      ref->aref[pos] = true;
    } else {
      if (add_word_bst(bt, lfiles, w, pos) == nullptr) {
        return -1;
      }
    }
  }
  return 0;
}

// is_separator : Teste si le caractère c est un séparateur au sens de tab,
// prend en compte la valeur de isPonct dans la structure d'option pointé par
// *optr.
// Temps : Constant.
// Espace : Constant.
bool is_separator(opts **optr, int c) {
  if (isspace(c)) {
    return true;
  }
  if (opts_isPonct(optr) && ispunct(c)) {
    return true;
  }
  return false;
}

// next_word : Lis le fichier pointé par file à la recherche d'un séparateur
// selon la structure d'option pointé par *optr.
// Temps : Linéaire en la taille du texte.
// Espace : Constant.
void next_word(opts **optr, FILE *file) {
  bool sep_found = false;
  int c;
  while ((c = fgetc(file)) != EOF) {
    if (is_separator(optr, c)) {
      sep_found = true;
    } else if (sep_found) {
      ungetc(c, file);
      return;
    }
  }
}

//--- ADD TO HASHTABLE --------------------------------------------------------

// add_word_hashtable : Ajoute la chaîne pointé par w dans la structure
// hashtable associée à ht et au deux holdall associées à has et hasbl, alloue
// et crée le tableau de booléen associée à la chaîne selon la structure de
// context associée à lfiles et pos représentant l'indice du texte dans lequelle
// la chaîne se situe. Renvoie nullptr en cas d'erreur d'allocations ou d'ajout,
// renvoie un pointeur vers la référence ajoutée sinon.
// Temps : Constant.
// Espace : Linéaire en le champ cur_pos de lfiles.
char *add_word_hashtable(hashtable *ht, holdall *has, holdall *hasbl,
    context *lfiles, char *w, size_t pos) {
  bool *b = calloc(context_pos(&lfiles), sizeof(*b));
  char *copie = malloc(strlen(w) + 1);
  cref *p = malloc(sizeof *p);
  if (b == nullptr || copie == nullptr || p == nullptr) {
    return nullptr;
  }
  b[pos] = true;
  strcpy(copie, w);
  p->ref = copie;
  p->aref = b;
  if (holdall_put(has, copie) == -1) {
    return nullptr;
  }
  if (holdall_put(hasbl, p) == -1) {
    return nullptr;
  }
  if (hashtable_add(ht, copie, p) == nullptr) {
    return nullptr;
  }
  return copie;
}

// str_hashfun : fonction de pré-hachage conseillées par Kernighan et Pike pour
// les chaines de caractères.
// Temps : Linéaire en la taille de la chaîne.
// Espace : Constant.
size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

//--- ADD TO BST --------------------------------------------------------------

// add_word_bst : Alloue les ressources nécessaires à l'allocation d'un élément
// de la structure cref contenant la chaîne pointé par w, puis ajoute cet
// élément à la structure bst associée à bt et au holdall associée à has, et
// enfin, alloue et crée le tableau de booléen associée à la chaîne selon la
// structure de context associée à lfiles et pos représentant l'indice du texte
// dans lequelle la chaîne se situe. Renvoie nullptr en cas d'erreur
// d'allocations ou d'ajout, renvoie un pointeur vers le cref crée sinon.
// Temps : Constant.
// Espace : Linéaire en le champ cur_pos de lfiles.
cref *add_word_bst(bst *bt, context *lfiles, const char *w,
    size_t pos) {
  bool *b = calloc(context_pos(&lfiles), sizeof(*b));
  char *copie = malloc(strlen(w) + 1);
  cref *p = cref_empty();
  if (b == nullptr || copie == nullptr || p == nullptr) {
    return nullptr;
  }
  strcpy(copie, w);
  b[pos] = true;
  p->ref = copie;
  p->aref = b;
  if (bst_add_endofpath(bt, p) == nullptr) {
    return nullptr;
  }
  return p;
}

// compar : compare la taille du champ ref de la structure cref pointé par x
// avec celle de la chaîne pointé par y, renvoie un entier inférieur à 1 si
// ref est inférieur à y, 0 si ref est égal à y et un entier supérieur à 1
// sinon.
// Temps : Constant.
// Espace : Constant.
int compar(const cref *x, const cref *y) {
  return strcoll(x->ref, y->ref);
}

//--- FONCTIONS INTERMEDIARY ---------------------------------------------------

// intermediary_repr_graph : Renvoie la valeur de retour de repr_graphic selon
// lfiles et ptr.
// Temps : Linéaire en lfiles->cur_pos.
// Espace : Constant.
int intermediary_repr_graph(context *lfiles, [[maybe_unused]] void *ref,
    cref *ptr) {
  return repr_graphic(lfiles, ptr);
}

// intermediary_dist_calc : Renvoie la valeur de retour de dist_calc selon dt et
// ptr.
// Temps : Linéaire en le champ nmemb de dist.
// Espace : Constant.
int intermediary_dist_calc(dist *dt, [[maybe_unused]] void *ref, cref *ptr) {
  return dist_calc(dt, ptr);
}

// intermediary_cref_free : Renvoie la valeur de retour de cref_free selon ptr.
// Temps : Constant.
// Espace : Constant.
int intermediary_cref_free(void *, cref *ptr) {
  return cref_free(ptr);
}

//--- FONCTION DIST ------------------------------------------------------------

// dist_init : Alloue les ressources nécessaires à la gestion d'un controlleur
// de type dist selon le champ cur_pos de lfiles, renvoie nullptr en cas
// d'échec, renvoie un pointeur vers le controlleur sinon.
// Temps : Constant.
// Espace : Linéaire en le champs cur_pos de lfiles.
dist *dist_init(context *lfiles) {
  dist *p = malloc(sizeof *p);
  if (p == nullptr) {
    return nullptr;
  }
  size_t pos = context_pos(&lfiles);
  size_t nbr_couples = (pos * (pos - 1)) / 2;
  p->uni = calloc(nbr_couples, sizeof *(p->uni));
  p->inter = calloc(nbr_couples, sizeof *(p->inter));
  p->nmemb = pos;
  if ((p->uni == nullptr) || (p->inter == nullptr)) {
    free(p->uni);
    free(p->inter);
    free(p);
    return nullptr;
  }
  return p;
}

// dist_dispose : Sans effet si *pp vaut un pointeur nul, libère sinon les
// ressources alloués à la gestion de la structure de dist associée à *pp puis
// affecte un pointeur nul à nullptr.
// Temps : Constant.
// Espace : Constant.
void dist_dispose(dist **pp) {
  if (*pp == nullptr) {
    return;
  }
  free((*pp)->uni);
  free((*pp)->inter);
  free(*pp);
  *pp = nullptr;
}

// dist_calc : Met à jour les champs uni et inter de la structure dist associée
// à dt selon le cref ptr, renvoie -1 en cas d'erreur d'arguments, renvoie 0
// sinon.
// Temps : Linéaire en le champ nmemb de dist.
// Espace : Constant.
int dist_calc(dist *dt, cref *ptr) {
  if (dt == nullptr || ptr == nullptr) {
    return -1;
  }
  size_t p1 = 0;
  size_t p2 = 1;
  size_t dpos = 0;
  while (p1 < (dt->nmemb - 1)) {
    if (ptr->aref[p1] && ptr->aref[p2]) {
      dt->inter[dpos] += 1;
    }
    if (ptr->aref[p1] || ptr->aref[p2]) {
      dt->uni[dpos] += 1;
    }
    p2 += 1;
    if (p2 == dt->nmemb) {
      p1 += 1;
      p2 = p1 + 1;
    }
    dpos += 1;
  }
  return 0;
}

//--- AFFICHAGE
// ------------------------------------------------------------------

// repr_graphic : Affiche la représentation graphique de la chaîne pointé par
// ref pour chacun des textes contenu dans la structure de context associée à
// lfiles et le tableau de booléen pointé par ptr si la macro constante
// HASH_TABLE est définie, dans le cas contraire, l'affichage du champ ref de
// cref est fait selon la structure de context associée à lfiles et la structure
// de cref associée à ptr.
// Temps : Linéaire en lfiles->cur_pos.
// Espace : Constant.
int repr_graphic(context *lfiles, cref *ptr) {
  int r = 0;
  printf("%s", ptr->ref);
  size_t k = 0;
  size_t pos = context_pos(&lfiles);
  while (k < pos) {
    ptr->aref[k] == true ? printf("\tX") : printf("\t-");
    k += 1;
  }
  printf("\n");
  return r;
}

// repr_simple : calcul et affiche la distance de jacard de chaques couples
// différents de textes parmi ceux contenu dans lfiles selon les champs uni et
// inter de dt puis renvoie 0.
// Temps : Linéaire en le nombre de textes.
// Espace : Constant.
int repr_simple(dist *dt, context *lfiles) {
  size_t pos = context_pos(&lfiles);
  size_t nbr_couples = (pos * (pos - 1)) / 2;
  size_t i = 0;
  size_t k = 1;
  size_t j = 0;
  size_t compteur = 0;
  while (i < nbr_couples) {
    if (k > (pos) - j - 1) {
      compteur++;
      k = 1;
      j++;
    }
    char **aref = context_aref(&lfiles);
    const char *file1
      = is_std_file(aref[compteur]) ? STD_NAME : aref[compteur];
    const char *file2
      = is_std_file(aref[k + j]) ? STD_NAME : aref[k + j];
    if (dt->uni[i] == 0) {
      float jacard = 0;
      printf("%s\t%s\t%.4f", file1, file2, jacard);
    } else {
      float jacard = 1.f - ((float) (dt->inter[i])) / ((float) (dt->uni[i]));
      printf("%s\t%s\t%.4f", file1, file2, jacard);
    }
    printf("\n");
    i++;
    k++;
  }
  return 0;
}
