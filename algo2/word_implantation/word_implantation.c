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

// -----------------------------------------------------------------------------

#define FUN_FAILURE 1
#define FUN_SUCCESS 0

#define STRING_LENGTH_MAX 63
  
// ---fonction d'insertion des mots dans la ht ---------------------------------

int insertion_hashtable(hashtable *ht,
 holdall *haword, char *word, void *file_name) {

  //  si espace alors test si le mot est deja dans la hashtabl
  listdyn *htfile_list = (listdyn *)hashtable_search(ht,(char *) word);
  if (htfile_list == NULL) {
    if (holdall_put(haword, word) != 0) {
      return FUN_FAILURE;
    }
     
  //  si pas dans la hash table alors ajouté avec hashtabble_add
  //  en creant une liste vide et mettant le word dedans
    listdyn *file_list = listdyn_empty();
    if (file_list == NULL) {
      return FUN_FAILURE;
    }
    if (listdyn_insert_tail(file_list, file_name) != 0) {
      listdyn_dispose(&file_list);
     return FUN_FAILURE;
    }
    if (hashtable_add(ht, word, file_list) == NULL) {
      listdyn_dispose(&file_list);
     return FUN_FAILURE;
    }
  } else {
  //  word deja dans la hashtable donc juste ajouter le nom du fiche
  //  si il est pas deja dedans
    if (listdyn_search(htfile_list, file_name) == NULL) {
      if (listdyn_insert_tail(htfile_list, file_name) != 0) {
        listdyn_dispose(&htfile_list);
        return FUN_FAILURE;
      }
    }
  }
  return FUN_SUCCESS;
}

// ---fonction d'insertion des numeros dans la ht_num --------------------------

int insertion_hashtable_line(hashtable *ht, char *word, long int line) {

  //  si espace alors test si le mot est deja dans la hashtabl
  list_line *htfile_num = (list_line *)hashtable_search(ht, word);
  if (htfile_num == NULL) {
    
  //  si pas dans la hash table alors ajouté avec hashtabble_add
  //  en creant une liste vide et mettant le word dedans
    list_line *line_list = list_line_empty();
    if (line_list == NULL) {
      return FUN_FAILURE;
    }
    if (list_line_insert_tail(line_list, line) != 0) {
      list_line_dispose(&line_list);
     return FUN_FAILURE;
    }
    if (hashtable_add(ht, word, line_list) == NULL) {
      list_line_dispose(&line_list);
     return FUN_FAILURE;
    }
  } else {
    
  //  word deja dans la hashtable donc juste ajouter le nom du fiche
  //  si il est pas deja dedans
    if (list_line_search(htfile_num, line) == FUN_FAILURE) {
      if (list_line_insert_tail(htfile_num, line) != 0) {
        list_line_dispose(&htfile_num);
        return FUN_FAILURE;
      }
    }
  }
  return FUN_SUCCESS;
}

// ------- insertion des mots recherchés  --------------------------------------

