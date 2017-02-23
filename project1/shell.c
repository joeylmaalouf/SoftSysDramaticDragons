/* shell.c
 * A C shell, capable of parsing input in batch or interactive modes,
 * loading settings from configuration files, and more.
 * Joey L. Maalouf
 * Apurva Raman
 * Sean Carter */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 256

typedef enum {
  false = 0,
  true = 1
} bool;

typedef struct Flags {
  bool in_comment;
  bool in_quote;
  bool in_whitespace;
} Flags;

typedef struct Alias {
  char* custom;
  char** original;
  int num_original;
} Alias;

Alias** aliases;

size_t num_aliases;

size_t num_args;

char* prompt;

/* alloc_copy: allocate enough memory for the given
               string at the given pointer and copy it
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

/* parse: checks an individual character and either
 *        add it to a currently-building command or
 *        start a new command based on the presence
 *        of whitespace or other separators
 * c: character to parse
 * args: array of character arrays to build commands into
 * returns: flag telling main code whether a command is fully parsed */
bool parse (char c, char args[BUFSIZE][BUFSIZE], Flags* flags) {
  static int i_cmd = 0;
  static int i_char = 0;
  if (flags->in_comment && (c != '\n')) {
    return false;
  }
  switch (c) {
    case '"':
      flags->in_quote = !flags->in_quote;
      return false;
    case EOF:
    case '\n':
      flags->in_comment = false;
      flags->in_whitespace = false;
    case '\0':
    case ';':
      /* characters separating commands */
      if (!flags->in_quote) {
        if (!flags->in_whitespace) {
          args[i_cmd][i_char] = '\0';
          num_args = i_cmd + 1;
          i_cmd = 0;
          i_char = 0;
        }
        flags->in_whitespace = true;
        return true;
      }
    case ' ':
    case '\t':
      /* characters separating arguments */
      if (!flags->in_quote) {
        if (!flags->in_whitespace) {
          args[i_cmd][i_char] = '\0';
          ++i_cmd;
          i_char = 0;
        }
        flags->in_whitespace = true;
        return false;
      }
    default:
      /* cases dependent on other flags */
      if ((c == '#') && !flags->in_quote) {
        flags->in_comment = true;
        return false;
      }
      /* characters composing tokens */
      args[i_cmd][i_char] = c;
      ++i_char;
      flags->in_whitespace = false;
      return false;
  }
}

/* is_blank: checks if the given string is just whitespace
 * string: the string to parse
 * returns: flag saying whether the string is only whitespace */
bool is_blank (char* string) {
  int l = strlen(string);
  size_t i;
  if (l == 0) {
    return true;
  }
  for (i = 0; i < l; ++i) {
    if (!isspace(string[i])) {
      return false;
    }
  }
  return true;
}

/* execute: forks a new process to run the given command,
 *          along with any arguments provided, after checking
 *          to make sure that the command is not empty
 * args: array of character arrays representing
 *       the command and any other arguments
 * returns: nothing */
void execute (char** args) {
  pid_t pid;
  int exec_status;
  int wait_status;

  /* make sure we can fork a new process */
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Error: could not fork process\n");
    return;
  }
  else if (pid == 0) {
    /* make sure we can execute the command */
    exec_status = execvp(*args, args);
    if (exec_status < 0) {
      fprintf(stderr, "Error: could not execute command \"%s\"\n", args[0]);
      return;
    }
  }
  else {
    while (wait(&wait_status) != pid) {
      /* wait for the command to finish */
    }
  }
}

/* unalias: iterates through all of the given arguments and
*           replaces any that are aliases, allocating more
 *          memory and shifting later elements as needed
 * args: array of character arrays representing
 *       the commands to check for aliases
 * returns: nothing */
void unalias (char** args) {
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
            args[i + k] = alloc_copy(args[i + k], aliases[j]->original[k], false);
          }
        }
      }
    }
  }
}

/* exec_loop: loops through every line of a file,
 *            parses it with parse(), and passes the result
 *            into execute()
 * fp: file pointer to read commands from
 * interactive: flag for the run mode
 * returns: nothing */
void exec_loop (FILE* fp, bool interactive) {
  char c;
  bool ready;
  char args[BUFSIZE][BUFSIZE];
  char** parsed_args;
  size_t i, j;
  num_args = 0;
  Flags* flags = malloc(sizeof(Flags));
  flags->in_comment = false;
  flags->in_quote = false;
  flags->in_whitespace = true;

  do {
    /* repeatedly check the input and split it into tokens */
    c = fgetc(fp);
    ready = parse(c, args, flags);
    if (ready) {
      /* copy the non-blank tokens into the argument array */
      parsed_args = calloc(num_args, sizeof(char*));
      j = 0;
      for (i = 0; i < num_args; ++i) {
        if(!is_blank(args[i])) {
          parsed_args[j] = alloc_copy(parsed_args[j], args[i], false);
          ++j;
        }
      }
      num_args = j;
      /* check for the special case of assigning an alias */
      if ((num_args >= 4) && (strcmp(parsed_args[0], "alias") == 0)
          && (strcmp(parsed_args[2], "=") == 0)) {
        Alias* a = malloc(sizeof(Alias));
        a->custom = alloc_copy(a->custom, parsed_args[1], false);
        a->num_original = num_args - 3;
        a->original = calloc(a->num_original, sizeof(char*));
        for (i = 3; i < num_args; ++i) {
          a->original[i - 3] = calloc(strlen(parsed_args[i]), sizeof(char));
          strcpy(a->original[i - 3], parsed_args[i]);
        }
        ++num_aliases;
        aliases = realloc(aliases, num_aliases * sizeof(Alias*));
        aliases[num_aliases - 1] = a;
      }
      /* check for the special case of customizing the prompt */
      else if ((num_args == 3) && (strcmp(parsed_args[0], "prompt") == 0)
          && (strcmp(parsed_args[1], "=") == 0)) {
        prompt = alloc_copy(prompt, parsed_args[2], true);
      }
      /* check for any alias replacements, then execute the given command */
      else {
        unalias(parsed_args);
        execute(parsed_args);
      }
      for (i = 0; i < num_args; ++i) {
        free(parsed_args[i]);
      }
      num_args = 0;
    }
    if (interactive && (c == '\n')) {
      printf("%s", prompt);
    }
  } while (c != EOF);
  free(parsed_args);
  free(flags);
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
}

/* main: reads commands and args from stdin (batch file or
 *       user input) and executes them in order after parsing
 * argc: the number of command line arguments
 * argv: the values of command line arguments
 * returns: exit code */
int main (int argc, char* argv[]) {
  FILE *fp;
  bool interactive;
  num_aliases = 0;
  aliases = calloc(num_aliases, sizeof(Alias*));
  prompt = calloc(3, sizeof(char));
  strcpy(prompt, "» ");

  /* execute the contents of the config file if it exists */
  if (access(".shellrc", F_OK) != -1) {
    fp = fopen(".shellrc", "r");
    exec_loop(fp, 0);
  }

  if (argc > 1) {
    // TODO: change to accept n-many files to run in order,
    // instead of restricting batch mode to 1 file?
    fp = fopen(argv[1], "r");
    interactive = false;
  }
  else {
    fp = stdin;
    interactive = true;
    printf("%s", prompt);
  }

  exec_loop(fp, interactive);
  printf("\n");

  cleanup();

  return 0;
}
