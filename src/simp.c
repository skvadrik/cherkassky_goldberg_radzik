#define NNULL (node*)NULL
#define VERY_FAR 1073741823

/* -----   golbal variables ---------------------*/

node *begin,
     *end;
long num_scans = 0;
long num_piv   = 0;
long num_impr  = 0;


/* status of node regarding to queue */ 
#define OUT_OF_QUEUE 0
#define ACTIVE 1
#define INACTIVE 2

#define INIT_QUEUE(source)\
{\
begin = end = source;\
source -> next   = NNULL;\
source -> status = ACTIVE;\
}

#define NONEMPTY_QUEUE           ( begin != NNULL )

#define NODE_IN_QUEUE(node)      ( node -> status != OUT_OF_QUEUE )

#define EXTRACT_FIRST(node)\
{\
node = begin;\
node -> status = OUT_OF_QUEUE;\
begin = begin -> next;\
}

#define INSERT_TO_QUEUE(node)\
{\
 if ( begin == NNULL )\
      begin = node;\
 else\
   end -> next = node;\
\
 node -> next = NNULL;\
 end = node;\
 node -> status = ACTIVE;\
}\

#define MAKE_ACTIVE(v)\
switch (v->status)\
{\
   case ACTIVE: break;\
   case INACTIVE: { v-> status = ACTIVE; break; }\
   case OUT_OF_QUEUE: { INSERT_TO_QUEUE(v); break; }\
}\
/* -------------------------------------- */

void pivot ( i, j, decr )

node *i, *j;
long decr;

{
node *v;


  num_piv++;

  /* update distances */
  j -> dist -= decr;
  num_impr++;
  j -> t_stamp = num_piv;
  v = j -> t_next;
  while ( v -> parent -> t_stamp == num_piv )
    {
      v -> dist -= decr;
      num_impr++;
      v -> t_stamp = num_piv;

      if ( v -> status == ACTIVE )
	v -> status = INACTIVE;

      v = v -> t_next;
    }

  /* update tree walk */
  v -> t_prev -> t_next = i -> t_next;
  i -> t_next -> t_prev = v -> t_prev;
  j -> t_prev -> t_next = v;
  v -> t_prev = j -> t_prev;
  i -> t_next = j;
  j -> t_prev = i;
}



void simp ( n, nodes, source )

long n;                         /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */

{


long dist_new,
     dist_from;

node *node_from,
     *node_to,
     *node_last,
     *i;

arc  *arc_ij,
     *arc_last;

/* initialization */

node_last = nodes + n ;
 
for ( i = nodes; i != node_last; i ++ )
   { 
      i -> parent   = NNULL;
      i -> t_next   = NNULL;
      i -> t_prev   = NNULL;
      i -> t_stamp  = 0;
      i -> dist     = VERY_FAR;
      i -> status   = OUT_OF_QUEUE;
   }

source -> parent = source;
source -> dist   = 0;
source -> t_next = source;
source -> t_prev = source;

INIT_QUEUE ( source );


/* main loop */

while ( NONEMPTY_QUEUE )
 {

   while ( begin -> status == INACTIVE )
     EXTRACT_FIRST ( node_from );
   if ( NONEMPTY_QUEUE ) {
     EXTRACT_FIRST ( node_from );
   }
   else
     break;

   num_scans ++;
   arc_last  = ( node_from + 1 ) -> first;
   dist_from = node_from -> dist;
   
   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     { /* scanning arcs outgoing from  node_from  */
       node_to  = arc_ij -> head;

       dist_new = dist_from + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	 {
	   if ( node_to -> dist == VERY_FAR )
	     { 
	       num_impr++;
	       node_to -> dist   = dist_new;

	       /* add node_to to tree */
	       node_to -> t_next = node_from -> t_next;
	       node_to -> t_next -> t_prev = node_to;
	       node_from -> t_next = node_to;
	       node_to -> t_prev = node_from;
	     }
	   else
	     {
	       pivot (node_from, node_to, node_to -> dist - dist_new );
	     }
	   node_to -> parent = node_from;
	 }

     } /* end of scanning  node_from */
   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     {
       node_to = arc_ij -> head;
       if ( node_to -> parent == node_from ) {
	 MAKE_ACTIVE ( node_to );
       }
     }

 } /* end of the main loop */

n_scans = num_scans;
n_piv = num_piv;
n_impr = num_impr;
}
