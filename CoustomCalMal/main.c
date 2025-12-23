#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define ARENA_SIZE (1024 * 1024)   // 1 MB
#define ALIGNMENT 8

#define ALIGN(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct Block { // will act as a book or journal
    size_t size;          
    int free;
    struct Block* next;
} Block;

static void* arena = NULL;
static Block* free_list = NULL;

/* ================= INITIALIZATION ================= */

void init_allocator(void) {
    arena = VirtualAlloc(
        NULL,
        ARENA_SIZE,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    if (!arena) {
        fprintf(stderr, "VirtualAlloc failed\n");
        ExitProcess(1);
    }

    free_list = (Block*)arena;
    free_list->size = ARENA_SIZE - sizeof(Block);
    free_list->free = 1;
    free_list->next = NULL;
}

/* ================= BLOCK SPLITTING ================= */

void split_block(Block* block, size_t size) {
    Block* new_block = (Block*)((char*)(block + 1) + size);

    new_block->size = block->size - size - sizeof(Block);
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

/* ================= MALLOC ================= */

void* my_malloc(size_t size) {
    if (!arena)
        init_allocator();

    size = ALIGN(size);

    Block* curr = free_list;

    while (curr) {
        if (curr->free && curr->size >= size) {

            if (curr->size >= size + sizeof(Block) + ALIGNMENT)
                split_block(curr, size);

            curr->free = 0;
            return (void*)(curr + 1);
        }
        curr = curr->next;
    }

    return NULL; // out of memory
}

/* ================= COALESCING ================= */

void coalesce(void) {
    Block* curr = free_list;

    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += sizeof(Block) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

/* ================= FREE ================= */

void my_free(void* ptr) {
    if (!ptr)
        return;

    Block* block = (Block*)ptr - 1;
    block->free = 1;

    coalesce();
}

/* ================= TEST ================= */

int main(void) {
    char* a = (char*)my_malloc(100);
    char* b = (char*)my_malloc(200);
    char* c = (char*)my_malloc(50);

    sprintf(a, "Hello");
    sprintf(b, "Custom malloc");
    sprintf(c, "World");

    printf("%s %s %s\n", a, b, c);

    my_free(b);
    my_free(a);
    my_free(c);

    printf("Memory freed successfully\n");
    return 0;
}
