/* shell.h
 * Contains  definitions for this
 * program's methods and types. */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 256

char* prompt;

typedef enum {
  false = 0,
  true = 1
} bool;

typedef struct State {
  bool in_comment;
  bool in_quote;
  bool in_whitespace;
  int i_cmd;
  int i_char;
} State;

typedef struct Alias {
  char* custom;
  char** original;
  int num_original;
} Alias;

Alias** aliases;

size_t num_aliases;

size_t num_args;

/* alias.c */
void set_alias (char** args);
void replace_alias (char** args);
/* memory.c */
char* alloc_copy (char* ptr, char* str, bool reallocate);
void cleanup ();
/* parsing.c */
bool parse (char c, char args[BUFSIZE][BUFSIZE], State* state);
bool is_blank (char* string);
