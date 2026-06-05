/*
 * heap.h
 */

#ifndef HEAP_H
#define HEAP_H

#include "list.h"
#include "bistree.h"
#include <limits.h>

#define is_pointer(p, i) ((p >> i) & 1)
#define pointers_size(p) (p >> 4)

#define deref(o) ((void*)heap->base + o)
#define toref(p) ((unsigned int)((void*)(p) - (void*)heap->base))

#define NONE UINT_MAX
#define TREE_POINTERS 6
#define TREE_BLOCK_NUMBER 3
#define INIT_TREE_POINTER_SIZE (3 << 4)

typedef unsigned int gc_pointer;

typedef struct {
   unsigned int  marked;
   char          size;
   gc_pointer    forward_pointer;
   char          pointers;
} _block_header;

typedef struct {
   unsigned int  size;
   char*         base;
   gc_pointer    top;
   gc_pointer    limit;
   #ifdef _CC
   gc_pointer    to_space;
   gc_pointer    from_space;
   #endif
   #ifdef _MS
   gc_pointer    freeb;
   #endif
   void (*collector)(BisTree*);
} Heap;

void heap_init(Heap* heap, unsigned int size, void (*collector)(BisTree*));

void heap_destroy(Heap* heap);

void* my_malloc(unsigned int nbytes);

#endif
