//  op.c : partie implantation du module op.

#include <sys/ioctl.h>
#include <unistd.h>
#include "op.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct opts {
  size_t length_max;
  bool ponct;
  bool graph;
  bool isAvl;
};

struct context {
  size_t entry_max;
  size_t cur_pos;
  char **aref;
};

bool is_std_file(const char *ref) {
  return strcmp(ref, STD_ENTRY_FILE) == 0;
}

//--- OPTS -----------------------------------------------------------------

opts *opts_empty() {
  opts *p = malloc(sizeof *p);
  if (p == nullptr) {
    return nullptr;
  }
  p->length_max = 0;
  p->ponct = false;
  p->graph = false;
  p->isAvl = false;
  return p;
}

void opts_dispose(opts **p) {
  free(*p);
  p = nullptr;
}

size_t opts_length_max(opts **optr) {
  if (*optr == nullptr) {
    return 0;
  }
  return (*optr)->length_max;
}

bool opts_isPonct(opts **optr) {
  if (*optr == nullptr) {
    return false;
  }
  return (*optr)->ponct;
}

bool opts_isGraph(opts **optr) {
  if (*optr == nullptr) {
    return false;
  }
  return (*optr)->graph;
}

bool opts_isAvl(opts **optr) {
  if (*optr == nullptr) {
    return false;
  }
  return (*optr)->isAvl;
}

//--- CONTEXT ----------------------------------------------------------

context *context_empty() {
  context *p = malloc(sizeof *p);
  if (p == nullptr) {
    return nullptr;
  }
  p->entry_max = ENTRY_MAX;
  p->cur_pos = 0;
  p->aref = malloc(sizeof(char *) * ENTRY_MAX);
  if (p->aref == nullptr) {
    free(p);
    return nullptr;
  }
  return p;
}

void context_dispose(context **cntxptr) {
  free((*cntxptr)->aref);
  free(*cntxptr);
  cntxptr = nullptr;
}

size_t context_entry(context **lfiles) {
  if (*lfiles == nullptr) {
    return 0;
  }
  return (*lfiles)->entry_max;
}

size_t context_pos(context **lfiles) {
  if (*lfiles == nullptr) {
    return 0;
  }
  return (*lfiles)->cur_pos;
}

char **context_aref(context **lfiles) {
  if (*lfiles == nullptr) {
    return nullptr;
  }
  return (*lfiles)->aref;
}

//--- LECTURE DES OPTIONS -----------------------------------------------

// isInt : Teste si la chaine de caractère pointé par arg contient uniquement
// des entiers.
// Temps : Linéaire en la taille de arg.
// Espace : Constant.
static bool isInt(const char *arg) {
  while (*arg != '\0') {
    if (!isdigit(*arg)) {
      return false;
    }
    arg += 1;
  }
  return true;
}

// add_file : Ajoute l'élément pointé par fname au context pointé par *lfiles,
// réalise une réallocation si nécessaire. Renvoie nullptr en cas d'échec,
// renvoie fname sinon.
// Temps : Constant.
// Espace : Au plus linéaire en la taille de (*lfiles)->aref.
static void *add_file(context **lfiles, char *fname) {
  if ((*lfiles)->cur_pos == (*lfiles)->entry_max) {
    char **p = realloc((*lfiles)->aref,
        (*lfiles)->entry_max * MUL * sizeof((*lfiles)->aref));
    if (*p == nullptr) {
      return nullptr;
    }
    (*lfiles)->aref = p;
    (*lfiles)->entry_max *= MUL;
  }
  (*lfiles)->aref[(*lfiles)->cur_pos] = fname;
  (*lfiles)->cur_pos += 1;
  return fname;
}

