#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while (1) {
    printf("Enter programs to run.\n> ");

    nread = getline(&line, &len, stdin);

    // Check if nread fails
    if (nread == -1) {
      break;
    }

    // Get the linefeed character

    // Fork a new process

    pid_t pid = fork();

    // Check if fork fails
    if (pid < 0) {
      perror("Fork failure");
    } else if (pid == 0) {
      execlp(line, line, NULL);
      printf("Exec failure\n");
      free(line);
      exit(1);
    } else {
      int status;
      if (waitpid(pid, &status, 0) == -1) {
        perror("Waitpid failure");
      }
    }
  }

  // Free buffer memory
  free(line);
  return 0;
}
