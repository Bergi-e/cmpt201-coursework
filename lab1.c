#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while (1) {
    char *saveptr;

    printf("Please enter some text: ");

    // Grab the inputted text
    nread = getline(&line, &len, stdin);

    // Checks for getline failure or end of file
    if (nread == -1) {
      break;
    }

    // Checks for an empty line
    if (nread == 1 && line[0] == '\n') {
      break;
    }

    // Strips the linefeed character
    if (nread > 0 && line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }

    printf("Tokens:\n");

    char *token = strtok_r(line, " ", &saveptr);

    // Loop through each remaining token
    while (token != NULL) {
      printf("%s\n", token);

      token = strtok_r(NULL, " ", &saveptr);
    }
  }

  // Free the buffer memory
  free(line);
  return 0;
}
