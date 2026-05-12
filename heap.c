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
    heap->limit = heap->base + size;
    heap->top   = heap->base;
    heap->freeb = (List*)malloc(sizeof(List));
    list_init(heap->freeb);
    heap->collector = collector;

    #ifdef _CC
    heap->to_space = heap->base;
    heap->from_space = heap->base + heap->size / 2;
    heap->limit = heap->base + size / 2;
    #endif

    return;
}

void heap_destroy(Heap* heap) {
    munmap(heap->base, heap->size);
    return;
}

void* my_malloc(unsigned int nbytes) {
    if( heap->top + sizeof(_block_header) + nbytes < heap->limit ) {
       _block_header* q = (_block_header*)(heap->top);
       q->marked = 0;
       q->size   = nbytes;
       #ifdef _CC
       q->forward_pointer = NULL;
       #endif
       char *p = heap->top + sizeof(_block_header);
       heap->top = heap->top + sizeof(_block_header) + nbytes;
       return p;
    } else {
        printf("my_malloc: not enough space in heap, checking freeb list...");

        if (!list_isempty(heap->freeb)) return list_popfirst(heap->freeb);

        printf("my_malloc: not enough space, performing GC...");

        heap->collector(roots);

        if(heap->top + sizeof(_block_header) + nbytes < heap->limit) {
           _block_header* q = (_block_header*)(heap->top);
           q->marked = 0;
           q->size   = nbytes;
           #ifdef _CC
           q->forward_pointer = NULL;
           #endif
           char *p = heap->top + sizeof(_block_header);
           heap->top = heap->top + sizeof(_block_header) + nbytes;
           return p;
        }

        if (list_isempty(heap->freeb)) {
            printf("my_malloc: not enough space after GC...");
            return NULL;
        }

       return list_popfirst(heap->freeb);
    }
}
