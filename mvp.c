#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[]) {
  char* args[1];
  printf("» ");
  while (scanf("%s", args[0]) != EOF) {
    printf("%s\n", args[0]);
    execute(args);
    printf("» ");
  }
  puts("\n");
  return 0;
}
