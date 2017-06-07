/* Dijkstra with Fibonacci heap */

#define nod(node) (long)(node-nodes+1)
#define VERY_FAR  1073741823
#define NNULL     (node*)NULL

/*******************   functions for F-heap  *****************/

typedef /* F-heap */
   struct fheap_st
{
   node              *min;        /* the minimal node */
   long              dist;        /* tentative distance of min. node */
   long                 n;        /* number of nodes in the heap */
   node     **deg_pointer;        /* pointer to the node with given degree */
   long           deg_max;        /* maximal degree */
}
   f_heap;

f_heap fh;

node    *after, *before, *father, *child, *first, *last,
	*node_c, *node_s, *node_r, *node_n, *node_l;
long    dg;

#define BASE           1.61803
#define NOT_ENOUGH_MEM 2

#define OUT_OF_HEAP    0
#define IN_HEAP        1
#define MARKED         2

#define NODE_IN_FHEAP( node ) ( node -> status > OUT_OF_HEAP )

void Init_fheap ( n ) 

long n;

{
fh.deg_max = (long) ( log ( (double) n ) / log ( BASE ) + 1 );

if (
 ( fh.deg_pointer = (node**) calloc ( fh.deg_max, sizeof(node*) ) )
   ==
   (node**)NULL
   )
   exit ( NOT_ENOUGH_MEM );

  for ( dg = 0; dg < fh.deg_max; dg ++ )
     fh.deg_pointer[dg] = NNULL;

fh.n  = 0;
fh.min = NNULL;
}

void Check_min ( nd )

node *nd;

{
  if ( nd -> dist < fh.dist )
    {
      fh.dist = nd -> dist;
      fh.min  = nd;
    }
}

void Insert_after_min ( nd )

node *nd;

{
  after = fh.min -> next;
  nd    -> next = after;
  after   -> prev = nd;
  fh.min  -> next = nd;
  nd    -> prev = fh.min;

  Check_min ( nd );
}

void Insert_to_root ( nd )

node *nd;
{
  nd -> heap_parent = NNULL;
  nd -> status      = IN_HEAP;

  Insert_after_min ( nd );
}

void Cut_node ( nd, father )

node *nd, *father;

{
  after = nd -> next;
  if ( after != nd )
    { before = nd -> prev;
      before -> next = after;
      after  -> prev = before;
    }

  if ( father -> son == nd ) father -> son = after;
  ( father -> deg ) --;
  if ( father -> deg == 0 ) father -> son = NNULL;
}

void Insert_to_fheap ( nd )

node *nd;

{
  nd -> heap_parent = NNULL;
  nd -> son         = NNULL;
  nd -> status      = IN_HEAP;
  nd -> deg         = 0;

  if ( fh.min == NNULL )
    {
      nd -> prev  =  nd -> next  =  nd;
      fh.min        =  nd;
      fh.dist       =  nd -> dist;
    }
  else
      Insert_after_min ( nd );
  fh.n ++;
}

void Fheap_decrease_key ( nd ) 

node *nd;

{
  if (
        ( father = nd -> heap_parent )
        ==
        NNULL
     )
    Check_min ( nd );
  else /* node isn't in the root */ 
    {
      if ( nd -> dist  <  father -> dist )
        {

          node_c = nd;

          while ( father != NNULL )
             {
               Cut_node ( node_c, father );
               Insert_to_root ( node_c );
               if ( father -> status == IN_HEAP )
                 {
                   father -> status = MARKED;
                   break;
                 }
               node_c = father;
	       father =  father -> heap_parent ;
             }
        }
    }
}

node* Extract_min ( ) 

