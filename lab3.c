#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 5

char *input_history[MAX_LEN];
int history_count = 0;

// Removes the oldest record, and then reshuffles the rest
//  of the records back a slot in the order.
void remove_oldest_record() {
  free(input_history[0]);
  for (int i = 1; i < history_count; i++) {
    input_history[i - 1] = input_history[i];
  }
  history_count--;
}

// Add a new input to the history count, and remove the oldest
// if the max length is reached
void add_to_history(char *input) {
  if (history_count == MAX_LEN) {
    remove_oldest_record();
  }
  input_history[history_count] = input;
  history_count++;
}

// Print the history count in order
void print_history() {
  for (int i = 0; i < history_count; i++) {
    printf("%s", input_history[i]);
  }
}
// Grab whatever stdin input is inputted
char *get_input() {
  char *buff = NULL;
  size_t bufsize = 0;

  printf("Enter input: ");
  if (getline(&buff, &bufsize, stdin) != -1) {
    return buff;
  }

  free(buff);
  return NULL;
}

// Runs the program.
// Take in a bunch of inputs, store them and then print them
// when prompted. Pretty cool!
int main() {
  char *input;

  while ((input = get_input()) != NULL) {
    add_to_history(input);

    if (strcmp(input, "print\n") == 0) {
      print_history();
    }
  }

  for (int i = 0; i < history_count; i++) {
    free(input_history[i]);
  }

  return 0;
}
