#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**/
int parse () {
  //
}

/* execute: forks a new process to run the given command,
 *          along with any arguments provided
 * args: array of strings (character arrays)
 *       representing the command and other arguments 
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
      // do nothing
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
  int flag;
  char* args[256];

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
    flag = parse(c, args);
    /* handle exit codes from parse to know if it's time to execute a command or just split into next arg */
    if (interactive && c == '\n') {
      printf("» ");
    }
  } while (c != EOF);
  printf("\n");

  return 0;
}
