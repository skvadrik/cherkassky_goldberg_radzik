int dikbd ( n, nodes, source, maxlen )

long n;                         /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */
long maxlen;                    /* maximal arc length */ 
{


/*******************   definitions for double heap  *****************/


typedef /* bucket-heap */
   struct bucket_st
{
   /* internal part of heap */
   long     size_int;      /* the number of buckets 0..size_int */
   long     base_int;      /* distance from the source to 0-bucket */
   long     curr_int;      /* the current number of active bucket */
   long     max_int;
   long     min_int;
   /* external part of heap */
   long     size_ext;      /* the number of buckets 0..size_ext */
   long     base_ext;      /* distance from the source to 0-bucket */
   long     curr_ext;      /* the current number of active bucket */
   long     max_ext;
   long     min2_ext;      /*  < .. if we need to rescan buckets */
   long     max2_ext;
   /* common part of heap */
   long     size;           /* total number of buckets */
   node   **first;         /* first node in the bucket */ 
} 
   bheap;

node *entry, *node_next;
long k;
int int_heap_logsize;

#define NNULL             (node*)NULL
#define NOT_ENOUGH_MEM    2

/* status of node regarding to heap */ 
#define IN_BHEAP     0
#define OUT_OF_BHEAP 1

#define INIT_BHEAP( source, maxlen )\
{\
/* bh.size_int may depend on maxlen */\
for (int_heap_logsize = 1; ((1 << int_heap_logsize) < maxlen);\
    int_heap_logsize++);\
int_heap_logsize = int_heap_logsize / 2;\
int_heap_logsize -= 1;\
if (int_heap_logsize < 4) int_heap_logsize = 4;\
bh.size_int = ( 1 << int_heap_logsize );\
bh.size_ext = ( maxlen >> int_heap_logsize ) + 3;  /* = maxlen / size.int */ \
bh.size = bh.size_int + bh.size_ext;\
/*printf("%d %d\n", bh.size_int, bh.size_ext);*/\
if (\
( bh.first = (node**) calloc ( bh.size, sizeof(node*) ) )\
  ==\
  (node**)NULL\
   )\
   return (NOT_ENOUGH_MEM);\
\
source -> status = IN_BHEAP;\
bh.first[0] = source -> next = source -> prev = source;\
\
bh.base_int = 0;\
bh.curr_int = 0;\
bh.max_int  = 0;\
bh.base_ext = 0;\
bh.curr_ext = bh.size_int + 1;\
bh.max_ext  = bh.size_int;\
bh.max2_ext = bh.size_int;\
bh.min2_ext = bh.size;\
\
for ( k = 1; k < bh.size; k ++ ) bh.first[k] = NNULL;\
/*printf("size %d int %d ext %d\n", bh.size, bh.size_int, bh.size_ext );*/\
}

#define NODE_IN_BHEAP( node ) ( node -> status == IN_BHEAP )

#define DIST_TO_POS( distance, pos )\
{\
  if ( /* internal level */ \
    ( pos = distance - bh.base_int )\
       >=\
      bh.size_int\
     )\
       {\
	if ( /* external level */ \
         (pos = ( ( distance-bh.base_ext )>>int_heap_logsize ) + bh.size_int )\
	    >=\
	  bh.size\
	   )\
	     pos -= bh.size_ext;\
       }\
  else\
    if ( pos < bh.curr_int ){ pos = bh.curr_int; \
/*printf ( "node: %d\n", node_to-nodes+1 );*/\
}\
}
        

#define REMOVE_FROM_BHEAP( node, pos )\
{\
  if ( node -> next == node )\
      bh.first[pos] = NNULL;\
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
  if ( pos < bh.size_int )\
    { /* insert into internal backet */ \
       if ( pos > bh.max_int ) bh.max_int = pos;\
    }\
  else /* insert into external bucket */ \
    if ( pos >= bh.curr_ext )\
      { /* actual external buckets */ \
	 if ( pos > bh.max_ext ) bh.max_ext = pos;\
      }\
    else\
      { /* next pass external buckets */ \
	 if ( pos > bh.max2_ext ) bh.max2_ext = pos;\
	 if ( pos < bh.min2_ext ) bh.min2_ext = pos;\
      }\
}

#define EXTRACT_MIN( node ) \
{\
/* internal level */ \
repeat_int:\
for ( ;\
     bh.curr_int <= bh.max_int && bh.first[bh.curr_int] == NNULL ;\
     bh.curr_int ++ )\
 ;\
\
if ( bh.curr_int <= bh.max_int )\
  { /* min found in internal buckets */ \
    node = bh.first[bh.curr_int];\
    node -> status = OUT_OF_BHEAP;\
\
    if ( node -> next == node )\
        bh.first[bh.curr_int] = NNULL;\
    else\
      { ( ( node -> prev ) -> next ) = ( node -> next );\
        ( ( node -> next ) -> prev ) = ( node -> prev );\
	bh.first[bh.curr_int] = node -> next;\
      }\
  }\
else\
  { /* internal buckets are empty */ \
    repeat_ext:\
     /* external level */ \
/*printf("external\n");*/\
     for ( ;\
          bh.curr_ext <= bh.max_ext && bh.first[bh.curr_ext] == NNULL ;\
          bh.curr_ext ++ )\
     ;\
\
/*printf (" ext: curr %d max %d\n", bh.curr_ext, bh.max_ext );*/\
     if ( bh.curr_ext <= bh.max_ext )\
       { /* distributing external backet into internal backets */ \
	 bh.base_int = bh.base_ext + \
                       ( ( bh.curr_ext - bh.size_int ) << int_heap_logsize );\
/*printf("int base: %d\n", bh.base_int );*/\
         bh.min_int = bh.size_int;\
	 bh.max_int  = 0;\
         bh.curr_int = 0;\
\
         node = bh.first[bh.curr_ext];\
	 bh.first[bh.curr_ext] = NNULL;\
         ( ( node -> prev ) -> next ) = NNULL;\
\
         for ( ; node != NNULL; node = node_next )\
	       {\
	          node_next = node -> next;\
	          dist_new = node -> dist;\
                  DIST_TO_POS ( dist_new, pos_new )\
                  if ( pos_new < bh.min_int ) bh.min_int = pos_new;\
	          INSERT_TO_BHEAP ( node, pos_new )\
/*printf("to-int: Node %d pos: %d \n", node-nodes+1, pos_new );*/\
               }\
         bh.curr_int = bh.min_int;\
         bh.curr_ext ++;\
	 goto repeat_int;\
	}\
      else\
	{ /* next pass through external buckets */ \
/*printf("next external\n");*/\
          if ( bh.min2_ext <= bh.max2_ext )\
            { bh.curr_ext = bh.min2_ext;\
              bh.max_ext  = bh.max2_ext;\
              bh.max2_ext = bh.size_int;\
              bh.min2_ext = bh.size;\
              bh.base_ext += ( bh.size_ext << int_heap_logsize );\
              goto repeat_ext;\
            }\
          else /* all buckets are empty */ \
            node = NNULL;\
	 }\
   }\
}

/**************   end of heap definitions   ****************/

#define VERY_FAR  1073741823

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
 
for ( i = nodes; i != node_last; i ++ )
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
/*
DIST_TO_POS(dist_from, pos_new)
printf( "scan: node %d dist %d d(pos) %d\n", node_from-nodes+1, 
        dist_from, bh.base_int+pos_new ); */
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
		     {
/* printf ( "rm: node %d pos %d\n", node_to-nodes+1, pos_old );*/
	              REMOVE_FROM_BHEAP ( node_to, pos_old )
		      }
		 }
               else
		 ins = 1;

             if ( ins )
	       INSERT_TO_BHEAP ( node_to, pos_new )
/*
printf( "scan: node down %d dist %d ext base %d pos %d\n", node_to-nodes+1, 
        dist_new, bh.base_ext, pos_new );
*/
             node_to -> dist   = dist_new;
             node_to -> parent = node_from;
n_impr ++;
	   }
     }
 }
n_scans = num_scans;
return (0);
}

