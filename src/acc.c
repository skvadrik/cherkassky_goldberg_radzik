int acc ( n, nodes, source )

long n;                         /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */

{

#define NNULL (node*)NULL
#define VERY_FAR  1073741823

/* -----   stacks definitions ----- */

typedef struct str_stack
           {
	     long     top;
             node   **arr;
	   }
             stack;


/* status of node regarding to stacks */ 

#define OUT_OF_STACKS  0
#define IN_TOP_SORT    2

#define INIT_STACK( stack)\
{\
  stack.arr = (node**) calloc ( n, sizeof (node*) );\
  stack.top = -1;\
\
  if ( stack.arr == (node**) NULL ) \
    return ( 1 );\
}

#define NONEMPTY_STACK( stack )     ( stack.top >= 0 )

#define POP( stack, node )\
{\
node = stack.arr [ stack.top ];\
stack.top -- ;\
}

#define PUSH( stack, node )\
{\
  stack.top ++ ;\
  stack.arr [ stack.top ] = node;\
}\

/* -------------------------------------- */

stack      pass, top_sort;

long dist_new,
     dist_i;

node *node_last,
     *i,
     *j;

arc  *arc_ij,
     *arc_last;

long num_scans = 0;

/* initialization */

node_last = nodes + n ;
 
for ( i = nodes; i != node_last; i ++ )
   { 
      i -> parent   = NNULL;
      i -> dist     = VERY_FAR;
      i -> status   = OUT_OF_STACKS;
   }

source -> parent = source;
source -> dist   = 0;

INIT_STACK (pass)
INIT_STACK (top_sort)


/* topological sorting */
        /* deapth first search */

         i = source;
         i -> status = IN_TOP_SORT;
         i -> current = i -> first;

         while ( 1 )
           {         
            arc_last = ( i + 1 ) -> first;

/* printf("top i %d\n", i-nodes+1); */
            for ( arc_ij = i -> current; arc_ij != arc_last; arc_ij ++ )
	      { 
	        j = arc_ij -> head;

                    if ( j -> status != IN_TOP_SORT )
		    {
		      i -> current = arc_ij + 1;
                      PUSH ( top_sort, i )
		      j -> status  = IN_TOP_SORT;
                      j -> current = j -> first;
		      i = j;

		      break;
		    }
	      }
	    
	    if ( arc_ij == arc_last )
              {
		PUSH ( pass, i );
/*                num_scans ++; */

	        if ( NONEMPTY_STACK ( top_sort ) )
		    POP ( top_sort, i )
		else
		    break;
	      }
	  } /* end of deapth first search */
/* end of topological sorting */
	      
/* Bellman - Ford pass */

  while ( NONEMPTY_STACK ( pass ) )
   {

      POP ( pass, i )

      num_scans ++;
      arc_last = ( i + 1 ) -> first;
      dist_i   = i -> dist;

      for ( arc_ij = i -> first; arc_ij != arc_last; arc_ij ++ )
         { /* scanning arcs outgoing from  i  */
           j  = arc_ij -> head;

           dist_new = dist_i + ( arc_ij -> len );

           if ( dist_new <  j -> dist )
	     { 
	         j -> dist   = dist_new;
                 j -> parent = i;
	     }
	 
          } /* end of scanning  i  */
    } /* end of one pass */

n_scans = num_scans;
return ( 0 );
}
