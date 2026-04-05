#include <assert.h>
#include <inttypes.h>
#include <stdalign.h>
#include <stdio.h>

typedef struct {
    uint8_t *buffer; // the actual memory — start address
    size_t size; // the total capacity of the arena
    size_t offset; // how far we've bumped - current position
} arena;

void arena_init(arena *arena, void *buffer, const size_t capacity) {
    arena->buffer = buffer;
    arena->size = capacity;
    arena->offset = 0;
}

void arena_reset(arena *arena) {
    arena->offset = 0;
}

void* arena_alloc(arena *arena, const size_t size, size_t alignment) {
    if (alignment == 0) alignment = 1;
    assert((alignment & (alignment - 1)) == 0 && "Alignment must be power of 2");

    const uintptr_t aligned_offset =( arena->offset + (alignment - 1)) & ~(alignment - 1);


    if (aligned_offset  + size > arena->size) {
        return NULL;
    }

    uint8_t *ptr = arena->buffer + aligned_offset;
    arena->offset = aligned_offset + size;
    return ptr;
}


int main(void) {
    // Create a 1KB arena
    alignas(16) uint8_t memory[1024]; // align base to 16
    arena arena;
    arena_init(&arena, memory, sizeof(memory));

    // Test 1: Basic allocation
    int *a = arena_alloc(&arena, sizeof(int), 8);
    int *b = arena_alloc(&arena, sizeof(int), 8);
    printf("Allocated at: %p and %p (diff: %td bytes)\n",
           (void*)a, (void*)b, (uint8_t*)b - (uint8_t*)a);

    // Test 2: Out of memory
    void *big = arena_alloc(&arena, 2000, 8);
    printf("Large alloc (should fail): %p\n", big);

    // Test 3: Reset and reuse
    arena_reset(&arena);
    int *c = arena_alloc(&arena, sizeof(int), 8);
    printf("After reset: %p (should equal first alloc)\n", (void*)c);

    // Test 4: Alignment with values not multiples of 2
    int *d = arena_alloc(&arena, 10, 8);
    printf("Allocated at: %p and %p (diff: %td bytes)\n",
           (void*)c, (void*)d, (uint8_t*)d - (uint8_t*)c);

    int *e = arena_alloc(&arena, 10, 8);
    printf("Allocated at: %p and %p (diff: %td bytes)\n",
           (void*)d, (void*)e, (uint8_t*)e - (uint8_t*)d);

    // Test 5: Alignment stress test
    printf("\n=== Alignment Stress Test ===\n");
    arena_reset(&arena);

    void *p1 = arena_alloc(&arena, 1, 1);   // 1 byte, 1-byte aligned
    void *p2 = arena_alloc(&arena, 1, 16);  // 1 byte, 16-byte aligned
    void *p3 = arena_alloc(&arena, 1, 4);   // 1 byte, 4-byte aligned
    void *p4 = arena_alloc(&arena, 7, 8);   // 7 bytes, 8-byte aligned

    printf("p1: %p (offset: 0)\n", p1);
    printf("p2: %p (should be 16-byte aligned)\n", p2);
    printf("p3: %p (should be 4-byte aligned)\n", p3);
    printf("p4: %p (should be 8-byte aligned)\n", p4);
    printf("Total arena used: %zu bytes\n", arena.offset);

    // Verify alignment
    assert(((uintptr_t)p2 & 15) == 0);  // p2 is 16-byte aligned
    assert(((uintptr_t)p3 & 3) == 0);   // p3 is 4-byte aligned
    assert(((uintptr_t)p4 & 7) == 0);   // p4 is 8-byte aligned

    return 0;
}
