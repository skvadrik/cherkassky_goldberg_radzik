
void dikq ( n, nodes, source )

long n;                          /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */

{

#define NNULL (node*)NULL
#define VERY_FAR 2147483647

/* -----   queues definitions ----- */

node *begin,
     *node_min,
     *node_min_prev,
     *i_prev;

long  dist_min;

/* status of node regarding to bag */ 
#define IN_BAG     0
#define OUT_OF_BAG 1

#define INIT_BAG(source)\
{\
begin = source;\
source -> next   = NNULL;\
source -> status = IN_BAG;\
}

#define NONEMPTY_BAG       ( begin != NNULL )

#define NODE_IN_BAG(node)  ( node -> status == IN_BAG )


#define EXTRACT_MIN(node)\
{\
dist_min = VERY_FAR;\
\
for ( i_prev = NNULL, i = begin; i != NNULL; i_prev = i, i = i -> next )\
  {\
     if ( i -> dist < dist_min )\
       {\
	  dist_min      = i -> dist;\
	  node_min      = i;\
	  node_min_prev = i_prev;\
       }\
  }\
\
node = node_min;\
node -> status = OUT_OF_BAG;\
\
if ( node_min_prev != NNULL )\
  node_min_prev -> next = node_min -> next;\
else\
  begin = begin -> next;\
}

#define INSERT_TO_BAG(node)\
{\
  node -> next   = begin;\
  begin          = node;\
  node -> status = IN_BAG;\
}\

/* -------------------------------------- */


long dist_new;

node *node_from, *node_to, *node_last, *i;

arc *arc_ij, *arc_last;

long num_scans = 0; 

/* initialization */

node_last = nodes + n ; 

for ( i = nodes; i < node_last; i ++ )
 { i -> parent   = NNULL;
   i -> dist     = VERY_FAR;
   i -> status   = OUT_OF_BAG;
 }

source -> parent = source; source -> dist = 0;

INIT_BAG ( source )

/* main loop */

while ( NONEMPTY_BAG )
 { 
   num_scans ++;

   EXTRACT_MIN ( node_from )

   arc_last = ( node_from + 1 ) -> first;
   
   for ( arc_ij = node_from -> first; arc_ij < arc_last; arc_ij ++ )
     {
       node_to  = arc_ij -> head;

       dist_new = ( node_from -> dist ) + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	   { node_to -> dist   = dist_new;
             node_to -> parent = node_from;
	     if ( ! NODE_IN_BAG ( node_to ) )
         	       INSERT_TO_BAG ( node_to )
n_impr ++;
	   }
     }
 }

n_scans = num_scans;
}

