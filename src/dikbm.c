int dikbm ( n, nodes, source, maxlen )

long n;                          /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */
long maxlen;                    /* maximal arc length */ 
{


/*******************   definitions for heap  *****************/


typedef /* bucket-heap */
   struct bucket_st
{
   long              size;          /* the number of buckets 0..size */
   long              base;          /* distance from the source to 0-bucket */
   long              curr;          /* the current number of active bucket */
   node            **first;         /* first node in the bucket */ 
} 
   bheap;

long  k, pos, bound;

node *entry, *node_next, *last_m, *first_m;

#define BUCKETS_MAX 50000
#define BUCKETS_MIN 1
#define BUCKETS_DIV 3
#define NNULL          (node*)NULL
#define NOT_ENOUGH_MEM 2

/* status of node regarding to heap */ 
#define IN_BHEAP     0
#define OUT_OF_BHEAP 1

#define INIT_BHEAP( source, maxlen )\
{\
/* bh.size may depend on maxlen */\
bh.size = maxlen / BUCKETS_DIV;\
if ( bh.size < BUCKETS_MIN ) bh.size = BUCKETS_MIN;\
if ( bh.size > n ) bh.size = n;\
if ( bh.size > BUCKETS_MAX ) bh.size = BUCKETS_MAX;\
\
if (\
( bh.first = (node**) calloc ( ( bh.size + 1 ), sizeof(node*) ) )\
  ==\
  (node**)NULL\
   )\
   return (NOT_ENOUGH_MEM);\
\
source -> status = IN_BHEAP;\
bh.first[0] = source -> next = source -> prev = source;\
\
bh.base = bh.curr = 0;\
\
for ( k = 1; k <= bh.size; k ++ ) bh.first[k] = NNULL;\
}

#define NODE_IN_BHEAP( node ) ( node -> status == IN_BHEAP )

#define DIST_TO_POS( distance, pos )\
{\
  if (\
    ( pos = distance - bh.base )\
       >\
      bh.size\
     )\
       pos = bh.size;\
  else\
    if ( pos < bh.curr ) pos = bh.curr;\
}
        

#define REMOVE_FROM_BHEAP( node, pos )\
{\
  if ( node -> next == node )\
      bh.first[pos] = NULL;\
  else\
    { ( ( node -> prev ) -> next ) = ( node -> next );\
      ( ( node -> next ) -> prev ) = ( node -> prev );\
      if ( bh.first[pos] == node )\
        bh.first[pos] = node -> next;\
    }\
}

#define INSERT_TO_BHEAP( node, pos )\
{\
  if ( bh.first[pos] == NNULL )\
    bh.first[pos] = node -> next = node -> prev = node;\
  else\
    { entry = bh.first[pos];\
      ( entry -> prev ) -> next = node;\
      node  -> prev             = entry -> prev;\
      node  -> next             = entry;\
      entry -> prev             = node;\
    }\
\
  node -> status = IN_BHEAP;\
\
}

#define EXTRACT_MIN( node ) \
{\
repeat:\
for ( ; bh.curr < bh.size &&  bh.first[bh.curr] == NNULL ; bh.curr ++ )\
 ;\
if ( bh.curr == bh.size )\
  if ( bh.first[bh.size] != NNULL )\
    {\
      node = bh.first[bh.size];\
      ( ( node -> prev ) -> next ) = NNULL;\
\
      for ( bh.base = VERY_FAR;\
            node != NNULL;\
	    node = node -> next\
	  )\
	    {\
           if ( (node -> dist) < bh.base )\
             bh.base = node -> dist;\
	     }\
\
      bound = bh.base + bh.size;\
      bh.curr = 0;\
      last_m = first_m = NNULL;\
\
      for ( node = bh.first[bh.size];\
            node != NNULL;\
	    node = node_next\
	  )\
	    {\
	       node_next = node -> next;\
\
	       if ( ( dist_new = (node -> dist) ) < bound )\
		 {\
		   DIST_TO_POS ( dist_new, pos_new )\
		   INSERT_TO_BHEAP ( node, pos_new )\
		 }\
	       else\
		 {\
		    if ( last_m != NNULL )\
		      {\
		        last_m -> next = node;\
		        node   -> prev = last_m;\
              	      }\
                    else\
		        first_m = node;\
\
                    last_m = node;\
		 }\
	     }\
\
      if ( last_m != NNULL )\
	{\
	   last_m  -> next = first_m;\
	   first_m -> prev = last_m;\
	}\
      bh.first[bh.size] = first_m;\
      goto repeat;\
    }\
  else\
    node = NNULL;\
else\
  {\
    node = bh.first[bh.curr];\
    node -> status = OUT_OF_BHEAP;\
\
    if ( node -> next == node )\
        bh.first[bh.curr] = NNULL;\
    else\
      { ( ( node -> prev ) -> next ) = ( node -> next );\
        ( ( node -> next ) -> prev ) = ( node -> prev );\
	bh.first[bh.curr] = node -> next;\
      }\
  }\
}

/**************   end of heap definitions   ****************/

#define VERY_FAR 1073741823

long dist_new,
     dist_old,
     dist_from;

long pos_new,
     pos_old;

node *node_from,
     *node_to,
     *node_last,
     *i;

arc  *arc_ij,
     *arc_last;

long  num_scans = 0;

bheap bh;

int in=0;

int   ins;
/* initialization */

node_last = nodes + n ;
 
for ( i = nodes; i < node_last; i ++ )
   { 
      i -> parent   = NNULL;
      i -> dist     = VERY_FAR;
      i -> status   = OUT_OF_BHEAP;
   }

source -> parent = source;
source -> dist   = 0;


INIT_BHEAP ( source, maxlen )

/* main loop */

while ( 1 )
 {
   EXTRACT_MIN ( node_from )

   if ( node_from == NNULL ) break;

   num_scans ++;
/* printf ("from: %d\n",node_from-nodes+1); */

      arc_last = ( node_from + 1 ) -> first;
      dist_from = node_from -> dist;
   
   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     {
       node_to  = arc_ij -> head;
       dist_new = dist_from + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	   { 
	       DIST_TO_POS ( dist_new, pos_new )
               if ( NODE_IN_BHEAP ( node_to ) )
		 { 
		   dist_old = node_to -> dist;
		   DIST_TO_POS ( dist_old, pos_old )

		   if ( ins = ( pos_old != pos_new ) )
	              REMOVE_FROM_BHEAP ( node_to, pos_old )
		 }
               else
		 ins = 1;

             if ( ins )
	       INSERT_TO_BHEAP ( node_to, pos_new )

             node_to -> dist   = dist_new;
             node_to -> parent = node_from;
n_impr ++;
	   }
     }
 }
n_scans = num_scans;
return (0);
}

