#include "arena.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/mman.h"

arena_t* create_arena(uint64_t capacity) {
    uint64_t* mem = (uint64_t*)mmap(NULL, ARENA_SZ + capacity, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (mem == NULL) return NULL;

    mem[0] = capacity;
    mem[1] = 0;
    mem[2] = (uint64_t)mem + ARENA_SZ;

    return (arena_t*)mem;
}

int destroy_arena(arena_t* arena) {
    return munmap(arena->mem - ARENA_SZ, arena->capacity + ARENA_SZ);
}

void* arena_push(arena_t* arena, uint64_t size) {
    if (size == 0) return NULL;

    uint64_t align = (size + sizeof(uint64_t)) % 16;
    size += 16 - align;

    if (arena->pos + size + sizeof(uint64_t) > arena->capacity) return NULL;

    uint8_t* addr = arena->mem + arena->pos;
    memcpy(addr + size, &size, sizeof(uint64_t));

    arena->pos += size + sizeof(uint64_t);

    return (void*)addr;
}

int arena_pop(arena_t* arena) {
    if (arena->pos < sizeof(uint64_t) + 1) return -1;

    size_t size;
    memcpy(&size, arena->mem + arena->pos - sizeof(uint64_t), sizeof(uint64_t));
    arena->pos -= (sizeof(uint64_t) + size);
    return (int)size;
}

uint64_t arena_pop_n(arena_t* arena, uint64_t n) {
    for (uint64_t i = 0; i < n; i++) {
        if (arena_pop(arena) < 0) {
            return i;
        }
    }
    return n;
}

void arena_clear(arena_t* arena) {
    arena->pos = 0;
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    arena_t* a = create_arena(5000);
    if (a == NULL) return -1;
    printf("sucessfully created arena\n");

    arena_push(a, 2400);

    while (1) {
        uint64_t sz = (uint64_t)rand() % 100 + 1;
        if (!arena_push(a, sz)) break;
        printf("allocated a %llu byte region\n", sz);
    }

    while (1) {
        int res = arena_pop(a);
        if (res < 0) break;
        printf("freed a %d byte region\n", res);
    }

    int res = destroy_arena(a);
    printf("destroyed arena with res=%d\n", res);
    return 0;
}
