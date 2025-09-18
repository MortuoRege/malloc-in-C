#include <unistd.h>
#include <stdio.h>

int main() {
    void* old_brk = sbrk(0);            // current break
    printf("Old break: %p\n", old_brk);

    void* new_brk = sbrk(4096);         // move up by 4 KB
    printf("Allocated 4 KB at: %p\n", old_brk);  // usable memory starts here
    printf("New break: %p\n", sbrk(0));

    return 0;
}
