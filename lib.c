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

/* globals */

int            VM_threshold;
int            VM_loop_counter;
int            VM_stack_size;
int            VM_stack_top;
int            VM_max_rounds;
int*           VM_stack;
unsigned char* VM_program;

void vm(int            heap_size,
        int            threshold_arg,
        int            max_roots_arg,
        int            max_rounds_arg,
        int            stack_size_arg,
        unsigned char* program,
        int            program_size) {
    /* initialize threshold */
    VM_threshold = threshold_arg;

    /* initialize roots */
    max_roots = max_roots_arg;
    roots = (BisTree*)malloc(max_roots * sizeof(BisTree));

    for (int i = 0; i < max_roots; i++)
         bistree_init(&roots[i]);

    /* max rounds */
    VM_max_rounds = max_rounds_arg;

    /* initialize stack */
    VM_stack_size = stack_size_arg;
    VM_stack = (int*)malloc(VM_stack_size * sizeof(int));
    VM_stack_top = 0;

    /* initialize the heap */
    heap = (Heap*)malloc(sizeof(Heap));
    #ifdef _MS
    heap_init(heap, heap_size, mark_sweep_gc);
    #endif

    #ifdef _MC
    heap_init(heap, heap_size, mark_compact_gc);
    #endif

    #ifdef _CC
    heap_init(heap, heap_size, copy_collection_gc);
    #endif

    /* initialize program */
    VM_program = (unsigned char[]) {
        LLP,  0xc4,
        RND,  0x14,
        SEL,  PAD,
        RND,  0xc4,
        BLT,  0x0e,
        DEL,  PAD,
        J,    0x10,
        ADD,  PAD,
        JLP,  0x02,
        QUIT, 0x00
    };

    /* run program */
    unsigned char* pc = VM_program;

    for (int i = 0; i < VM_max_rounds; i++) {
        unsigned char opcode = pc[0];

        int number;
        BisTree* root;

        switch (opcode) {
            case LLP:
                /* llp */
                VM_loop_counter = pc[1];
                pc = pc + 2;

                printf("**gc-virtual-machine**    executing lpp: loop_counter = %d, pc = %p\n", VM_loop_counter, pc);

                break;

            case JLP:
                /* jlp */
                VM_loop_counter--;

                if (VM_loop_counter != 0)
                    pc = &VM_program[pc[1]];
                else
                    pc = pc + 2;

                printf("**gc-virtual-machine**    executing jpl: pc = %p\n", pc);

                break;

            case J:
                /* j */
                pc = &VM_program[pc[1]];

                printf("**gc-virtual-machine**    executing j: pc = %p\n", pc);

                break;

            case BLT:
                /* blt */
                if (VM_stack[--VM_stack_top] < VM_threshold) {
                    pc = &VM_program[pc[1]];
                }
                else pc = pc + 2;

                printf("**gc-virtual-machine**    executing blt: pc = %p, stack_top = %d\n", pc, VM_stack_top);

                break;

            case RND:
                /* rnd */
                VM_stack[VM_stack_top++] = rand() % pc[1];
                pc = pc + 2;

                printf("**gc-virtual-machine**    executing blt: stack_top = %d, pc = %p\n", VM_stack[VM_stack_top], pc);

                break;

            case SEL:
                /* sel */
                VM_stack[VM_stack_top++] = rand() % max_roots;
                pc = pc + 2;

                printf("**gc-virtual-machine**    executing sel: stack_top = %d, pc = %p\n", VM_stack[VM_stack_top], pc);
                break;

            case ADD:
                /* add */
                number = VM_stack[VM_stack_top - 2];

                int index = VM_stack[VM_stack_top - 1];
                root = &roots[index];

                bistree_insert(root, number);

                VM_stack_top = VM_stack_top - 2;
                pc = pc + 2;

                printf("**gc-virtual-machine**    executing add: number = %d, root = %p, stack_top = %d, pc = %p\n", number, root, VM_stack[VM_stack_top], pc);

                break;

            case DEL:
                /* del */
                number = VM_stack[VM_stack_top - 2];
                root = &roots[VM_stack[VM_stack_top - 1]];

                bistree_remove(root, number);

                VM_stack_top = VM_stack_top - 2;
                pc = pc + 2;

                printf("**gc-virtual-machine**    executing del: number = %d, root = %p, stack_top = %d, pc = %p\n", number, root, VM_stack[VM_stack_top], pc);

                break;

            case QUIT:
                /* quit */
                printf("**gc-virtual-machine**    see you space cowboy!\n");
                exit(0);

            default:
                /* error, exit */
                printf("**gc-virtual-machine**    %x: unkown opcode\n", opcode);
                exit(1);
        }
    }
}
