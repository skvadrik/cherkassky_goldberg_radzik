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
/* heap memory */
   struct node_st   *heap_parent;     /* heap parent pointer */
   struct node_st   *son;             /* heap successor */
   struct node_st   *next;            /* next brother */
   struct node_st   *prev;            /* previous brother */
   long              deg;             /* number of children */
   int               status;          /* status of node */

   int               temp;            /* for temporary labels */
} node;

