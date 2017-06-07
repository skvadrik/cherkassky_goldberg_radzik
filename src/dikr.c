static long llog2 ( r )

long r;
{
  static long pow;

  if ( r > 1 )
    {
       for ( pow=0; r != 1; pow ++, r >>= 1 ) ;
       return ( pow );
    }
  else
       return ( 0 );
}


/*******************  functions for R-heap  *****************/


typedef /* R-heap */
   struct rbucket_st
{
   long              size;        /* the number of buckets 0..size-1 */
   long             *base;        /* distance from the source to the bucket */
   node            **first;       /* first node in the bucket */ 
} 
   r_heap;

long  pos, pos_n;
node *entry, *node_next;
long  d_dist, bound, new_base, bucket_len;
r_heap rh;


#define VERY_FAR  1073741823
#define MAXLEN         VERY_FAR
#define NNULL          (node*)NULL
#define TOO_LONG_ARC   1
#define NOT_ENOUGH_MEM 2

void Init_rheap ( source, maxlen )

node *source;
long maxlen;

{
if ( maxlen > MAXLEN )
  exit (TOO_LONG_ARC);

rh.size = llog2 ( maxlen ) + 3;

rh.base  = (long*)  calloc ( ( rh.size + 1 ), sizeof(long)  );
rh.first = (node**) calloc ( ( rh.size     ), sizeof(node*) );
if (
     rh.base  ==  (long*)NULL  || 
     rh.first == (node**)NULL
   )
   exit (NOT_ENOUGH_MEM);

source -> bucket = 0;
rh.first[0] = source -> next = source -> prev = source;
rh.base [0] = 0;
bucket_len  = 1;

for ( pos = 1; pos < rh.size; pos ++ )
  {
    rh.first[pos] = NNULL;
    rh.base [pos] = rh.base[pos-1] + bucket_len;
    if ( pos != 1 )
      bucket_len *= 2;
  }

rh.base [rh.size] = VERY_FAR;
}


void Remove_from_rheap ( nd, pos )

node* nd;
long  pos;

{
  if ( nd -> next == nd )
      rh.first[pos] = NNULL;
  else
    { ( ( nd -> prev ) -> next ) = ( nd -> next );
      ( ( nd -> next ) -> prev ) = ( nd -> prev );
      if ( rh.first[pos] == nd )
        rh.first[pos] = nd -> next;
    }
}

void Insert_to_rheap ( nd, pos )

node* nd;
long  pos;


{
  if ( rh.first[pos] == NNULL )
    rh.first[pos] = nd -> next = nd -> prev = nd;
  else
    { entry = rh.first[pos];
      ( entry -> prev ) -> next = nd;
      nd  -> prev             = entry -> prev;
      nd  -> next             = entry;
      entry -> prev             = nd;
    }

  nd -> bucket = pos;
}

void Heap_decrease_key ( nd, dist ) 

node* nd;
long  dist;

{
  for ( pos = nd -> bucket; pos > 0; pos -- )
    if ( dist >= rh.base[pos] ) break;

   if ( pos            != nd -> bucket &&
        nd -> bucket != rh.size
     )
       Remove_from_rheap ( nd, nd -> bucket );

   if ( pos < 0 ) pos = 0;

   if ( pos != nd -> bucket )
     {
	Insert_to_rheap ( nd, pos );
	nd -> bucket = pos;
     }
}

node* Extract_min ( ) 

{
node* nd;

  if ( rh.first[0] == NNULL )
    {
       for ( pos_n = 1; pos_n < rh.size; pos_n ++ )
	 if ( rh.first [pos_n] != NNULL ) break;

       if ( pos_n < rh.size )
	 {
            nd = rh.first[pos_n];
            ( ( nd -> prev ) -> next ) = NNULL;

            for ( new_base = VERY_FAR;
                  nd != NNULL;
	          nd = nd -> next
	        )
	         if ( ( nd -> dist) < new_base )
                   new_base = nd -> dist;

            rh.base [0] = new_base;
            bucket_len  = 1;
            bound       = rh.base [ pos_n + 1 ];

            for ( pos = 1; pos <= pos_n; pos ++ )
               {
                 if (
                       ( rh.base [pos] = rh.base[pos-1] + bucket_len )
                          >
	                 bound
                     )
                        rh.base [pos] = bound;

                 if ( pos != 1 )
                    bucket_len *= 2;
               }

            for ( nd = rh.first[pos_n];
                  nd != NNULL;
	          nd = node_next
	        )
	          {
	             node_next = nd -> next;

                     d_dist = ( nd -> dist ) - new_base;
	             pos = ( d_dist == 0 ) ? 0 : llog2 ( d_dist ) + 1;
	             Insert_to_rheap ( nd, pos );
	          }
            rh.first[pos_n] = NNULL;
         }
    }

  nd = rh.first[0];

  if ( nd != NNULL )
    {
      nd -> bucket = -1;

    if ( nd -> next == nd )
        rh.first[0] = NNULL;
    else
      { ( ( nd -> prev ) -> next ) = ( nd -> next );
        ( ( nd -> next ) -> prev ) = ( nd -> prev );
	rh.first[0] = nd -> next;
      }
    }
 return nd;
}

/**************   end of R-heap functions   ****************/

int dikr ( n, nodes, source, maxlen )

long n;                         /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */
long maxlen;                    /* maximal arc length */ 

{


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


/* initialization */

Init_rheap ( source, maxlen );

node_last = nodes + n ;
 
for ( i = nodes; i != node_last; i ++ )
   { 
      i -> parent   = NNULL;
      i -> dist     = VERY_FAR;
      i -> bucket   = rh.size;
   }

source -> parent = source;
source -> dist   = 0;


/* main loop */

while ( 1 )
 {
   node_from = Extract_min ( );
   if ( node_from == NNULL ) break;

   num_scans ++;
/*
if (num_scans > 10 ) return;
 printf ("from: %d %d\n",node_from-nodes+1, num_scans); 
 for (pos=0; pos<= rh.size; pos++)
   { printf("pos %d base %d first %d\n", pos, rh.base[pos], rh.first[pos]-nodes+1); }
printf("\n");
 for ( pos=1; pos<=n; pos++ )
printf("i %d dist %d\n", pos, (nodes+pos-1)->dist );
*/
      arc_last = ( node_from + 1 ) -> first;
      dist_from = node_from -> dist;
   
   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     {
       node_to  = arc_ij -> head;
       dist_new = dist_from + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	   { 
	     Heap_decrease_key ( node_to, dist_new );
             node_to -> dist   = dist_new;
             node_to -> parent = node_from;
/*
printf ("to: %d dist %d bucket: %d\n",node_to-nodes+1, dist_new, node_to -> bucket); 
*/
	   }
     }
 }
n_scans = num_scans;
return (0);
}


