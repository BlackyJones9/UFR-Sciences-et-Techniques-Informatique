//  holdall.h : partie interface du module holdall (fourretout) avec extension
//    optionnelle.
//
//  Un fourretout permet la mémorisation d'une liste de références d'objets
//    quelconques, l'exécution de fonctions sur les objets repérés par les
//    références dans la liste selon l'ordre dans lequel les références y
//    figurent ainsi que la réorganisation de la liste.

//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

//  Fonctionnement général :
//  - la structure de données ne stocke pas d'objets mais des références vers
//      ces objets. Les références sont du type générique « void * » ;
//  - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
//  - les fonctions qui possèdent un paramètre de type « holdall * » ou
//      « holdall ** » ont un comportement indéterminé lorsque ce paramètre ou
//      sa déréférence n'est pas l'adresse d'un contrôleur préalablement
//      renvoyée avec succès par la fonction holdall_empty et non révoquée
//      depuis par la fonction holdall_dispose ;
//  - l'insertion d'une référence à un fourretout ne donne lieu à aucun test de
//      la valeur de la référence ;
//  - l'endroit où a lieu l'insertion d'une référence à un fourretout n'est pas
//      spécifié.

//  L'extension est formée des éventuelles déclarations et définitions qui
//    figurent aux lignes 112-115.

//  Les identificateurs introduits par l'extension ainsi que les identificateurs
//    de macro HOLDALL_EXT et WANT_HOLDALL_EXT sont réservés pour être utilisés
//    comme indiqué ci-après :
//  - lorsque le module peut mettre l'intégralité de l'extension à disposition
//      de l'utilisateurice, la macro HOLDALL_EXT doit être définie dans
//      l'en-tête "holdall_ip.h" ;
//  - dans le cas contraire, la macro HOLDALL_EXT n'est pas définie dans
//      l'en-tête "holdall_ip.h" et ne doit pas l'être au sein de l'unité de
//      traduction de prétraitement  ;
//  - la visibilité et l'implantation de l'extension ne sont effectives qu'à la
//      double condition que la macro HOLDALL_EXT soit définie dans l'en-tête
//      "holdall_ip.h" et que la macro WANT_HOLDALL_EXT soit définie par
//      l'utilisateurice ;
//  - aucune modification du statut défini/non défini des macros HOLDALL_EXT et
//      WANT_HOLDALL_EXT ne doit intervenir au sein de l'unité de traduction de
//      prétraitement après que le présent en-tête ait été inclus pour la
//      première fois.

#ifndef HOLDALL__H
#define HOLDALL__H

//- STANDARD --v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v

#include <stdlib.h>

//  struct holdall, holdall : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un fourretout.
typedef struct holdall holdall;

//  holdall_empty : tente d'allouer les ressources nécessaires pour gérer un
//    nouveau fourretout initialement vide. Renvoie un pointeur nul en cas de
//    dépassement de capacité. Renvoie sinon un pointeur vers le contrôleur
//    associé au fourretout.
extern holdall *holdall_empty();

//  holdall_dispose : sans effet si *haptr vaut un pointeur nul. Libère sinon
//    les ressources allouées la gestion du fourretout associé à *haptr puis
//    affecte un pointeur nul à *haptr.
extern void holdall_dispose(holdall **haptr);

//  holdall_put : tente d'insérer ref dans le fourretout associé à ha. Renvoie
//    une valeur non nulle en cas de dépassement de capacité. Renvoie sinon
//    zéro.
extern int holdall_put(holdall *ha, void *ref);

//  holdall_count : renvoie le nombre d'insertions effectuées avec succès dans
//    le fourretout associé à ha depuis sa création.
extern size_t holdall_count(holdall *ha);

//  holdall_apply, holdall_apply_context, holdall_apply_context2 : parcourt le
//    fourretout associé à ha en appelant (respectivement) fun(ref),
//    fun2(ref, fun1(context, ref)), fun2(context2, ref, fun1(context1, ref))
//    pour chacune des références ref dans l'ordre dans lequel elles figurent
//    dans le fourretout. Si, lors du parcours, la valeur de l'appel n'est pas
//    nulle, l'exécution de la fonction prend fin et la fonction renvoie cette
//    valeur. Sinon, la fonction renvoie zéro.
extern int holdall_apply(holdall *ha, int (*fun)(void *));
extern int holdall_apply_context(holdall *ha,
    void *context, void *(*fun1)(void *context, void *ref),
    int (*fun2)(void *ref, void *resultfun1));
    
extern int holdall_apply_context2(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ref),
    void *context2, int (*fun2)(void *context2, void *ref, void *resultfun1));
//- STANDARD --^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^

#undef HOLDALL_EXT
#include "holdall_ip.h"

#if defined HOLDALL_EXT && defined WANT_HOLDALL_EXT

//- EXTENSION -v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v

//  Sont ajoutées au standard quelques fonctions qui peuvent être utiles.
//  Certaines de ces fonctions ne peuvent être exprimées selon le standard sans
//    avoir affaire à un problème du dépassement de capacité que n'est pas
//    terminal. Il est exigé de leurs implantations qu'elles évitent cet écueil.
//  Les autres pourraient être exprimées selon le standard sans avoir affaire
//    à un problème de dépassement de capacité ou, si elles ont affaire à lui,
//    de manière terminale uniquement. Il est attendu que leurs implantations se
//    montrent au moins aussi performantes que toute expression selon le
//    standard.

//  holdall_sort : trie le fourretout associé à ha selon la fonction compar
//    appliquée aux références qui y ont insérées avec succès.
extern void holdall_sort(holdall *ha,
    int (*compar)(const void *, const void *));

//- EXTENSION -^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^

#endif

#endif
