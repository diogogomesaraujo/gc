/*
 * collector.c
 */

#include <stdio.h>

#include "bistree.h"

void mark_sweep_gc(BisTree* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */

   /*
    * sweep phase:
    * go through entire heap,
    * add unmarked to free list
    */
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
