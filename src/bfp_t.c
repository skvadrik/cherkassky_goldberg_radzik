/***********************************************************/
/*                                                         */
/*               Executor of SP codes                      */
/*               (for Bellman-Ford)                      */
/*                                                         */
/***********************************************************/


#include <stdio.h>
#include <stdlib.h>

/* statistical variables */
long n_scans;

/* definitions of types: node & arc */

#include "types_bf.h"

/* parser for getting extended DIMACS format input and transforming the
   data to the internal representation */

#include "parser_dh.c"

/* function 'timer()' for mesuring processor time */

#include "timer.c"

/* function for constructing shortest path tree */

#include "bfp.c"


main ()

{

float t;
arc *arp, *ta;
node *ndp, *source, *k;
long n, m, nmin, i; 
char name[30];
double sum_d = 0;

 parse( &n, &m, &ndp, &arp, &source, &nmin, name );

t = timer();
bf ( n, ndp, source );
t = timer() - t;

for ( k= ndp; k< ndp + n; k++ )
  if ( k -> parent != (node*) NULL )
   sum_d += (double) (k -> dist);

  printf ("%7ld %7ld %7.2f %10ld%30s%15.0f\n",
         n, m, t, n_scans, name, sum_d );

}
