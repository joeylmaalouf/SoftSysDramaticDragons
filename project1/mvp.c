#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* parse: check an individual character and either
 *        add it to a currently-building command or
 *        start a new command based on the presence
 *        of whitespace or other separators
 * c: character to parse
 * args: array of character arrays to build commands into
 * i_cmd: the position in the array of the currently-building command
 * i_char: the position in the command of the current character
 * count: the tracker for how many arguments were parsed in this command
 * returns: flag telling main code whether a command is fully parsed
 */
int parse (char c, char args[256][256], int* i_cmd, int* i_char, int* count) {
  static int in_quote = 0;
  static int in_whitespace = 1;
  switch(c) {
    case '"':
      in_quote = !in_quote;
      return 0;
    case EOF:
      // do extra processing so ctrl-D exits immediately, even if other commands preceded it?
    case ';':
      // figure out why ; stopped working
    case '\n':
    case '\0':
      /* characters separating commands */
      if (!in_quote) {
        if (!in_whitespace) {
          args[*i_cmd][*i_char] = '\0';
          *count = *i_cmd + 1;
          *i_cmd = 0;
          *i_char = 0;
        }
        in_whitespace = 1;
        return 1;
      }
    case ' ':
    case '\t':
      /* characters separating arguments */
      if (!in_quote) {
        if (!in_whitespace) {
          args[*i_cmd][*i_char] = '\0';
          ++*i_cmd;
          *i_char = 0;
        }
        in_whitespace = 1;
        return 0;
      }
    default:
      /* characters composing tokens */
      args[*i_cmd][*i_char] = c;
      ++*i_char;
      in_whitespace = 0;
      return 0;
  }
}

/* is_blank: checks if the given string is just whitespace
 * string: the string to parse
 * returns: flag saying whether the string is only whitespace
 */
int is_blank (char* string) {
  int i;
  for (i = 0; i < strlen(string); ++i) {
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
  int i = 0;
  int blank_status = 1;
  int wait_status;
  pid_t pid;

  /* make sure we have at least one non-empty word */
  while (args[i] != NULL) {
    if (!is_blank(args[i++])) {
      blank_status = 0;
    }
  }
  if (blank_status) {
    return;
  }

  /* make sure we can fork a new process */
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Error: could not fork process\n");
    return;
  }
  else if (pid == 0) {
    /* make sure we can execute the command */
    if (execvp(*args, args) < 0) {
      fprintf(stderr, "Error: could not execute command\n");
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
  int interactive;
  char tmp[256][256];
  char** args;
  int i_cmd = 0;
  int i_char = 0;
  int count = 0;
  int i;

  if (argc > 1) {
    fp = fopen(argv[1], "r");
    interactive = 0;
  }
  else {
    fp = stdin;
    interactive = 1;
    printf("» ");
  }
  do {
    c = fgetc(fp);
    if (parse(c, tmp, &i_cmd, &i_char, &count)) {
      args = malloc(count * sizeof(char*));
      for (i = 0; i < count; ++i) {
        args[i] = malloc(256 * sizeof(char));
        strcpy(args[i], tmp[i]);
      }
      execute(args);
      count = 0;
    }
    if (interactive && c == '\n') {
      printf("» ");
    }
  } while (c != EOF);
  printf("\n");
  free(args);

  return 0;
}

// static int declarations to avoid flag/counter args?
// args counter var (count)?
// free args[i]
// free tmp
// try not to use tmp at all? currently we have static memory allocation for parse and dynamic for execute
// increment pointer instead of separate index vars?
