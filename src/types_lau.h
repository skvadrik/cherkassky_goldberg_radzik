#ifndef _TYPES_LAU_
#define _TYPES_LAU_

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
   struct node_st   *prev;            /* next node in queue */
   int               status;          /* status of node */
   long              indeg;
   long              indeg_backup;
} node;

#endif // _TYPES_LAU_
