//
// Created by Elijah Wandimi on 05/04/2026.
//

typedef struct {
    uint8_t *buffer; // the actual memory — start address
    size_t size; // the total capacity of the arena
    size_t offset; // how far we've bumped - current position
} Arena;

void arena_init(Arena *arena, void *buffer, const size_t capacity) {
    arena->buffer = buffer;
    arena->size = capacity;
    arena->offset = 0;
}

void arena_reset(Arena *arena) {
    arena->offset = 0;
}

void* arena_alloc(Arena *arena, const size_t size, size_t alignment) {
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