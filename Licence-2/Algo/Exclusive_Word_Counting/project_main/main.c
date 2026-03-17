#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <locale.h>

#include "hashtable.h"
#include "holdall.h"

//Options
#define OPT__SHORT "-"
#define OPT__LONG "--"

#define TAILLE_MOT "i"
#define PONCTUATION "p"
#define RESTRICT "r"
#define LEXICO "l"
#define NUMERIC "n"
#define SORT "s"
#define REVERSE "R"
#define HELP "?"

//Macros
#define MAX_STR_LENGTH 5
#define CAPACITY_MUL 2

typedef struct opt opt;

struct opt {
  int taille_max;
  bool punct;
  char *file_r;
  bool lexico;
  bool numeric;
  bool tri_reverse;
};

typedef struct hashval hashval;

struct hashval {
  int filenum;
  size_t compteur;
};

size_t str_hashfun(const char *s);

//opt_init initialise la structure pour que toutes les options soient fausses
void opt_init(opt *op);

int opt_check(opt *op, char *s, char **tabfile, size_t *indice);

//rfree : libère les ressources associées à ptr et renvoie 0
int rfree(void *ptr);

//print_hold :
int print_hold(int *length_max, char *str, hashval *result);

//compLexico :
int compLexico(const char *s1, const char *s2);

int compNum(const int *s1, const int *s2, hashtable *h);

//get_number : si la chaîne pointée par s contient un nombre renvoie ce nombre
// sous forme d'entier sinon -1
int get_number(char *s);

//opt_sort_apply : vérifie les options liées au tri des mots et applique le
//tri concerné.
void opt_sort_apply(holdall *hold, opt *op, hashtable *h);

void fprintf_help();

int main(int argc, char *argv[]) {
  setlocale(LC_COLLATE, "");
  char *tabfile[argc];
  opt *op = malloc(sizeof(*op));
  if (op == NULL) {
    return EXIT_FAILURE;
  }
  opt_init(op);
  hashtable *h = hashtable_empty((int (*)(const void *, const void *))strcoll,
      (size_t (*)(const void *))str_hashfun);
  if (h == NULL) {
    goto opt_dispose;
  }
  holdall *hold2 = holdall_empty();
  if (hold2 == NULL) {
    goto hash_dispose;
  }
  holdall *hold = holdall_empty();
  if (hold == NULL) {
    goto hold2_dispose;
  }
  size_t indice = 0;
  //GESTIONS DES OPTIONS ET TABLEAU DE NOM DE FICHIERS
  for (int k = 1; k < argc; ++k) {
    char *s = argv[k];
    if (opt_check(op, s, tabfile, &indice) != 0) {
      fprintf(stderr, "Erreur : option non reconnue\n");
      goto hold_dispose;
    }
  }
  //LECTURE DANS LE FICHIER/AJOUT DANS LA HASHTABLE
  int length_max = 0;
  for (size_t k = 0; k < indice; ++k) {
    FILE *f;
    if (strcoll(tabfile[k], "stdin") != 0) {
      f = fopen(tabfile[k], "r");
    } else {
      f = stdin;
    }
    if (f == NULL) {
      goto hold_dispose;
    }
    char c;
    char *str = (char *) calloc(MAX_STR_LENGTH + 1, sizeof(char));
    if (str == NULL) {
      fprintf(stderr, "Erreur d'allocation");
      goto hold_dispose;
    }
    size_t iword = 0;
    size_t size_max = MAX_STR_LENGTH;
    while ((int) (c = (char) fgetc(f)) != EOF) {
      //test option de ponctuation
      if (!(op->punct && ispunct(c))) {
        if (iword < size_max) {
          //test si c'est un caractère d'espacement ou non (fin de mot)
          if (!isspace(c)) {
            if (op->taille_max == 0
                || (op->taille_max != 0 && (int) iword + 1 <= op->taille_max)) {
              //ajout de la lettre au mot
              str[iword] = c;
              ++iword;
            }
          } else {
            //le mot est complet donc ajout dans la table et
            //formation d'un mot vide
            str[iword] = '\0';
            if ((int) iword + 1 > length_max) {
              length_max = (int) iword + 1;
            }
            //gestion/update de la valeur associée au mot.
            hashval *srch = hashtable_search(h, str);
            //test si le mot est déjà présent
            if (srch != NULL) {
              // test si le numéro de fichier est différent
              if (srch->filenum != (int) k) {
                srch->filenum = -1;
              } else {
                srch->compteur += 1;
              }
              hashtable_add(h, str, srch);
              free(str);
            } else {
              holdall_put(hold, str);
              hashval *hv = malloc(sizeof(*hv));
              if (hv == NULL) {
                fprintf(stderr, "Erreur d'allocation");
                goto hold_dispose;
              }
              holdall_put(hold2, hv);
              hv->filenum = (int) k;
              hv->compteur = 1;
              hashtable_add(h, str, hv);
            }
            str = (char *) calloc(MAX_STR_LENGTH + 1, sizeof(char));
            if (str == NULL) {
              fprintf(stderr, "Erreur d'allocation");
              goto hold_dispose;
            }
            iword = 0;
            size_max = MAX_STR_LENGTH;
          }
        } else {
          //mot trop long (donc coupure et nouveau mot commencé)
          char *tmp = realloc(str, size_max * CAPACITY_MUL);
          if (tmp == NULL) {
            free(str);
            fprintf(stderr, "Erreur d'allocation");
            goto hold_dispose;
          }
          str = tmp;
          size_max *= CAPACITY_MUL;
          ungetc(c, f);
        }
      } else {
        while (!isspace(c) && (int) c != EOF) {
          c = (char) fgetc(f);
        }
        ungetc(c, f);
      }
    }
    if (f != stdin) {
      if (!feof(f)) {
        fprintf(stderr, "Erreur de lecture dans le fichier\n");
        goto hold_dispose;
      }
      if (fclose(f) != 0) {
        fprintf(stderr, "Erreur de lecture dans le fichier\n");
        goto hold_dispose;
      }
    } else {
      rewind(f);
    }
    fprintf(stdout, "\n");
    free(str);
  }
  //TRI ET AFFICHAGE
  if (op->file_r == NULL) {
    fprintf(stdout, "\t");
  } else {
    fprintf(stdout, "%-*s", length_max, op->file_r);
  }
  for (size_t k = 0; k < indice; ++k) {
    if (strcoll(tabfile[k], "stdin") == 0) {
      fprintf(stdout, "\t\"\"");
    } else {
      fprintf(stdout, "\t%s", tabfile[k]);
    }
  }
  fprintf(stdout, "\n");
  opt_sort_apply(hold, op, h);
  holdall_apply_context2(hold, h, (void *(*)(void *,
      void *))hashtable_search, &length_max,
      (int (*)(void *, void *, void *))print_hold);
  //LIBERATION DES RESSOURCES
hold_dispose:
  holdall_apply(hold, rfree);
  holdall_dispose(&hold);
hold2_dispose:
  holdall_apply(hold2, rfree);
  holdall_dispose(&hold2);
hash_dispose:
  hashtable_dispose(&h);
opt_dispose:
  free(op);
  return 0;
}

