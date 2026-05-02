/*
 * collector.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "bistree.h"
#include "heap.h"
#include "globals.h"
#include "list.h"

void mark(BiTreeNode *n) {
    if (n == NULL) return;

    _block_header *bh = (_block_header*) ((void*) n - sizeof(_block_header));

    if (bh->marked) return;
    bh->marked = true;

    mark(n->right);
    mark(n->left);
}

void sweep() {
    list_removeall(heap->freeb);

    char* limit = heap->limit;
    char* base = heap->base;

    for (char *bh = base;
        (char*) bh < limit;
        bh = bh + sizeof(_block_header) + ((_block_header*) bh)->size)
    {
        _block_header *bhh = (_block_header*) bh;

        void* data = (void*)(bh + sizeof(_block_header));

        if (!bhh->marked) list_addlast(heap->freeb, data);

        bhh->marked = false;
    }
}

void mark_sweep_gc(BisTree* roots) {
   /*
    * mark phase
    * - every object has live bit set to 0
    * - start from GC roots
    * - traverse graph, set bit to 1 for every object visited
    */

   printf("marking()...");

   for (int i = 0; i < max_roots; i++) mark(roots[i].root);

   /*
    * sweep phase:
    * go through entire heap,
    * add unmarked to free list
    */

   printf("sweeping()...");

   sweep();

   printf("gcing()...\n");
   return;
 }

void mark_compact_gc(BisTree* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */

   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */
   printf("gcing()...\n");
   return;
 }

void copy_collection_gc(BisTree* roots) {
   /*
    * go throught all roots,
    * traverse trees in from_space,
    * copy reachable to to_space
    */
   printf("gcing()...\n");
   return;
}
