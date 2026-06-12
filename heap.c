/*
 * the heap
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "heap.h"
#include "bistree.h"
#include "globals.h"
#include "list.h"

void heap_init(Heap* heap, unsigned int size, void (*collector)(BisTree*)){
    heap->base  = mmap ( NULL, size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
    heap->size  = size;
    heap->limit = size;
    heap->top   = 0;
    #ifdef _MS
    heap->freeb = NONE;
    #endif
    heap->collector = collector;

    #ifdef _CC
    heap->to_space = 0;
    heap->from_space = heap->size / 2;
    heap->limit = size / 2;
    #endif

    return;
}

void heap_destroy(Heap* heap) {
    munmap(heap->base, heap->size);
    return;
}

void* init_block(unsigned int nbytes) {
    _block_header* q = (_block_header*)(deref(heap->top));
    q->marked = 0;
    q->size   = nbytes;
    #ifdef _CC
    q->forward_pointer = NONE;
    #endif
    q->pointers = INIT_TREE_POINTER_SIZE | TREE_POINTERS;
    char *p = deref(heap->top) + sizeof(_block_header);
    heap->top = heap->top + sizeof(_block_header) + nbytes;
    return p;
}

#ifdef _MS
void* freeb_pop(char nbytes) {
    gc_pointer p    = heap->freeb;
    gc_pointer prev = NONE;

    while (p != NONE) {
        _block_header *bh = (_block_header*) deref(p);
        if (bh->size >= nbytes) break;

        prev = p;
        p = bh->forward_pointer;
    }

    _block_header *bh = (_block_header*) deref(p);

    if (prev != NONE) {
        _block_header *bh_prev = (_block_header*) deref(prev);
        bh_prev->forward_pointer = bh->forward_pointer;
    } else heap->freeb = bh->forward_pointer;

    return (void*) bh + sizeof(_block_header);
}
#endif

void* my_malloc(unsigned int nbytes) {
    if( heap->top + sizeof(_block_header) + nbytes < heap->limit ) {
       return init_block(nbytes);
    } else {
        #ifdef _MS
        printf("**malloc**                not enough space in heap, checking freeb list...\n");

        if (heap->freeb != NONE)
            return freeb_pop(nbytes);
        #endif

        printf("**malloc**                not enough space, performing GC...\n");

        printf("**malloc**                ");
        heap->collector(roots);

        if(heap->top + sizeof(_block_header) + nbytes < heap->limit) {
            return init_block(nbytes);
        }

        #ifdef _MS
        if (heap->freeb == NONE) {
            printf("**malloc**                not enough space after GC..\n.");
            return NULL;
        }
        return freeb_pop(nbytes);
        #else
        printf("**malloc**                not enough space after GC...\n");
        return NULL;
        #endif
    }
}
