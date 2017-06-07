#include <stdlib.h>

#include "types_lau.h"

typedef struct str_stack
{
    long top;
    node **arr;
} stack;

static void stack_alloc(stack *s, long n)
{
    s->arr = (node**)malloc(n * sizeof(node*));
    s->top = 0;
}

static void stack_free(stack *s)
{
    free(s->arr);
}

static int stack_empty(const stack *s)
{
    return s->top == 0;
}

static node *stack_pop(stack *s)
{
    return s->arr[(s->top)--];
}

static void stack_push(stack *s, node *n)
{
    s->arr[++(s->top)] = n;
}

void lau_indegree(node *first, long count, node *source)
{
    node *i, *j, *last;
    arc *arc_ij, *arc_last;

    for (i = first, last = first + count; i < last; ++i) {
        i->indeg = i->indeg_backup = 0;
    }

    stack stk;
    stack_alloc(&stk, count);
    stack_push(&stk, source);

    while (!stack_empty(&stk)) {
        i = stack_pop(&stk);
        arc_last = (i + 1)->first;
        for (arc_ij = i->first; arc_ij < arc_last; ++arc_ij) {
            j = arc_ij->head;
            j->indeg++;
            j->indeg_backup++;
            if (j->indeg == 1) {
                stack_push(&stk, j);
            }
        }
    }

    stack_free(&stk);
}
