#ifndef _LAU_STACK_H_
#define _LAU_STACK_H_
 
#include <assert.h>
#include <stdlib.h>

static const long MAX = 1024 * 1024 * 16;

struct node;

typedef struct str_stack
{
    long max;
    long top;
    node **arr;
} stack;

static void stack_alloc(stack *s)
{
    s->max = MAX;
    s->top = -1;
    s->arr = (node**)malloc(s->max * sizeof(node*));
}

static void stack_free(stack *s)
{
    free(s->arr);
}

static int stack_empty(const stack *s)
{
    return s->top == -1;
}

static node *stack_pop(stack *s)
{
    assert(s->top >= 0);
    return s->arr[s->top--];
}

static void stack_push(stack *s, node *n)
{
    ++s->top;
    assert(s->top < s->max);
    s->arr[s->top] = n;
}

#endif // _LAU_STACK_H_
