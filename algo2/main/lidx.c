#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "holdall.h"  
#include "hashtable.h" 
#include "listdyn.h"
#include "list_line.h" 
#include "ctype.h"  
#include "word_implantation.h"
    
#define STR(s)  #s 
#define XSTR(s) STR(s)
 
#define STRING_LENGTH_MAX 63
#define NB_MAX_MOTS       20

#define LONG           "--" 
#define SHORT          "-"
#define EQUAL          "="
#define ESPACE         " "

#define LONG_HELP      "help"
#define SHORT_HELP     "h"
#define LONG_SORT      "sort"
#define SHORT_SORT     "S"
#define CASE           "case"
#define LONG_LOWER     "lower"
#define SHORT_LOWER     "l"
#define LONG_UPPER     "upper"
#define SHORT_UPPER    "u"
#define LONG_AS_IS     "as-is"
#define SHORT_AS_IS     "s"
#define LONG_OUPUT     "ouput"
#define SHORT_OUPUT    "o"
   
#define FUN_FAILURE 1
#define FUN_SUCCESS 0      
    
// Fonction de hashage pour le module hashtable.
static size_t str_hashfun(const char *s); 

// Renvoie sur la sortie standard l'aide à l'utilisation de ce programme.
void print_help(void);

// Renvoie la chaine donnée en parametre mais sans les caractéres de
//  ponctuation de la fin et du debut.
char *copy_second(char *str,size_t len);
char *copy_last(char *str,size_t len);

// Renvoie la chaine pointé en parametre avec chaque lettre en majuscule
// ou en minuscule.
char *min_to_maj(char *str);
char *maj_to_min(char *str);

//---Fonction main--------------------------------------------------------------

int main(int argc, char *argv[]) { 
   
  char *option = "";
  char *up_low = "";
  bool ouput = false;
  char *ouput_file;
             
  //  ----- Lecture des arguments -----------
  for (int k = 1; k < argc; ++k) {  
    
    // ---------------- aide ----------------
    if (strcmp(argv[k], LONG LONG_HELP) == 0
        || strcmp(argv[k], SHORT SHORT_HELP) == 0) {
      print_help(); 
      return EXIT_SUCCESS;
      
      // --------- trie --------------------
    } else if (strcmp(argv[k], LONG LONG_SORT) == 0
        || strcmp(argv[k], SHORT SHORT_SORT) == 0) {
      option = "sort";
  
      // ------mettre en majuscule --------------  
    } else if (strcmp(argv[k], SHORT SHORT_UPPER) == 0
    || strcmp(argv[k], LONG CASE EQUAL LONG_UPPER) == 0)  {
      up_low = "upper";

      // --------------- mettre en minuscule -------------------
    } else if (strcmp(argv[k], SHORT SHORT_LOWER) == 0
    || strcmp(argv[k], LONG CASE EQUAL LONG_LOWER) == 0) { 
      up_low = "lower";

      // --------------- laisser sans modification -------------------
    } else if (strcmp(argv[k], SHORT SHORT_AS_IS) == 0
    || strcmp(argv[k], LONG CASE EQUAL LONG_AS_IS) == 0) { 
      up_low = "";
    }
  }   
    
  // --- creation des tables de hachage et des holdalls ----------
  hashtable *ht = hashtable_empty((int (*)(const void *, const void *))strcmp,
      (size_t (*)(const void *))str_hashfun);
  hashtable *ht_num = hashtable_empty(
          (int (*)(const void *, const void *))strcmp,
              (size_t (*)(const void *))str_hashfun);
  holdall *hafname = holdall_empty(); 
  holdall *haword = holdall_empty();
  if (ht == NULL || haword == NULL || hafname == NULL || ht_num == NULL) {
    goto error_capacity; 
  }

  //  --- Initialisation des parametres-------
  char *string[NB_MAX_MOTS];   
  int k = 1;           
  size_t i = 0;      
            
  //  ---Lecture des texts de la ligne de commande : fichier . txt et string ---     
  while(k < argc) { 
    if (strcmp(argv[k], SHORT) == 0){
      k += 1;

      // ------ Lecture des arguments de type .txt --------
      if (word_read_arg(ht, hafname, haword, argv[k], up_low) != 0) {
        goto error_capacity;
      }     
    } else {
      char *t = strtok(argv[k], EQUAL);
      if (strcmp(t, LONG LONG_OUPUT) == 0) {
        t = strtok(NULL, EQUAL);  
        ouput = true;
        ouput_file = t;
      } else  if (strcmp(argv[k-1], SHORT SHORT_OUPUT) == 0) {
        ouput_file = argv[k];
        ouput = true;
        if (strcmp(ouput_file, "lidx.c") == 0 && ouput == true) {
          return EXIT_FAILURE;
        }
      } else {
        if (strcmp(argv[k], SHORT SHORT_SORT) != 0 &&
            strcmp(argv[k], LONG LONG_SORT) != 0 &&
             strcmp(argv[k], SHORT SHORT_OUPUT) != 0 &&
             strcmp(argv[k], SHORT SHORT_LOWER) != 0 &&
             strcmp(argv[k], SHORT SHORT_UPPER) != 0 &&
             strcmp(argv[k], SHORT SHORT_AS_IS) != 0  &&
             strcmp(argv[k], LONG CASE) != 0) {
                
          // --- Initialisation puis lecture des arguments de type string -----
          char *test = argv[k];
          char *d = ESPACE;   
          char *p = strtok(test, d); 
          while (p != NULL) {
            if (ispunct(p[0])) {   
              p = copy_second(p, strlen(p));
            }   
            if (ispunct(p[strlen(p)- 1])) {
              p = copy_last(p, strlen(p));
            }
            if (strcmp(up_low, "upper") == 0) {
              p = min_to_maj(p);
            }
            if (strcmp(up_low, "lower") == 0) {
              p = maj_to_min(p);
            }
            if (listdyn_search((listdyn *)haword, p) == NULL) {
              string[i] = p;
            } else {
                string[i] = (char *)listdyn_search((listdyn *)haword, p);
            }  
             i += 1;
            if (insertion_hashtable(ht, haword, p, "string") != 0) {
                goto error_capacity;
            }
            p = strtok(NULL, d);
          }  
        }     
      }      
    } 
  k += 1; 
  } 
  string[i] = "EOF"; // Fin de lecture de argv donc mettre EOF a la fin
        
  // -------- -Lecture final sur l'entrée standare ou grace a input ------------
  if (word_read_stdin(ht_num, haword, up_low) != 0) {
    goto error_capacity;
  }    
  
  // --------- Affichage ------------------  
    if (ouput) {
      print(ht, ht_num, hafname, haword, string, ouput_file, option);
    } else {
      print(ht, ht_num, hafname, haword, string,  "stdout", option);
    } 
  
  goto dispose; 
  return EXIT_SUCCESS; 
    
   // ------ Erreurs et désallocation---------
error_capacity: 
  fprintf(stderr, "*** Error: not enough memor y\n");
  goto dispose; 
        
dispose:    
  hashtable_dispose(&ht);
  hashtable_dispose(&ht_num);
  holdall_dispose(&hafname);
  holdall_dispose(&haword);
  return EXIT_FAILURE;         
}  
   
