#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#define ALIGN8(x) ((x + 7) & ~7)


struct __attribute__((aligned(8))) block
{
    size_t size;
    bool is_free;
    struct block* next;
};
static struct block* head = NULL;

void* my_malloc(size_t size)
{
    size_t aligned_size = ALIGN8(size);
    void* ptr = NULL;
    if(head == NULL)
    {
        ptr = sbrk(aligned_size + sizeof(struct block));
        if (ptr == (void*)-1) return NULL;
        struct block* b = (struct block*)ptr;
        b->size = aligned_size;
        b->is_free = false;
        b->next = NULL;
        head = b;
        return (void*)((char*)ptr + sizeof(struct block));
    }
    else {
    {
        struct block* prev = NULL;
        struct block* current = head;
        while(current != NULL)
        {
            if(current->is_free && (current->size >= aligned_size))
            {
                current->is_free = false;
                return (void*)((char*)current + sizeof(struct block));
            }
            prev = current;
            current = current->next;
        }
        struct block* new_block = sbrk(aligned_size + sizeof(struct block));
        if (new_block == (void*)-1) return NULL;
        struct block* b = (struct block*)new_block;
        b->size = aligned_size;
        b->is_free = false;
        b->next = NULL;
        prev->next = b;
        return (void*)((char*)b + sizeof(struct block));

    }
}


int main() {
    int *p = (int*)my_malloc(sizeof(int));
    *p = 42;
    printf("Allocated memory: %d\n", *p);
    return 0;
}
