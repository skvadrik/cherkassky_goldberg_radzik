
#define NNULL (node*)NULL
#define VERY_FAR 2147483647

#define OFF_QUEUE 0
#define ON_QUEUE  1

#define INIT_QUEUE(source) \
{ \
    begin = end = source; \
    source->next   = NNULL; \
    source->prev   = NNULL; \
    source->status = ON_QUEUE; \
}

#define NONEMPTY_QUEUE (begin != NNULL)

#define NODE_ON_QUEUE(node) (node->status == ON_QUEUE)

#define POP_FRONT(node) \
do { \
    node = begin; \
    node->status = OFF_QUEUE; \
    begin = begin->next; \
} while(0)

#define PUSH_FRONT(node)\
do {\
    if (begin == NNULL) { \
        end = node; \
    } else { \
        begin->prev = node; \
    } \
    node->next = begin; \
    node->prev = NNULL; \
    begin = node;\
    node->status = ON_QUEUE;\
} while(0)

#define REINSERT_TO_FRONT(node) \
do { \
    if (node != begin) { \
        if (node == end) { \
            end = node->prev; \
        } else { \
            node->next->prev = node->prev; \
        } \
        node->prev->next = node->next; \
        PUSH_FRONT(node_to); \
    } \
} while(0)

#define PUSH_BACK(node)\
do {\
    if (begin == NNULL) { \
        begin = node; \
    } else { \
        end->next = node; \
    } \
    node->prev = end; \
    end = node; \
    end->next = NNULL; \
    node->status = ON_QUEUE;\
} while(0)

void lau(long n, node *nodes, node *source)
{
    node *begin, *end, *node_from, *node_to, *node_last, *i, *j;
    arc *arc_ij, *arc_last;
    long dist_new, num_scans = 0; 

    lau_indegree(nodes, n, source);

    /* initialization */
    node_last = nodes + n;
    for (i = nodes; i < node_last; ++i) {
        i->parent = NNULL;
        i->dist = VERY_FAR;
        i->status = OFF_QUEUE;
    }
    source->parent = source;
    source->dist = 0;

    /* main loop */
    INIT_QUEUE(source)
    while (NONEMPTY_QUEUE) {
        num_scans++;
        POP_FRONT(node_from);
//        assert(node_from->indeg == 0);

        arc_last = (node_from + 1)->first;
        for (arc_ij = node_from->first; arc_ij < arc_last; ++arc_ij) {
            node_to = arc_ij->head;
            dist_new = node_from->dist + arc_ij->len;

//            assert(node_to->indeg > 0);
            if (node_to->indeg == 0) {
                node_to->indeg = node_to->indeg_backup;
            }
            node_to->indeg--;
            if (node_to->indeg == 0 && NODE_ON_QUEUE(node_to)) {
                REINSERT_TO_FRONT(node_to);
            }

            if (dist_new < node_to->dist) {
                node_to->dist = dist_new;
                node_to->parent = node_from;

                if (!NODE_ON_QUEUE(node_to)) {
                    if (node_to->indeg == 0) {
                        PUSH_FRONT(node_to);
                    } else {
                        PUSH_BACK(node_to);
                    }
                }
            }
        }
    }

    n_scans = num_scans;
}

