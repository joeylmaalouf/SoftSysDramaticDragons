/* memory.c
 * Contains functions for allocating
 * and freeing memory. */

 #include "shell.h"

/* alloc_copy: allocate enough memory for the given string
 *             at the given pointer and copy it over
 * ptr: the pointer to assign
 * str: the string to copy over
 * reallocate: whether to reallocate space at the given pointer
 * returns: the updated pointer */
char* alloc_copy (char* ptr, char* str, bool reallocate) {
  if (reallocate) {
    ptr = realloc(ptr, strlen(str) * sizeof(char));
  }
  else {
    ptr = calloc(strlen(str), sizeof(char));
  }
  strcpy(ptr, str);
  return ptr;
}

/* cleanup: free any leftover memory that we've dynamically allocated
 * returns: nothing */
void cleanup () {
  size_t i, j;
  for (i = 0; i < num_aliases; ++i) {
    for (j = 0; j < aliases[i]->num_original; ++j) {
      free(aliases[i]->original[j]);
    }
    free(aliases[i]->custom);
    free(aliases[i]);
  }
  free(aliases);
  free(prompt);
  printf("\n");
}
