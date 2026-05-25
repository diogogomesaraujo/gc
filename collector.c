/*
 * collector.c
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "bistree.h"
#include "heap.h"
#include "globals.h"
#include "list.h"

#if defined(_MS) || defined(_MC)
void mark(void *n) {
    if (n == NULL) return;

    _block_header *bh = (_block_header*) ((char*) n - sizeof(_block_header));

    if (bh->marked) return;
    bh->marked = true;

    typeof(n) **blocks = (typeof(n)**) n;

    for (char i = 0; i < pointers_size(bh->pointers); i++)
        if (is_pointer(bh->pointers, i))
            mark(blocks[i]);
}
#endif

#ifdef _MS
void sweep() {
    gc_pointer top  = heap->top;
    gc_pointer base = 0;

    heap->freeb = NONE;

    for (gc_pointer bh = base;
        bh < top;
        bh = bh + sizeof(_block_header) + ((_block_header*) offset_to_pointer(bh))->size)
    {
        _block_header *bhh = (_block_header*) offset_to_pointer(bh);

        if (!bhh->marked) {
            bhh->forward_pointer = heap->freeb;
            heap->freeb = pointer_to_offset((void*) bhh);
        }

        bhh->marked = false;
    }
}
#endif

#ifdef _MC
gc_pointer compute_locations() {
    gc_pointer limit = heap->top;
    gc_pointer free  = sizeof(_block_header);
    gc_pointer base  = 0;

    for (gc_pointer bh = base;
        bh < limit;
        bh += sizeof(_block_header) + ((_block_header*) offset_to_pointer(bh))->size)
    {
        _block_header *bhh = (_block_header*) offset_to_pointer(bh);

        if (bhh->marked) {
            bhh->forward_pointer = free;
            free += sizeof(_block_header) + bhh->size;
        }
    }

    return free - sizeof(_block_header);
}

void update_references() {
    // update roots
    for (int i = 0; i < max_roots; i++) {
        typeof(roots[i].root)* data = roots[i].root;
        if (data == NULL) continue;

        _block_header *bh = (_block_header*) ((void*) roots[i].root - sizeof(_block_header));
        roots[i].root = (typeof(roots[i].root)*) offset_to_pointer(bh->forward_pointer);
    }

    // update fields
    gc_pointer top  = heap->top;
    gc_pointer base = 0;

    for (gc_pointer bh = base;
        bh < top;
        bh += sizeof(_block_header) + ((_block_header*) offset_to_pointer(bh))->size)
    {
        _block_header *bhh = (_block_header*) offset_to_pointer(bh);

        if (bhh->marked) {
            void* data_ptr = (void*) bhh + sizeof(_block_header);
            typeof(data_ptr) *data = (typeof(data_ptr)*) data_ptr;

            typeof(data_ptr) **blocks = (typeof(data_ptr)**) data;

            for (char i = 0; i < pointers_size(bhh->pointers); i++) {
                if (is_pointer(bhh->pointers, i) && blocks[i] != NULL) {
                    _block_header *p = (_block_header*) ((void*)blocks[i] - sizeof(_block_header));
                    blocks[i] = (BiTreeNode*) offset_to_pointer(p->forward_pointer);
                }
            }
        }
    }
}

void relocate() {
    gc_pointer top = heap->top;
    gc_pointer base = 0;

    for (gc_pointer bh = base;
        bh < top;
        bh += sizeof(_block_header) + ((_block_header*) offset_to_pointer(bh))->size)
    {
        _block_header *bhh = (_block_header*) offset_to_pointer(bh);

        if (bhh->marked) {
            bhh->marked = false;
            memcpy(offset_to_pointer(bhh->forward_pointer) - sizeof(_block_header), bhh, bhh->size + sizeof(_block_header));
        }
    }
}

void compact() {
    gc_pointer free = compute_locations();
    update_references();
    relocate();
    heap->top = free;
}
#endif

#ifdef _CC
void flip() {
    gc_pointer   temp = heap->from_space;
    heap->from_space  = heap->to_space;
    heap->to_space    = temp;

    heap->top   = heap->to_space;
    heap->limit = heap->to_space + heap->size / 2;
}

void *copy_ref(void* from_ref) {
    void *to_ref = offset_to_pointer(heap->top);
    heap->top += sizeof(_block_header) + sizeof(BiTreeNode);

    _block_header *bh = (_block_header*) (from_ref - sizeof(_block_header));
    memcpy(to_ref, bh, sizeof(_block_header) + sizeof(BiTreeNode));

    return to_ref + sizeof(_block_header);
}

void *copy_tree(BiTreeNode* root) {
    if (root == NULL) return root;

    _block_header *bh = (_block_header*) ((void*) root - sizeof(_block_header));

    if (bh->forward_pointer != NONE) return offset_to_pointer(bh->forward_pointer);

    typeof(root) *to_ref = (typeof(root)*) copy_ref(root);

    bh->forward_pointer = pointer_to_offset(to_ref);

    typeof(root) **to_ref_blocks = (typeof(root)**) to_ref;
    typeof(root) **blocks = (typeof(root)**) root;

    for (char i = 0; i < pointers_size(bh->pointers); i++)
        if (is_pointer(bh->pointers, i))
            to_ref_blocks[i] = copy_tree(blocks[i]);

    _block_header *bh_to = (_block_header*) ((void*) to_ref - sizeof(_block_header));
    bh_to->forward_pointer = NONE;

    return (void*) to_ref;
}
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

   // flip

   flip();

   // update roots recursively

   printf("copying()...");

   for (int i = 0; i < max_roots; i++)
       roots[i].root = copy_tree(roots[i].root);

   printf("gcing()...\n");
   return;
}
#endif
