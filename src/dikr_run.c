/***********************************************************/
/*                                                         */
/*               Executor of SP codes                      */
/*               (for R-heap Dijkstra)                      */
/*                                                         */
/***********************************************************/


#include <stdio.h>
#include <stdlib.h>

/* statistical variables */
long n_scans = 0;

/* definitions of types: node & arc */

#include "types_dr.h"

/* parser for getting extended DIMACS format input and transforming the
   data to the internal representation */

#include "parser_db.c"

/* function 'timer()' for mesuring processor time */

#include "timer.c"

/* function for constructing shortest path tree */

#include "dikr.c"


main ()

{

float t;
arc *arp, *ta;
node *ndp, *source, *k;
long n, m, nmin, i; 
long mlen;
char name[21];
double sum_d = 0;
int cc;

 parse( &n, &m, &ndp, &arp, &source, &nmin, name, &mlen );
/*
printf ( "%s\nn= %ld, m= %ld, nmin= %ld, source = %ld\n",
        name,
        n,m,nmin, (source-ndp)+nmin );

printf ("\nordered arcs:\n");
for ( k = ndp; k< ndp + n; k++ )
  { i = (k-ndp)+nmin;
    for ( ta=k -> first; ta != (k+1)-> first; ta++ )
      printf ( " %2ld %2ld %4ld\n",
               i, ((ta->head)-ndp)+nmin, ta->len
             );

  }
*/
t = timer();
cc =
dikr ( n, ndp, source, mlen );

t = timer() - t;

for ( k= ndp; k< ndp + n; k++ )
  if ( k -> parent != (node*) NULL )
   sum_d += (double) (k -> dist);

printf ("\nDijkstra with R-heap -> problem name: %s\n\n\
Nodes: %ld    Arcs: %ld  cc: %d\n\
Number of scans: %ld\n\
Sum of distances: %.0f\n\n\
Running time of SP computation: %.2f\n\n",
         name, n, m, cc, n_scans, sum_d, t ); 
 
#define nd(ptr) (int)(ptr-ndp+nmin)

/*
for ( k=ndp; k< ndp+n; k++ )
printf (" %d %d %d\n", nd(k), nd(k->parent), k->dist);
*/
}
