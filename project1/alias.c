/* alias.c
 * Contains functions for setting and replacing
 * custom aliases with their original commands. */

 #include "shell.h"

/* set_alias: adds an alias to the list
 *            from the given args assignment
 * args: the arguments list
 * returns: nothing */
void set_alias (char** args) {
  size_t i;
  Alias* a = malloc(sizeof(Alias));
  a->custom = alloc_copy(a->custom, args[1], false);
  a->num_original = num_args - 3;
  a->original = calloc(a->num_original, sizeof(char*));
  for (i = 3; i < num_args; ++i) {
    a->original[i - 3] = calloc(strlen(args[i]), sizeof(char));
    strcpy(a->original[i - 3], args[i]);
  }
  ++num_aliases;
  aliases = realloc(aliases, num_aliases * sizeof(Alias*));
  aliases[num_aliases - 1] = a;
}

/* unalias: iterates through all of the given arguments and
*           replaces any that are aliases, allocating more
 *          memory and shifting later elements as needed
 * args: array of character arrays representing
 *       the commands to check for aliases
 * returns: nothing */
void replace_alias (char** args) {
  size_t i, j, k;
  size_t token_count, old_count, new_count;
  size_t old_ind, new_ind;
  old_count = num_args;
  for (i = 0; i < old_count; ++i) {
    for (j = 0; j < num_aliases; ++j) {
      token_count = aliases[j]->num_original;
      /* if any of the tokens match an alias, swap them out */
      if (strcmp(args[i], aliases[j]->custom) == 0) {
        /* if it's a single token, it's a simple replacement */
        if (token_count == 1) {
          args[i] = alloc_copy(args[i], aliases[j]->original[0], true);
        }
        /* however, if we need to insert multiple tokens, we have
         * to allocate enough memory for the new ones, then shift
         * any that come after back by the right amount */
        else {
          new_count = num_args;
          num_args = num_args - 1 + token_count;
          args = realloc(args, num_args * sizeof(char*));
          for (k = i + 1; k < new_count; ++k) {
            old_ind = k;
            new_ind = old_ind - 1 + token_count;
            args[new_ind] = alloc_copy(args[new_ind], args[old_ind], false);
            free(args[old_ind]);
          }
          /* now that we've made enough room, we can actually
           * insert the new tokens into the args array */
          for (k = 0; k < token_count; ++k) {
            args[i + k] = alloc_copy(args[i + k], aliases[j]->original[k],
                                     false);
          }
        }
      }
    }
  }
}
