int gor ( n, nodes, source )

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

stack      new_pass, pass, top_sort;

/* status of node regarding to stacks */ 

#define OUT_OF_STACKS  0
#define IN_NEW_PASS    1
#define IN_TOP_SORT    2
#define IN_PASS        3

#define INIT_STACKS( source )\
{\
  new_pass.arr = (node**) calloc ( n, sizeof (node*) );\
  new_pass.arr[0] = source;\
  new_pass.top    = 0;\
  source -> status = IN_NEW_PASS;\
\
  pass.arr = (node**) calloc ( n, sizeof (node*) );\
  pass.top = -1;\
\
  top_sort.arr = (node**) calloc ( n, sizeof (node*) );\
  top_sort.top = -1;\
\
  if ( new_pass.arr == (node**) NULL || \
       pass.arr     == (node**) NULL || \
       top_sort.arr == (node**) NULL    \
     )\
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

INIT_STACKS (source)


/* main loop */

while ( NONEMPTY_STACK ( new_pass ) )
{
/* topological sorting */

  while ( NONEMPTY_STACK ( new_pass ) )
    {
      POP ( new_pass, i )

      if (  i -> status == IN_NEW_PASS )
	{ /* looking for any arc with negative reduced cost outgoing from i
             if any - start deapth first search */

          arc_last = ( i + 1 ) -> first;
          dist_i   = i -> dist;
   
          for ( arc_ij = i -> first; arc_ij != arc_last; arc_ij ++ )

             if ( dist_i + arc_ij -> len
                  <
                  ( arc_ij -> head ) -> dist
                )
             	 break;
	
          if ( arc_ij != arc_last )
            {
              i -> status  = IN_TOP_SORT;
              i -> current = i -> first;
	    }
          else
              i -> status = OUT_OF_STACKS;
	}


      if ( i -> status == IN_TOP_SORT )
	{ /* deapth first search */

         while ( 1 )
           {         
            arc_last = ( i + 1 ) -> first;
            dist_i   = i -> dist;

            for ( arc_ij = i -> current; arc_ij != arc_last; arc_ij ++ )
	      { 
	        j = arc_ij -> head;

                if ( dist_i + arc_ij -> len  <  j -> dist )
		  {
		      j -> dist    = dist_i + arc_ij -> len;
		      j -> parent  = i;

                    if ( j -> status < IN_TOP_SORT )
		    {
		      i -> current = arc_ij + 1;
                      PUSH ( top_sort, i )
		      j -> status  = IN_TOP_SORT;
                      j -> current = j -> first;
		      i = j;

		      break;
		    }
		  }
	      }
	    
	    if ( arc_ij == arc_last )
              {
                if ( i -> status == IN_TOP_SORT ) 
		   i -> status = IN_PASS;
		PUSH ( pass, i );
                num_scans ++;

	        if ( NONEMPTY_STACK ( top_sort ) )
		    POP ( top_sort, i )
		else
		    break;
	      }
	  } /* end of deapth first search */
       }
    } /* end of topological sorting */
	      
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

                 if (  j -> status == OUT_OF_STACKS  )
		   {
		     PUSH ( new_pass, j )
		     j -> status = IN_NEW_PASS;

		   }
	     }
	 
           } /* end of scanning  i  */
      i -> status = OUT_OF_STACKS;
    } /* end of one pass */

 } /* end of the main loop */

n_scans = num_scans;
return ( 0 );
}
