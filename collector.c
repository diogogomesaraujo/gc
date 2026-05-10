/*
 * collector.c
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bistree.h"
#include "heap.h"
#include "globals.h"
#include "list.h"

#if defined(_MS) || defined(_MC)
void mark(BiTreeNode *n) {
    if (n == NULL) return;

    _block_header *bh = (_block_header*) ((char*) n - sizeof(_block_header));

    if (bh->marked) return;
    bh->marked = true;

    mark(n->right);
    mark(n->left);
}
#endif

#ifdef _MS
void sweep() {
    char* top = heap->top;
    char* base = heap->base;

    for (char *bh = base;
        (char*) bh < top;
        bh = bh + sizeof(_block_header) + ((_block_header*) bh)->size)
    {
        _block_header *bhh = (_block_header*) bh;

        void* data = (void*)(bh + sizeof(_block_header));

        if (!bhh->marked) list_addfirst(heap->freeb, data);

        bhh->marked = false;
    }
}
#endif

#ifdef _MC
void* compute_locations() {
    char* limit = heap->top;
    char* free = heap->base + sizeof(_block_header);
    char* base = heap->base;

    for (char *bh = base;
        (char*) bh < limit;
        bh += sizeof(_block_header) + ((_block_header*) bh)->size)
    {
        _block_header *bhh = (_block_header*) bh;

        if (bhh->marked) {
            bhh->forward_pointer = (void*) free;
            free += sizeof(_block_header) + ((_block_header*) bh)->size;
        }
    }

    return free - sizeof(_block_header);
}

void update_references() {
    // update roots
    for (int i = 0; i < max_roots; i++) {
        BiTreeNode* data = roots[i].root;
        if (data == NULL) continue;

        _block_header *bh = (_block_header*) ((void*) roots[i].root - sizeof(_block_header));
        roots[i].root = (BiTreeNode*) bh->forward_pointer;
    }

    // update fields
    char* top = heap->top;
    char* base = heap->base;

    for (char *bh = base;
        (char*) bh < top;
        bh += sizeof(_block_header) + ((_block_header*) bh)->size)
    {
        _block_header *bhh = (_block_header*) bh;

        if (bhh->marked) {
            BiTreeNode *data = (BiTreeNode*) ((void*) bhh + sizeof(_block_header));

            if (data->left != NULL) {
                _block_header *left = (_block_header*) ((void*)data->left - sizeof(_block_header));
                data->left = (BiTreeNode*) left->forward_pointer;
            }

            if (data->right != NULL) {
                _block_header *right = (_block_header*) ((void*) data->right - sizeof(_block_header));
                data->right = (BiTreeNode*) right->forward_pointer;
            }
        }
    }
}

void relocate() {
    char* top = heap->top;
    char* base = heap->base;

    for (char *bh = base;
        (char*) bh < top;
        bh += sizeof(_block_header) + ((_block_header*) bh)->size)
    {
        _block_header *bhh = (_block_header*) bh;

        if (bhh->marked) {
            bhh->marked = false;
            memcpy(bhh->forward_pointer - sizeof(_block_header), bhh, bhh->size + sizeof(_block_header));
        }
    }
}

void compact() {
    char* free = (char*) compute_locations();
    update_references();
    relocate();
    heap->top = free;
}
#endif

#ifdef _CC
#endif

#ifdef _MS
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
#endif

#ifdef _MC
void mark_compact_gc(BisTree* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */

   printf("marking()...");

   for (int i = 0; i < max_roots; i++) mark(roots[i].root);

   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */

   printf("compacting()...");

   compact();

   printf("gcing()...\n");
   return;
 }
#endif

#ifdef _CC
void copy_collection_gc(BisTree* roots) {
   /*
    * go throught all roots,
    * traverse trees in from_space,
    * copy reachable to to_space
    */
   printf("gcing()...\n");
   return;
}
#endif
