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
  switch(c) {
    // re-add checks for quote blocks and repeated whitespace?
    case EOF:
      // do extra processing so ctrl-D exits immediately, even if other commands preceded it?
    case ';':
    case '\n':
    case '\0':
      /* characters separating commands */
      args[*i_cmd][*i_char] = '\0';
      *count = *i_cmd + 1;
      *i_cmd = 0;
      *i_char = 0;
      return 1;
    case ' ':
    case '\t':
      /* characters separating arguments */
      args[*i_cmd][*i_char] = '\0';
      ++*i_cmd;
      *i_char = 0;
      return 0;
    default:
      /* characters composing tokens */
      args[*i_cmd][*i_char] = c;
      ++*i_char;
      return 0;
  }
}

/* execute: forks a new process to run the given command,
 *          along with any arguments provided
 * args: array of character arrays representing
 *       the command and any other arguments 
 * returns: nothing
 */
void execute (char** args) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Error: could not fork process\n");
    return;
  }
  else if (pid == 0) {
    if (execvp(*args, args) < 0) {
      fprintf(stderr, "Error: could not execute command\n");
      return;
    }
  }
  else {
    while (wait(&status) != pid) {
      /* do nothing */
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
