#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <values.h>

#include "random.c"

#define DASH '-'
#define VERY_FAR 100000000

#define DOUBLE_CYCLE   0
#define CYCLE          1
#define PATH           2

#define NO             0
#define YES            1

#define NODE( x, y ) (x*Y + y + 1)

#define PRINT_ARC( i, j, length )\
{\
l = length;\
if ( p_f ) l += ( p[i] - p[j] );\
printf ("a %8ld %8ld %12ld\n", i, j, l );\
}

char   *graph_type[] =
  {
    "double cycle",
    "cycle",
    "path"
  };

/* generator of layered networks for the shortest paths problem;
   extended DIMACS format for output */

main ( argc, argv )

int argc;
char* argv[];

{

char   args[30];

long   X,   /* horizontal size of grid */
       Y;   /* vertical size of grid */

long   x,
       y,
       y1, y2, yp,
       dl, dx, xn, yn, count,
       *mess;

double n;
long   n0,
       source,
       i,
       i0,
       j,
       dij;

double m;
long   m0,
       mc,
       k;

long   *p,
       p_t,
       l,
       lx;

long   seed,
       seed1,
       seed2;

int    ext=0;

FILE   *fout;

/* initialized by default values */

/* variables for generating one layer */

/* variables for generating spanning graph */
int    c_f = 0, cw_f = 0, cm_f = 0, cl_f = 0;

int    cw = DOUBLE_CYCLE;  /* type of spanning graph */
long   cm = 0,             /* lower bound of the interval */
       cl = 100;           /* upper bound of the interval */

/* variables for generating additional arcs */
int    a_f = 0, ax_f = 0, am_f = 0, al_f = 0;

long   ax = 0,             /* number of additional arcs */
       am = 0,             /* lower bound of the interval */
       al = 100;           /* upper bound of the interval */
       
/* variables for inter-layer arcs */
int    i_f = 0, ip_f = 0, ix_f = 0, ih_f = 0,
       im_f = 0, il_f = 0, in_f = 0, is_f = 0;

int    ip = NO;       /* to mess or not to mess */
long   ix = 1,        /* number of interlayered arcs in a NODE */
       ih = 1,        /* step between two layeres */
       il = 10000,    /* upper bound of the interval */
       im = 1000;     /* lower bound of the interval */
double in = 1,        /* l *=  in * |x1-x2| */ 
       is = 0;        /* l *=  is * |x1-x2|^2 */

/* variables for artifical source */
int    s_f = 0, sl_f = 0, sm_f = 0;
long   sl   = VERY_FAR, /* upper bound of artifical arc */
       sm,              /* lower bound of artifical arc */
       s;  

/* variables for potentials */
int    p_f = 0, pl_f = 0, pm_f = 0, pn_f = 0, ps_f = 0;

long   pl,            /* upper bound of the interval */
       pm;            /* lower bound of the interval */
double pn = 0,        /* p +=  ln * (x+1) */ 
       ps = 0;        /* p +=  ls * (x+1)^2 */

int np;               /* number of parameter parsing now */


  /* parsing  parameters */

if ( argc < 2 ) goto usage;

np = 0;

strcpy ( args, argv[1] );

  if ( ( args[0] == DASH ) && ( args[1] == 'h')
     )
      goto help;

if ( argc < 4 ) goto usage;

/* first parameter - horizontal size */
np = 1;
if ( ( X = atoi ( argv[1] ) )  <  1  )  goto usage;

/* second parameter - vertical size */
np = 2;
if ( ( Y = atoi ( argv[2] ) )  <  1  )  goto usage;

/* third parameter - seed */
np=3;
if ( ( seed = atoi ( argv[3] ) )  <=  0  )  goto usage;

/* other parameters */

for ( np = 4; np < argc; np ++ )
  {
    strcpy ( args, argv[np] );
    if ( args[0] != DASH ) goto usage;

    switch ( args[1] )
      {

      case 'c' : /* spanning graph in one layer */
	  c_f = 1;
	switch ( args[2] )
	  { 
	  case 'l': /* upper bound of the interval */
	    cl_f = 1;
	    cl  =  (long) atof ( &args[3] );
	    break;
	  case 'm': /* lower bound */
	    cm_f = 1;
	    cm  = (long ) atof ( &args[3] );
	    break;
	  case 'c': /* type - cycle */
	    cw_f = 1;
	    cw   = CYCLE;
	    break;
	  case 'd': /* type - double cycle */
	    cw_f = 1;
	    cw   = DOUBLE_CYCLE;
	    break;
	  case 'p': /* type - path */
	    cw_f = 1;
	    cw   = PATH;
	    break;

	  default:  /* unknown switch  value */
	    goto usage;
	  }
	break;

      case 'a' : /* additional arcs in one layer */
	 a_f = 1;
	switch ( args[2] )
	  { 
	  case 'l': /* upper bound of the interval */
	    al_f = 1;
	    al  =  (long) atof ( &args[3] );
	    break;
	  case 'm': /* lower bound */
	    am_f = 1;
	    am  = (long ) atof ( &args[3] );
	    break;
	  case 'x': /* number of additional arcs */
	    ax_f = 1;
	    ax   = (long ) atof ( &args[3] );
	    if ( ax < 0 ) goto usage;
	    break;

	  default:  /* unknown switch  value */
	    goto usage;
	  }
	break;


      case 'i' : /* interlayered arcs */
	i_f = 1;

	switch ( args[2] )
	  { 
	  case 'l': /* upper bound */
	    il_f = 1;
	    il  =  (long) atof ( &args[3] );
	    break;
	  case 'm': /* lower bound */
	    im_f = 1;
	    im  = (long ) atof ( &args[3] );
	    break;
	  case 'n': /* additional length: l *= in*|i1-i2| */
	    in_f = 1;
	    in  = atof ( &args[3] );
	    break;
	  case 's': /* additional length: l *= is*|i1-i2|^2 */
	    is_f = 1;
	    is  = atof ( &args[3] );
	    break;
	  case 'p': /* mess interlayered arcs */
	    ip_f = 1;
	    ip = YES;
	    break;
	  case 'x': /* number of interlayered arcs */
	    ix_f = 1;
	    ix  = atof ( &args[3] );
	    if ( ix < 1 ) goto usage;
	    break;
	  case 'h': /* step between two layeres */
	    ih_f = 1;
	    ih  = atof ( &args[3] );
	    if ( ih < 1 ) goto usage;
	    break;
	  default:  /* unknown switch  value */
	    goto usage;
	  }
	break;

      case 's' : /* additional source */
        s_f = 1;
	if ( strlen ( args ) > 2 )
	{  
	switch ( args[2] )
	  { 
	  case 'l': /* upper bound of art. arc */
	    sl_f = 1;
	    sl  =  (long) atof ( &args[3] );
            break;
	  case 'm': /* lower bound of art. arc */
	    sm_f = 1;
	    sm  =  (long) atof ( &args[3] );
            break;
	  default:  /* unknown switch  value */
	    goto usage;
          }
         }
	break;

      case 'p' : /* potentials */
	p_f = 1;
	if ( strlen ( args ) > 2 )
	{  
	switch ( args[2] )
	  { 
	  case 'l': /* upper bound */
	    pl_f = 1;
	    pl  =  (long) atof ( &args[3] );
	    break;
	  case 'm': /* lower bound */
	    pm_f = 1;
	    pm  = (long ) atof ( &args[3] );
	    break;
	  case 'n': /* additional: p *= pn*(x+1) */
	    pn_f = 1;
	    pn  = atof ( &args[3] );
	    break;
	  case 's': /* additional: p = ps* (x+1)^2 */
	    ps_f = 1;
	    ps  = atof ( &args[3] );
	    break;
	  default:  /* unknown switch  value */
	    goto usage;
	  }
        }
	break;

      default: /* unknoun case */
	goto usage;
      }
  }
   

/* ----- ajusting parameters ----- */

/* spanning */
if ( cl < cm ) { lx = cl; cl = cm; cm = lx; }

/* additional arcs */
if ( al < am ) { lx = al; al = am; am = lx; }

/* interlayered arcs */
if ( il < im ) { lx = il; il = im; im = lx; }

/* potential parameters */
if ( p_f )
  {
   if ( ! pl_f ) pl = il;
   if ( ! pm_f ) pm = im;
   if ( pl < pm ) { lx = pl; pl = pm; pm = lx; }
  }

/* number of nodes and arcs */

n = (double)X *(double)Y + 1;

m  = (double)Y; /* arcs from source */

switch ( cw )
{
 case PATH:
  mc = (double)Y - 1;
  break;
 case CYCLE:
  mc = (double)Y;
  break;
 case DOUBLE_CYCLE:
  mc = 2*(double)Y;
}

m += (double)X * (double)mc;  /* spanning arcs */
m += (double)X * (double)ax;  /* additional arcs */

/* interlayered arcs */
for ( x = 0; x < X; x ++ )
{
  dl = ( ( X - x - 1 ) + ( ih - 1 ) ) / ih;
  if ( dl > ix ) dl = ix;
  m += (double)Y * (double)dl;
}

 /* artifical source parameters */
if ( s_f )          
   { m += n; n ++ ; 
     if ( ! sm_f ) sm = sl;
     if ( sl < sm ) { lx = sl; sl = sm; sm = lx; }
   }

if ( n >= (double)MAXLONG || m >= (double)MAXLONG )
{ fprintf ( stderr, "Too large problem. It can't be generated\n" );
  exit (4);
}
 else
{
  n0 = (long)n; m0 = (long)m;
}

if ( ip_f )
   mess = (long*) calloc ( Y, sizeof ( long ) );

/* printing title */
printf ("c grid network for shortest paths problem\n");
printf ("c extended DIMACS format\nc\n" );

/* name of the problem */
printf ("t gr_%ldx%ld_%ld_", X, Y, seed );
if ( c_f )  printf ("%c", 'c');
if ( a_f )  printf ("%c", 'a');
if ( i_f )  printf ("%c", 'i');
if ( s_f )  printf ("%c", 's');
if ( p_f )  printf ("%c", 'p');
printf ("\nc\n");

/* printing additional information  */
if ( cw_f )
  printf ("c type of spanning graph ->  %s\n",
           graph_type[cw] );
if ( a_f )
  printf ("c additional arcs -> number: %ld min length: %ld max length: %ld\n",
           ax, am, al );
if ( i_f )
{
  printf ("c inter-layer arcs -> min: %ld max: %ld k1: %.2f k2: %.2f\n",
	  im, il, in, is );
  printf ("c inter-layer arcs from one node -> number: %ld step: %ld\n",
	  ix, ih );
  if ( ip_f )
  printf ("c inter-layer arcs will be shuffled\n" );
}
if ( s_f )
  printf ("c length of arcs from artifical source -> min: %ld max: %ld\n",
           sm, sl );
if ( p_f )
  {
  printf ("c potentials -> min: %ld max: %ld k1: %.2f k2: %.2f\n",
	  pm, pl, pn, ps );
  }
printf ("c\n" );

printf ("p sp %8ld %8ld\nc\n", n0, m0 );

printf ("n %8ld\nc\n", n0 );
source = ( s_f ) ? n0-1 : n0;

if ( p_f ) /* generating potentials */
  {
    p = (long*) calloc ( n0+1, sizeof (long) );
    seed1 = 2*seed + 1;
    init_rand ( seed1);
    pl = pl - pm + 1;

    for ( x = 0; x < X; x ++ )
      for ( y = 0; y < Y; y ++ )
      {
	p_t = pm + nrand ( pl );
	if ( pn_f ) p_t *= (long) ( (1 + x) * pn );
	if ( ps_f ) p_t *= (long) ( (1 + x) * ( (1 + x) * ps ));

        p[ NODE ( x, y ) ] = p_t;
      }
    p[n0] = 0;
    if ( s_f ) p[n0-1] = 0;
  }


if ( s_f ) /* additional arcs from artifical source */
  {
    seed2 = 3*seed + 1;
    init_rand ( seed2 );
    sl = sl - sm + 1;

    for ( x = X - 1; x >= 0; x -- )
      for ( y = Y - 1; y >= 0; y -- )
      {
        i = NODE ( x, y );
	s = sm + nrand ( sl );
	PRINT_ARC ( n0, i, s ) 
      }

    PRINT_ARC ( n0, n0-1, 0 )
  }


/* ----- generating arcs within layers ----- */

init_rand ( seed );
cl = cl - cm + 1;
al = al - am + 1;

for ( x = 0; x < X; x ++ )
 {
/* generating arcs within one layer */
  for ( y = 0; y < Y-1; y ++ )
  {
     /* generating spanning graph */
     i = NODE ( x, y );
     j = NODE ( x, y+1 );
     l = cm + nrand ( cl );    
     PRINT_ARC ( i, j, l )

     if ( cw == DOUBLE_CYCLE )
       {
         l = cm + nrand ( cl );    
         PRINT_ARC ( j, i, l )
       }
   }
     
  if ( cw <= CYCLE ) 
    {
      i = NODE ( x, Y-1 );
      j = NODE ( x, 0 );
      l = cm + nrand ( cl );
      PRINT_ARC ( i, j, l )

      if ( cw == DOUBLE_CYCLE )
        {
	  l = cm + nrand ( cl );
          PRINT_ARC ( j, i, l )
        }
     }

/* generating additional arcs */

  for ( k = ax; k > 0; k -- )
     {
       y1 = nrand ( Y );
       do
          y2 = nrand ( Y );
       while ( y2 == y1 );
       i  = NODE ( x, y1 );
       j  = NODE ( x, y2 );
       l = am + nrand ( al );
       PRINT_ARC ( i, j, l )
     }
 }

/* ----- generating interlayered arcs ------ */

il = il - im + 1;

/* arcs from the source */

  for ( y = 0; y < Y; y ++ )
    {
      l = im + nrand ( il );
      i = NODE ( 0, y );
      PRINT_ARC ( source, i, l )
    }

for ( x = 0; x < X-1; x ++ )
 {
/* generating arcs from one layer */
   for ( count = 0, xn = x + 1; 
         count < ix && xn < X; 
         count ++, xn += ih )
    {
      if ( ip_f )
      for ( y = 0; y < Y; y ++ )
	mess[y] = y;

      for ( y = 0; y < Y; y ++ )
       {
          i = NODE ( x, y );
	  dx = xn - x;
	  if ( ip_f )
	    {
	      yp = nrand(Y-y);
	      yn = mess[ yp ];
              mess[ yp ] = mess[ Y - y - 1 ];
	    }
	  else
             yn =  y;
	  j = NODE ( xn, yn );
	  l = im + nrand ( il );
	  if ( in != 0 )
            l *= (long) ( in * dx );
          if ( is_f )
            l *= (long) ( ( is * dx ) * dx );
          PRINT_ARC ( i, j, l )
	}
    }
 }
/* all is done */
exit (ext);

/* ----- wrong usage ----- */

 usage:
fprintf ( stderr,
"\nusage: %s  X  Y  seed  [ -ll#i -lm#i -cl#i -p -pl#i -pm#i ... ]\n\
help:  %s -h\n\n", argv[0], argv[0] );

if ( np > 0 )
  fprintf ( stderr, "error in parameter # %d\n\n", np );   
exit (4);

/* ---- help ---- */

 help:

if ( args[2] == 'h') goto hhelp;

fprintf ( stderr, 
"\n'%s' - grid network generator for shortest paths problem.\n\
Generates problems in extended DIMACS format.\n\
\n\
  %s X Y seed [ -cl#i -cm#i -c{c|d|p} -ip -il#i -im#i -p -pl#i -pm#i... ]\n\
  %s -hh\n\
\n\
                        #i - integer number   #f - real number\n\
\n\
-cl#i - #i is the upper bound on layer arc lengths    (default 100)\n\
-cm#i - #i is the lower bound on layer arc lengths    (default 0)\n\
-c#t  - #t is the type of connecting graph: { c | d | p }\n\
           c - cycle, d - double cycle, p - path      (default d)\n\
-ip   - shuffle inter-layer arcs                     (default NO)\n\
-il#i - #i is the upper bound on inter-layer arc lengths (default 10000)\n\
-im#i - #i is the lower bound on inter-layer arc lengths (default 1000)\n\
-p    - generate potentials \n\
-pl#i - #i is the upper bound on potentials           (default il)\n\
-pm#i - #i is the lower bound on potentials           (default im)\n\
\n\
-hh    - extended help \n\n",
argv[0], argv[0], argv[0] );

exit (0);

/* --------- sophisticated help ------------ */
 hhelp:

if ( argc < 3 )
     fout = stderr;
else
     fout = fopen ( argv[2], "w" );

if ( fout == NULL )
{ fprintf ( stderr, "\nCan't open file  '%s' for writing help\n\n", argv[2] );
  exit ( 2 );
}

fprintf (fout, 
"\n'%s' - grid network generator for shortest paths problem.\n\
Generates problems in extended DIMACS format.\n\
\n\
   %s  X Y seed [ -cl#i -cm#i -c{c|d|p}\n\
                      -ax#i -al#i -am#i\n\
                      -ip   -il#i -im#i -in#i -is#i -ix#i -ih#i\n\
                      -p    -pl#i -pm#i -pn#f -ps#f\n\
                      -s    -sl#i -sm#i\n\
                    ]\n\
   %s -hh file_name\n\
\n\
                        #i - integer number   #f - real number\n\
\n\
      Parameters of connecting arcs within one layer:\n\
-cl#i - #i is the upper bound on arc lengths          (default 100)\n\
-cm#i - #i is the lower bound on arc lengths          (default 0)\n\
-c#t  - #t is the type of connecting graph: { c | d | p }\n\
           c - cycle, d - double cycle, p - path      (default d)\n\
\n\
      Parameters of additional arcs within one layer:\n\
-ax#i - #i is the number of additional arcs           (default 0)\n\
-al#i - #i is the upper bound on arc lengths          (default 100)\n\
-am#i - #i is the lower bound on arc lengths          (default 0)\n\
\n\
      Interlayerd arc parameters:\n\
-ip    - shuffle inter-layer arcs                         (default NO)\n\
-il#i  - #i is the upper bound on arc lengths          (default 10000)\n\
-im#i  - #i is the lower bound on arc lengths          (default 1000)\n\
-in#f  - multiply l(i, j) by #f * x(j)-x(i)           (default 1)\n\
         if #f=0 - don't multiply\n\
-is#f  - multiply l(i, j) by #f * (x(j)-x(i))^2       (default NO)\n\
-ix#i  - #i - is the number of arcs from a node        (default 1)\n\
-ih#i  - #i - is the step between connected layers     (default 1)\n\
\n\
      Potential parameters:\n\
-p     - generate potentials \n\
-pl#i  - #i is the upper bound on potentials           (default ll)\n\
-pm#i  - #i is the lower bound on potentials           (default lm)\n\
-pn#f  - multiply p(i) by #f * x(i)                    (default NO)\n\
-ps#f  - multiply p(i) by #f * x(i)^2                  (default NO)\n\
\n",
argv[0], argv[0], argv[0] );
fprintf (fout,
"     Artificial source parameters:\n\
-s     - generate artificial source with default connecting arc lengths\n\
-sl#i  - #i is the upper bound on art. arc lengths    (default 100000000)\n\
-sm#i  - #i is the lower bound on art. arc lengths    (default sl)\n\
\n\
-hh file_name  - save this help in the file 'file_name'\n\n"
);
exit (0);
}



