//  Partie interface du module list_line

#ifndef LIST_LINE__H
#define LIST_LINE__H

#include <stdio.h>
#include <stdbool.h>

//  struct list_line list_line : structure capable de gérer une liste dynamique
//    simplement chainée dont les éléments sont des objets quelconques. La
//    création de la structure est confiée à la fonction list_line_empty.
//  La structure ne stocke pas les objets mais des pointeurs vers ces objets.
//    Les pointeurs sont du type générique const void *.
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans la structure. Si des opérations d'allocation dynamique
//    sont effectuées, elles le sont pour la gestion propre de la structure,
//    et en aucun cas pour réaliser des copies ou des destructions d'objets
typedef struct list_line list_line;

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type list_line * n'est pas l'adresse d'un objet préalablement renvoyé par
//    list_line_empty et non révoqué depuis par list_line_dispose.
//    Cette règle ne souffre que d'une seule exception : list_line_dispose
//    tolère que la déréférence de son argument ait pour valeur NULL

//  list_line_empty : crée un objet de type list_line. La liste est initialement
//    vide. Renvoie NULL en cas de dépassement de capacité, un pointeur vers
//    l'objet en cas de succès.
extern list_line *list_line_empty();

//  list_line_is_empty : renvoie true ou false selon que la liste associée à s est
//    vide ou non
extern bool list_line_is_empty(const list_line *s);

//  list_line_insert_tail, list_line_insert_head: Ajout en queue (tête) de la liste
//    associé à s l'élément d'adresse ptr. Renvoie zéro en cas de succès, une
//    valeur non nulle si xptrvaut NULL ou en cas de dépassement de capacité
extern int list_line_insert_tail(list_line *s, long int ptr);
extern int list_line_insert_head(list_line *s, long int xptr);

//  list_line_length : renvoie la longueur de la liste associé à s
extern size_t list_line_length(const list_line *s);

//  list_line_index_number : cherche dans la liste pointée par s l'élément d'indice
//    n. Renvoie NULL si k est strictement supérieur à la longueur de s ou si s
//    est vide. Renvoie l'adresse de l'élément trouvée sinon
extern long int list_line_index_number(const list_line *s, size_t n);

//  list_line_dispose : si *sptr ne vaut pas NULL, libère les ressources allouées
//   à *sptr ainsi qu'à la liste associée puis affecte la valeur NULL à *sptr
extern void list_line_dispose(list_line **sptr);

//  list_line_search : recherche dans la liste associé s la première clé égale
//    au pointeur *xptr au sens de strcmp. Renvoie NULL si aucune clé n'a été
//    trouvé, renvoie l'adresse de la valeur associée sinon
extern long int list_line_search(const list_line *s, long int xptr);

#endif
