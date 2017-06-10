
#define NNULL (node*)NULL
#define VERY_FAR  1073741823

#define OUT_OF_STACKS  0
#define IN_NEW_PASS    1
#define IN_TOP_SORT    2

static void scan(node *i, stack *top_sort, long *num_scans)
{
    node *j;
    arc *a, *e;

    *num_scans = *num_scans + 1;
    for (a = i->first, e = (i + 1)->first; a != e; a++) {
        j = a->head;
        if (i->dist + a->len < j->dist) {
            j->dist = i->dist + a->len;
            j->parent = i;
            if (j->status != IN_TOP_SORT) {
                stack_push(top_sort, j);
                j->status = IN_NEW_PASS;
            }
        }
    }
}

/* simplified implementation of GOR1 */
int gor(long n, node *nodes, node *source)
{
    node *e, *i;
    long num_scans;
    stack top_sort, new_pass;

    /* initialization */
    for (i = nodes, e = nodes + n; i != e; i++) {
        i->parent = NNULL;
        i->dist = VERY_FAR;
        i->status = OUT_OF_STACKS;
    }
    source->parent = source;
    source->dist = 0;

    stack_alloc(&top_sort);
    stack_alloc(&new_pass);
    stack_push(&top_sort, source);
    source->status = IN_NEW_PASS;
    num_scans = 0;

    /* main loop */
    while (!stack_empty(&top_sort)) {
        /* topsort */
        while (!stack_empty(&top_sort)) {
            i = stack_pop(&top_sort);
            if (i->status == IN_TOP_SORT) {
                stack_push(&new_pass, i);
            } else if (i->status == IN_NEW_PASS) {
                stack_push(&top_sort, i);
                i->status = IN_TOP_SORT;
                scan(i, &top_sort, &num_scans);
            }
        }
        /* pass */
        while (!stack_empty(&new_pass)) {
            i = stack_pop(&new_pass);
            scan(i, &top_sort, &num_scans);
            i->status = OUT_OF_STACKS;
        }
    }

    n_scans = num_scans;
    stack_free(&top_sort);
    stack_free(&new_pass);

    return 0;
}
