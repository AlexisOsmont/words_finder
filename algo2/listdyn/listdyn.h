//  Partie interface du module listdyn

#ifndef LISTDYN__H
#define LISTDYN__H

#include <stdio.h>
#include <stdbool.h>

//  struct listdyn listdyn : structure capable de gérer une liste dynamique
//    simplement chainée dont les éléments sont des objets quelconques. La
//    création de la structure est confiée à la fonction listdyn_empty.
//  La structure ne stocke pas les objets mais des pointeurs vers ces objets.
//    Les pointeurs sont du type générique const void *.
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans la structure. Si des opérations d'allocation dynamique
//    sont effectuées, elles le sont pour la gestion propre de la structure,
//    et en aucun cas pour réaliser des copies ou des destructions d'objets
typedef struct listdyn listdyn;

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type listdyn * n'est pas l'adresse d'un objet préalablement renvoyé par
//    listdyn_empty et non révoqué depuis par listdyn_dispose.
//    Cette règle ne souffre que d'une seule exception : listdyn_dispose
//    tolère que la déréférence de son argument ait pour valeur NULL

//  listdyn_empty : crée un objet de type listdyn. La liste est initialement
//    vide. Renvoie NULL en cas de dépassement de capacité, un pointeur vers
//    l'objet en cas de succès.
extern listdyn *listdyn_empty();

//  listdyn_is_empty : renvoie true ou false selon que la liste associée à s est
//    vide ou non
extern bool listdyn_is_empty(const listdyn *s);

//  listdyn_insert_tail, listdyn_insert_head: Ajout en queue (tête) de la liste
//    associé à s l'élément d'adresse ptr. Renvoie zéro en cas de succès, une
//    valeur non nulle si xptrvaut NULL ou en cas de dépassement de capacité
extern int listdyn_insert_tail(listdyn *s, void *ptr);
extern int listdyn_insert_head(listdyn *s, void *xptr);

//  listdyn_length : renvoie la longueur de la liste associé à s
extern size_t listdyn_length(const listdyn *s);

//  listdyn_index_value : cherche dans la liste pointée par s l'élément d'indice
//    n. Renvoie NULL si k est strictement supérieur à la longueur de s ou si s
//    est vide. Renvoie l'adresse de l'élément trouvée sinon
extern void *listdyn_index_value(const listdyn *s, size_t n);

//  listdyn_comparaison : compare les éléments des deux listes associées à s1 et
//    s2 selon l'ordre lexicographique. Renvoie une valeur strictement
//    inférieure, égale ou strictement supérieure à zéro selon que le premier
//    objet est strictement inférieur, égal ou strictement supérieur au second
extern int listdyn_comparison(listdyn *s1, listdyn *s2);

//  listdyn_hashfun : l'une des fonctions de pré-hachage conseillées par
//    Kernighan et Pike traduite pour les listes
extern size_t listdyn_hashfun(const listdyn *s);

//  listdyn_key : crée une liste de longueur order avec tous les éléments qui
//    vallent la valeur pointée par xptr. Renvoie NULL en cas de dépassement de
//    capacité ou si xptr vaut NULL. Renvoie un pointeur vers l'objet qui gère
//    la liste créée sinon
extern listdyn *listdyn_key(void *xptr, size_t order);

//  listdyn_replace : crée une liste qui contient les mêmes éléments que celui
//    associé s sans le premier et ajoute en queue l'élément d'adresse xptr.
//    Renvoie NULL en cas de dépassement de capacité, si la liste associé à s
//    est vide ou si xptr vaut NULL. Renvoie un pointeur vers l'objet qui gère
//    la liste créée sinon
extern listdyn *listdyn_replace(const listdyn *s, void *xptr);

//  listdyn_fput : écrit sur le flot contrôlé par l'objet pointé par stream les
//    éléments de la liste associée à s. Les éléments sont écrits séparés par
//    une tabulation. Le délimiteur gauche est vide. Renvoie zéro en cas de
//    succès, une valeur non nulle en cas d'échec
extern int listdyn_fput(const listdyn *s, FILE *stream);

//  listdyn_reverse : crée une liste qui contient les mêmes éléments que celui
//    associé à s mais à l'ordre inverse (insertion en tête). Renvoie NULL en
//    cas de dépassement de capacité. Renvoie un pointeur vers l'objet qui gère
//    la liste créée sinon
extern listdyn *reverse(listdyn *src);

//  listdyn_dispose : si *sptr ne vaut pas NULL, libère les ressources allouées
//   à *sptr ainsi qu'à la liste associée puis affecte la valeur NULL à *sptr
extern void listdyn_dispose(listdyn **sptr);


//  lisdyn_search : recherche dans la liste associé s la première clé égale
//    au pointeur *xptr au sens de strcmp. Renvoie NULL si aucune clé n'a été
//    trouvé, renvoie l'adresse de la valeur associée sinon
extern void *listdyn_search(const listdyn *s, void *xptr);

//  listdyn_remove : recherche dans le multi-ensemble associé à s un élément
//    égal à celui d'adresse xptr au sens de compar. Si un tel élément existe,
//    le retire du multi-ensemble. Renvoie NULL si xptr vaut NULL
//    ou si aucun élément de la sorte n'existe. Renvoie sinon l'adresse de
//    l'élément trouvé
extern void *lisdyn_remove(listdyn *s, void *xptr);

#endif
