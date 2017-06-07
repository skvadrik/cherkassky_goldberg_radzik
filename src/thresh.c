void thresh ( n, nodes, source )

long n;                         /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */

{

#define NNULL (node*)NULL
#define VERY_FAR 1073741823

#define MINWT_S  45
#define WTCNG    .25

double  minwt = MINWT_S,
        avgwt,
        avg;

long    min,
        t;

/* -----   queues definitions ----- */

node *next_begin,
     *now_begin,
     *prev,
     *i_next;

/* status of node regarding to queue */ 
#define IN_QUEUE     0
#define OUT_OF_QUEUE 1

#define INIT_QUEUE(source)\
{\
now_begin = source;\
source -> next   = NNULL;\
source -> status = IN_QUEUE;\
next_begin = NNULL;\
}

#define NONEMPTY_NOW             ( now_begin != NNULL )
#define EMPTY_NEXT               ( next_begin == NNULL )

#define NODE_IN_QUEUE(node)      ( node -> status == IN_QUEUE )

#define EXTRACT_FIRST_FROM_NOW(node)\
{\
node = now_begin;\
node -> status = OUT_OF_QUEUE;\
now_begin = now_begin -> next;\
}

#define INSERT_TO_NEXT(node)\
{\
 node -> next = next_begin;\
 node -> status = IN_QUEUE;\
 next_begin = node;\
}\

#define REMOVE_FROM_NEXT(node)\
{\
 if ( node == next_begin )\
   next_begin = node -> next;\
 else\
   prev -> next = node -> next;\
}

#define INSERT_TO_NOW(node)\
{\
 node -> next = now_begin;\
 node -> status = IN_QUEUE;\
 now_begin = node;\
}\

#define FIND_MIN_AND_AVG( min, avg )\
{\
 min = VERY_FAR;\
 avg = 0.;\
 next_size = 0;\
\
 for ( i = next_begin; i != NNULL; i = i -> next )\
   {\
      next_size ++;\
      avg += i -> dist;\
      if ( i -> dist  <  min ) min = i -> dist;\
   }\
\
 avg /= (double) next_size;\
}

#define FROM_NEXT_TO_NOW( t )\
{\
 next_size = now_size = 0;\
\
  for ( i = next_begin; i != NNULL; i = i_next )\
    {\
       next_size ++ ;\
       i_next = i -> next;\
\
       if ( i -> dist <= t )\
	 {\
	    REMOVE_FROM_NEXT ( i )\
	    INSERT_TO_NOW    ( i )\
	    now_size ++;\
	 }\
       else\
         prev = i;\
    }\
}\
 
/* -------------------------------------- */

long dist_new,
     dist_from;

node *node_from,
     *node_to,
     *node_last,
     *i;

arc  *arc_ij,
     *arc_last;

long num_scans = 0;

long now_size,
     next_size;

int  t_f = 0;

/* initialization */

node_last = nodes + n ;
 
for ( i = nodes; i != node_last; i ++ )
   { 
      i -> parent   = NNULL;
      i -> dist     = VERY_FAR;
      i -> status   = OUT_OF_QUEUE;
   }

source -> parent = source;
source -> dist   = 0;

avgwt = 100. - minwt;

INIT_QUEUE (source)
   
/* main loop */

while ( 1 )
{

while ( NONEMPTY_NOW )
 {
     num_scans ++;

   EXTRACT_FIRST_FROM_NOW ( node_from )

   arc_last  = ( node_from + 1 ) -> first;
   dist_from = node_from -> dist;
   
   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     { /* scanning aecs outgoing from  node_from  */
       node_to  = arc_ij -> head;

       dist_new = ( node_from -> dist ) + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	   { 
	     node_to -> dist   = dist_new;
             node_to -> parent = node_from;

             if ( ! NODE_IN_QUEUE ( node_to ) )
	          INSERT_TO_NEXT ( node_to )
	   }
     } /* end of scanning  node_from */
 } /* end of scanning NOW */

if ( ! t_f ) goto find;

t_loop:

t = ( long ) ( 0.5 +
    ( minwt * (double) min  +  avgwt * avg ) / ( minwt + avgwt )
             );
t_f = 1;

FROM_NEXT_TO_NOW( t )

if ( now_size == 0 )
  { find:
      if ( EMPTY_NEXT ) goto finish;
      FIND_MIN_AND_AVG ( min, avg )

      goto  t_loop;
  }

/* NOW is not empty now */

avgwt = avgwt * WTCNG * ( (double)next_size / (double)now_size );
if ( 7   * now_size <= next_size ) avg *= 1.1;
if ( 1.3 * now_size >= next_size ) avg *= 0.9;

/* new scan of NOW starts */
}

 finish:
n_scans = num_scans;
}




