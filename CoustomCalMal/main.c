#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define ARENA_SIZE (1024 * 1024)
#define ALIGNMENT 8
#define ALIGN(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

static void* arena = NULL;
static Block* free_list = NULL;



void print_blocks(void) { // will act as a book or journal
    Block* curr = free_list;
    int i = 0;

    printf("\n[BLOCK LIST]\n");
    while (curr) {
        printf(
            "Block %d | header=%p | user=%p | size=%zu | free=%d | next=%p\n",
            i,
            (void*)curr,
            (void*)(curr + 1),
            curr->size,
            curr->free,
            (void*)curr->next
        );
        curr = curr->next;
        i++;
    }
    printf("\n");
}



void init_allocator(void) {
    arena = VirtualAlloc(
        NULL,
        ARENA_SIZE,
        MEM_RESERVE | MEM_COMMIT, //window make it necessary to add MEM_COMMIT and MEM_RESERVE together 
        PAGE_READWRITE
    );

    if (!arena) {
        fprintf(stderr, "VirtualAlloc failed\n");
        ExitProcess(1);
    }

    free_list = (Block*)arena;
    free_list->size = ARENA_SIZE - sizeof(Block); //Like linklist header size and remaining size
    free_list->free = 1;
    free_list->next = NULL;

    printf("[INIT] arena=%p size=%d\n", arena, ARENA_SIZE);
    print_blocks();
}



void split_block(Block* block, size_t size) {
    Block* new_block = (Block*)((char*)(block + 1) + size);

    new_block->size = block->size - size - sizeof(Block);
    new_block->free = 1;
    new_block->next = block->next;

    printf(
        "[SPLIT] block=%p -> new_block=%p | sizes: %zu / %zu\n",
        (void*)block,
        (void*)new_block,
        size,
        new_block->size
    );

    block->size = size;
    block->next = new_block;
}



void* my_malloc(size_t size) {
    if (!arena)
        init_allocator();

    size = ALIGN(size);
    printf("[MALLOC] request=%zu\n", size);

    Block* curr = free_list;

    while (curr) {
        printf(
            "  checking block=%p | free=%d | size=%zu\n",
            (void*)curr,
            curr->free,
            curr->size
        );

        if (curr->free && curr->size >= size) {
            printf("  -> ACCEPTED\n");

            if (curr->size >= size + sizeof(Block) + ALIGNMENT)
                split_block(curr, size);
            else
                printf("  -> NO SPLIT (exact/near fit)\n");

            curr->free = 0;

            void* user_ptr = (void*)(curr + 1);
            printf(
                "[MALLOC DONE] header=%p user=%p size=%zu\n",
                (void*)curr,
                user_ptr,
                curr->size
            );

            print_blocks();
            return user_ptr;
        }

        printf("  -> REJECTED\n");
        curr = curr->next;
    }

    printf("[MALLOC FAIL] out of memory\n");
    return NULL;
}



void coalesce(void) {
    Block* curr = free_list;

    while (curr && curr->next) { // free blacoks mnerge make it bigger
        if (curr->free && curr->next->free) {
            printf(
                "[COALESCE] %p + %p\n",
                (void*)curr,
                (void*)curr->next
            );

            curr->size += sizeof(Block) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}



void my_free(void* ptr) {
    if (!ptr)
        return;

    Block* block = (Block*)ptr - 1;

    printf(
        "[FREE] user=%p header=%p size=%zu\n",
        ptr,
        (void*)block,
        block->size
    );

    block->free = 1;
    coalesce();
    print_blocks();
}



int main(void) {
    void* a = my_malloc(100);
    void* b = my_malloc(200);
    void* c = my_malloc(50);

    my_free(b);
    my_free(a);
    my_free(c);

    return 0;
}