void opt_init(opt *op) {
  op->taille_max = 0;
  op->punct = false;
  op->lexico = false;
  op->numeric = false;
  op->tri_reverse = false;
  op->file_r = NULL;
}

int opt_check(opt *op, char *s, char **tabfile, size_t *indice) {
  //Options longues
  if (strncmp(s, OPT__LONG, strlen(OPT__LONG)) == 0) {
  }
  //options courtes
  else if (strncmp(s, OPT__SHORT, strlen(OPT__SHORT)) == 0) {
    if (strcmp(s, OPT__SHORT) == 0) {
      tabfile[*indice] = (char *) "stdin";
      *indice += 1;
    } else if (strncmp(s + strlen(OPT__SHORT), TAILLE_MOT,
        strlen(TAILLE_MOT)) == 0) {
      op->taille_max = get_number(s + strlen(OPT__SHORT) + strlen(TAILLE_MOT));
      if (op->taille_max == -1) {
        return -1;
      }
    } else if (strcmp(s + strlen(OPT__SHORT), PONCTUATION) == 0) {
      op->punct = true;
    } else if (strcmp(s + strlen(OPT__SHORT), RESTRICT) == 0) {
      //faire la gestion du -r
    } else if (strcmp(s + strlen(OPT__SHORT), LEXICO) == 0) {
      op->lexico = true;
      op->numeric = false;
    } else if (strcmp(s + strlen(OPT__SHORT), NUMERIC) == 0) {
      op->numeric = true;
      op->lexico = false;
    } else if (strcmp(s + strlen(OPT__SHORT), HELP) == 0) {
      fprintf_help();
    } else if (strcmp(s + strlen(OPT__SHORT), REVERSE) == 0) {
      op->tri_reverse = true;
    } else {
      return -1;
    }
  }
  //tableau de nom de fichiers
  else {
    tabfile[*indice] = s;
    *indice += 1;
  }
  return 0;
}

