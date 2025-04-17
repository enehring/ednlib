/// Homemade arena allocator. For learning purposes only.

#include <stdlib.h>
#include <stdio.h>

typedef struct {
  unsigned char *data;
  size_t size;
  size_t offset;
} Arena;

void arena_init(Arena *arena, unsigned char *memory, size_t memory_size) {
  arena->data = memory;
  arena->size = memory_size;
  arena->offset = 0;
}

void arena_deinit(Arena *arena) {
  arena->offset = 0;
}

void *arena_malloc(Arena *arena, size_t size) {
  unsigned char *pos;

  // align memory
  // TODO: This is O(n) for the size of the type allocated. Use a more
  // efficient algorithm to find the next aligned offset.
  while (arena->offset % size != 0) {
    arena->offset += 1;
  }
  
  if (arena->offset + size > arena->size) {
    return NULL;
  }
  else {
    pos = &(arena->data[arena->offset]);
    arena->offset = arena->offset + size;

    return pos;
  }
}

// For testing
int main(void) {
  unsigned char memory[512];
  Arena test;

  char *test_ptr;
  int *test2_ptr;

  puts("TEST: Arena initialization...");

  arena_init(&test, memory, 512);
  fputs("\tArena size equals backing memory allocation - ", stdout);
  if (test.size == 512) {
    puts("PASS");
  }
  else {
    puts("FAIL");
    return -1;
  }

  puts("TEST: Arena malloc...");

  test_ptr = (char *)arena_malloc(&test, sizeof(char));
  fputs("\tArena offset equals size of type - ", stdout);
  if (test.offset == sizeof(char)) {
    puts("PASS");
  }
  else {
    puts("FAIL");
    return -1;
  }

  test2_ptr = (int *)arena_malloc(&test, sizeof(int));
  fputs("\tPadding is inserted to align memory - ", stdout);
  if (test.offset == 8) {
    puts("PASS");
  }
  else {
    puts("FAIL");
    return -1;
  }
  
  arena_deinit(&test);
  
  return 0;
}
