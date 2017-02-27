/* shell.c
 * A C shell, capable of parsing input in batch or interactive modes,
 * loading settings from configuration files, and more.
 * Joey L. Maalouf
 * Apurva Raman
 * Sean Carter */

#include "shell.h"

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
      fprintf(stderr, "Error: could not execute command \"%s\"; (%d) %s\n",
              args[0], errno, strerror(errno));
      return;
    }
  }
  else {
    while (wait(&wait_status) != pid) {
      /* wait for the command to finish */
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
  State* state = malloc(sizeof(State));
  state->in_comment = false;
  state->in_quote = false;
  state->in_whitespace = true;
  state->i_cmd = 0;
  state->i_char = 0;

  do {
    /* repeatedly check the input and split it into tokens */
    c = fgetc(fp);
    ready = parse(c, args, state);
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
      parsed_args = realloc(parsed_args, (num_args + 1) * sizeof(char*));
      parsed_args[num_args] = NULL;
      /* check for the special case of assigning an alias */
      if ((num_args >= 4) && (strcmp(parsed_args[0], "alias") == 0)
          && (strcmp(parsed_args[2], "=") == 0)) {
        set_alias(parsed_args);
      }
      /* check for the special case of customizing the prompt */
      else if ((num_args == 3) && (strcmp(parsed_args[0], "prompt") == 0)
          && (strcmp(parsed_args[1], "=") == 0)) {
        prompt = alloc_copy(prompt, parsed_args[2], true);
      }
      /* check for any alias replacements, then execute the given command */
      else {
        replace_alias(parsed_args);
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
  free(state);
}

/* main: reads commands and args from stdin (batch file or
 *       user input) and executes them in order after parsing
 * argc: the number of command line arguments
 * argv: the values of command line arguments
 * returns: exit code */
int main (int argc, char* argv[]) {
  FILE *fp;
  size_t i;
  num_aliases = 0;
  aliases = calloc(num_aliases, sizeof(Alias*));
  prompt = calloc(3, sizeof(char));
  strcpy(prompt, "» ");

  /* execute the contents of the config file if it exists */
  if (access(".shellrc", R_OK) != -1) {
    fp = fopen(".shellrc", "r");
    exec_loop(fp, false);
    fclose(fp);
  }

  /* loop over multiple batch files if provided */
  if (argc > 1) {
    for (i = 1; i < argc; ++i) {
      if (access(argv[i], R_OK) != -1) {
        fp = fopen(argv[i], "r");
        exec_loop(fp, false);
        fclose(fp);
      }
      else {
        fprintf(stderr, "Error: could not find file \"%s\"\n", argv[i]);
      }
    }
  }
  /* otherwise, take interactive input */
  else {
    printf("%s", prompt);
    exec_loop(stdin, true);
  }

  cleanup();

  return 0;
}
