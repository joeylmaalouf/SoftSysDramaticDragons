#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ARRSIZE 256

typedef struct Alias {
  char custom[ARRSIZE];
  char original[ARRSIZE][ARRSIZE];
} Alias;

Alias aliases[ARRSIZE];
int num_aliases;

char* unalias (char* alias) {
  // TODO
  return alias;
}

/* parse: check an individual character and either
 *        add it to a currently-building command or
 *        start a new command based on the presence
 *        of whitespace or other separators
 * c: character to parse
 * args: array of character arrays to build commands into
 * count: the tracker for how many arguments were parsed in this command
 * returns: flag telling main code whether a command is fully parsed
 */
int parse (char c, char args[ARRSIZE][ARRSIZE], int* count) {
  static int in_comment = 0;
  static int in_quote = 0;
  static int in_whitespace = 1;
  static int i_cmd = 0;
  static int i_char = 0;
  if (in_comment && (c != '\n')) {
    return 0;
  }
  switch (c) {
    case '"':
      in_quote = !in_quote;
      return 0;
    case EOF:
    case '\n':
      in_comment = 0;
      in_whitespace = 0;
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
        in_whitespace = 1;
        return 1;
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
        in_whitespace = 1;
        return 0;
      }
    default:
      /* cases dependent on other flags */
      if ((c == '#') && !in_quote) {
        in_comment = 1;
        return 0;
      }
      /* characters composing tokens */
      args[i_cmd][i_char] = c;
      ++i_char;
      in_whitespace = 0;
      return 0;
  }
}

/* is_blank: checks if the given string is just whitespace
 * string: the string to parse
 * returns: flag saying whether the string is only whitespace
 */
int is_blank (char* string) {
  int i, l = strlen(string);
  if (l < 1) {
    return 1;
  }
  for (i = 0; i < l; ++i) {
    if (!isspace(string[i])) {
      return 0;
    }
  }
  return 1;
}

/* execute: forks a new process to run the given command,
 *          along with any arguments provided, after checking
 *          to make sure that the command is not empty
 * args: array of character arrays representing
 *       the command and any other arguments
 * returns: nothing
 */
void execute (char** args) {
  int wait_status;
  pid_t pid;

  /* make sure we can fork a new process */
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Error: could not fork process\n");
    return;
  }
  else if (pid == 0) {
    /* make sure we can execute the command */
    if (execvp(*args, args) < 0) {
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

/* main: reads commands and args from stdin (batch file or
 *       user input) and executes them in order after parsing
 * argc: the number of command line arguments
 * argv: the values of command line arguments
 * returns: exit code
 */
int main (int argc, char* argv[]) {
  FILE *fp;
  char c;
  int interactive, ready;
  char tmp[ARRSIZE][ARRSIZE];
  char** args;
  int i, j;
  char prompt[ARRSIZE] = "» ";
  int count = 0;
  num_aliases = 0;

  if (argc > 1) {
    // change to accept n-many files to run in order?
    fp = fopen(argv[1], "r");
    interactive = 0;
  }
  else {
    fp = stdin;
    interactive = 1;
    printf("%s", prompt);
  }
  do {
    c = fgetc(fp);
    ready = parse(c, tmp, &count);
    if (ready) {
      args = calloc(count, sizeof(char*));
      j = 0;
      for (i = 0; i < count; ++i) {
        if(!is_blank(tmp[i])) {
          args[j] = calloc(ARRSIZE, sizeof(char));
          strcpy(args[j++], tmp[i]);
        }
      }
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
      else if ((j == 3) && (strcmp(args[0], "prompt") == 0)
          && (strcmp(args[1], "=") == 0)) {
        strcpy(prompt, args[2]);
      }
      else {
        // unalias here? check if any substring in args is aliased to anything; if so, substitute it back
        // NOTE: if len(alias.original) > 1, they have to be new char*s so the array has to shift (realloc args, iterate in reverse)
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
  printf("\n");
  free(args);

  return 0;
}