int stdin_read_set(int argc, char **argv, opts **optr, context **lfiles) {
  int k = 1;
  while (k < argc) {
    if (strncmp(argv[k], OPT_LONG, strlen(OPT_LONG)) == 0) { //OPTION LONGUE
      if (strcmp(argv[k], SELECT_FILE) == 0) { // -- SELECT_FILE
        if (k + 1 < argc) {
          k += 1;
          if (add_file(lfiles, argv[k]) == nullptr) {
            goto error_capacity;
          }
        } else {
          goto error_missing_argument;
        }
      } else if (strncmp(argv[k], OPT_LONG LONG_INIT,
          strlen(OPT_LONG LONG_INIT)) == 0) { //--INIT=VALUE
        char *value;
        if (strlen(argv[k]) == strlen(OPT_LONG LONG_INIT)) {
          if (k + 1 < argc) {
            k += 1;
            value = argv[k];
          } else {
            goto error_missing_argument;
          }
        } else {
          value = argv[k] + strlen(OPT_LONG LONG_INIT);
        }
        if (!isInt(value)) {
          goto error_forbiden_value;
        }
        size_t res = (size_t) strtoll(value, nullptr, 10);
        (*optr)->length_max = res;
      } else if (strcmp(argv[k], OPT_LONG LONG_PONCT) == 0) { //--PONCT
        (*optr)->ponct = true;
      } else if (strcmp(argv[k], OPT_LONG LONG_GRAPH) == 0) { //--GRAPH
        (*optr)->graph = true;
      } else if (strncmp(argv[k], OPT_LONG LONG_IMPLE,
          strlen(OPT_LONG LONG_IMPLE)) == 0) { //--words-processing
        char *value;
        if (strlen(argv[k]) == strlen(OPT_LONG LONG_IMPLE)) {
          if (k + 1 < argc) {
            k += 1;
            value = argv[k];
          } else {
            goto error_missing_argument;
          }
        } else {
          value = argv[k] + strlen(OPT_LONG LONG_IMPLE);
        }
        if (strcmp(value, TYPE_AVL) == 0) {
          (*optr)->isAvl = true;
        } else if (strcmp(value, TYPE_HASH) == 0) {
          (*optr)->isAvl = false;
        } else {
          goto error_missing_argument;
        }
      } else {
        printf("*** Error %s: Unrecognized option '%s'.\n",
            strrchr(argv[0], '/') + 1, argv[k]);
        goto error;
      }
    }
    //OPTION COURTE
    else if (strncmp(argv[k], OPT_SHORT, strlen(OPT_SHORT)) == 0) {
      if (strncmp(argv[k], OPT_SHORT SHORT_INIT,
          strlen(OPT_SHORT SHORT_INIT)) == 0) { // -i
        char *value;
        if (strlen(argv[k]) == strlen(OPT_SHORT SHORT_INIT)) {
          if (k + 1 < argc) {
            k += 1;
            value = argv[k];
          } else {
            goto error_missing_argument;
          }
        } else {
          value = argv[k] + strlen(OPT_SHORT SHORT_INIT);
        }
        if (!isInt(value)) {
          goto error_forbiden_value;
        }
        size_t res = (size_t) strtoll(value, nullptr, 10);
        (*optr)->length_max = res;
      } else if (strcmp(argv[k], STD_ENTRY_FILE) == 0) { // - =STD_ENTRY_FILE
        if (add_file(lfiles, argv[k]) == nullptr) {
          goto error_capacity;
        }
      } else if (strcmp(argv[k], OPT_SHORT SHORT_PONCT) == 0) { // -p
        (*optr)->ponct = true;
      } else if (strcmp(argv[k], OPT_SHORT SHORT_GRAPH) == 0) { // -g
        (*optr)->graph = true;
      } else if (strcmp(argv[k], OPT_SHORT SHORT_AVL) == 0) { // -a
        (*optr)->isAvl = true;
      } else if (strcmp(argv[k], OPT_SHORT SHORT_HASH) == 0) { // -h
        (*optr)->isAvl = false;
      } else if (strncmp(argv[k], OPT_SHORT SHORT_IMPLE,
          strlen(OPT_SHORT SHORT_IMPLE)) == 0) { // -w
        char *value;
        if (strlen(argv[k]) == strlen(OPT_SHORT SHORT_IMPLE)) {
          if (k + 1 < argc) {
            k += 1;
            value = argv[k];
          } else {
            goto error_missing_argument;
          }
        } else {
          value = argv[k] + strlen(OPT_SHORT SHORT_IMPLE);
        }
        if (strcmp(value, TYPE_AVL) == 0) {
          (*optr)->isAvl = true;
        } else if (strcmp(value, TYPE_HASH) == 0) {
          (*optr)->isAvl = false;
        } else {
          goto error_missing_argument;
        }
      } else {
        goto error_unknow_opt;
      }
    }
    //NOT AN OPTION
    else if (add_file(lfiles, argv[k]) == nullptr) {
      goto error_capacity;
    }
    k += 1;
  }
  int r = EXIT_SUCCESS;
  goto end;
error_unknow_opt:
  printf("*** Error %s: Unrecognized option '%s'.\n", strrchr(argv[0], '/') + 1,
      argv[k]);
  goto error;
error_missing_argument:
  printf("*** Error %s: Wrong or Missing value.\n", strrchr(argv[0], '/') + 1);
  goto error;
error_forbiden_value:
  printf("*** Error %s: Forbidden value '%s'.\n", strrchr(argv[0], '/') + 1,
      argv[k]);
  goto error;
error_capacity:
  printf("*** Error %s: Not enough memories\n", strrchr(argv[0], '/') + 1);
  goto error;
error:
  r = EXIT_FAILURE;
end:
  return r;
}

//--- Affichage
// -----------------------------------------------------------------------
 
void print_files(context *lfiles) {
  size_t k = 0;
  while (k < lfiles->cur_pos) {
	printf("\t%s",lfiles->aref[k]);
	k += 1;
  }
  printf("\n");
}

