#define VERY_FAR  1073741823

/*******************   definitions for heap  *****************/

typedef /* heap */
   struct heap_st
{
   long              size;          /* the number of the last heap element */
   node              **nd;          /* heap of the pointers to nodes       */ 
} 
   heap;

heap d_heap;

long h_current_pos,
     h_new_pos,
     h_pos,
     h_last_pos;

node *node_j,
     *node_k;

long dist_k,
     dist_min;

int  h_degree;

#define HEAP_POWER   2
#define NILL        -1
#define NODE_IN_HEAP( node_i ) ( node_i -> heap_pos != NILL )
#define NONEMPTY_HEAP  ( d_heap.size > 0 )

#define PUT_TO_POS_IN_HEAP( node_i, pos )\
{\
d_heap.nd[pos]        = node_i;\
node_i -> heap_pos = pos;\
}

void Init_heap ( n, source )

node* source;
long  n;

{
h_degree = 1 << HEAP_POWER;
printf("ddeg %ld\n", h_degree );
d_heap.size = 1;
d_heap.nd = (node**) calloc ( (n+1), sizeof(node*) );
PUT_TO_POS_IN_HEAP( source, 0 )
}

void Heap_decrease_key ( node_i, dist_i )

node* node_i;
long  dist_i;

{
for ( h_current_pos =  node_i -> heap_pos;
      h_current_pos > 0;
      h_current_pos = h_new_pos
    )
      {
        h_new_pos = ( h_current_pos - 1 ) >> HEAP_POWER;

        node_j = d_heap.nd[h_new_pos];
        if ( dist_i  >=  node_j -> dist ) break;

        PUT_TO_POS_IN_HEAP ( node_j, h_current_pos )
      }

PUT_TO_POS_IN_HEAP ( node_i, h_current_pos )
}

void Insert_to_heap ( node_i )

node* node_i;

{
PUT_TO_POS_IN_HEAP ( node_i, d_heap.size )
d_heap.size ++;
Heap_decrease_key( node_i, node_i -> dist);
}

node* Extract_min (  ) 

{
node* node_0;

node_0             = d_heap.nd[0];
node_0 -> heap_pos = NILL;

d_heap.size -- ;

if ( d_heap.size > 0 )
  {
     node_k =  d_heap.nd [ d_heap.size ];
     dist_k =  node_k -> dist;

     h_current_pos = 0;

     while ( 1 )
       {
         h_new_pos = ( h_current_pos << HEAP_POWER ) +  1;
         if ( h_new_pos >= d_heap.size ) break;

         dist_min  = d_heap.nd[h_new_pos] -> dist;

         h_last_pos  = h_new_pos + h_degree;
	 if ( h_last_pos > d_heap.size ) h_last_pos = d_heap.size;

         for ( h_pos = h_new_pos + 1; h_pos < h_last_pos; h_pos ++ )
            {
 	      if ( d_heap.nd[h_pos] -> dist < dist_min )
		{
		  h_new_pos = h_pos;
		  dist_min  = d_heap.nd[h_pos] -> dist;
		}
	    }

         if ( dist_k <= dist_min ) break;

         PUT_TO_POS_IN_HEAP ( d_heap.nd[h_new_pos], h_current_pos )

         h_current_pos = h_new_pos;
       }

    PUT_TO_POS_IN_HEAP ( node_k, h_current_pos )
  }
return node_0;
}

/**************   end of heap functions   ****************/

void dikh ( n, nodes, source )

long n;                          /* number of nodes */

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

long num_scans = 0;

/* initialization */

node_last = nodes + n ;
 
for ( i = nodes; i != node_last; i ++ )
   { 
      i -> parent   = (node*) NULL;
      i -> dist     = VERY_FAR;
      i -> heap_pos = NILL;
   }

source -> parent = source;
source -> dist   = 0;

Init_heap (  n, source );

/* main loop */


while ( NONEMPTY_HEAP )
 { 

   node_from = Extract_min (  );

   arc_last = ( node_from + 1 ) -> first;
   dist_from = node_from -> dist;
   num_scans ++;
   
   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     {
       node_to  = arc_ij -> head;

       dist_new = dist_from  + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	   { node_to -> dist   = dist_new;
             node_to -> parent = node_from;
	     if ( ! NODE_IN_HEAP ( node_to ) )
         	       Insert_to_heap ( node_to );
	       Heap_decrease_key (  node_to, dist_new );
n_impr ++;
	   }
     }
 }

n_scans = num_scans;
}

