#define _DEFAULT_SOURCE
#define _ISOC99_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE 128
#define HEAP_SIZE 256
#define BUF_SIZE 64

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main(void) {
  void *start_address = sbrk(HEAP_SIZE);
  if (start_address == (void *)-1) {
    handle_error("sbrk failed");
  }

  struct header *first_block = (struct header *)start_address;
  struct header *second_block = (struct header *)((char *)start_address + BLOCK_SIZE);

  first_block->size = BLOCK_SIZE;
  first_block->next = NULL;

  second_block->size = BLOCK_SIZE;
  second_block->next = first_block;

  void *first_data = (void *)(first_block + 1);
  void *second_data = (void *)(second_block + 1);

  memset(first_data, 0, BLOCK_SIZE - sizeof(struct header));
  memset(second_data, 1, BLOCK_SIZE - sizeof(struct header));

  print_out("first block:       %p\n", &first_block, sizeof(first_block));
  print_out("second block:      %p\n", &second_block, sizeof(second_block));

  print_out("first block size:  %lu\n", &first_block->size, sizeof(first_block->size));
  print_out("first block next:  %p\n", &first_block->next, sizeof(first_block->next));

  print_out("second block size: %lu\n", &second_block->size, sizeof(second_block->size));
  print_out("second block next: %p\n", &second_block->next, sizeof(second_block->next));

  char zero[2] = "0\n";
  for (size_t i = 0; i < BLOCK_SIZE - sizeof(struct header); i++) {
    write(STDOUT_FILENO, zero, 2);
  }

  char newline = '\n';
  write(STDOUT_FILENO, &newline, 1);

  char one[2] = "1\n";
  for (size_t i = 0; i < BLOCK_SIZE - sizeof(struct header); i++) {
    write(STDOUT_FILENO, one, 2);
  }

  write(STDOUT_FILENO, &newline, 1);

  return 0;
}