// print_str : Affiche la chaîne de caractère pointé par text puis un saut de
// ligne avant et après le texte avec un décalage vers la gauche de gap *
// REPR_TAB selon la longueur du terminal calculé par la fonction ioctl, en cas
// d'erreur de celle ci, il est considéré que la longueur du terminal vaut
// BASE_LENGTH.
// Temps : Linéaire en la longueur du texte.
// Espace : Constant.
static void print_str(const char *text, size_t gap) {
  size_t nbrcol = INIT_LENGTH_WORD;
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
    nbrcol = (size_t) w.ws_col;
  }
  size_t taille_ligne = 0;
  size_t taille_mot = 0;
  const char *start_word = text;
  const char *actual_char = text;
  size_t len = strlen(text);
  printf("%*s", REPR_TAB * (int) (gap), " ");
  for (size_t i = 0; i <= len; i++) {
    if (text[i] == ' ' || text[i] == '\0') {
      taille_ligne += taille_mot + 1;
      if (taille_ligne > nbrcol - TERMINAL_MARGIN) {
        printf("\n");
        printf("%*s", REPR_TAB * (int) (gap + 1), " ");
        taille_ligne = taille_mot + REPR_TAB * (gap + 1);
      }
      for (size_t k = 0; k < taille_mot + 1; k++) {
        printf("%c", start_word[k]);
      }
      start_word = actual_char;
      taille_mot = 0;
    } else {
      taille_mot++;
    }
    actual_char++;
  }
  printf("\n");
}

void print_help() {
  const char *usage = "Usage:  jdis [OPTION]... FILE1 FILE2 [FILE]...";
  const char *use
    = "Use:  Computes Jaccard dissimilarities of sets of words in FILEs.";
  const char *description
    =
      "Description:  For any pair of FILEs, the two FILEs in the pair is "
      "displayed first, followed by dissimilarity to four decimal places. A "
      "word is, by default, a maximum length sequence of characters that do not"
      " belong to the white-space characters set.";
  const char *input
    =
      "Input:  Read the standard input for any FILE that is '-' on command "
      "line. The standard input is displayed as a pair of double quotation "
      "marks in productions. '--' can be used to specify the next argument to "
      "be read as a file.";
  const char *info = "Program  Information :";
  print_str(usage, BASE_GAP);
  print_str(use, BASE_GAP);
  print_str(description, BASE_GAP);
  print_str(input, BASE_GAP);
  printf("\n");
  print_str(info, BASE_GAP);
  printf("%*s", REPR_TAB, " ");
  printf("%s%s, %s%s\n", OPT_SHORT, SHORT_HELP, OPT_LONG, LONG_HELP);
  const char *help = "Help:  Print this help message and exit.";
  print_str(help, BASE_GAP + 1);
  printf("\n");
  const char *imple_info = "Possible  Implementation :";
  print_str(imple_info, BASE_GAP);
  printf("%*s", REPR_TAB, " ");
  printf("%s%s, %s%s, %s%s VALUE, %s%sVALUE \n", OPT_SHORT, SHORT_AVL,
      OPT_SHORT, SHORT_HASH, OPT_SHORT, SHORT_IMPLE, OPT_LONG,
      LONG_IMPLE);
  const char *imple
    =
      "Imple:  Choose the implementation used by the program as VALUE, base "
      "implementation is hash table. Possible values : ";
  print_str(imple, BASE_GAP + 1);
  printf("%*s", (BASE_GAP + 2) * REPR_TAB, " ");
  printf("%s, %s\n\n", TYPE_HASH, TYPE_AVL);
  const char *input_ctrl = "Input  Control :";
  print_str(input_ctrl, BASE_GAP);
  printf("%*s", REPR_TAB, " ");
  printf("%s%s VALUE, %s%sVALUE\n", OPT_SHORT, SHORT_INIT, OPT_LONG, LONG_INIT);
  const char *initial
    =
      "Init:  Set the maximal number of significant initial letters for words "
      "to VALUE. 0 means without limitation. Default is 0.";
  print_str(initial, BASE_GAP + 1);
  printf("\n%*s", REPR_TAB, " ");
  printf("%s%s, %s%s\n", OPT_SHORT, SHORT_PONCT, OPT_LONG, LONG_PONCT);
  const char *ponct
    =
      "Ponct:  Make the punctuation characters play the same role as "
      "white-space characters in the meaning of words.";
  print_str(ponct, BASE_GAP + 1);
  printf("\n");
  const char *output_ctrl = "Output  Control :";
  print_str(output_ctrl, BASE_GAP);
  printf("%*s", REPR_TAB, " ");
  printf("%s%s, %s%s\n", OPT_SHORT, SHORT_GRAPH, OPT_LONG, LONG_GRAPH);
  const char *graph
    =
      "Graph:  Suppress normal output. Instead, for each word found in any "
      "FILE, jdis list the FILEs in which it does or does not appear. A header "
      "line indicates the FILE names: the name of the first FILE appears in "
      "the second column, that of the second in the third, and so on. For the "
      "subsequent lines, a word appears in the first column, followed by "
      "appearance marks: 'x' for yes, '-' for no. The list is "
      "lexicographically sorted. The locale specified by the environment "
      "affects the sort order. Set 'LC_ALL=C' or 'LC_COLLATE=C' to get the "
      "traditional sort order that uses native byte values.";
  print_str(graph, BASE_GAP + 1);
  const char *extra_info
    =
      "White-space and punctuation characters conform to the standard.";
  print_str(extra_info, BASE_GAP);
}
