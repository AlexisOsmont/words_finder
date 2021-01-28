#include <stdio.h>
#include "hashtable.h"
#include "holdall.h"
#include "listdyn.h"

//  insertion_hashtable: cherche dans la table de hachage associé à ht une
//    clé égale à celle d'adresse word au sens de compar. Si une telle clé
//    existe, insère le word dans le holdall haword et insère (en queue) dans
//    la liste correspondante l'adresse de file_name donné en parametre s'il
//    n'est pas déjà dedans. Sinon crée une liste avec le file_name inseré queue
//    et l'ajoute à la table de hachage avec word pour clé. Retourne -1 en cas
//    d'echéc, 0 sinon.
int insertion_hashtable(hashtable *ht,
    holdall *haword, char *word, void *file_name);

//  insertion_hashtable_line: cherche dans la table de hachage associé à ht
//    une clé égale à celle d'adresse word au sens de compar. Si une telle clé
//    n'existe pas, insère (en queue) dans la liste correspondante le numéro de
//    ligne line donné en parametre s'il n'est pas deja dedans. Sinon crée une
//    liste avec line inseré en queue et l'ajoute à la table de hachage avec
//    word pour clé. Retourne -1 en cas d'echéc, 0 sinon.
int insertion_hashtable_line(hashtable *ht, char *word, long int line);

//  word_read_arg : lit les mots du fichier entré en paramètre, cherche si le
//    mot est déjà dans la table de hachage associé à ht. S'il existe déjà alors
//    insère la variable file_name en faisant appel à la fonction
//    insertion_hashtable, sinon insère le mot et le file_name en faisant appel
//    à la fonction insertion_hashtabe (avec allocation). La variable pointée
//    par up_low rèpresente une option. Retourne 0 si tout les mots ont été lus
//    et -1 en cas d'echéc. 
int word_read_arg(hashtable *ht, holdall *hafname,
    holdall *haword, char * file_name, char *up_low);

//  word_read_stdin : lit les mots sur l'éntrée standard, cherche si le
//    mot est déjà dans la table de hachage associé à ht_num. S'il existe déjà
//    alors insère le numéro de ligne courant en faisant appel à la fonction
//    insertion_hashtable_line.  La variable pointée par up_low rèpresente une
//    option.Retourne 0 si tout les mots ont été lus et -1 en cas d'echéc. 
int word_read_stdin(hashtable *ht_num, holdall *haword, char *up_low);

//  print : affiche tout les mots en commencant par les mots pointés par string.
//    Si l'option est différente de "sort", test chaque listes associées au mot
//    dans la hashtable associé à ht si le fichier courant est dedans, alors
//    affiche 'X' sinon '\t'. En fin de ligne affiche la liste associé au mot
//    dans la hashtable associé à ht_num. Renvoie rien.
void print(hashtable *ht, hashtable *ht_num, holdall *hafname,
    holdall *haword, char *string[], char *ouput_file, char *option);

//  is_real_punct : Renvoie true si le caractère pris en paramètre est
//    différent de '\'' et de '-' et false sinon. 
bool is_real_pucnt(char c);

int rfree__word(void *ptr);

