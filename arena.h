#include <stddef.h>
#include <stdint.h>

#ifndef ARENA_H
#define ARENA_H

#define ARENA_SZ sizeof(uint64_t) * 4

typedef struct {
    uint64_t capacity;
    uint64_t pos;
    uint8_t* mem;
} arena_t;

arena_t* create_arena(uint64_t capacity);
int destroy_arena(arena_t* arena);

void* arena_push(arena_t* arena, uint64_t size);
int arena_pop(arena_t* arena);
uint64_t arena_pop_n(arena_t* arena, uint64_t n);
void arena_clear(arena_t* arena);

#endif  // ARENA_H
