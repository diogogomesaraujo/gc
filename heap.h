/*
 * heap.h
 */

#ifndef HEAP_H
#define HEAP_H

#include "list.h"
#include "bistree.h"

#define is_pointer(p, i) (p >> i) & 1
#define pointers_size(p) (p >> 4)

#define TREE_POINTERS 6
#define TREE_BLOCK_NUMBER 3
#define INIT_TREE_POINTER_SIZE (3 << 4)

typedef struct {
   unsigned int marked;
   char         size;
   void*        forward_pointer;
   char         pointers;
} _block_header;

typedef struct {
   unsigned int size;
   char*        base;
   char*        top;
   char*        limit;
   #ifdef _CC
   char*        to_space;
   char*        from_space;
   #endif
   #ifdef _MS
   void*        freeb;
   #endif
   void (*collector)(BisTree*);
} Heap;

void heap_init(Heap* heap, unsigned int size, void (*collector)(BisTree*));

void heap_destroy(Heap* heap);

void* my_malloc(unsigned int nbytes);

#endif
