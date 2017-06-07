void stack ( n, nodes, source )

long n;                         /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */

{

#define NNULL (node*)NULL
#define VERY_FAR 2147483647

/* -----   stack definitions ----- */

node *begin;

/* status of node regarding to stack */ 
#define IN_STACK     0
#define OUT_OF_STACK 1

#define INIT_STACK(source)\
{\
begin = source;\
source -> next   = NNULL;\
source -> status = IN_STACK;\
}

#define NONEMPTY_STACK           ( begin != NNULL )

#define NODE_IN_STACK(node)      ( node -> status == IN_STACK )

#define EXTRACT_FIRST(node)\
{\
node = begin;\
node -> status = OUT_OF_STACK;\
begin = begin -> next;\
}

#define INSERT_TO_STACK(node)\
{\
  node -> next = begin;\
  begin = node;\
  node -> status = IN_STACK;\
}\

/* -------------------------------------- */

long dist_new;

node *node_from,
     *node_to,
     *node_last,
     *i;

arc  *arc_ij,
     *arc_last;

long num_scans = 0;

/* initialization */

node_last = nodes + n ;
 
for ( i = nodes; i < node_last; i ++ )
   { 
      i -> parent   = NNULL;
      i -> dist     = VERY_FAR;
      i -> status   = OUT_OF_STACK;
   }

source -> parent = source;
source -> dist   = 0;

INIT_STACK (source)


/* main loop */

while ( NONEMPTY_STACK )
 {
     num_scans ++;

   EXTRACT_FIRST ( node_from )

   arc_last = ( node_from + 1 ) -> first;
   
   for ( arc_ij = node_from -> first; arc_ij < arc_last; arc_ij ++ )
     { /* scanning aecs outgoing from  node_from  */
       node_to  = arc_ij -> head;

       dist_new = ( node_from -> dist ) + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	   { 
	     node_to -> dist   = dist_new;
             node_to -> parent = node_from;

             if ( ! NODE_IN_STACK ( node_to ) )
	          INSERT_TO_STACK ( node_to )
	   }
     } /* end of scanning  node_from */
 } /* end of the main loop */

n_scans = num_scans;
}
