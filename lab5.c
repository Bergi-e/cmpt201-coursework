#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // Implement first fit
  struct header *current = free_list_ptr;

  while (current != NULL) {
    if (current->size >= size) {
      return current->id;
    }
    current = current->next;
  }

  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  // Implement best fit
  struct header *current = free_list_ptr;
  uint64_t best_option = UINT64_MAX;

  while (current != NULL) {
    if (current->size >= size) {
      uint64_t difference = current->size - size;
      if (difference < best_option) {
        best_option = difference;
        best_fit_id = current->id;
      }
    }
    current = current->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  // Implement worst fit
  struct header *current = free_list_ptr;
  uint64_t worst_option = 0;

  while (current != NULL) {
    if (current->size >= size) {
      uint64_t difference = current->size - size;
      if (difference > worst_option) {
        worst_option = difference;
        worst_fit_id = current->id;
      }
    }
    current = current->next;
  }

  return worst_fit_id;
}

int main(void) {
  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // Print out IDs
  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

// Coalescing algorithm
// Coalescing is the process of merging consecutive free blocks into larger blocks. This can get rid
// of memory fragmentation.
//
// Step 1: Initialize a pointer at the head of a free block linked list
// Step 2: Loop through this list while both the current and next blocks aren't null
// Step 3: At each part, check if the next block is directly after the current block in memory.
// If it is, merge the two blocks together by adding the next block's size to the current block's
// size and then update the current block's pointer to skip through the newly merged block and point
// the block after it.
// Step 4: Then, move the pointer to the next block if the current and next blocks aren't together.
