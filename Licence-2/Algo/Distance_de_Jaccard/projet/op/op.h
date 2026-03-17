#include <stdlib.h>
#include <ctype.h>

#ifndef OP_H
#define OP_H

#endif

// Définition Macro

#define MUL 2
#define ENTRY_MAX 2
#define REPR_TAB 4
#define BASE_GAP 0
#define TERMINAL_MARGIN 5
#define INIT_LENGTH_WORD  2

#define OPT_SHORT "-"
#define OPT_LONG "--"

#define STD_ENTRY_FILE "-"

#define TYPE_AVL "avl-binary-tree"
#define TYPE_HASH "hash-table"

#define LONG_INIT "init="
#define LONG_PONCT "ponct"
#define LONG_GRAPH "graph"
#define LONG_IMPLE "words-processing="
#define LONG_HELP "help"
#define SELECT_FILE "--"

#define SHORT_INIT "i"
#define SHORT_PONCT "p"
#define SHORT_GRAPH "g"
#define SHORT_AVL "a"
#define SHORT_HASH "h"
#define SHORT_IMPLE "w"
#define SHORT_HELP "?"

//  struct opt, opt : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer les options.
typedef struct opts opts;

//  struct context, context : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer les fichiers.
typedef struct context context;

//  is_std_file : Teste si ref correspond à la macro constante STD_ENTRY_FILE.
extern bool is_std_file(const char *ref);

//  opts_empty : Alloue les ressources nécessaires à la gestion d'un élément de
//    type opts, renvoie nullptr en cas d'échec, renvoie la référence vers la
//    structure alloué sinon.
extern opts *opts_empty();

//  opt_dispose : sans effet si *optr vaut un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de la structure d'options associée à *optr
//    puis affecte un pointeur nul à *optr.
extern void opts_dispose(opts **optr);

//  opts_length_max : Renvoie l'attribut length_max de *optr, renvoie 0 si *optr
//    vaut nullptr.
extern size_t opts_length_max(opts **optr);

// opts_isPonct : Renvoie l'attribut ponct de *optr, renvoie faux si *optr vaut
// nullptr.
extern bool opts_isPonct(opts **optr);

// opts_isGraph : Renvoie l'attribut graph de *optr, renvoie faux si *optr vaut
// nullptr.
extern bool opts_isGraph(opts **optr);

// opts_isAvl : Renvoie l'attribut isAvl de *optr, renvoie faux si *optr vaut
// nullptr.
extern bool opts_isAvl(opts **optr);

//  context_empty : Alloue les ressources nécessaires à la gestion d'un élément
//    de type context, renvoie nullptr en cas d'échec, renvoie la référence vers la
//    structure alloué sinon.
extern context *context_empty();

//  context_dispose : sans effet si *cntxptr vaut un pointeur nul. Libère sinon
//    les ressources allouées à la gestion de la structure de context associée à
//    *cntxptr puis affecte un pointeur nul à *cntxptr.
extern void context_dispose(context **cntxptr);

// context_entry : Renvoie l'attribut entry_max de *lfiles, renvoie 0 si *lfiles
// vaut nullptr.
extern size_t context_entry(context **lfiles);

// context_pos : Renvoie l'attribut cur_pos de *lfiles, renvoie 0 si *lfiles
// vaut nullptr.
extern size_t context_pos(context **lfiles);

// context_aref : Renvoie l'attribut aref de *lfiles, renvoie nullptr si *lfiles
// vaut nullptr.
extern char **context_aref(context **lfiles);

//  stdin_read_set : Lis les arguments contenus dans argv et met à jour les
//    structures d'options et de contexte pointés respectivement par *optr et
//    *lfiles. Renvoie -2 en cas d'option inconnu, -1 en cas d'erreur d'allocation,
//    0 en cas d'argument inconnu et 1 en cas de succés.
extern int stdin_read_set(int argc, char **argv, opts **optr, context **lfiles);

//  print_files : Affiche l'ensemble des éléments contenu dans lfiles->aref.
extern void print_files(context *lfiles);

//  print_help : Affiche l'aide.
extern void print_help();
