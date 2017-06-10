#include <stdlib.h>

#include "types_lau.h"
#include "lau_stack.h"

void lau_indegree(node *first, long count, node *source)
{
    node *i, *j, *last;
    arc *arc_ij, *arc_last;

    for (i = first, last = first + count; i < last; ++i) {
        i->indeg = i->indeg_backup = 0;
    }

    stack stk;
    stack_alloc(&stk);
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