size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

int print_hold(int *length_max, char *str, hashval *result) {
  if (result->filenum != -1) {
    fprintf(stdout, "%-*s", *length_max, str);
    for (int k = 0; k < (result->filenum) + 1; ++k) {
      fprintf(stdout, "\t");
    }
    fprintf(stdout, "%zu\n", result->compteur);
  }
  return 0;
}

int compNum(const int *s1, const int *s2, hashtable *h) {
  hashval *s1val = hashtable_search(h, s1);
  hashval *s2val = hashtable_search(h, s2);
  if (s1val->compteur > s2val->compteur) {
    return 1;
  }
  if (s1val->compteur < s2val->compteur) {
    return -1;
  }
  return 0;
}

int compNum_reverse(const int *s1, const int *s2, hashtable *h) {
  hashval *s1val = hashtable_search(h, s1);
  hashval *s2val = hashtable_search(h, s2);
  if (s1val->compteur > s2val->compteur) {
    return -1;
  }
  if (s1val->compteur < s2val->compteur) {
    return 1;
  }
  return 0;
}

int compLexico(const char *s1, const char *s2) {
  return strcoll(s1, s2);
}

int compLexico_reverse(const char *s1, const char *s2) {
  int a = strcoll(s1, s2);
  if (a == -1) {
    return 1;
  }
  if (a == 1) {
    return -1;
  }
  return 0;
}

int get_number(char *s) {
  bool isnumber = true;
  int number = 0;
  int c = *s;
  while (!isspace(c) && (char) c != '\0' && isnumber) {
    if (!isdigit(c)) {
      isnumber = false;
    }
    number = number * 10 + (c - '0');
    s = s + 1;
    c = *s;
  }
  return isnumber ? number : -1;
}

void opt_sort_apply(holdall *hold, opt *op, hashtable *h) {
  if (op->lexico) {
    if (op->tri_reverse) {
      holdall_sort(hold,
          (int (*)(const void *, const void *))compLexico_reverse);
    } else {
      holdall_sort(hold, (int (*)(const void *, const void *))compLexico);
    }
  }
  if (op->numeric) {
    if (op->tri_reverse) {
      holdall_sort_context(hold, h,
          (int (*)(const void *, const void *, const void *))compNum_reverse);
    } else {
      holdall_sort_context(hold, h,
          (int (*)(const void *, const void *, const void *))compNum);
    }
  }
}

void fprintf_help() {
  fprintf(stdout,
      "Usage: xwc [OPTION]... [FILE]...\n\n\
 Exclusive word counting. Print the number of occurrences of each word that\
 appears in one and only one of given text FILES.\n Process words using a\
 hash table.\n\n");
  fprintf(stdout,
      "A word is, by default, a maximum length sequence of\
 characters that do not belong to the white-space characters set.\n\n");
  fprintf(stdout,
      "Results are displayed in columns on the standard output.\
 Columns are separated by the tab character. Lines are terminated by the\
 end-of-line character. A header line shows the FILE names: the name of the\
 first FILE appears in the second column, that of the second in the third,\
 and so on. For the following lines, a word appears in the first column, its\
 number of occurrences in the FILE in which it appears to the exclusion of\
 all others in the column associated with the FILE. No tab characters are\
 written on a line after the number of occurrences.\n\n");
  fprintf(stdout,
      "Read the standard input when no FILE is given or for any\
 FILE which is \" - \". In such cases, \" \" is displayed in the column\
 associated with the FILE on the header line.\n\n");
  fprintf(stdout,
      "The locale specified by the environment affects sort order.\
 Set 'LC_ALL=C' to get the traditional sort order that uses native byte\
 values.\n\n");
  fprintf(stdout,
      "Mandatory arguments to long options are mandatory for short\
 options too.\n\n");
  fprintf(stdout,
      "  " TAILLE_MOT "X\t" "Set the maximal number of\
  significant initial letters for words to X. 0 means without limitation.\
  Default is 0.\n");
  fprintf(stdout,
      "  " PONCTUATION "\t"
      "assign the same role to punctuation symbols as the characters in the\
 isspace class\n");
  fprintf(stdout,
      "  " LEXICO "\t" "lexicographical sort of the results in\
  ascending order\n");
  fprintf(stdout,
      "  " NUMERIC "\t" "numeric sort of the results in ascending order\n");
  fprintf(stdout,
      "  " REVERSE "\t" "sorts in descending order of the enabled sorting\n");
  fprintf(stdout, "  " HELP "\t" "display this help\n");
}