// str_hashfun : fonction de hashage pour utilisation de la hashtable 
size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }   
  return h;
}   

//  copy_second : fonction de copy d'un string sans les element repéré par
//     ispunct() en debut de string.
char *copy_second(char *str,size_t len) {
  char t[len];
  size_t n = 0;
 for (size_t k = 0; k < len; ++k) {
    if (!ispunct(str[k]) || !is_real_pucnt(str[k])) {
      t[n] = str[k];
      n += 1;
    }
  }  
  t[n] = '\0';
  strncpy(str, t, n + 1);
  return str;
}                            
    
//  copy_last : fonction de copy d'un string sans les element repéré par
//    ispunct() en fin de string.
char *copy_last(char *str,size_t len) {
  for (size_t k = 0; k < len/2; ++k) {
    if (ispunct(str[k]) && is_real_pucnt(str[k])){
        str[k] ='\0';
    } 
    str[k] = str[k];
  }
  return str;
}

//  min_to_maj : fonction changement de string de minuscule a majuscule
char *min_to_maj(char *str) {
  for (size_t i = 0; i < strlen(str); ++i) {
    str[i] = (char)toupper(str[i]);
  }                  
  return str;                    
}    

//  maj_to_min : fonction changement de string de majuscule a minuscile
char *maj_to_min(char *str) {
  for (size_t i = 0; i < strlen(str); ++i) {
    str[i] = (char)tolower(str[i]);
  }
  return str;      
}            

//  printf_help : fonction d'affichage de l'aide
void print_help(void) {
  fprintf(stdout, "Usage: ./lidx\n");
  fprintf(stdout, "word research in files\n\t");
  fprintf(stdout, "Guide\n");
  fprintf(stdout, "\tEnter the program name followed by :\n\t");
  fprintf(stdout, "\t- file_name.txt \t \"write a sentence\" \t word\n");
  fprintf(stdout, "for exemple you can enter without options :"
      " \"./lidx bonjour \"ca va bien?\" - abcd.txt\n");
  fprintf(stdout, "You can also enter options :\n\t");
  fprintf(stdout,
      "\t-S or --sort -> fix the order of the printed words in"
      "lexicografic order\n\t");
  fprintf(stdout,
      "\t-u or --case=upper -> fix all the words to uppers words\n\t");
  fprintf(stdout,
      "\t-l or --case=lower -> fix all the words to lowers words\n\t");
  fprintf(stdout,
      "\t-s or --case=as-is or nothing -> fix all the words to"
      " reading order\n\t");
  fprintf(stdout,
      "\t -o 'file_name'.txt or --ouput='file_name'.txt -> writes the"
      " result in the file (the file is created if it doesn't exist).\n\t");
  fprintf(stdout, "\tChoose --help or -h if you need help\n");
}
