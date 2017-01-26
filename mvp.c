#include <stdio.h>
#include <stdlib.h>

void execute (char* arg) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Error: could not fork process\n");
    return;
  }
  else if (pid == 0) {
    if (execvp(*arg, arg) < 0) {
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
  char cmd[64];
  scanf("%s", cmd);
  printf("%s\n", cmd);
  execute(cmd);
  return 0;
}