{
node *nd;

  nd = fh.min;
if ( fh.n > 0 )
{
  fh.n --;
  fh.min -> status = OUT_OF_HEAP;

/* connecting root-list and sons-of-min-list */ 
  first = fh.min -> prev;
  child = fh.min -> son;
  if ( first == fh.min )
      first = child;
  else
    {
      after = fh.min -> next;
      if ( child == NNULL )
        {
          first -> next = after;
          after -> prev = first;
        }
      else
        {
          before = child -> prev;

          first  -> next = child;
          child  -> prev = first;

          before -> next = after;
          after  -> prev = before;
        }
    }

if ( first != NNULL )
{ /* heap is not empty */ 

/* squeezing root */ 

  node_c = first;
  last   = first -> prev;
  while ( 1 )
  {
    node_l = node_c;
    node_n = node_c -> next;
/*    printf ( "node_c=%ld  node_n=%ld\n", nod(node_c), nod(node_n) );*/

  while ( 1 )
   {
    dg = node_c -> deg;
    node_r = fh.deg_pointer[dg];

/*
    printf ( "dg=%ld  node_r=%ld\n", dg, nod(node_r) );
    for ( dgx = 0; dgx < fh.deg_max; dgx ++ )
    printf (" %ld ", nod(fh.deg_pointer[dgx]) );
    printf ("\n");
*/
    if ( node_r == NNULL )
      {
         fh.deg_pointer[dg] = node_c;
         break;
      }
    else
      {
         if ( node_c -> dist  <  node_r -> dist )
           {
              node_s = node_r;
              node_r = node_c;
           }
         else
              node_s = node_c;

/*    printf ( "node_r=%ld  node_s=%ld\n", nod(node_r), nod(node_s) );*/
/* detach node_s from root */ 
    after  = node_s -> next;
    before = node_s -> prev;

    after  -> prev = before;
    before -> next = after;

/* attach node_s to node_r */ 
    node_r -> deg ++;
    node_s -> heap_parent = node_r;
    node_s -> status = IN_HEAP;

    child = node_r -> son;

    if ( child == NNULL )
      node_r -> son = node_s -> next = node_s -> prev = node_s;
    else
      {
         after = child -> next;
         child  -> next = node_s;
         node_s -> prev = child;
         node_s -> next = after;
         after  -> prev = node_s;
      }

     } /* node_r now is father of node_s */ 
     node_c = node_r;
     fh.deg_pointer[dg] = NNULL;
/*
 printf ( "INHEAP node  dist parent  son next prev deg status\n" );
for ( i = nodes; i != node_last; i ++ )
   {
   if ( i -> status > OUT_OF_HEAP )
printf (" %ld %ld %ld %ld  %ld %ld %ld %d\n",
nod(i), i->dist, nod(i->heap_parent), nod(i->son), nod(i->next),
nod(i->prev), i->deg, i->status );
   }

 fgetc(inp);
*/

    }

    if ( node_l == last ) break;
    node_c = node_n;
  }

/* finding minimum */ 

  fh.dist = VERY_FAR;

  for ( dg = 0; dg < fh.deg_max; dg ++ )
     {
       if ( fh.deg_pointer[dg] != NNULL )
         {
           node_r = fh.deg_pointer[dg];
           fh.deg_pointer[dg] = NNULL;
	   Check_min ( node_r );
	   node_r -> heap_parent = NNULL;
         }
     }
}
else /* heap is empty */ 
  fh.min = NNULL;
}
return nd;
}

/**************   end of heap functions   ****************/

int dikf ( n, nodes, source )

long n;                     /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */
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

/*
 FILE *inp; long dgx; node *i0, *i1;
inp = fopen ("con","r");
*/
/* initialization */

Init_fheap ( n );

node_last = nodes + n ;

for ( i = nodes; i != node_last; i ++ )
   {
      i -> parent   = NNULL;
      i -> dist     = VERY_FAR;
   }

source -> parent = source;
source -> dist   = 0;
Insert_to_fheap ( source );

/* main loop */

while ( 1 )
 {
/*
 printf ( "before extr. min - n=%ld  min=%ld\n", fh.n, fh.min-nodes+1 );
*/
   node_from = Extract_min ( );
/*
 printf ( "after extr. min - n=%ld  min=%ld\n", fh.n, fh.min-nodes+1 );
 printf ( "node  dist parent  son next prev deg status\n" );
for ( i = nodes; i != node_last; i ++ )
   {
   if ( i -> status > OUT_OF_HEAP )
printf (" %ld %ld %ld %ld  %ld %ld %ld %d\n",
nod(i), i->dist, nod(i->heap_parent), nod(i->son), nod(i->next),
nod(i->prev), i->deg, i->status );
   }

for ( i = nodes; i != node_last; i++ )
{
   if ( i -> status > OUT_OF_HEAP )
   {
i0 = i -> heap_parent;
if ( i0 == NNULL ) i0=fh.min; else i0= i0->son;
i1 = i0;
while (1)
{
if ( i1==i ) goto OKe;
i1 = i1->next;
if ( i1==i0 ) break;
}
printf ("out of tree %ld\n", nod(i));
OKe:;
}
}
 fgetc(inp);
*/
/*
printf("node_from: %ld\n", nod(node_from));
*/
   if ( node_from == NNULL ) break;

   num_scans ++;

      arc_last = ( node_from + 1 ) -> first;
      dist_from = node_from -> dist;

   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     {
       node_to  = arc_ij -> head;
       dist_new = dist_from + ( arc_ij -> len );

       if ( dist_new <  node_to -> dist )
	   {
             node_to -> dist   = dist_new;
             node_to -> parent = node_from;

	     if ( NODE_IN_FHEAP ( node_to ) )
	     {
/*
		printf ( "decreased %ld\n",nod(node_to));
*/
		Fheap_decrease_key ( node_to );
		}
	     else
	     {
/*
		printf ( "inserted %ld\n",nod(node_to));
*/
		Insert_to_fheap ( node_to );
		}
		/*
 printf ( "node  dist parent  son next prev deg status\n" );
for ( i = nodes; i != node_last; i ++ )
   {
   if ( i -> status > OUT_OF_HEAP )

printf (" %ld %ld %ld %ld  %ld %ld %ld %d\n",
nod(i), i->dist, nod(i->heap_parent), nod(i->son), nod(i->next),
nod(i->prev), i->deg, i->status );

   }

for ( i = nodes; i != node_last; i++ )
{
  if ( i -> status > OUT_OF_HEAP )
 {
i0 = i -> heap_parent;
if ( i0 == NNULL ) i0=fh.min; else i0= i0->son;
i1 = i0;
while (1)
{

if ( i1==i ) goto OKu;
i1 = i1->next;
if ( i1==i0 ) break;
}
printf ("2out of tree %ld\n", nod(i));
OKu:;
}
}
fgetc(inp);
*/
	   }
     }
 }
n_scans = num_scans;
return (0);
}
