# malloc-in-C
This project is for me to better understand how virtual memory and dynamic memory allocation work under the hood.

When a program starts running, the operating system assigns it a virtual address space. This creates the illusion that the program owns the entire main memory, even though it actually shares physical RAM with many other processes.

On a 32-bit system the virtual address space is up to 2³² bytes (4 GB). On a 64-bit system it could, in theory, be up to 2⁶⁴ bytes, though in practice operating systems only make part of that space available.

Because the virtual address space is so large, and not all of it is needed at once, the OS divides it into fixed-size chunks called pages. The most common page size is 4 KB, but larger pages (like 2 MB or even 1 GB) also exist.

Inside this virtual address space, different regions are set aside for:

Code (text segment) – the compiled instructions of the program.

Data segment – global and static variables.

Heap – memory for dynamically allocated data.

Stack – memory for function calls, local variables, and return addresses.

All of these are just groups of virtual pages arranged by the operating system.

Now, for the part about the heap:
The heap is where dynamic memory allocation happens, and it’s managed through the function malloc.

Here’s the key point:

malloc itself doesn’t talk directly to the operating system.

Instead, it is a library-level abstraction that manages a pool of memory inside the process.

When it needs more memory, malloc uses system calls like brk / sbrk and mmap to request additional pages from the OS.

This two-layer system is what makes dynamic memory allocation efficient:

The OS handles virtual memory and page management.

malloc handles allocation and bookkeeping inside the heap.

As for my implementation of malloc:
The actual memory allocator is much more complex compared to this simple version.

One of the biggest problems that needed to be solved was data alignment. I handled this by ensuring that every allocation request is a multiple of 8 bytes. This is done using a C macro I defined:
```c
#define ALIGN8(x) ((x + 7) & ~7)
```  
The allocator also uses a struct, which I aligned to 8 bytes with __attribute__((align(8))), to keep track of allocated segments of memory. Each struct contains:

a size_t size field,

a bool isFree flag,

and a pointer to the next struct.

This essentially makes the allocator’s bookkeeping a linked list.

When the user requests memory, the malloc function searches through the list for a free block that fits the request. If a free block is found, the user data is placed there. If no suitable free block is found, then another block is requested and added to the list.
