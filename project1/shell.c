/* shell.c
 * A C shell, capable of parsing input in batch or interactive modes,
 * loading settings from configuration files, and more.
 * Joey L. Maalouf
 * Apurva Raman
 * Sean Carter
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ARRSIZE 256

typedef enum {
  false = 0,
  true = 1
} bool;

// struct for parse flags/indices?

typedef struct Alias {
  char custom[ARRSIZE];
  char original[ARRSIZE][ARRSIZE];
} Alias;

Alias aliases[ARRSIZE];
int num_aliases;

char prompt[ARRSIZE];

/* parse: check an individual character and either
 *        add it to a currently-building command or
 *        start a new command based on the presence
 *        of whitespace or other separators
 * c: character to parse
 * args: array of character arrays to build commands into
 * count: the tracker for how many arguments were parsed in this command
 * returns: flag telling main code whether a command is fully parsed
 */
bool parse (char c, char args[ARRSIZE][ARRSIZE], int* count) {
  static bool in_comment = false;
  static bool in_quote = false;
  static bool in_whitespace = true;
  static int i_cmd = 0;
  static int i_char = 0;
  if (in_comment && (c != '\n')) {
    return false;
  }
  switch (c) {
    case '"':
      in_quote = !in_quote;
      return false;
    case EOF:
    case '\n':
      in_comment = false;
      in_whitespace = false;
    case '\0':
    case ';':
      /* characters separating commands */
      if (!in_quote) {
        if (!in_whitespace) {
          args[i_cmd][i_char] = '\0';
          *count = i_cmd + 1;
          i_cmd = 0;
          i_char = 0;
        }
        in_whitespace = true;
        return true;
      }
    case ' ':
    case '\t':
      /* characters separating arguments */
      if (!in_quote) {
        if (!in_whitespace) {
          args[i_cmd][i_char] = '\0';
          ++i_cmd;
          i_char = 0;
        }
        in_whitespace = true;
        return false;
      }
    default:
      /* cases dependent on other flags */
      if ((c == '#') && !in_quote) {
        in_comment = true;
        return false;
      }
      /* characters composing tokens */
      args[i_cmd][i_char] = c;
      ++i_char;
      in_whitespace = false;
      return false;
  }
}

/* is_blank: checks if the given string is just whitespace
 * string: the string to parse
 * returns: flag saying whether the string is only whitespace
 */
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
 * returns: nothing
 */
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

/* unalias: ...
 * alias: ...
 * returns: ...
 */
void unalias (char* args[ARRSIZE]) {
  // size_t i;
  // for (i = 0; i < count; i++) {
  // }
  // check if any command in args is aliased to anything; if so, substitute it back
  // if len(alias.original) > 1, they have to be new char*s so the array has to shift
  // new_size = len(args) + len(alias.original) - 1
  // args = realloc(args, new_size)
  // new[i] = old[i - len(alias.original) - 1] for i = new_size -> replacement index
}

/* exec_loop: loops through every line of a file,
 *            parses it with parse(), and passes the result
 *            into execute()
 * fp: file pointer to read commands from
 * interactive: flag for the run mode
 * returns: nothing
 */
void exec_loop (FILE* fp, bool interactive) {
  char c;
  bool ready;
  char tmp[ARRSIZE][ARRSIZE];
  char** args;
  size_t i, j;
  int count = 0;

  do {
    /* repeatedly check the input and split it into tokens */
    c = fgetc(fp);
    ready = parse(c, tmp, &count);
    if (ready) {
      /* copy the non-blank tokens into the argument array */
      args = calloc(count, sizeof(char*));
      j = 0;
      for (i = 0; i < count; ++i) {
        if(!is_blank(tmp[i])) {
          args[j] = calloc(ARRSIZE, sizeof(char));
          strcpy(args[j++], tmp[i]);
        }
      }
      /* check for the special case of assigning an alias */
      if ((j >= 4) && (strcmp(args[0], "alias") == 0)
          && (strcmp(args[2], "=") == 0)) {
        Alias a;
        memset(&a, 0, sizeof(Alias));
        strcpy(a.custom, args[1]);
        for (i = 0; i < j - 3; ++i) {
          strcpy(a.original[i], args[3 + i]);
        }
        aliases[num_aliases++] = a;
      }
      /* check for the special case of customizing the prompt */
      else if ((j == 3) && (strcmp(args[0], "prompt") == 0)
          && (strcmp(args[1], "=") == 0)) {
        strcpy(prompt, args[2]);
      }
      /* execute the given command */
      else {
        unalias(args);
        execute(args);
      }
      for (i = 0; i < count; ++i) {
        free(args[i]);
      }
      count = 0;
    }
    if (interactive && (c == '\n')) {
      printf("%s", prompt);
    }
  } while (c != EOF);
  free(args);
}

/* main: reads commands and args from stdin (batch file or
 *       user input) and executes them in order after parsing
 * argc: the number of command line arguments
 * argv: the values of command line arguments
 * returns: exit code
 */
int main (int argc, char* argv[]) {
  FILE *fp;
  bool interactive;
  num_aliases = 0;
  strcpy(prompt, "» ");

  /* execute the contents of the config file if it exists */
  if (access(".shellrc", F_OK) != -1) {
    fp = fopen(".shellrc", "r");
    exec_loop(fp, 0);
  }

  if (argc > 1) {
    // TODO: change to accept n-many files to run in order, instead of restricting batch mode to 1 file?
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

  return 0;
}
