#ifndef _LAU_DEQUE_H_
#define _LAU_DEQUE_H_

#include <assert.h>
#include <stdbool.h>
#include "types_lau.h"

#define MAX (1024 * 1024 * 16)
 
typedef struct struct_deque_t
{
    node  **arr;
    long  front;
    long  rear;
    long  size;
} deque_t;

void deque_create(deque_t *d)
{
    d->size = MAX;
    d->arr = (node**)malloc(d->size * sizeof(node*));
    d->front = -1;
    d->rear = 0;
}
 
void deque_free(deque_t *d)
{
    free(d->arr);
}

bool deque_full(deque_t *d)
{
    return (d->front == 0 && d->rear == d->size-1)
        || d->front == d->rear + 1;
}
 
bool deque_empty(deque_t *d)
{
    return (d->front == -1);
}
 
void deque_push_front(deque_t *d, node *key)
{
    assert(!deque_full(d));
    if (d->front == -1) {
        d->front = 0;
        d->rear = 0;
    } else if (d->front == 0) {
        d->front = d->size - 1;
    } else {
        d->front = d->front - 1;
    }
    d->arr[d->front] = key;
}

void deque_push_back(deque_t *d, node *key)
{
    assert(!deque_full(d));
    if (d->front == -1) {
        d->front = 0;
        d->rear = 0;
    } else if (d->rear == d->size - 1) {
        d->rear = 0;
    } else {
        d->rear = d->rear + 1;
    }
    d->arr[d->rear] = key;
}
 
node *deque_pop_front(deque_t *d)
{
    node *key = d->arr[d->front];
    if (d->front == d->rear) {
        d->front = -1;
        d->rear = -1;
    } else {
        if (d->front == d->size - 1)
            d->front = 0;
        else
            d->front = d->front + 1;
    }
    return key;
}

#endif // _LAU_DEQUE_H_