int word_read_arg(hashtable *ht, holdall *hafname,
  holdall *haword, char * file_name, char *up_low) {
  char t[STRING_LENGTH_MAX];   
  size_t char_count = 0;
  int l = -1;
  size_t length = STRING_LENGTH_MAX;
  FILE *fichier = fopen(file_name, "r");
  if (listdyn_search((listdyn *)hafname, file_name) == NULL) {
    if (holdall_put(hafname, file_name) != 0) {
      fclose(fichier);  
      return FUN_FAILURE; 
    }
  } 
  int c;
  if (strcmp(up_low, "upper") == 0) {
    c = toupper(fgetc(fichier));
  } else if (strcmp(up_low, "lower") == 0) {
    c = tolower(fgetc(fichier));
  } else {
    c = fgetc(fichier);
  }
  
  // initalisation et debut de boucle de lecture du fichier 
  while (1) {
      if (isspace(c) || c == EOF) {
        if (char_count > 0) {
          t[char_count] = '\0';
        length = STRING_LENGTH_MAX;
        
  //  réinitialisation de la longueur du buffer
  //  si espace alors ajout a la hashtable grace a la fonction
        if (hashtable_search(ht, t) == NULL) {
          char *word = malloc(sizeof (char) * (strlen(t) + 1));
          if (word == NULL) {
            fclose(fichier);
            return FUN_FAILURE;
          }
          strcpy(word, t);
          if (insertion_hashtable(ht,haword, word, file_name) != 0) {
            free(word);
            fclose(fichier);
            return FUN_FAILURE;
          }
        } else {
          if (insertion_hashtable(ht,haword, t, file_name) != 0) {
            fclose(fichier);
            return FUN_FAILURE;
          }
        }
        char_count = 0;
      }
      if (c == EOF) {
        fclose(fichier);
        return FUN_SUCCESS;
      }
      l = -1;
    } else {
      if (char_count >= length) {
        fprintf(stderr, "*** Warning: String '%s...' possibly sliced.\n",
              t);
        length *= 2;             // on multiplie donc sa longueur par 2
      }
      if ((!ispunct(c) || char_count != 0) && (char)c != EOF) {
        if (!ispunct(c)) {
          t[char_count] = (char)c;
          ++char_count;
        } else {
          l = c;
        }
      }
    }
    if (strcmp(up_low, "upper") == 0) {
      c = toupper(fgetc(fichier));
    } else if (strcmp(up_low, "lower") == 0) {
      c = tolower(fgetc(fichier));
    } else {
      c = fgetc(fichier);
    }
    if (!isspace(c) && l != -1 && !ispunct(c) && (char)c != EOF) {
      t[char_count] = (char)l;
      ++char_count;
        l = -1;    
    }
  }
  fclose(fichier);
}

// -- insertion des nums de ligne de stdin--------------------------------------

int word_read_stdin(hashtable *ht_num,holdall *haword,
    char *up_low) {
  size_t char_count = 0;
  int l = -1;
  size_t length = STRING_LENGTH_MAX;
  char t[STRING_LENGTH_MAX];
  int m;
  long int number = 1;
  if (strcmp(up_low, "upper") == 0) {
    m = toupper(fgetc(stdin));
  } else if (strcmp(up_low, "lower") == 0) {
    m = tolower(fgetc(stdin));
  } else {
    m = fgetc(stdin);
  }
  while(1) {
    if (isspace(m) || m == EOF) {
      if (char_count > 0) {
        t[char_count] = '\0';
        length = STRING_LENGTH_MAX;

  //si espace alors ajout a la hashtable grace a la fonction;
        char *word = listdyn_search((listdyn *)haword, t);
        if (word != NULL) {
          if (insertion_hashtable_line(ht_num, word, number) != 0) {
            return FUN_FAILURE;
          }
        }
        char_count = 0; //réinitialisation du compteur de caractère.
      }
      if ((char)m == '\n') {
        number += 1;
      }
      if (m == EOF) {
        return FUN_SUCCESS;
      }
      l = -1;
    } else {
      if (char_count >= length) {
        fprintf(stderr, "*** Warning: String '%s...' possibly sliced.\n",
              t);
        length *= 2;
        //buffer = realloc(buffer, sizeof (int) * (length + 1));
      }
      if (!ispunct(m) && (char)m != EOF) {
        t[char_count] = (char)m;
        ++char_count;
      }
      l = m;
    }  
    if (strcmp(up_low, "upper") == 0) {
      m = toupper(fgetc(stdin));
    } else if (strcmp(up_low, "lower") == 0) {
      m = tolower(fgetc(stdin));
    } else {
      m = fgetc(stdin);
    }
    if (!isspace(m) && !ispunct(m) && ispunct(l) && (char)m != EOF) {
      if (!is_real_pucnt((char)l)) {
        t[char_count] = (char)l;
        ++char_count;
      }
    }
  }
}

#define LONG_INT_MAX 40
#define NB_CHAR_MAX_TAB 8
#define NB_FICHIER_MAX_TAB_BIS 2
#define NB_FICHIER_MAX_TAB 1

// ----- affichage sur un fichier .txt crée avant ------------------------------

