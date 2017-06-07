/* defs.h */

typedef  /* arc */
   struct arc_st
{
   long              len;            /* length of the arc */
   struct node_st   *head;           /* head node */
}
  arc;

typedef  /* node */
   struct node_st
{
   arc              *first;           /* first outgoing arc */
   long              dist;	      /* tentative shortest path length */
   struct node_st   *parent;          /* parent pointer */
   struct node_st   *next;            /* next node in queue */
   struct node_st   *prev;            /* previous node in queue */
   long              bucket;          /* number of containing bucket */
   int               temp;            /* for temporary labels */
} node;
