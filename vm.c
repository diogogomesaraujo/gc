#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "heap.h"
#include "bistree.h"
#include "globals.h"
#include "collector.h"

/* language */

#define PAD  0x00 /* padding for function without arguments */
#define LLP  0x01 /* loads loop-counter with n */
#define JLP  0x02 /* decrements loop-counter, jumps to L if not 0 */
#define J    0x03 /* jumps to L */
#define BLT  0x04 /* jumps to L if Stack[top] < threshold */
#define RND  0x05 /* generates integer up to n, stores it in Stack[top] */
#define SEL  0x06 /* generates integer up to roots-size, stores it in Stack[top] */
#define ADD  0x07 /* adds integer Stack[top-1] to tree Roots[Stack[top-2]] */
#define DEL  0x08 /* deletes integer Stack[top-1] from tree Roots[Stack[top-2]] */
#define QUIT 0x09 /* exits the program */

/* constants */

#define HEAP_SIZE        (2 * 1024)  /* 2 KByte */

int      VM_threshold;
int      VM_loop_counter;
int      VM_roots_size;
int      VM_stack_size;
int      VM_stack_top;
int      VM_max_rounds;
int*     VM_stack;
Heap*    VM_heap;
char*    VM_program;
BisTree* VM_roots;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("arguments should be: ./vm <threshold> <roots_size> <max rounds> <stack_size>");
        exit(0);
    }

    /* initialize threshold */
    VM_threshold = atoi(argv[1]);

    /* initialize roots */
    VM_roots_size = atoi(argv[2]);
    VM_roots = (BisTree*)malloc(VM_roots_size * sizeof(BisTree));

    for (int i = 0; i < VM_roots_size; i++)
         bistree_init(&VM_roots[i]);

    /* max rounds */
    VM_max_rounds = atoi(argv[3]);

    /* initialize stack */
    VM_stack_size = atoi(argv[4]);
    VM_stack = (int*)malloc(VM_stack_size * sizeof(int));
    VM_stack_top = 0;

    /* initialize the heap */
    VM_heap = (Heap*)malloc(sizeof(Heap));
    #ifdef _MS
    heap_init(VM_heap, HEAP_SIZE, mark_sweep_gc);
    #endif

    #ifdef _MC
    heap_init(VM_heap, HEAP_SIZE, mark_compact_gc);
    #endif

    #ifdef _CC
    heap_init(VM_heap, HEAP_SIZE, copy_collection_gc);
    #endif

    /* initialize program */
    VM_program = (char[]) {
        LLP,  0xc4,
        RND,  0x14,
        SEL,  PAD,
        RND,  0xc4,
        BLT,  0x0e,
        DEL,  PAD,
        J,    0x10,
        ADD,  PAD,
        LLP,  PAD,
        JLP,  0x02,
        QUIT, 0x00
    };

    /* run program */
    char* pc = VM_program;

    for (int i = 0; i < VM_max_rounds; i++) {
        char opcode = pc[0];
        int number;
        BisTree* root;

        switch (opcode) {
            case LLP:
                /* llp */
                VM_loop_counter = pc[1];
                pc = pc + 2;
                printf("executing lpp\n");
                break;

            case JLP:
                /* jlp */
                VM_loop_counter--;

                if (VM_loop_counter != 0)
                    pc = &VM_program[(int) pc[1]];
                else
                    pc = pc + 2;

                printf("executing jpl\n");
                break;

            case J:
                /* j */
                pc = &VM_program[(int) pc[1]];

                printf("executing j\n");
                break;

            case BLT:
                /* blt */
                if (VM_stack[--VM_stack_top] < VM_threshold) {
                    printf("executing blt\n");
                    pc = &VM_program[(int) pc[1]];
                }
                else
                    printf("executing blt\n");
                    pc = pc + 2;

                break;

            case RND:
                /* rnd */
                VM_stack[VM_stack_top++] = rand() % pc[1];
                pc = pc + 2;

                printf("executing rnd\n");
                break;

            case SEL:
                /* sel */
                VM_stack[VM_stack_top++] = rand() % VM_roots_size;
                pc = pc + 2;

                printf("executing sel\n");
                break;

            case ADD:
                /* add */
                number = VM_stack[VM_stack_top - 1];
                root = &VM_roots[VM_stack[VM_stack_top - 2]];

                bistree_insert(root, number);

                VM_stack_top = VM_stack_top - 2;
                pc = pc + 2;

                printf("executing add\n");
                break;

            case DEL:
                /* del */
                number = VM_stack[VM_stack_top - 1];
                root = &VM_roots[VM_stack[VM_stack_top - 2]];

                bistree_remove(root, number);

                VM_stack_top = VM_stack_top - 2;
                pc = pc + 2;

                printf("executing del\n");
                break;

            case QUIT:
                /* quit */
                exit(0);

            default:
                /* error, exit */
                printf("%x: unkown opcode\n", opcode);
                exit(1);
        }
    }

    return 0;
}