void print(hashtable *ht, hashtable *ht_num, holdall *hafname,
    holdall *haword, char *string[], char *ouput_file, char *option) {
  FILE *fichier;
  if (strcmp (ouput_file, "stdout") == 0) {
    fichier = stdout;
  } else {
    fichier = fopen(ouput_file, "w+");
  }
  size_t k = 0;
  if (fputc('\t', fichier) == EOF) {
    goto close;
    return;
  }
  // --- test des options d'affichage -------
  if (strcmp(option, "sort") == 0) { 
      holdall_sort(haword, (int (*)(const void *, const void *))strcmp);
  }
  if (string != NULL) { 
    if (fputc('\t', fichier) == EOF) {
      goto close;
      return;
    }
  }
  // --- affichage de la ligne des noms de fichiers --------
  listdyn_fput((listdyn *)hafname, fichier);

  // ---- si string est non vide et l'affichage non trié -------
  if (strcmp(option, "sort") != 0) { 
    if (string != NULL) {
      while(strcmp(string[k],"EOF") != 0) {
        if (fputs((char *)string[k], fichier) == EOF) {
          goto close;
          return;
        }
        if (fputc('\t', fichier) == EOF) {
          goto close;
          return;
        } 
        const listdyn *p = hashtable_search(ht, string[k]);
        if (strlen(string[k]) < NB_CHAR_MAX_TAB) {
          if (fputs("\tX", fichier) == EOF) {
            goto close;
            return;
          }
        } else {
          if (fputc('X', fichier) == EOF) {
            goto close;
            return;
          }
        }
        size_t n =  holdall_count(hafname);
        for (size_t i = 0; i < n; ++i) {
          char *file_name = (char *)listdyn_index_value((listdyn *)hafname, i);
          if (listdyn_search(p, file_name) != NULL) {
            if (fputs("\t\tX", fichier) == EOF) {
              goto close;
              return;
            }
          } else {
            if (fputs("\t\t", fichier) == EOF) {
              goto close;
              return;
            }
          }
        }
        if (strlen(string[k]) < NB_CHAR_MAX_TAB) {
          if (fputc('\t', fichier) == EOF) {
            goto close;
            return;
          } 
        }
        if (fputc('\t', fichier) == EOF) {
          goto close;
          return;
        }
        listdyn_dispose((listdyn **)&p);
        
        // ----- affichage des numeros de ligne si q non vide ------------
        const list_line *q = hashtable_search(ht_num, string[k]);
        if (q != NULL) {
          size_t length = list_line_length(q);
          for (size_t j = 0; j < list_line_length(q); ++j) {
            long int li = list_line_index_number(q, j);
            if (strlen(string[k]) >= NB_CHAR_MAX_TAB) {
              if (fputc('\t', fichier) == EOF) {
                return;
              }
            }
            char t[LONG_INT_MAX];
            sprintf(t, "%ld",li);
            if (length == 1) {
              if (fputs(t, fichier) == EOF) {
                goto close;
                return;
              }
              if (fputc(' ', fichier) == EOF) {
                goto close;
                return;
              }
            } else {
              if (fputs(t, fichier) == EOF) {
                goto close;
                return;
              }
              if (fputs(", ", fichier) == EOF) {
                goto close;
                return;
              }
            }
            --length;
          }
        }
        list_line_dispose((list_line **)&q);
        if (fputs("¶\n", fichier) == EOF) {
          goto close;
          return;
        }
        lisdyn_remove((listdyn *)haword, string[k]);
        k += 1;
      }
    } 
  }

  // ---- affichage si trié || tout hors string --------
  char *file_name;
  k = 0;
    while(!listdyn_is_empty((listdyn *)haword)) {
      char *t = (char *)listdyn_index_value((listdyn *)haword, 0);
      if (fputs(t, fichier) == EOF) {
        return;
      }
      if (fputc('\t', fichier) == EOF) {
        goto close;
        return;
      }
      const listdyn *p = hashtable_search(ht, t);
      for (size_t i = 0; i < holdall_count(hafname); ++i) {

        // ---- si string est non vide et l'affichage trié (cas special sort)---
        if (string != NULL && strcmp(option,"sort") == 0) {
          if (strlen(t) < NB_CHAR_MAX_TAB && i <= NB_FICHIER_MAX_TAB) {
            if (fputc('\t', fichier) == EOF) {
              goto close;
              return;
            }
          }
          if (listdyn_search(p, "string") != NULL) {
            if (fputs("X", fichier) == EOF) {
              goto close;
              return;
            }
          } else {
            if (strcmp(option,"sort") != 0 && i <= NB_FICHIER_MAX_TAB) {
              if (fputs("\t", fichier) == EOF) {
                goto close;
                return;
              }
            }
          }
        }
        // ---------------------------------------------------------------------

        // ----------- affchage des colonnes hors string --------------
        if (strlen(t) < NB_CHAR_MAX_TAB && strcmp(option,"sort") != 0 &&
            i <= NB_FICHIER_MAX_TAB) {
          if (fputs("\t", fichier) == EOF) {
            goto close;
            return;
          } 
        }
        file_name = listdyn_index_value((listdyn *)hafname, i);
        if (listdyn_search(p, file_name) != NULL) {
          if (i >= NB_FICHIER_MAX_TAB && strlen(t) < NB_CHAR_MAX_TAB) {
            if (fputs("\tX", fichier) == EOF) {
              goto close;
              return;
            }
          } else {
            if (fputs("\t\tX", fichier) == EOF) {
              goto close;
              return;
            }
          }
        } else {
          if (i >= NB_FICHIER_MAX_TAB && strlen(t) < NB_CHAR_MAX_TAB) {
            if (fputc('\t', fichier) == EOF) {
              goto close;
              return;
            }
          } else {
            if (fputs("\t\t", fichier) == EOF) {
              goto close;
            return;
            }
          }
        }
      }
      if (fputc('\t', fichier) == EOF) {
        goto close;
        return;
      } 
      if (strlen(t) >= NB_CHAR_MAX_TAB &&
        holdall_count(hafname) >= NB_FICHIER_MAX_TAB_BIS) {
        if (fputs("\t", fichier) == EOF) {
          goto close;
          return;
        } 
      }
      if (fputc('\t', fichier) == EOF) {
        goto close;
        return;
      }
      
      // ----- affichage des numeros de ligne si q non vide ------------
      const list_line *q = hashtable_search(ht_num, t);
      if (q != NULL) {
      size_t length = list_line_length(q);
      for (size_t j = 0; j < list_line_length(q); ++j) {
        long int ln = list_line_index_number(q, j);
        if (strlen(t) >= NB_CHAR_MAX_TAB) {
          if (fputc('\t', fichier) == EOF) {
            goto close;
            return;
          }
        }
        char lg[LONG_INT_MAX];
        sprintf(lg, "%ld",ln);
        if (length == 1) {
          if (fputs(lg, fichier) == EOF) {
            goto close;
            return;
          }
          if (fputc(' ', fichier) == EOF) {
            goto close;
            return;
          }
        } else {
          if (fputs(lg, fichier) == EOF) {
            goto close;
            return; 
          }
          if (fputs(", ", fichier) == EOF) {
            goto close;
            return;
          }
        }
        --length;
      }
    }
    if (fputs("¶\n", fichier) == EOF) {
      goto close;
      return;
    }
    list_line_dispose((list_line **)&q);
    if (listdyn_search(p, "string") != NULL || strcmp(option,"sort") != 0) {
      lisdyn_remove((listdyn *)haword, t);
    } else {
      free(lisdyn_remove((listdyn *)haword, t));
    }
    listdyn_dispose((listdyn **)&p);
    k += 1;
  }
  goto close;
 
close :
  if (strcmp(ouput_file,"stdout") != 0) {
    fclose(fichier);
    return;
  }
}// ------------------------- fin affichage ------------------------------------

bool is_real_pucnt(char c) {
  if (c == '\'' || c == '-') {
    return false;
  }
  return true;
}

int rfree__word(void *ptr) {
  free(ptr);
  return FUN_SUCCESS;
}
