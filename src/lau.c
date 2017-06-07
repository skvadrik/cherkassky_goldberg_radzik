
#define NNULL (node*)NULL
#define VERY_FAR 2147483647

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
    }
    source->parent = source;
    source->dist = 0;

    /* main loop */
    deque_t dd, *d = &dd;
    deque_create(d);
    deque_push_back(d, source);

    while (!deque_empty(d)) {
        num_scans ++;
        node_from = deque_pop_front(d);
//        assert(node_from->indeg == 0);

        arc_last = (node_from + 1)->first;
        for (arc_ij = node_from->first; arc_ij < arc_last; ++arc_ij) {
            node_to = arc_ij->head;
            dist_new = node_from->dist + arc_ij->len;

            if (dist_new < node_to->dist) {
                node_to->dist = dist_new;
                node_to->parent = node_from;

                assert(node_to->indeg > 0);
                node_to->indeg--;
                if (node_to->indeg == 0) {
                    node_to->indeg = node_to->indeg_backup;
                    deque_push_front(d, node_to);
                } else {
                    deque_push_back(d, node_to);
                }
            }
        }
    }
    deque_free(d);

    n_scans = num_scans;
}

