/*
 * the mutator
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "heap.h"
#include "bistree.h"
#include "globals.h"
#include "collector.h"

#define  MAX_KEY_VALUE  15

#define  HEAP_SIZE      (2 * 1024)  /* 2 KByte */

int main(int argc, char** argv) {
   /* initialize the main parameters */
   int threshold  = atoi(argv[1]);  /* an integer in (0,100) */
   max_roots  = atoi(argv[2]);  /* a positive integer */
   int max_rounds = atoi(argv[3]);  /* a positive integer */

   /* initialize the heap space used to store tree nodes */
   heap  = (Heap*)malloc(sizeof(Heap));
   #ifdef _MS
   heap_init(heap, HEAP_SIZE, mark_sweep_gc);
   #endif

   #ifdef _MC
   heap_init(heap, HEAP_SIZE, mark_compact_gc);
   #endif

   #ifdef _CC
   heap_init(heap, HEAP_SIZE, copy_collection_gc);
   #endif

   /* initialize set of tree roots */
   roots = (BisTree*)malloc(max_roots * sizeof(BisTree));
   for( int i = 0; i < max_roots; i++ )
      bistree_init(&roots[i]);

   /* add/delete integers to/from the trees */
   srandom(getpid());
   for( int i = 0; i < max_rounds; i++ ) {
      /* select bistree */
      BisTree* aroot = &roots[random() % max_roots];
      /* toss coin */
      int toss = random() % 100;
      if( toss > threshold ) {
         /* add integer to one of the roots */
         if (!bistree_insert(aroot, random() % MAX_KEY_VALUE)) continue;
         fprintf(stdout, "tree size is %d\n", bistree_size(aroot));
         fprintf(stdout, "[%d] (inorder traversal adding)\n", i);
         bistree_inorder(aroot);
      }
      else {
         /* remove integer from one of the roots */
         if(!bistree_remove(aroot, random() % MAX_KEY_VALUE)) continue;
         fprintf(stdout, "tree size is %d\n", bistree_size(aroot));
         fprintf(stdout, "[%d] (inorder traversal removing)\n", i);
         bistree_inorder(aroot);
      }
   }
   /* exit gracefully */
   heap_destroy(heap);
   free(roots);
   return 0;
}
