#include <inttypes.h>
#include <stdio.h>

typedef struct {
    uint8_t *buffer; // the actual memory — start address
    size_t size; // the total capacity of the arena
    size_t offset; // how far we've bumped - current position
} Arena;

/**
 * functions needed
 * arena_init
 * areno_alloc
 * arena_reset
 */

void arena_init(Arena *arena, void *buffer, const size_t capacity) {
    arena->buffer = buffer;
    arena->size = capacity;
    arena->offset = 0;
}

void arena_reset(Arena *arena) {
    arena->offset = 0;
}

void* arena_alloc(Arena *arena, const size_t size) {
    if (arena->offset + size > arena->size) {
        return NULL;
    }

    uint8_t *ptr = arena->buffer + arena->offset;
    arena->offset += size;
    return ptr;
}


int main(void) {
    // Create a 1KB arena
    uint8_t memory[1024];
    Arena arena;
    arena_init(&arena, memory, sizeof(memory));

    // Test 1: Basic allocation
    int *a = arena_alloc(&arena, sizeof(int));
    int *b = arena_alloc(&arena, sizeof(int));
    printf("Allocated at: %p and %p (diff: %td bytes)\n",
           (void*)a, (void*)b, (uint8_t*)b - (uint8_t*)a);

    // Test 2: Out of memory
    void *big = arena_alloc(&arena, 2000);
    printf("Large alloc (should fail): %p\n", big);

    // Test 3: Reset and reuse
    arena_reset(&arena);
    int *c = arena_alloc(&arena, sizeof(int));
    printf("After reset: %p (should equal first alloc)\n", (void*)c);

    return 0;
}
